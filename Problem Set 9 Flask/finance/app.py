import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():

    # Table
    table = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])

    # Complete table
    for tab in table:
        symbol = tab["symbol"]
        stock = lookup(symbol)
        tab["name"] = stock["name"]
        tab["current_price"] = stock["price"]
        tab["total_value"] = float(stock["price"]) * float(tab["shares"])

    # Total portfolio in stocks
    total_portfolio = sum(tab["total_value"] for tab in table)

    # Cash
    cash = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"]

    # Grand Total
    grand_total = total_portfolio + cash

    return render_template("index.html", total_portfolio = total_portfolio, cash = cash, grand_total = grand_total, table = table)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check if symbol empty
        if not symbol:
            return apology("You must provide a symbol")

        symbol_info = lookup(symbol)

        # Check if valid symbol
        if symbol_info is None:
            return apology("Invalid symbol")

        # Check shares
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("must provide a positive integer")

        except (ValueError, TypeError):
            return apology("must provide a positive integer")

        # Check stock price
        stock_price = symbol_info["price"]

        # Check user cash
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"]

        # Check if enough money
        total_cost = shares * stock_price
        if cash < total_cost:
            return apology("Not enough money to buy!")

        # Insert in transactions the new shares bought
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",session["user_id"], symbol, shares, stock_price)

        # update the user cash
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?",total_cost, session["user_id"])
        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    # Table
    table = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    # Add action column where is : purchase or sale
    for tab in table:
        shares = int(tab["shares"])
        if shares < 0:
            tab["action"] = "sale"
        else:
            tab["action"] = "purchase"
        symbol = tab["symbol"]
        stock = lookup(symbol)
        tab["name"] = stock["name"]

    return render_template("history.html", table = table)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Check if symbol empty
        if not symbol:
            return apology("You must provide a symbol")

        symbol_info = lookup(symbol)

        # Chek if valid symbol
        if symbol_info is None:
            return apology("Invalid symbol")

        return render_template("quoted.html", stock=symbol_info)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check username
        if not username:
            return apology("username can't be blank")

        # Check password
        if not password:
            return apology("password can't be blank")

        # Check confirmation
        if password != confirmation:
            return apology("password and confirmation don't match")

        # Generate the hash from the password
        hash_pw = generate_password_hash(password)

        # Try to insert the new user (with hashed password)
        try:
            id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash_pw)
        except:
            return apology("this username already exists")

        # log in the user automatically after registering
        session["user_id"] = id
        flash("Registered!")
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check if symbol empty
        if not symbol:
            return apology("You must provide a symbol")

        symbol_info = lookup(symbol)

        # Check if valid symbol
        if not symbol_info:
            return apology("Invalid symbol")

        # Check shares
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("must provide a positive integer")

        except (ValueError, TypeError):
            return apology("must provide a positive integer")

        # Check stock price
        stock_price = symbol_info["price"]

        # Check user number of shares of the current symbol (company)
        num_shares = db.execute("SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol =  ?", session["user_id"], symbol)

        if len(num_shares) == 0 or num_shares[0]["shares"] is None or num_shares[0]["shares"] <= 0:
            return apology("You don't own this stock")

        num_shares = num_shares[0]["shares"]

        # chek if enough shares to sell
        if shares > num_shares:
            return apology("You don't have enough shares to sell")


        # Insert in transactions the new shares sold
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",session["user_id"], symbol, -shares, stock_price)

        # update the user cash
        total_price = shares*stock_price
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",total_price, session["user_id"])
        flash("Sold!")
        return redirect("/")

    else:
        # Geting the list of symbols to put in the select
        list_symbols = db.execute("SELECT DISTINCT symbol FROM transactions WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", list_symbols = list_symbols)



@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        user_id = session["user_id"]
        current_password_input = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Check empty fields
        if not current_password_input:
            return apology("Current password can't be blank")
        if not new_password:
            return apology("New password can't be blank")
        if not confirmation:
            return apology("Confirmation can't be blank")

        # Get current hash from database
        current_hash = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]["hash"]

        # Verify current password
        if not check_password_hash(current_hash, current_password_input):
            return apology("Incorrect current password")

        # Check confirmation
        if new_password != confirmation:
            return apology("Passwords do not match")

        # Generate new hash
        new_hash = generate_password_hash(new_password)

        # Update user hash in DB
        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id)

        flash("Password changed successfully!")
        return redirect("/")

    else:
        return render_template("change_password.html")


import csv
import sys


def main():

    # Checking command-line if correct and save the CSV in a dictionary and read the text and save it into a variable
    len_sys = len(sys.argv)
    if len_sys != 3:
        print("Error: could not read the files from the command line. Please provide one .csv file and one .txt file.")
        sys.exit(1)

    try:
        rows = []
        with open(sys.argv[1], "r") as file:
            reader = csv.DictReader(file)
            for row in reader:
                rows.append(row)

    except Exception:
        print(f"Error: could not open the file '{sys.argv[1]}'.")
        sys.exit(1)

    try:
        with open(sys.argv[2], "r") as f:
            text_content = f.read()
    except Exception:
        print(f"Error: could not open the file '{sys.argv[2]}'.")
        sys.exit(1)
    # Obtain the subsequences of DNA
    columns = []
    for col in rows[0].keys():
        if col != "name":
            columns.append(col)

    # Check the longest matches and save them
    matches = {}
    for col in columns:
        long_match = longest_match(text_content, col)
        matches[col] = long_match

    # Check if the current matches correspond to a person
    for row in rows:
        ok = True
        for key, value in row.items():
            if key != "name":
                if int(value) != int(matches[key]):
                    ok = False
                    break
        if ok == True:
            print(row["name"])
            return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

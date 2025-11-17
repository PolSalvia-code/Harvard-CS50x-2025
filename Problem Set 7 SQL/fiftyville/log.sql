-- 1. To see the tables
.tables
-- 2. To see the schema of the crime scene reports
.schema crime_scene_reports
-- 3. To see the crime description :
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = 'Humphrey Street';
-- 4. Searching for the interviews:
--| 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
--| 162 | Eugene  | I don't know the thief's name, but it was someone I recognized.
--Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
--| 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. |
.schema interviews
SELECT id, name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2024 AND transcript LIKE '%bakery%';
-- 5. We found that might be in this list:
--| id  | activity | license_plate | minute |
--| 260 | exit     | 5P2BI95       | 16     |
--| 261 | exit     | 94KL13X       | 18     |
--| 262 | exit     | 6P58WS2       | 18     |
--| 263 | exit     | 4328GD8       | 19     |
--| 264 | exit     | G412CB7       | 20     |
--| 265 | exit     | L93JTIZ       | 21     |
--| 266 | exit     | 322W7JE       | 23     |
--| 267 | exit     | 0NTHK55       | 23     |
.schema bakery_security_logs
SELECT id, activity, license_plate, minute FROM bakery_security_logs WHERE month = 7 AND day = 28 AND year = 2024 AND hour = 10;
-- 6. Consult the atm transactions
-- +-----+----------------+------------------+--------+
--| id  | account_number | transaction_type | amount |
--+-----+----------------+------------------+--------+
--| 246 | 28500762       | withdraw         | 48     |
--| 264 | 28296815       | withdraw         | 20     |
--| 266 | 76054385       | withdraw         | 60     |
--| 267 | 49610011       | withdraw         | 50     |
--| 269 | 16153065       | withdraw         | 80     |
--| 288 | 25506511       | withdraw         | 20     |
--| 313 | 81061156       | withdraw         | 30     |
--| 336 | 26013199       | withdraw         | 35     |
--+-----+----------------+------------------+--------+
SELECT id,account_number,transaction_type, amount FROM atm_transactions  WHERE month = 7 AND day = 28 AND year = 2024 AND atm_location = 'Leggett Street';

-- 7. We search for calls
--+-----+----------------+----------------+-----+----------+
--| id  |     caller     |    receiver    | day | duration |
--+-----+----------------+----------------+-----+----------+
--| 221 | (130) 555-0289 | (996) 555-8899 | 28  | 51       |
--| 224 | (499) 555-9472 | (892) 555-8872 | 28  | 36       |
--| 233 | (367) 555-5533 | (375) 555-8161 | 28  | 45       |
--| 234 | (609) 555-5876 | (389) 555-5198 | 28  | 60       |
--| 251 | (499) 555-9472 | (717) 555-1342 | 28  | 50       |
--| 254 | (286) 555-6063 | (676) 555-6554 | 28  | 43       |
--| 255 | (770) 555-1861 | (725) 555-3243 | 28  | 49       |
--| 261 | (031) 555-6622 | (910) 555-3251 | 28  | 38       |
--| 279 | (826) 555-1652 | (066) 555-9701 | 28  | 55       |
--| 281 | (338) 555-6650 | (704) 555-2131 | 28  | 54       |
--+-----+----------------+----------------+-----+----------+
SELECT id, caller, receiver , day, duration FROM phone_calls WHERE month = 7 AND day = 28 AND year = 2024;

-- 8. Selects people who appeared in the bakery security log on July 28, 2024, at 10 a.m. (matched by license_plate) and who made a phone call on the same day (matched by c.caller = p.phone_number).
--+---------+--------+
--|  name   |   id   |
--+---------+--------+
--| Vanessa | 221103 |
--| Barry   | 243696 |
--| Sofia   | 398010 |
--| Taylor  | 449774 |
--| Diana   | 514354 |
--| Kelsey  | 560886 |
--| Bruce   | 686048 |
--+---------+--------+
SELECT DISTINCT p.name, p.id FROM people AS p JOIN bakery_security_logs AS b ON b.license_plate = p.license_plate JOIN phone_calls AS c ON c.caller = p.phone_number WHERE b.month = 7 AND b.day = 28 AND b.year = 2024 AND b.hour = 10 AND c.month = 7 AND c.day = 28 AND c.year = 2024;

--9. Selected the IDs of people who own a bank account that was used to make a withdrawal at the Leggett Street ATM on July 28, 2024.
----+--------+
--|   id   |
--+--------+
--| 686048 |
--| 514354 |
--| 458378 |
--| 395717 |
--| 396669 |
--| 467400 |
--| 449774 |
--| 438727 |
-- +--------+
SELECT p.id FROM people AS p JOIN bank_accounts AS b ON p.id = b.person_id WHERE b.account_number IN (SELECT account_number FROM atm_transactions  WHERE month = 7 AND day = 28 AND year = 2024 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');

--10. Select the names and IDs of people who were at the bakery on July 28, 2024, at 10 a.m., made a phone call that same day, and withdrew money from the Leggett Street ATM.
--+--------+--------+
--|  name  |   id   |
--+--------+--------+
--| Taylor | 449774 |
--| Diana  | 514354 |
--| Bruce  | 686048 |
--+--------+--------+
SELECT DISTINCT p.name, p.id FROM people AS p JOIN bakery_security_logs AS b ON b.license_plate = p.license_plate JOIN phone_calls AS c ON c.caller = p.phone_number JOIN bank_accounts AS ba ON ba.person_id = p.id JOIN atm_transactions AS t ON t.account_number = ba.account_number WHERE b.year = 2024 AND b.month = 7 AND b.day = 28 AND b.hour = 10 AND c.year = 2024 AND c.month = 7 AND c.day = 28 AND t.year = 2024 AND t.month = 7 AND t.day = 28 AND t.atm_location = 'Leggett Street' AND t.transaction_type = 'withdraw';

-- 11.Selects the passport numbers of passengers who flew on July 29, 2024, and who were at the bakery between 10:15 and 10:25 a.m. on July 28, 2024, made a phone call that day, and withdrew money from the Leggett Street ATM.
--+-----------------+
--| passport_number |
--+-----------------+
--| 3592750733      |
--| 5773159633      |
--+-----------------+

SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE month = 7 AND day = 29 AND year = 2024) AND passport_number IN (SELECT DISTINCT p.passport_number FROM people AS p JOIN bakery_security_logs AS b ON b.license_plate = p.license_plate JOIN phone_calls AS c ON c.caller = p.phone_number JOIN bank_accounts AS ba ON ba.person_id = p.id JOIN atm_transactions AS t ON t.account_number = ba.account_number WHERE b.year = 2024 AND b.month = 7 AND b.day = 28 AND b.hour = 10 AND c.year = 2024 AND c.month = 7 AND c.day = 28 AND b.minute BETWEEN 15 AND 25 AND t.year = 2024 AND t.month = 7 AND t.day = 28 AND t.atm_location = 'Leggett Street' AND t.transaction_type = 'withdraw');


-- 12. Selects the names, IDs, passport numbers, and flight details of people who departed from Fiftyville on July 29, 2024, and who were at the bakery between 10:15 and 10:25 a.m. on July 28, 2024, made a phone call that day, and withdrew money from the Leggett Street ATM.

SELECT DISTINCT p.name, p.id AS person_id, f.id AS flight_id, f.hour, pa.passport_number FROM flights f JOIN passengers pa ON pa.flight_id = f.id JOIN airports a ON a.id = f.origin_airport_id JOIN people p ON p.passport_number = pa.passport_number WHERE f.year = 2024 AND f.month = 7 AND f.day = 29 AND a.city = 'Fiftyville' AND pa.passport_number IN (SELECT DISTINCT p2.passport_number FROM people p2 JOIN bakery_security_logs b ON b.license_plate = p2.license_plate JOIN phone_calls c ON c.caller = p2.phone_number JOIN bank_accounts ba ON ba.person_id = p2.id JOIN atm_transactions t ON t.account_number = ba.account_number WHERE b.year = 2024 AND b.month = 7 AND b.day = 28 AND b.hour = 10 AND c.year = 2024 AND c.month = 7 AND c.day = 28 AND t.year = 2024 AND b.minute BETWEEN 15 AND 25 AND t.month = 7 AND t.day = 28 AND t.atm_location = 'Leggett Street' AND t.transaction_type = 'withdraw');
--+-------+-----------+-----------+------+-----------------+
--| name  | person_id | flight_id | hour | passport_number |
--+-------+-----------+-----------+------+-----------------+
--| Diana | 514354    | 18        | 16   | 3592750733      |
--| Bruce | 686048    | 36        | 8    | 5773159633      |
--+-------+-----------+-----------+------+-----------------+

-- Bruce is the Thief

-- 13. And to see where he scaped
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE id = 36);

-- He scaped to New York City 

-- 13. Here we find the phone of the ACCOMPLICE:

SELECT receiver FROM phone_calls WHERE caller = '(367) 555-5533' AND month = 7 AND day = 28 AND year = 2024 AND duration < 60;
--+----------------+
--|    receiver    |
--+----------------+
--| (375) 555-8161 |
--+----------------+

--14. And here the name of the accomplice
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE caller = '(367) 555-5533' AND month = 7 AND day = 28 AND year = 2024 AND duration < 60);
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+

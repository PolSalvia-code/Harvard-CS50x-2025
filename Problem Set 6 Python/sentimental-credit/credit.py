while True:
    try:
        number = int(input("Number: "))
        if (number > 0):
            sum_mulip_2 = 0
            sum = 0
            len_number = len(str(number))
            # Calc the two sums
            for i in range(len(str(number))):
                if i % 2 == 0:
                    sum += int(str(number)[len_number-i-1])
                else:
                    # first multiply by 2
                    new_num = 2*int(str(number)[len_number-i-1])
                    # sum each digit
                    for i in range(len(str(new_num))):
                        sum_mulip_2 += int(str(new_num)[i])
            # Check if valid car number
            print(sum+sum_mulip_2)
            if ((sum+sum_mulip_2) % 10 != 0):
                print("INVALID")
                break
            if (int(str(number)[:2]) == 34 or int(str(number)[:2]) == 37 and len(str(number)) == 15):
                print("AMEX")
                break
            elif (int(str(number)[:2]) in [51, 52, 53, 54, 55] and len(str(number)) == 16):
                print("MASTERCARD")
                break
            elif (int(str(number)[0]) == 4 and len(str(number)) in [16, 13]):
                print("VISA")
                break
            else:
                print("INVALID")
                break
    except ValueError:
        continue

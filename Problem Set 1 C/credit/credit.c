#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number;
    int index_number = 0;               // index of the current digit, from right (0) to left
    int sum_unmultiplied_digits = 0;    // Sum of digits not multiplied by 2
    int sum_multiplied_digits_by_2 = 0; // sum of digits multiplied by 2

    do
    {
        number = get_long("Number: ");
    }

    while (number < 1); // Here we check if the value recived is a positive number

    for (long temp = number; temp != 0; temp /= 10)
    {
        int digit = temp % 10;
        if (index_number % 2 == 0)
        {
            sum_unmultiplied_digits += digit;
        }
        else if (index_number % 2 == 1)
        {
            int digit_multiplies = digit * 2;
            for (int i = digit_multiplies; i != 0; i /= 10)
            {
                int digit_of_multiplied_digits_by_2 = i % 10;
                sum_multiplied_digits_by_2 += digit_of_multiplied_digits_by_2;
            }
        }
        index_number++;
    }

    int sum =
        sum_multiplied_digits_by_2 +
        sum_unmultiplied_digits; // Sum of digits not multiplied by 2 and digits multiplied by 2
    int last_digit_sum;          // we use this variable to check if the las number of the sum is 0

    if (sum % 10 == 0)
    {
        long fisrt_2digit;     // Here we store the first 2 digits of the Card
        int lenght_number = 2; // Here we store the lenght of the number

        fisrt_2digit = number;

        // here we find the first 2 digit of the number
        while (fisrt_2digit >= 100)
        {
            fisrt_2digit /= 10;
            lenght_number++;
        }

        if ((fisrt_2digit == 51 || fisrt_2digit == 52 || fisrt_2digit == 53 || fisrt_2digit == 54 ||
             fisrt_2digit == 55) &&
            (lenght_number == 16))
        {
            printf("MASTERCARD\n");
        }
        else if ((fisrt_2digit == 34 || fisrt_2digit == 37) && (lenght_number == 15))
        {
            printf("AMEX\n");
        }
        else if (fisrt_2digit / 10 == 4 && (lenght_number == 13 || lenght_number == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

require daslib/faker

require daslib/regex_boost
require daslib/random

[export]
def main
    // for y in range(1980,2025)
    //    print("{y} is a leap year? {is_leap_year(uint(y))}\n")
    fake_number(10) <| $ ( str )
        print("number {str}\n")
    fake_date(10) <| $ ( str )
        print("month {str}\n")
    fake_month(10) <| $ ( str )
        print("month {str}\n")
    fake_day(10) <| $ ( str )
        print("day {str}\n")
    fake_positive_int(10) <| $ ( str )
        print("positive int {str}\n")
    fake_int(10) <| $ ( str )
        print("int {str}\n")

options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module faker shared private

// TODO: add control for the RNG source

require daslib/regex_boost
require daslib/random
require daslib/static_let
require daslib/strings
require daslib/defer

struct MonthNameAndDay
    name: string
    days: int

var private g_months <- [[MonthNameAndDay
    name="January",days=31;
    name="February",days=28;
    name="March",days=31;
    name="April",days=30;
    name="May",days=31;
    name="June",days=30;
    name="July",days=31;
    name="August",days=31;
    name="September",days=30;
    name="October",days=31;
    name="November",days=30;
    name="December",days=31
]]

var private g_days <- [[string
    "Sunday";
    "Monday";
    "Tuesday";
    "Wednesday";
    "Thursday";
    "Friday";
    "Saturday"
]]

struct public Faker
    min_year : uint = 1980u                     //! minimal faker's year
    total_years : uint = (2022u-1980u)          //! faker year's range
    rnd : iterator<uint> <- each_random_uint()  //! fakers random number generator
    max_long_string : uint = 4096u              //! maximal length of generated string

def public Faker ( var rng : iterator<uint> )
    return <- [[ Faker() rnd<-rng ]]

var private re_positive_int <- %regex~[0-9][0-9]*%%
var private re_int <- %regex~[\+\-]?[0-9][0-9]*%%
var private re_uint <- %regex~[0-9][0-9]*%%
var private re_hex <- %regex~[0-9a-zA-Z][0-9a-zA-Z]*%%
var private re_string <- %regex~.*%%
var private re_float <- %regex~[\+\-]?[0-9][0-9]*(\.[0-9]*)?%%
var re_number <- %regex~(zero|one|two|three|four|five|six|seven|eight|nine|ten)%%

def public long_string ( var faker:Faker )
    var len = 0u
    faker.rnd |> next(len)
    len %= faker.max_long_string
    var arr : array<uint8>; defer_delete(arr)
    arr |> reserve(int(len))
    for t in urange(len)
        var Ch = 0u
        faker.rnd |> next(Ch)
        arr |> push(uint8(Ch))
    return string(arr)

def public any_string ( var faker:Faker )
    return re_gen(re_string,faker.rnd)

def public random_int ( var faker:Faker )
    var i = 0u
    faker.rnd |> next(i)
    return int(i)

def public random_uint ( var faker:Faker )
    var i = 0u
    faker.rnd |> next(i)
    return i

def public any_set ( var faker:Faker )
    var ch : uint[8]
    for c in ch
        faker.rnd |> next( c)
    return ch

def public any_char ( var faker:Faker )
    var ch = 0u
    faker.rnd |> next(ch)
    return int((ch % 255u)+1u)

def public number ( var faker:Faker )
    return re_gen(re_number,faker.rnd)

def public positive_int ( var faker:Faker )
    return re_gen(re_positive_int,faker.rnd)

def public any_int ( var faker:Faker )
    return re_gen(re_int,faker.rnd)

def public any_hex ( var faker:Faker )
    return re_gen(re_hex,faker.rnd)

def public any_float ( var faker:Faker )
    return re_gen(re_float,faker.rnd)

def public any_uint ( var faker:Faker )
    return re_gen(re_uint,faker.rnd)

def public month ( var faker:Faker )
    var month = 0u
    faker.rnd |> next(month)
    month = month % 12u
    return g_months[month].name

def public day ( var faker:Faker )
    var day = 0u
    faker.rnd |> next(day)
    day = day % 7u
    return g_days[day]

def public is_leap_year ( year:uint )
    return (year % 4u == 0u) && ( year % 100u != 0u || year % 400u == 0u )

def public week_day ( year, month, day : uint )
    return week_day(int(year),int(month),int(day))

def public week_day ( year, month, day : int )
    static_let <|
        let offset <- [{auto 0; 31; 59; 90; 120; 151; 181; 212; 243; 273; 304; 334}]
    var afterFeb = 1
    if month > 2
        afterFeb = 0
    let aux = year - 1700 - afterFeb
    // dayOfWeek for 1700/1/1 = 5, Friday
    var dayOfWeek  = 5
    // partial sum of days betweem current date and 1700/1/1
    dayOfWeek += (aux + afterFeb) * 365
    // leap year correction
    dayOfWeek += aux / 4 - aux / 100 + (aux + 100) / 400
    // sum monthly and day offsets
    dayOfWeek += offset[month] + (day - 1)
    dayOfWeek %= 7
    return dayOfWeek

def public date ( var faker:Faker )
    var month = 0u
    var day = 0u
    var year = 0u
    faker.rnd |> next(month)
    faker.rnd |> next(year)
    faker.rnd |> next(day)
    month = month % 12u
    year = faker.min_year + (year % faker.total_years)
    var max_days = uint(g_months[month].days)
    if ( is_leap_year(year) && month == 1u )
        max_days = 29u
    day = 1u + (day % max_days)
    let dow = week_day(year,month,day)
    return "{g_days[dow]}, {g_months[month].name} {int(day)}, {int(year)}"

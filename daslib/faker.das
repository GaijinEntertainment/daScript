options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module faker shared private

// TODO: add control for the RNG source

require daslib/regex_gen
require daslib/regex_boost
require daslib/random
require daslib/static_let

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

var private g_min_year = 1980u                  //! minimal faker's year
var private g_total_years = (2022u-1980u)       //! faker year's range

var private re_positive_int <- %regex~[1-9][0-9]*%%
var private re_int <- %regex~\-?[1-9][0-9]*%%
var re_number <- %regex~(zero|one|two|three|four|five|six|seven|eight|nine|ten)%%

var private g_rnd : iterator<uint> <- each_random_uint()

def public set_fake_rnd_source ( var rnd : iterator<uint> )
    delete g_rnd
    g_rnd <- rnd

def public fake ( var count:int; var re:Regex; blk : block<(tmp:string):void> )
    var reg <- re_gen_compile(re)
    for c in range(count)
        let str = re_gen(re,reg,g_rnd)
        invoke(blk,str)
    delete reg

def public fake_number ( var count:int; blk : block<(tmp:string):void> )
    fake ( count, re_number, blk )

def public fake_positive_int ( var count:int; blk : block<(tmp:string):void> )
    fake ( count, re_positive_int, blk )

def public fake_int ( var count:int; blk : block<(tmp:string):void> )
    fake ( count, re_int, blk )

def public fake_month ( var count:int; blk : block<(tmp:string):void> )
    for i in range(count)
        var month = 0u
        next(g_rnd,month)
        month = month % 12u
        invoke(blk, g_months[month].name)

def public fake_day ( var count:int; blk : block<(tmp:string):void> )
    for i in range(count)
        var day = 0u
        next(g_rnd,day)
        day = day % 7u
        invoke(blk, g_days[day])

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

def public fake_date ( var count:int; blk : block<(tmp:string):void> )
    for i in range(count)
        var month = 0u
        var day = 0u
        var year = 0u
        next(g_rnd,month)
        next(g_rnd,year)
        next(g_rnd,day)
        month = month % 12u
        year = g_min_year + (year % g_total_years)
        var max_days = uint(g_months[month].days)
        if ( is_leap_year(year) && month == 1u )
            max_days = 29u
        day = 1u + (day % max_days)
        let dow = week_day(year,month,day)
        invoke(blk, "{g_days[dow]}, {g_months[month].name} {int(day)}, {int(year)}")

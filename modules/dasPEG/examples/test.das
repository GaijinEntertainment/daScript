//require daslib/profiler

options stack = 1000000000

require daslib/debug
options debugger

require daslib/strings_boost
require daslib/fio
require daslib/math
require daslib/templates_boost
require dastest/testing_boost
require daslib/utf8_utils
require peg/peg


struct Email
    username: string
    domain: string


[export]
def main

    parse test
        var greeting: bool

        rule("Hello, ", "{+not_bang}" as name, "!") <|
            return true

        rule("Hello, ", "!", "?") <|
            return true

        option(tracing)
        option(color)

        var not_bang: void?

        rule(!set('!', '1'..'9'), any) <|
            return null

    var parser <- [[testParser]]
    parser.color_output = true

    var input = "Hello, ! world"
    for char in input
        parser.input |> push(char |> uint8())

    parser |> parse_greeting

    // Now again with error reporting
    parser.error_reporting = true
    parser |> parse_greeting


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


def one
    parse greeting
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

    var parser <- [[greetingParser]]
    parser.color_output = true

    var input = "Hello, ! world"
    for char in input
        parser.input |> push(char |> uint8())

    parser |> parse_greeting

    // Now again with error reporting
    parser.error_reporting = true
    parser |> parse_greeting


def two
    parse cut
        var prog: bool

        option(tracing)
        option(color)

        rule(decl) <|
            return true





        var decl: bool

        rule("def ", commit,
                    "{ident_}" as name,
                    log("Parsed function name: {name}"),
                    "(", *argument as args, argument_last as last, ")"
                ) <|
            return true

        rule("var ", commit,
                    "{ident_}" as name,
                    log("Parsed var name: {name}"),
                    MB(typing),
                    MB(initializer)
                ) <|
            return true

        rule("let ", commit,
                    "{ident_}" as name,
                    log("Parsed var name: {name}"),
                    MB(typing),
                    MB(initializer)
                ) <|
            return true





        var argument_last: string
        rule("{+alpha}" as name, WS) <|
            return name

        var argument: string
        rule("{+alpha}" as name, WS, ",", WS) <|
            return name




        var typing: void?
        rule(":", "void?") <|
            return null

        var initializer: void?
        rule("=", "null") <|
            return null




        var ident: string
        rule("{ident}" as ident) <|
            return ident

        var ident_: void?
        rule(alpha, *alnum) <|
            return null

        var alpha: void?

        rule(set('a'..'z', 'A'..'Z')) <|
            return null

        var alnum: void?

        rule(alpha) <|
            return null

        rule(set('1'..'9', '_')) <|
            return null

    var parser <- [[cutParser]]
    parser.color_output = true

    var input = "def main(argc, argv) \{"
    var input = "def main(123, argv) \{"

    for char in input
        parser.input |> push(char |> uint8())

    print("{parser |> parse_decl}\n")

    // Now again with error reporting
    parser.error_reporting = true
    print("{parser |> parse_decl}\n")


[export]
def main
    two()

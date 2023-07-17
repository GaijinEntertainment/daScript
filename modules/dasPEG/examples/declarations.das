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

def one( greeting: string; blk: block<( a:bool; err:array<ParsingError> ):void>)
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


def two( prog:string; blk: block<( a:bool; err:array<ParsingError> ):void>)
    parse prog
        var prog: bool

        // option(tracing)
        // option(color)

        rule(decl) <|
            return true

        var decl: bool

        rule("def ", commit,
                    "{ident_}" as name,
                    log("Parsed function name: {name}\n"),
                    "(", *argument as args, argument_last as last, ")"
                ) <|
            return true

        rule("var ", commit,
                    "{ident_}" as name,
                    log("Parsed var name: {name}\n"),
                    MB(typing),
                    MB(initializer)
                ) <|
            return true

        rule("let ", commit,
                    "{ident_}" as name,
                    log("Parsed var name: {name}\n"),
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


[export]
def main
    var input1 = "def main(argc, argv) \{"
    var input2 = "def main(123, argv) \{"

    two(input1) <| $(val; err)
        print("Success: {val}\n")

    two(input2) <| $(val; err)
        for e in err
            "{e}\n" |> print

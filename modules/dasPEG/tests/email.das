//require daslib/profiler

require daslib/debug
options debugger

require dastest/testing_boost
require daslib/strings_boost
require daslib/utf8_utils
require daslib/fio
require peg/peg

options stack = 1000000000


struct Email
    username: string
    domain: string


def get_parser_with(input: string): emailParser
    parse email
        var email: Email

        rule("{alnums}" as username, "@", "{domain}" as domain) <|
            return [[Email username = username, domain = domain]]

        var alnums: void? // Just a pattern to be interpreted later

        rule(*set('0'..'9', 'a'..'z', 'A'..'Z')) <|
            return null

        var domain: void?

        rule(subdomain, *subdomain, tld) <|
            return null

        var tld: void?

        rule("com") <|
            return null
        rule("net") <|
            return null
        rule("org") <|
            return null

        var subdomain: void?

        rule( *set('0'..'9', 'a'..'z', 'A'..'Z', '-'), "." ) <|
            return null

    var parser <- [[emailParser]]

    for char in input
        parser.input |> push(char |> uint8())

    return <- parser


[test]
def test_email(t: T?)
    var input = "abc@mail.example.com"

    var parser <- get_parser_with(input)
    var res <- parser |> parse_email
    var email = res.value

    t |> equal("abc", email.username)
    t |> equal("mail.example.com", email.domain)

options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

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


def get_parser_with( email:string; blk: block<( email:Email; err:array<ParsingError> ):void> )
    parse email
        var email: Email

        rule("{alnums}" as username, "@", "{domain}" as domain) <|
            return [[Email username = username, domain = domain]]

        var alnums: void? // Just a pattern to be interpreted later

        rule(+set('0'..'9', 'a'..'z', 'A'..'Z', '-', '.', '_')) <|
            return null

        var domain: void?

        rule(+subdomain, tld) <|
            return null

        var tld: void?

        rule("com") <|
            return null
        rule("net") <|
            return null
        rule("org") <|
            return null

        var subdomain: void?

        rule( +set('0'..'9', 'a'..'z', 'A'..'Z', '-'), "." ) <|
            return null


[test]
def test_email_1(t: T?)
    var input = "abc@mail.example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("abc", email.username)
        t |> equal("mail.example.com", email.domain)

[test]
def test_email_2(t: T?)
    var input = "john.doe@example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("john.doe", email.username)
        t |> equal("example.com", email.domain)

[test]
def test_email_3(t: T?)
    var input = "this_is_a_very_long_username@example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("this_is_a_very_long_username", email.username)
        t |> equal("example.com", email.domain)

[test]
def test_email_4(t: T?)
    var input = "12345@example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("12345", email.username)
        t |> equal("example.com", email.domain)

[test]
def test_email_5(t: T?)
    var input = "jane-doe@mail-example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("jane-doe", email.username)
        t |> equal("mail-example.com", email.domain)

[test]
def test_email_6(t: T?)
    var input = "user_name@example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("user_name", email.username)
        t |> equal("example.com", email.domain)

[test]
def test_email_7(t: T?)
    var input = "user@subdomain1.subdomain2.example.com"

    get_parser_with(input) <| $(email; err)
        t |> equal("user", email.username)
        t |> equal("subdomain1.subdomain2.example.com", email.domain)

// Test parsing failures

[test]
def test_email_error_1(t: T?)
    var input = "@"

    get_parser_with(input) <| $(email; err)
        if err |> empty
            t |> failure("Should fail")

[test]
def test_email_error_2(t: T?)
    var input = "john.gmail.com"

    get_parser_with(input) <| $(email; err)
        if err |> empty
            t |> failure("Should fail")

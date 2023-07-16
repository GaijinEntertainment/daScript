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


def get_parser_with(email: string)
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

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("abc", email.username)
    t |> equal("mail.example.com", email.domain)


[test]
def test_email_2(t: T?)
    var input = "john.doe@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("john.doe", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_3(t: T?)
    var input = "this_is_a_very_long_username@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("this_is_a_very_long_username", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_4(t: T?)
    var input = "12345@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("12345", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_5(t: T?)
    var input = "jane-doe@mail-example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("jane-doe", email.username)
    t |> equal("mail-example.com", email.domain)


[test]
def test_email_6(t: T?)
    var input = ".username@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal(".username", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_7(t: T?)
    var input = "user_name@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("user_name", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_8(t: T?)
    var input = "john.doe@example.net"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("john.doe", email.username)
    t |> equal("example.net", email.domain)


[test]
def test_email_9(t: T?)
    var input = "JOHN.DOE@EXAMPLE.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("JOHN.DOE", email.username)
    t |> equal("EXAMPLE.com", email.domain)


[test]
def test_email_10(t: T?)
    var input = "a@example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("a", email.username)
    t |> equal("example.com", email.domain)


[test]
def test_email_11(t: T?)
    var input = "user@subdomain1.subdomain2.example.com"

    var res <- get_parser_with(input)
    var email = res.value

    t |> equal("user", email.username)
    t |> equal("subdomain1.subdomain2.example.com", email.domain)

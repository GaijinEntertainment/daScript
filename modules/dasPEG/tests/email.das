options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require daslib/debug
options debugger

require dastest/testing_boost
require daslib/strings_boost
require peg/peg

options stack = 1000000000


struct Email {
    username : string
    domain : string
}


def get_parser_with(email : string; blk : block<(email : Email; err : array<ParsingError>) : void>) {
    parse(email) {
        var email : Email

        rule("{alnums}" as username, "@", "{domain}" as domain) {
            return Email(username = username, domain = domain)
        }

        var alnums : void?// Just a pattern to be interpreted later

        rule(+set('0'..'9', 'a'..'z', 'A'..'Z', '-', '.', '_')) {
            return null
        }

        var domain : void?

        rule(+subdomain, tld) {
            return null
        }

        var tld : void?

        rule("com") {
            return null
        }
        rule("net") {
            return null
        }
        rule("org") {
            return null
        }

        var subdomain : void?

        rule(+set('0'..'9', 'a'..'z', 'A'..'Z', '-'), ".") {
            return null
        }
    }
}


[test(description = "The email grammar splits abc@mail.example.com into the user abc and the domain mail.example.com.")]
def test_email_1(t : T?) {
    let input = "abc@mail.example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("abc", email.username)
        t |> equal("mail.example.com", email.domain)
    }
}

[test(description = "The email grammar keeps a dot inside the user name: john.doe@example.com.")]
def test_email_2(t : T?) {
    let input = "john.doe@example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("john.doe", email.username)
        t |> equal("example.com", email.domain)
    }
}

[test(description = "The email grammar reads a long user name made of underscored words.")]
def test_email_3(t : T?) {
    let input = "this_is_a_very_long_username@example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("this_is_a_very_long_username", email.username)
        t |> equal("example.com", email.domain)
    }
}

[test(description = "The email grammar reads an all-digit user name: 12345@example.com.")]
def test_email_4(t : T?) {
    let input = "12345@example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("12345", email.username)
        t |> equal("example.com", email.domain)
    }
}

[test(description = "The email grammar keeps hyphens in both the user name and the domain: jane-doe@mail-example.com.")]
def test_email_5(t : T?) {
    let input = "jane-doe@mail-example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("jane-doe", email.username)
        t |> equal("mail-example.com", email.domain)
    }
}

[test(description = "The email grammar keeps an underscore inside the user name: user_name@example.com.")]
def test_email_6(t : T?) {
    let input = "user_name@example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("user_name", email.username)
        t |> equal("example.com", email.domain)
    }
}

[test(description = "The email grammar reads a domain with several subdomains: user@subdomain1.subdomain2.example.com.")]
def test_email_7(t : T?) {
    let input = "user@subdomain1.subdomain2.example.com"

    get_parser_with(input) $(email; _err) {
        t |> equal("user", email.username)
        t |> equal("subdomain1.subdomain2.example.com", email.domain)
    }
}

// Test parsing failures

[test(description = "The email grammar reports a parse error for a bare @ with no user or domain.")]
def test_email_error_1(t : T?) {
    let input = "@"

    get_parser_with(input) $(_email; err) {
        if (err |> empty) {
            t |> failure("Should fail")
        }
    }
}

[test(description = "The email grammar reports a parse error for an address with no @: john.gmail.com.")]
def test_email_error_2(t : T?) {
    let input = "john.gmail.com"

    get_parser_with(input) $(_email; err) {
        if (err |> empty) {
            t |> failure("Should fail")
        }
    }
}

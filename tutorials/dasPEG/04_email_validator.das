// Tutorial PEG-04: Email Validator
//
// This tutorial covers:
//   - not_set() for inverted character classes
//   - Composing small pattern rules into larger ones
//   - Struct return types from parsers
//   - Multiple character set arguments to set()
//   - Validation vs extraction patterns
//
// Prerequisites: Tutorial PEG-03
//
// Run: daslang.exe tutorials/dasPEG/04_email_validator.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require peg/peg


// ──────────────────────────────────────────────────────────────────────────
// Email structure
// ──────────────────────────────────────────────────────────────────────────

struct Email {
    user : string
    domain : string
}


// ──────────────────────────────────────────────────────────────────────────
// The Email Grammar
// ──────────────────────────────────────────────────────────────────────────
//
// Simplified email format: user@domain.tld
// user: letters, digits, dots, hyphens, underscores
// domain: letters, digits, hyphens, separated by dots

def parse_email(input : string; blk : block<(val : Email; err : array<ParsingError>) : void>) {
    parse(input) {
        var email : Email
        rule("{+user_char}" as user, "@", "{+domain_char}" as domain, EOF) {
            return Email(user = user, domain = domain)
        }

        // User part: letters, digits, and a few special chars
        var user_char : void?
        rule(set('a'..'z', 'A'..'Z', '0'..'9', '.', '-', '_', '+')) {
            return null
        }

        // Domain part: letters, digits, hyphens, dots
        var domain_char : void?
        rule(set('a'..'z', 'A'..'Z', '0'..'9', '-', '.')) {
            return null
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Testing the parser
// ──────────────────────────────────────────────────────────────────────────

def check_email(input : string) {
    parse_email(input) $(val; err) {
        if (val.user != "" && val.domain != "") {
            print("  {input} -> user=\"{val.user}\", domain=\"{val.domain}\"\n")
        } else {
            print("  {input} -> INVALID\n")
        }
    }
}


[export]
def main {
    print("=== Valid emails ===\n")
    check_email("alice@example.com")
    check_email("bob.smith@company.co.uk")
    check_email("user+tag@gmail.com")
    check_email("test-email@sub.domain.org")

    print("\n=== Invalid emails ===\n")
    check_email("no-at-sign")
    check_email("@missing-user.com")
    check_email("spaces not@allowed.com")
}

// Expected output:
// === Valid emails ===
//   alice@example.com -> user="alice", domain="example.com"
//   bob.smith@company.co.uk -> user="bob.smith", domain="company.co.uk"
//   user+tag@gmail.com -> user="user+tag", domain="gmail.com"
//   test-email@sub.domain.org -> user="test-email", domain="sub.domain.org"
//
// === Invalid emails ===
//   no-at-sign -> INVALID
//   @missing-user.com -> INVALID
//   spaces not@allowed.com -> INVALID

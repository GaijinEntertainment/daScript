options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// STYLE035: uint variables compared with uint-cast character literals should
// be int variables compared directly with character literals.
//
// Bad:
//   def is_open(b : uint) : bool => b == uint('(')
//
// Good:
//   def is_open(b : int) : bool => b == '('
//
// The fix belongs at the variable declaration even for one comparison. A chain
// on one variable therefore emits one warning, not one warning per character.

expect 31209:4

require daslib/style_lint

// --- Bad patterns ---

def bad_single(b : uint) : bool {
    return b == uint('(')                    // STYLE035 -> make b int
}

def bad_chain(b : uint) : bool {
    return (b == uint('*') || b == uint('_') || b == uint('~')
        || b == uint('`') || b == uint('[') || b == uint(']')
        || b == uint('(') || b == uint(')') || b == uint('!'))
                                                // one STYLE035 for b
}

def bad_relational(b : uint) : bool {
    return b >= uint('a') && b <= uint('z')  // one STYLE035 for b
}

def bad_yoda_not_equal(b : uint) : bool {
    return uint('\n') != b                   // STYLE035, const-on-left
}

// --- Good patterns (no warnings) ---

def good_int(b : int) : bool {
    return b == '(' || b == ')'
}

def good_local_conversion(b : uint) : bool {
    let c = int(b)
    return c == '('
}

def good_numeric_uint(b : uint) : bool {
    return b == uint(40)                     // not a character literal
}

def good_non_variable(values : array<uint>) : bool {
    return values[0] == uint('(')             // changing a field/index type is out of scope
}

def good_uint8(b : uint8) : bool {
    return b == uint('(')                     // STYLE035 targets plain uint only
}

def good_cast_argument() : uint {
    return uint('(')                          // not a comparison
}

def suppressed(b : uint) : bool {             // nolint:STYLE035
    return b == uint('(')
}

// Tutorial PEG-02: Arithmetic Calculator
//
// This tutorial covers:
//   - Operator precedence via grammar layering
//   - The `number` built-in terminal
//   - PEEK for lookahead
//   - commit (cut operator) for error reporting
//   - WS for whitespace handling
//   - Recursive rules (parenthesized expressions)
//
// Prerequisites: Tutorial PEG-01
//
// Run: daslang.exe tutorials/dasPEG/02_calculator.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require peg/peg


// ──────────────────────────────────────────────────────────────────────────
// The Calculator Grammar
// ──────────────────────────────────────────────────────────────────────────
//
// Operator precedence is encoded in the grammar structure:
//   expr  ->  add       (lowest precedence: + and -)
//   add   ->  mul       (middle: * and /)
//   mul   ->  unary     (high: unary -)
//   unary ->  prim      (highest: numbers and parentheses)
//
// Each level tries its operator, then falls through to the next level.
// This is the classic PEG approach to precedence.

def calc(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
    parse(input) {
        // Entry rule: parse the full expression, expect EOF
        var expr : int
        rule(add as a, WS, EOF) {
            return a
        }

        // Addition and subtraction (left-associative via left-recursion)
        var add : int
        rule(add as a, WS, "+", commit, WS, mul as m) {
            return a + m
        }
        rule(add as a, WS, "-", commit, WS, mul as m) {
            return a - m
        }
        rule(mul as m) {
            return m
        }

        // Multiplication and division
        var mul : int
        rule(mul as m, WS, "*", commit, WS, unary as u) {
            return m * u
        }
        rule(mul as m, WS, "/", commit, WS, unary as u) {
            return m / u
        }
        rule(unary as u) {
            return u
        }

        // Unary negation
        var unary : int
        rule("-", commit, WS, unary as u) {
            return -u
        }
        rule(prim as p) {
            return p
        }

        // Primary: parenthesized expression or number
        var prim : int
        rule("(", commit, WS, add as a, WS, ")") {
            return a
        }
        rule(PEEK(set('0'..'9')), commit, number as n) {
            return n
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Evaluating expressions
// ──────────────────────────────────────────────────────────────────────────

def eval_and_print(input : string) {
    calc(input) $(val; err) {
        if (empty(err)) {
            print("  {input} = {val}\n")
        } else {
            print("  {input} -> ERROR: {err[0].text}\n")
        }
    }
}


[export]
def main {
    print("=== Basic arithmetic ===\n")
    eval_and_print("2 + 3")
    eval_and_print("10 - 4")
    eval_and_print("6 * 7")
    eval_and_print("20 / 4")

    print("\n=== Precedence ===\n")
    eval_and_print("2 + 3 * 4")       // 14, not 20
    eval_and_print("(2 + 3) * 4")     // 20
    eval_and_print("10 - 2 * 3")      // 4

    print("\n=== Unary minus ===\n")
    eval_and_print("-5")
    eval_and_print("-5 + 3")
    eval_and_print("-(2 + 3)")

    print("\n=== Complex expressions ===\n")
    eval_and_print("1 + 2 * 2 + 1 - (3 * 2 + 1 * 2) - 1 - 1 - 1 * 2")
    eval_and_print("((((1 + 2))))")

    print("\n=== Error cases ===\n")
    eval_and_print("2 +")
    eval_and_print("(2 + 3")
    eval_and_print("* 3")
}

// Expected output:
// === Basic arithmetic ===
//   2 + 3 = 5
//   10 - 4 = 6
//   6 * 7 = 42
//   20 / 4 = 5
//
// === Precedence ===
//   2 + 3 * 4 = 14
//   (2 + 3) * 4 = 20
//   10 - 2 * 3 = 4
//
// === Unary minus ===
//   -5 = -5
//   -5 + 3 = -2
//   -(2 + 3) = -5
//
// === Complex expressions ===
//   1 + 2 * 2 + 1 - (3 * 2 + 1 * 2) - 1 - 1 - 1 * 2 = -6
//   ((((1 + 2)))) = 3
//
// === Error cases ===
//   (error messages for invalid expressions)

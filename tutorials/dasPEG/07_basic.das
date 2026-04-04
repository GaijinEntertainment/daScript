// Tutorial PEG-07: C64 BASIC Interpreter
//
// This tutorial covers:
//   - Building a complete parser -> interpreter pipeline
//   - A grammar with 15+ rules for statement parsing
//   - Runtime expression evaluation using PEG (parsed on every execution)
//   - Keyword disambiguation with negative lookahead
//   - Line-number-based program storage with GOTO/GOSUB
//   - FOR/NEXT loops, IF/THEN branching
//
// The interpreter stores program lines as parsed statements with expression
// text preserved as strings.  Expressions are re-parsed at runtime using PEG
// with the current variable table — this is how classic BASIC works.
//
// Prerequisites: Tutorials PEG-01 through PEG-06
//
// Run: daslang.exe tutorials/dasPEG/07_basic.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 10000000

require peg/peg
require daslib/strings_boost
require math


// ══════════════════════════════════════════════════════════════════════════
// Expression evaluator — a small PEG parser used at runtime
// ══════════════════════════════════════════════════════════════════════════
//
// Expressions are stored as strings and evaluated each time they execute.
// The variable table is captured via closure.

var eval_vars : table<string; double>

def eval_expr_parse(input : string;
                    blk : block<(val : double; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : double
        rule(comparison as c, WS, EOF) {
            return c
        }

        var comparison : double
        rule(arith as a, WS, "<=", WS, arith as b) {
            return a <= b ? 1.0lf : 0.0lf
        }
        rule(arith as a, WS, ">=", WS, arith as b) {
            return a >= b ? 1.0lf : 0.0lf
        }
        rule(arith as a, WS, "<>", WS, arith as b) {
            return a != b ? 1.0lf : 0.0lf
        }
        rule(arith as a, WS, "=", WS, arith as b) {
            return a == b ? 1.0lf : 0.0lf
        }
        rule(arith as a, WS, "<", WS, arith as b) {
            return a < b ? 1.0lf : 0.0lf
        }
        rule(arith as a, WS, ">", WS, arith as b) {
            return a > b ? 1.0lf : 0.0lf
        }
        rule(arith as a) {
            return a
        }

        var arith : double
        rule(arith as a, WS, "+", WS, term as b) {
            return a + b
        }
        rule(arith as a, WS, "-", WS, term as b) {
            return a - b
        }
        rule(term as t) {
            return t
        }

        var term : double
        rule(term as t, WS, "*", WS, unary as u) {
            return t * u
        }
        rule(term as t, WS, "/", WS, unary as u) {
            return u != 0.0lf ? t / u : 0.0lf
        }
        rule(unary as u) {
            return u
        }

        var unary : double
        rule("-", unary as u) {
            return -u
        }
        rule(primary as p) {
            return p
        }

        var primary : double
        rule("(", WS, comparison as c, WS, ")") {
            return c
        }
        rule(PEEK(set('0'..'9')), double_ as d) {
            return d
        }
        rule(!"TO", !"STEP", !"THEN", "{+ident_ch}" as name) {
            return eval_vars?[name] ?? 0.0lf
        }

        var ident_ch : void?
        rule(set('A'..'Z', 'a'..'z', '0'..'9', '_')) {
            return null
        }
    }
}

def eval_expr(input : string; vars : table<string; double>) : double {
    eval_vars := vars
    var result = 0.0lf
    eval_expr_parse(input) $(val; err) {
        result = val
    }
    return result
}

def eval_string_parse(input : string;
                      blk : block<(val : string; err : array<ParsingError>) : void>) {
    parse(input) {
        var expr : string
        rule(WS, string_ as s, WS, EOF) {
            return s
        }
        rule(WS, "{+any_ch}" as text, WS, EOF) {
            return text
        }
        var any_ch : void?
        rule(any) {
            return null
        }
    }
}

def eval_string(input : string) : string {
    var result = ""
    eval_string_parse(input) $(val; err) {
        result = val
    }
    return result
}


// ══════════════════════════════════════════════════════════════════════════
// Statement types — simple, fully copyable
// ══════════════════════════════════════════════════════════════════════════

variant Stmt {
    print_expr : string         // PRINT <expr-text>
    print_str : string          // PRINT "literal"
    print_semi : string         // PRINT <expr>;  (no newline)
    print_nl : bool             // bare PRINT
    let_stmt : tuple<string; string>    // var = <expr-text>
    if_goto : tuple<string; int>        // IF <cond-text> THEN linenum
    goto_stmt : int
    gosub_stmt : int
    return_stmt : bool
    for_stmt : tuple<string; string; string; string>  // var, from-text, to-text, step-text
    next_stmt : string
    rem_stmt : string
    end_stmt : bool
    nop : bool
}

struct Line {
    number : int
    stmt : Stmt
}


// ══════════════════════════════════════════════════════════════════════════
// Line parser — parses statement structure, preserves expression text
// ══════════════════════════════════════════════════════════════════════════

def parse_line(input : string; blk : block<(val : Line; err : array<ParsingError>) : void>) {
    parse(input) {
        var basic_line : Line

        rule(WS, number as n, WS, statement as s, EOF) {
            return Line(number = n, stmt = s)
        }
        rule(WS, statement as s, EOF) {
            return Line(number = -1, stmt = s)
        }

        var statement : Stmt

        // PRINT variants: string literal, expression with ;, expression, bare
        rule("PRINT", WS, string_ as s, WS, ";") {
            return Stmt(print_semi = "\"{s}\"")
        }
        rule("PRINT", WS, string_ as s) {
            return Stmt(print_str = s)
        }
        rule("PRINT", WS, "{+expr_text}" as e, WS, ";") {
            return Stmt(print_semi = e)
        }
        rule("PRINT", WS, "{+expr_text}" as e) {
            return Stmt(print_expr = e)
        }
        rule("PRINT") {
            return Stmt(print_nl = true)
        }

        // LET / assignment
        rule("LET", WS, "{+ident_ch}" as name, WS, "=", WS, "{+expr_text}" as e) {
            return Stmt(let_stmt = (name, e))
        }
        rule("{+ident_ch}" as name, WS, "=", WS, "{+expr_text}" as e) {
            return Stmt(let_stmt = (name, e))
        }

        // Control flow
        rule("IF", WS, "{+until_then}" as cond, WS, "THEN", WS, number as target) {
            return Stmt(if_goto = (cond, target))
        }
        rule("GOTO", WS, number as n) {
            return Stmt(goto_stmt = n)
        }
        rule("GOSUB", WS, number as n) {
            return Stmt(gosub_stmt = n)
        }
        rule("RETURN") {
            return Stmt(return_stmt = true)
        }

        // FOR/NEXT
        rule("FOR", WS, "{+ident_ch}" as name, WS, "=", WS,
                "{+until_to}" as from_e, WS, "TO", WS,
                "{+until_step}" as to_e, WS, "STEP", WS, "{+expr_text}" as step_e) {
            return Stmt(for_stmt = (name, from_e, to_e, step_e))
        }
        rule("FOR", WS, "{+ident_ch}" as name, WS, "=", WS,
                "{+until_to}" as from_e, WS, "TO", WS, "{+expr_text}" as to_e) {
            return Stmt(for_stmt = (name, from_e, to_e, "1"))
        }
        rule("NEXT", WS, "{+ident_ch}" as name) {
            return Stmt(next_stmt = name)
        }

        // Other
        rule("REM", "{*any_ch}" as text) {
            return Stmt(rem_stmt = text)
        }
        rule("END") {
            return Stmt(end_stmt = true)
        }

        // Text extraction helpers — capture expression text without parsing it
        var expr_text : void?
        rule(not_set('\n', '\r', ';')) {
            return null
        }

        var until_then : void?
        rule(!"THEN", any) {
            return null
        }

        var until_to : void?
        rule(!"TO", any) {
            return null
        }

        var until_step : void?
        rule(!"STEP", not_set('\n', '\r')) {
            return null
        }

        var ident_ch : void?
        rule(set('A'..'Z', 'a'..'z', '0'..'9', '_')) {
            return null
        }

        var any_ch : void?
        rule(any) {
            return null
        }
    }
}


// ══════════════════════════════════════════════════════════════════════════
// Interpreter
// ══════════════════════════════════════════════════════════════════════════

struct ForState {
    var_name : string
    limit : double
    step : double
    return_pc : int
}

struct BasicState {
    program : array<Line>
    vars : table<string; double>
    gosub_stack : array<int>
    for_stack : array<ForState>
    running : bool
}

def find_line(state : BasicState; target : int) : int {
    let idx = lower_bound(state.program, target) <| $(a : Line; b : int) {
        return a.number < b
    }
    return idx < length(state.program) && state.program[idx].number == target ? idx : -1
}

def store_line(var state : BasicState; var line : Line) {
    let idx = lower_bound(state.program, line.number) <| $(a : Line; b : int) {
        return a.number < b
    }
    if (idx < length(state.program) && state.program[idx].number == line.number) {
        state.program[idx] = line
    } else {
        state.program |> emplace(line, idx)
    }
}

def print_num(n : double) {
    if (n == double(int(n))) {
        print("{int(n)}")
    } else {
        print("{n}")
    }
}

def exec_stmt(var state : BasicState; stmt : Stmt; var pc : int&) : bool {
    if (stmt is print_str) {
        print("{stmt as print_str}\n")
    } elif (stmt is print_expr) {
        print_num(eval_expr(strip(stmt as print_expr), state.vars))
        print("\n")
    } elif (stmt is print_semi) {
        let text = strip(stmt as print_semi)
        // Check if it's a string literal
        if (length(text) >= 2 && starts_with(text, "\"")) {
            print(eval_string(text))
        } else {
            print_num(eval_expr(text, state.vars))
        }
    } elif (stmt is print_nl) {
        print("\n")
    } elif (stmt is let_stmt) {
        let pair = stmt as let_stmt
        state.vars[pair._0] = eval_expr(strip(pair._1), state.vars)
    } elif (stmt is if_goto) {
        let pair = stmt as if_goto
        if (eval_expr(strip(pair._0), state.vars) != 0.0lf) {
            pc = find_line(state, pair._1)
            if (pc < 0) {
                print("?UNDEF'D STATEMENT ERROR\n")
                state.running = false
            }
            return true
        }
    } elif (stmt is goto_stmt) {
        pc = find_line(state, stmt as goto_stmt)
        if (pc < 0) {
            print("?UNDEF'D STATEMENT ERROR\n")
            state.running = false
        }
        return true
    } elif (stmt is gosub_stmt) {
        state.gosub_stack |> push(pc)
        pc = find_line(state, stmt as gosub_stmt)
        if (pc < 0) {
            print("?UNDEF'D STATEMENT ERROR\n")
            state.running = false
        }
        return true
    } elif (stmt is return_stmt) {
        if (length(state.gosub_stack) == 0) {
            print("?RETURN WITHOUT GOSUB ERROR\n")
            state.running = false
        } else {
            pc = state.gosub_stack[length(state.gosub_stack) - 1]
            state.gosub_stack |> pop()
            return true
        }
    } elif (stmt is for_stmt) {
        let f = stmt as for_stmt
        state.vars[f._0] = eval_expr(strip(f._1), state.vars)
        let limit = eval_expr(strip(f._2), state.vars)
        let step = eval_expr(strip(f._3), state.vars)
        state.for_stack |> push(ForState(var_name = f._0, limit = limit, step = step, return_pc = pc))
    } elif (stmt is next_stmt) {
        let name = stmt as next_stmt
        if (length(state.for_stack) == 0) {
            print("?NEXT WITHOUT FOR ERROR\n")
            state.running = false
            return false
        }
        let fs = state.for_stack[length(state.for_stack) - 1]
        let cur = state.vars?[name] ?? 0.0lf
        let next_val = cur + fs.step
        state.vars[name] = next_val
        if (fs.step > 0.0lf ? next_val > fs.limit : next_val < fs.limit) {
            state.for_stack |> pop()
        } else {
            pc = fs.return_pc
            return true
        }
    } elif (stmt is end_stmt) {
        state.running = false
    }
    return false
}

def run_program(var state : BasicState) {
    state.running = true
    state.gosub_stack |> clear()
    state.for_stack |> clear()
    var pc = 0
    while (state.running && pc < length(state.program)) {
        let stmt = state.program[pc].stmt
        pc++
        if (exec_stmt(state, stmt, pc)) {
            continue
        }
    }
}

def list_program(state : BasicState) {
    for (line in state.program) {
        print("{line.number} ")
        if (line.stmt is print_str) {
            print("PRINT \"{line.stmt as print_str}\"")
        } elif (line.stmt is print_expr) {
            print("PRINT {line.stmt as print_expr}")
        } elif (line.stmt is print_semi) {
            print("PRINT {line.stmt as print_semi};")
        } elif (line.stmt is print_nl) {
            print("PRINT")
        } elif (line.stmt is let_stmt) {
            let p = line.stmt as let_stmt
            print("LET {p._0} = {strip(p._1)}")
        } elif (line.stmt is if_goto) {
            let p = line.stmt as if_goto
            print("IF {strip(p._0)} THEN {p._1}")
        } elif (line.stmt is goto_stmt) {
            print("GOTO {line.stmt as goto_stmt}")
        } elif (line.stmt is gosub_stmt) {
            print("GOSUB {line.stmt as gosub_stmt}")
        } elif (line.stmt is return_stmt) {
            print("RETURN")
        } elif (line.stmt is for_stmt) {
            let f = line.stmt as for_stmt
            print("FOR {f._0} = {strip(f._1)} TO {strip(f._2)}")
            if (strip(f._3) != "1") {
                print(" STEP {strip(f._3)}")
            }
        } elif (line.stmt is next_stmt) {
            print("NEXT {line.stmt as next_stmt}")
        } elif (line.stmt is rem_stmt) {
            print("REM{line.stmt as rem_stmt}")
        } elif (line.stmt is end_stmt) {
            print("END")
        }
        print("\n")
    }
}


// ══════════════════════════════════════════════════════════════════════════
// Command processor
// ══════════════════════════════════════════════════════════════════════════

def process(var state : BasicState; input : string) {
    if (input == "LIST") {
        list_program(state)
        return
    } elif (input == "RUN") {
        run_program(state)
        return
    } elif (input == "NEW") {
        state.program |> clear()
        state.vars |> clear()
        return
    }

    parse_line(input) $(line; err) {
        if (line.number > 0) {
            var mline = line
            store_line(state, mline)
        } elif (line.number == -1) {
            var pc = 0
            state.running = true
            exec_stmt(state, line.stmt, pc)
        } else {
            if (!empty(err)) {
                print("?SYNTAX ERROR\n")
            }
        }
    }
}


// ══════════════════════════════════════════════════════════════════════════
// Demo
// ══════════════════════════════════════════════════════════════════════════

[export]
def main {
    var state : BasicState

    print("**** DASLANG BASIC V1 ****\n\n")

    // Program 1: Hello World + FOR loop
    process(state, "10 PRINT \"HELLO WORLD\"")
    process(state, "20 FOR I = 1 TO 5")
    process(state, "30 PRINT I;")
    process(state, "35 PRINT \" \";")
    process(state, "40 NEXT I")
    process(state, "50 PRINT")
    process(state, "60 END")

    print("LIST\n")
    process(state, "LIST")
    print("\nRUN\n")
    process(state, "RUN")

    // Program 2: Countdown with IF/GOTO
    process(state, "NEW")
    print("\n")
    process(state, "10 I = 5")
    process(state, "20 PRINT I")
    process(state, "30 I = I - 1")
    process(state, "40 IF I >= 0 THEN 20")
    process(state, "50 PRINT \"LIFTOFF!\"")

    print("RUN (Countdown)\n")
    process(state, "RUN")

    // Program 3: GOSUB/RETURN
    process(state, "NEW")
    print("\n")
    process(state, "10 X = 5")
    process(state, "20 GOSUB 100")
    process(state, "30 X = 10")
    process(state, "40 GOSUB 100")
    process(state, "50 END")
    process(state, "100 REM Print X * X")
    process(state, "110 PRINT X * X")
    process(state, "120 RETURN")

    print("RUN (GOSUB - X squared)\n")
    process(state, "RUN")

    // Program 4: Fibonacci
    process(state, "NEW")
    print("\n")
    process(state, "10 A = 0")
    process(state, "20 B = 1")
    process(state, "30 FOR I = 1 TO 10")
    process(state, "40 PRINT A;")
    process(state, "50 PRINT \" \";")
    process(state, "60 C = A + B")
    process(state, "70 A = B")
    process(state, "80 B = C")
    process(state, "90 NEXT I")
    process(state, "100 PRINT")

    print("RUN (Fibonacci)\n")
    process(state, "RUN")
}

// Expected output:
// **** DASLANG BASIC V1 ****
//
// LIST
// 10 PRINT "HELLO WORLD"
// 20 FOR I = 1 TO 5
// 30 PRINT I;
// 35 PRINT " ";
// 40 NEXT I
// 50 PRINT
// 60 END
//
// RUN
// HELLO WORLD
// 1 2 3 4 5
//
// RUN (Countdown)
// 5
// 4
// 3
// 2
// 1
// 0
// LIFTOFF!
//
// RUN (GOSUB - X squared)
// 25
// 100
//
// RUN (Fibonacci)
// 0 1 1 2 3 5 8 13 21 34

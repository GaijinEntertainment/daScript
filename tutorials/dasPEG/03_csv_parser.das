// Tutorial PEG-03: CSV Parser
//
// This tutorial covers:
//   - *rule (zero-or-more) repetition with array binding
//   - +rule (one-or-more) repetition
//   - !rule (negative lookahead)
//   - any terminal (matches one character)
//   - EOL terminal (end of line)
//   - TS (tab-or-space, no newlines)
//   - void? pattern rules (match without producing a value)
//   - string_ and double_ built-in terminals
//   - Comma-separated list idiom
//
// Prerequisites: Tutorial PEG-02
//
// Run: daslang.exe tutorials/dasPEG/03_csv_parser.das

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options stack = 1000000

require peg/peg
require daslib/stringify


// ──────────────────────────────────────────────────────────────────────────
// Data types for CSV
// ──────────────────────────────────────────────────────────────────────────

variant Cell {
    text : string
    number : double
}

typedef Row = array<Cell>


// ──────────────────────────────────────────────────────────────────────────
// The CSV Grammar
// ──────────────────────────────────────────────────────────────────────────
//
// Key patterns demonstrated:
// - *rule as name:  collects zero-or-more matches into an array
// - The "trailing separator" idiom:  *comma_cell, last_cell
//   This parses "a, b, c" as [a, b] (comma-terminated) + c (last, no comma)

def parse_csv(input : string; blk : block<(val : array<Row>; err : array<ParsingError>) : void>) {
    parse(input) {
        // Entry: rows separated by newlines
        var csv : array<Row>
        rule(*newline_row as rows, last_row as last, MB(trailing_eol), EOF) {
            rows |> emplace(last)
            return <- rows
        }
        rule(EOF) {
            var empty_rows : array<Row>
            return <- empty_rows
        }

        // Row ending with newline
        var newline_row : Row
        rule(row as r, EOL) {
            return <- r
        }

        // Last row (may not end with newline)
        var last_row : Row
        rule(row as r) {
            return <- r
        }

        // A row is: zero or more comma-terminated cells, plus one final cell
        var row : Row
        rule(TS, *comma_cell as cells, cell as last) {
            cells |> emplace(last)
            return <- cells
        }

        // A cell followed by comma
        var comma_cell : Cell
        rule(cell as c, TS, ",", TS) {
            return c
        }

        // A cell is either a quoted string or a number
        var cell : Cell
        rule(string_ as text, TS) {
            return Cell(text = text)
        }
        rule(double_ as value, TS) {
            return Cell(number = value)
        }

        // Optional trailing newline
        var trailing_eol : bool
        rule(EOL) {
            return true
        }
    }
}


// ──────────────────────────────────────────────────────────────────────────
// Pretty-printing
// ──────────────────────────────────────────────────────────────────────────

def print_cell(c : Cell) {
    if (c is text) {
        print("\"{c as text}\"")
    } else {
        print("{c as number}")
    }
}

def print_table(rows : array<Row>) {
    for (row, i in rows, range(length(rows))) {
        print("  Row {i}: ")
        for (cell, j in row, range(length(row))) {
            if (j > 0) {
                print(", ")
            }
            print_cell(cell)
        }
        print("\n")
    }
}


[export]
def main {
    // Single row
    print("=== Single row ===\n")
    let csv1 = %stringify~1,"hello",3.14%%
    print("  Input: {csv1}\n")
    parse_csv(csv1) $(val; err) {
        print_table(val)
    }

    // Multiple rows
    print("\n=== Multiple rows ===\n")
    let csv2 = %stringify~1,"Alice",95.5
2,"Bob",87.0
3,"Charlie",92.3%%
    print("  Input:\n    {csv2}\n")
    parse_csv(csv2) $(val; err) {
        print_table(val)
    }

    // Empty input
    print("\n=== Empty input ===\n")
    parse_csv("") $(val; err) {
        print("  Rows: {length(val)}\n")
    }
}

// Expected output:
// === Single row ===
//   Input: 1,"hello",3.14
//   Row 0: 1, "hello", 3.14
//
// === Multiple rows ===
//   Input:
//     1,"Alice",95.5\n2,"Bob",87.0\n3,"Charlie",92.3
//   Row 0: 1, "Alice", 95.5
//   Row 1: 2, "Bob", 87
//   Row 2: 3, "Charlie", 92.3
//
// === Empty input ===
//   Rows: 0

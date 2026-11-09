options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

options stack = 1000000000

require daslib/debug
options debugger

require dastest/testing_boost
require daslib/strings_boost
require daslib/stringify
require peg/peg


def get_parser_with(csv : string; blk : block<(email : array<string>; err : array<ParsingError>) : void>) { // nolint:STYLE038 - flat PEG grammar declaration; the rules are the CSV grammar, one per production
    parse(csv) {
        var Row : array<string>

        rule(*CommarSeparatedCell as cells, Cell as c, EOL) {
            cells |> push <| c
            return <- cells
        }

        var CommarSeparatedCell : string

        rule(Cell as c, ",") {
            return <- c
        }

        var Cell : string

        rule(PEEK("\""), commit, // Now we can not turn back
             QuotedCell as qc) {
            return <- qc;
        }
        rule(NonQuotedCell as nqc) {
            return <- nqc;
        }
        rule(PEEK(",")) {
            // allows empty cells
            return "";
        }






        var QuotedCell : string;

        rule("\"", "{EscapedChars}" as chars, "\"") {
            return chars;
        }

        var EscapedChars : void?;

        rule(*EscapedChar) {
            return null;
        }

        var EscapedChar : void?;

        rule("\"\"") {
            return null;
        }
        rule(!"\"", any) {
            return null;
        }




        var NonQuotedCell : string;

        rule("{CellContents}" as contents) {
            return contents;
        }

        var CellContents : void?;

        rule(*CellSymbol) {
            return null;
        }

        var CellSymbol : void?;

        // Cell symbol is any symbol that is not NotCellContents
        rule(!NotCellContents, any) {
            return null;
        }

        var NotCellContents : void?;

        rule(EOL) {
            return null;
        }
        rule(",") {
            return null;
        }


        var EOL : void?;

        rule("\r\n") {
            return null;
        }
        rule("\n") {
            return null;
        }
        rule(EOF) {
            return null;
        }
    }
}


[test(description = "The CSV grammar splits a plain newline-terminated row into its cells.")]
def test_basic_csv(t : T?) {
    let input = "John,Doe,30\n";
    var inscope expected_res <-  array<string>("John", "Doe", "30");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}


[test(description = "The CSV grammar reads quoted cells without their quotes, keeping the commas inside them as text.")]
def test_quoted_fields(t : T?) {
    let input = %stringify~"John Doe","123, Main Street, NY","123-456-7890"
    %%;
    var inscope expected_res <-  array<string>("John Doe", "123, Main Street, NY", "123-456-7890");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
            return ;
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}


[test(description = "The CSV grammar keeps spaces, dots and an at sign inside unquoted cells.")]
def test_special_chars(t : T?) {
    let input = "John Doe,john.doe@example.com\n";
    var inscope expected_res <-  array<string>("John Doe", "john.doe@example.com");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
            return ;
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}

[test(description = "The CSV grammar reads a quoted cell holding doubled quotes and commas, keeping the doubled quotes as written.")]
def test_escaped_quotes(t : T?) {
    let input = %stringify~1,"She said, ""Hello, world!"""
    %%;
    var inscope expected_res <-  array<string>("1", "She said, \"\"Hello, world!\"\"");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
            return ;
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}

[test(description = "The CSV grammar keeps the leading and trailing spaces of unquoted cells.")]
def test_leading_trailing_spaces(t : T?) {
    let input = " John Doe , 123 Main Street, NY , 123-456-7890 \n";
    var inscope expected_res <-  array<string>(" John Doe ", " 123 Main Street", " NY ", " 123-456-7890 ");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
            return ;
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}

[test(description = "The CSV grammar reads a row of only commas as that many empty cells.")]
def test_all_empty_fields(t : T?) {
    let input = ",,\n";
    var inscope expected_res <-  array<string>("", "", "");

    get_parser_with(input) $(email; errors) {
        if (!errors |> empty) {
            t |> failure("Parsing failed");
            return ;
        }

        for (a, b in expected_res, email) {
            t |> equal(a, b);
        }
    }
}


// Test errors

[test(description = "The CSV grammar reports a parse error for the row 1,True with no line ending.")]
def test_no_eol(t : T?) {
    let input = %stringify~1,True%%;

    get_parser_with(input) $(_email; errors) {
        if (errors |> empty) {
            t |> failure("Parsing did not fail");
            return ;
        }
    }
}

[test(description = "The CSV grammar reports a parse error for a quoted cell that is never closed.")]
def test_missing_quote(t : T?) {
    let input = %stringify~1,"True
    %%;

    get_parser_with(input) $(_email; errors) {
        if (errors |> empty) {
            t |> failure("Parsing did not fail");
            return;
        }
    }
}


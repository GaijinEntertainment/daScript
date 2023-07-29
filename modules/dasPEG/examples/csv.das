require daslib/stringify
require daslib/algorithm
require peg/peg

variant Cell
    text : string
    value : double

typedef Row = array<Cell>

struct Table
    rows  : array<Row>
    errors : array<ParsingError>

def parse_csv ( CSV:string; blk: block<( val:array<Row>; err:array<ParsingError>):void>) // The same as the first `parse` arg
    parse CSV
        // The parsing begins from the first rule
        var rows : array<Row>

        rule(*new_line_row as rows, MB(eol), EOF) <|
            return <- rows

        rule(*new_line_row as rows, row as tail, MB(eol), EOF) <|
            rows |> emplace(tail)
            return <- rows

        var cell : Cell
        rule(string_ as text, TS) <|
            return [[Cell text=text]]
        rule(double_ as value, TS) <|
            return [[Cell value=value]]

        var comma_separated_cell : Cell
        rule(cell as cell, ",", TS) <|
            return cell

        var row : Row
        rule(TS, *comma_separated_cell as row, cell as tail) <|
            row |> emplace(tail)
            return <- row

        var new_line_row : Row
        rule(row as new_line_row, EOL) <|
            return <- new_line_row

        var eol : bool
        rule("\r\n") <|
            return true
        rule("\n") <|
            return true

        // option(tracing)
        // option(color)
        // option(print_generated)


let s1 = %stringify~1,"hello",3.141%%
let s2 = %stringify~1,"hello",3.141,%%

let s3 = %stringify~1,"hello",3.141
2,"goodbye",6.789
%%

[export]
def main
    parse_csv(s1) <| $(s1; err)
        print("c1 = {s1}\n")

    parse_csv(s2) <| $(s2; err)
        print("c2 = {s2}\n")

        for e in err
            "{e}\n" |> print

    parse_csv(s3) <| $(c3; err)
        print("c3 = {c3}\n")

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

def parse_csv ( CSV:string ) // The same as the first `parse` arg
    parse CSV
        // The parsing begins from the first rule
        var rows : array<Row>
        rule(*new_line_row as rows, MB(row) as tail, MB(eol), EOF) <|
            for r in tail
                rows |> emplace(r)
            return <- rows

        var cell : Cell
        rule(string_ as text,TS) <|
            return [[Cell text=text]]
        rule(double_ as value,TS) <|
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
/*
    var c1 <- parse_csv(s1)
    print("c1 = {c1}\n")

    var c2 <- parse_csv(s2)
    print("c2 = {c2}\n")
*/
    var c3 <- parse_csv(s3)
    print("c3 = {c3}\n")

/*
    print("s3 = {escape(s3)}\n")
    peek_data(s3) <| $ ( inp )
        print("inp = {inp}\n")
        print("length(inp) = {length(inp)}\n")
*/

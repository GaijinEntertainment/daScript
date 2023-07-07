options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require peg/peg

[export]
def main
    var parser <- [[calcParser]]
    parser.input = "1"
    parse calc
        var add: int
        rule("1") <|
            return 1
    var res <- parser |> parse_add
    print("Assert {res.value} == 1\n")

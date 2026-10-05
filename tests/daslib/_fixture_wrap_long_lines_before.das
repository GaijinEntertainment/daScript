options gen2
require math

def target(var first : array<int>; second : array<int>; third : int) : array<int> => <- first

def combine_three(first, second, third : int) : int => first + second + third

def apply(value : int; blk : block<(x : int) : int>) : int => blk(value)

def break_after_arrow(first_argument, second_argument : int; third_argument : float) : float => float(first_argument * second_argument)

def break_after_arrow_then_comma(var first_input : array<int>; second_input : array<int>; count : int) : array<int> => <- target(first_input, second_input, count + count * count + count * count * count + 1)

def break_after_semicolon(tab : table<string; array<int>>; key_with_long_name : string; another_long_argument_name : int) : int {
    return another_long_argument_name + length(key_with_long_name) + length(tab)
}

def break_after_comma(first_argument, second_argument : int) : int {
    return combine_three(first_argument * 1000, second_argument * 2000, first_argument + second_argument + 3000)
}

def break_before_or(first_value, second_value : int; third_value, fourth_value : bool) : bool {
    if (first_value > second_value && third_value || second_value > first_value && fourth_value || first_value == 0) {
        return true
    }
    return false
}

def keep_block_body(value : int) : int {
    return apply(value, $(x : int) : int { return x * value + value * value - value + x * x * x - value * 2 + x * 7 })
}

def keep_short_groups(first_value, second_value : int) : int {
    return max(first_value, second_value) + min(first_value, second_value) + max(second_value, 1) + max(first_value, 2)
}

def keep_commented_line(value : int) : int {
    return clamp(value, 0, 100) + clamp(value * 2, 0, 100) + clamp(value * 3, 0, 100) // a comment keeps the line
}

def keep_string() : string {
    return "a long string with, commas; and (parens) that must never be broken at all, by anything, ever"
}

def keep_table_literal() : table<string; int> {
    return <- { "first key" => 1, "second key" => 2, "third key" => 3, "fourth key" => 4, "fifth key" => 5 }
}

def continuation_keeps_its_indent(value : int) : int {
    let total = combine_three(value,
        value * 1000 + value * 2000 + value * 3000 + value * 4000 + value * 5000, value * 6000 + value * 7000)
    return total
}

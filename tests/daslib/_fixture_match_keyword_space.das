options gen2
require daslib/match

def value_match(x : int) : string {
    return match (x) {
        0 => "zero"
        _ => "other"
    }
}

def operand_match(x : int) : string {
    return "<" + match (x) {
        0 => "zero"
        _ => "other"
    } + ">"
}

def generic_match(what) : string {
    static_match (what) {
        match_type(type<int>, n) => $ { return "int {n}" }
        _ => $ { return "other" }
    }
    return ""
}

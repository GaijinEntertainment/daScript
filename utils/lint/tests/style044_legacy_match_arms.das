options gen2

expect 31209:2

require daslib/style_lint
require daslib/match

def bad_match(x : int) : string {
    match (x) {                                         // STYLE044
        if (0) {
            return "zero"
        }
        if (_) {
            return "other"
        }
    }
    return ""
}

def bad_static_match(what) : string {
    static_match (what) {                               // STYLE044
        // a comment before the first arm
        if (match_type(type<int>, $v(n))) {
            return "int {n}"
        }
        if (_) {
            return "other"
        }
    }
    return ""
}

def good_match(x : int) : string {
    return match (x) {
        0 => "zero"
        _ => "other"
    }
}

def good_if_after_block(x : int) : string {
    for (i in range(x)) {
        if (i == 3) {
            return "three"
        }
    }
    return ""
}

[export]
def main {
    print("{bad_match(0)}{bad_static_match(1)}{good_match(0)}{good_if_after_block(4)}\n")
}

options gen2

expect 31209:4

require daslib/style_lint
require daslib/match

def note(var log : array<string>; s : string) {
    log |> push(s)
}

def bad_call_arms(x : int; var log : array<string>) {
    match (x) {
        0 => $ {
            note(log, "zero")
        }
        1 | 2 => $ {
            log |> push("small")
        }
        _ => $ {
            note(log, "other {x}")
        }
    }
}

def bad_one_line_arm(x : int; var log : array<string>) {
    match (x) {
        0 => $ { note(log, "zero") }
        _ => note(log, "other")
    }
}

def good_one_line_move_and_empty(x : int; var log : array<string>) {
    var moved : array<string>
    match (x) {
        0 => $ { moved <- log }
        _ => $ {}
    }
}

def good_return_arm(x : int) : string {
    match (x) {
        0 => $ {
            return "zero"
        }
    }
    return "other"
}

def good_assignment_arm(x : int) : string {
    var res = ""
    match (x) {
        0 => $ {
            res = "zero"
        }
        _ => $ {
            res := "other"
        }
    }
    return res
}

def good_two_statements(x : int; var log : array<string>) {
    match (x) {
        0 => $ {
            note(log, "zero")
            note(log, "again")
        }
        _ => note(log, "other")
    }
}

def good_commented_and_guarded(x : int; var log : array<string>) {
    match (x) {
        0 => $ {
            note(log, "zero")   // the comment would be lost on one line
        }
        _ => $ {
            note(log, "other") if (x > 10)
        }
    }
}

def good_too_long_for_one_line(x : int; var log : array<string>) {
    match (x) {
        0 => $ {
            note(log, "a sample message whose one-line arm form lands one column past the limit!!!")
        }
        _ => note(log, "other")
    }
}

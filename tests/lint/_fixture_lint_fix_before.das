//fmt:ignore-file
options gen2
options auto_inline_functions = false

class Names {
    names : table<string>

    def contains(k : string) : bool {
        return names |> key_exists(k)
    }
}

def two { return 2 }

def tight(){ return 6 }

def take(var a : array<int>) : array<int> {
    return <- a
}

def long_signature_and_body(first_argument, second_argument : int; third_argument : float) : float {
    return float(first_argument * second_argument) + third_argument * 2.0
}

def commented() : int {
    return 3    // a single comment moves to the end of the new line
}

def two_comments() : int {   // two comments
    return 4    // keep the block
}

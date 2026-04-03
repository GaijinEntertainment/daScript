options gen2
// This file should produce zero style warnings.

require strings

def good_trailing_block() {
    let s = build_string() $(var w) {
        w |> write("hello")
    }
}

def good_bare_block() {
    var x = 1
    {
        x = 2
    }
}

def good_conditional() {
    let x = 5
    if (x > 0) {
        print("positive\n")
    }
}

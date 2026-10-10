options gen2
options indenting = 4, no_aot = true

module example shared public

require daslib/strings_boost
require ../common/utils as utils

expect 30101:2

typedef IntArray = array<int>

tuple Pair {
    first : int
    second : string
}

variant Value {
    i : int
    s : string
}

bitfield Flags : uint8 {
    visible
    enabled
}

enum Color : uint8 {
    Red
    Green = 4
    Blue
}

struct Point {
    x, y : float = 0.0
    @tag name : string
}

class Shape : Base {
    origin : Point
    def override area : float {
        return 0.0
    }
    def abstract describe : string
}

let private scale = 2.5
var counter : int

[export]
def main(args : array<string>; verbose = false) : int {
    var total = 0
    for (arg, i in args, count()) {
        total += length(arg) * i
    }
    let squares <- [for (x in range(10)); x * x; where x % 2 == 0]
    let lookup = {"one" => 1, "two" => 2}
    if (verbose) {
        print("total {total} from {length(args)} arguments\n")
    } elif (total > 100) {
        return 1
    } else {
        print("small\n")
    }
    let pick = @(p : Point) => p.x + p.y * scale
    squares |> each() <| $(v) {
        counter++
    }
    let shape = new Shape(origin = Point(x = 1.0, y = 2.0))
    try {
        unsafe {
            delete shape
        }
    } recover {
        return -1
    }
    return total if (total > 0) else return 0
}

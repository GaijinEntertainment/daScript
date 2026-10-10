options gen2
module shapes
//     ^ definition.module

// Builds the greeting.
def greet(name : string) : string {
//  ^ definition.function
    return "hello, {name}"
}

def operator +(a, b : Point) : Point {
//  ^ definition.function
    return Point(x = a.x + b.x)
}

let LIMIT = 10
//  ^ definition.constant

typedef Meters = float
//      ^ definition.type

tuple Pair {
//    ^ definition.type
    first : int
}

enum Color {
//   ^ definition.class
    Red
}

// A point on a plane.
struct Point {
//     ^ definition.class
    x : float
}

class Shape {
//    ^ definition.class

    // The area of the shape.
    def area : float {
    //  ^ definition.method
        return 0.0
    }
}

class Square : Shape {
//    ^ definition.class
//             ^ reference.class
    side : float
}

def main {
    greet("x")
    // <- reference.call
    let square = new Square()
    square->area()
    //      ^ reference.call
    print(math::sqrt(LIMIT) |> to_string)
    // <- reference.call
    //          ^ reference.call
    //                         ^ reference.call
}

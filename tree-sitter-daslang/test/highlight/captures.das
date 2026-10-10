options gen2, indenting = 4
// <- keyword.directive
//      ^ property
//          ^ punctuation.delimiter
//                      ^ operator
//                        ^ number
module captures shared public
// <- keyword.import
//     ^ module
//              ^ keyword.modifier
//                     ^ keyword.modifier
require daslib/strings_boost as sb
// <- keyword.import
//      ^ module
//            ^ punctuation.delimiter
//                           ^ keyword.import
//                              ^ module
expect 30101:2
// <- keyword.directive
//     ^ number
include common/shared.das
// <- keyword.import

typedef distinct Meters = float
// <- keyword.type
//      ^ keyword.modifier
//               ^ type
//                        ^ type.builtin
tuple Pair {
// <- keyword.type
//    ^ type
//         ^ punctuation.bracket
    first : int
    // <- variable.member
    //    ^ punctuation.delimiter
    //      ^ type.builtin
}
bitfield Flags : uint8 {
// <- keyword.type
//       ^ type
//               ^ type.builtin
    visible
    // <- constant
}
enum Color : uint8 {
// <- keyword.type
//   ^ type
    Red = 1
    // <- constant
    //    ^ number
}

struct Point {
// <- keyword.type
//     ^ type
    @tag x : float = 0.0
    // <- attribute
//   ^ property
    //   ^ variable.member
    //               ^ number.float
    def length : float {
    // <- keyword.function
    //  ^ function.method
        return sqrt(self.x * self.x)
        // <- keyword.return
        //     ^ function.call
        //          ^ variable.builtin
        //              ^ punctuation.delimiter
        //               ^ variable.member
        //                 ^ operator
    }
}

class Shape : Base {
// <- keyword.type
//    ^ type
//            ^ type
    def abstract area : float
    //  ^ keyword.modifier
    //           ^ function.method
}

let private SCALE = 2.5
// <- keyword
//  ^ keyword.modifier
//          ^ constant
var counter : int
// <- keyword
//  ^ variable

def operator +(a, b : Point) : Point {
// <- keyword.function
//  ^ keyword.function
//           ^ operator
//            ^ punctuation.bracket
//             ^ variable.parameter
//                ^ variable.parameter
//                    ^ type
    return Point(x = a.x + b.x)
    //     ^ type
    //           ^ variable.member
}

[export, unused_argument(args)]
// <- punctuation.bracket
 // <- attribute
//       ^ attribute
//                       ^ property
def main(args : array<string>; verbose = false) : int {
//  ^ function
//       ^ variable.parameter
//              ^ type.builtin
//                   ^ punctuation.bracket
//                    ^ type.builtin
//                          ^ punctuation.bracket
//                           ^ punctuation.delimiter
//                             ^ variable.parameter
//                                       ^ boolean
    var total = 0
    for (arg, i in args, count()) {
    // <- keyword.repeat
    //   ^ variable
    //          ^ keyword.operator
    //                   ^ function.call
        total += length(arg) * i
        //    ^ operator
        //       ^ function.call
    }
    let squares <- [for (x in range(10)); x * x; where x % 2 == 0]
    //          ^ operator
    //                        ^ type.builtin
    //                                           ^ keyword.repeat
    //                                                       ^ operator
    let lookup = {"one" => 1}
    //           ^ punctuation.bracket
    //            ^ string
    //                  ^ operator
    if (verbose) {
    // <- keyword.conditional
        print("total {total:x}\n")
        //    ^ string
        //           ^ punctuation.special
        //                 ^ punctuation.delimiter
        //                  ^ string.special
        //                   ^ punctuation.special
        //                    ^ string.escape
    } elif (total > 100) {
    //^ keyword.conditional
    //            ^ operator
        return 1
    } else {
    //^ keyword.conditional
        pass
        // <- keyword
    }
    let pick = @(p : Point) => p.x + p.y * SCALE
    //         ^ keyword.function
    //           ^ variable.parameter
    //               ^ type
    //                      ^ operator
    //                           ^ variable.member
    squares |> each() <| $(v) {
    //      ^ operator
    //         ^ function.call
    //                ^ operator
    //                   ^ keyword.function
    //                     ^ variable.parameter
        counter++
        //     ^ operator
    }
    let shape = new Shape(origin = Point(x = 1.0))
    //          ^ keyword.operator
    //              ^ type
    //                    ^ variable.member
    let first = sb::to_upper("a")
    //          ^ module
    //            ^ punctuation.delimiter
    //              ^ function.call
    let area = shape->area()
    //              ^ punctuation.delimiter
    //                ^ function.method.call
    let size = typeinfo sizeof(total)
    //         ^ keyword.operator
    //                  ^ function.builtin
    let code = 'a' + cast<int>(1.0lf)
    //         ^ character
    //               ^ keyword.operator
    //                         ^ number.float
    let none = null
    //         ^ constant.builtin
    let fallback = shape?.origin ?? Point(x = 0.0)
    //                  ^ punctuation.delimiter
    //                           ^ operator
    let red = Color.Red
    //        ^ variable
    //              ^ variable.member
    let half = total > 1 ? total : 1
    //                   ^ keyword.conditional.ternary
    //                           ^ keyword.conditional.ternary
    let text = %stringify~ any text %%
    //         ^ function.macro
    //          ^ function.macro
    //                   ^ function.macro
    //                     ^ string.special
    let sum = %sum! 1, 2 %%
    //        ^ function.macro
    let fn = @@main
    //       ^ operator
    //         ^ function
    try {
    // <- keyword.exception
        unsafe {
        // <- keyword
            delete shape
            // <- keyword.operator
        }
    } recover {
    //^ keyword.exception
        label 1:
        // <- keyword
        //    ^ label
        goto label 1
        // <- keyword
    }
    while (total > 0) {
    // <- keyword.repeat
        total--
        break
        // <- keyword.repeat
    } finally {
    //^ keyword.exception
        counter = 0
    }
    return total if (total > 0) else return 0
    //           ^ keyword.conditional
    //                          ^ keyword.conditional
}
// A comment.
// <- comment
/* A block comment. */
// <- comment

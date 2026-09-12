options gen2
options indenting = 4


require daslib/safe_addr

require shapes_c
require units_c
require greetings_c

[export]
def main() : int {
    var sh = shapes_create()
    var un = units_create()
    var gr = greetings_create()
    if (sh == null || un == null || gr == null) {
        print("a library refused to create an instance: {shapes_last_error(null)}\n")
        return 1
    }
    var a = shapes_Vec2(x = 3.0, y = 4.0)
    var b = shapes_Vec2(x = 1.0, y = 2.0)
    let dot = shapes_dot(sh, safe_addr(a), safe_addr(b))
    let fahrenheit = units_celsius_to_fahrenheit(un, 100.0)
    let greeting = greetings_greet(gr, "mundo")
    print("shapes.dot((3,4),(1,2))          = {dot}\n")
    print("units.celsius_to_fahrenheit(100) = {fahrenheit}\n")
    print("greetings.greet(\"mundo\")         = {greeting}\n")
    greetings_destroy(gr)
    units_destroy(un)
    shapes_destroy(sh)
    var bad = 0
    if (dot != 11.0) {
        bad++
    }
    if (fahrenheit != 212.0) {
        bad++
    }
    if (greeting != "hola mundo") {
        bad++
    }
    print(bad == 0 ? "three daslang libraries, one process - ok\n" : "{bad} of three answered wrong\n")
    return bad
}

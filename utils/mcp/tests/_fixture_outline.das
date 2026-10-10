options gen2

typedef Coord = float2

let ORIGIN = float2(0.0, 0.0)

class Shape {
    area : float
    typedef Inner = int
    def get_area() : float {
        return area
    }
}

def local_alias() : int {
    typedef Local = int
    var x : Local = 1
    return x
}

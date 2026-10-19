options gen2

require strings

struct Point {
    x : int
    y : int
}

def operator <(a, b : Point) : bool {
    if (a.x != b.x) return a.x < b.x
    return a.y < b.y
}

def sort_all(var points : array<Point>; var names : array<string>) {
    sort(points) $(a, b) => a.y > b.y
    sort(names, $(a, b) => length(a) < length(b))
}

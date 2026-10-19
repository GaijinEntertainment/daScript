options gen2

require daslib/cmp
require strings

[ordered]
struct Point {
    x : int
    y : int
}

def sort_all(var points : array<Point>; var names : array<string>) {
    sort(points) <| cmp_by(-_.y)
    sort(names, cmp_by(length(_)))
}

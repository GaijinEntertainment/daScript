options gen2

expect 31209:8

require daslib/style_lint
require daslib/cmp

struct Point {
    x : int
    y : int
}

def operator <(a, b : Point) : bool {
    if (a.x != b.x) return a.x < b.x
    return a.y < b.y
}

struct Player {
    name : string
    score : int
}

def operator <(a, b : Player) : bool => a.score > b.score

struct Opaque {
    x : int
    y : int
}

def operator <(a, b : Opaque) : bool => a.x + a.y < b.x + b.y

[never_inline]
def name_length(p : Player) : int => length(p.name)

[export]
def bad_comparators(var players : array<Player>; var points : array<Point>;
                    weight : array<int>; var order : array<int>) {
    sort(players) $(a, b) => a.score < b.score
    sort(players) <| $(a, b) => b.score < a.score
    sort(players) $(a, b) {
        if (a.score != b.score) {
            return a.score > b.score
        }
        return a.name < b.name
    }
    sort(points, $(p, q) => p.x != q.x ? p.x < q.x : p.y < q.y)
    sort(order) $(i, j) {
        let wi = weight[i]
        let wj = weight[j]
        return wi < wj || (wi == wj && i < j)
    }
    sort(players) $(a, b) {
        if (a.score == b.score) {
            return name_length(a) < name_length(b)
        } else {
            return a.score < b.score
        }
    }
}

[export]
def good_comparators(var players : array<Player>; var order : array<int>) {
    sort(players) <| cmp_by(-_.score, _.name)
    sort(players) $(a, b) => a.score < b.name |> length()
    sort(order) $(i, j) => i * 2 < j
    sort(players) $(a, b) => a.score <= b.score
    sort(players) $(a, b) {
        print("{a.name}\n")
        return a.score < b.score
    }
}

[ordered(score = greater)]
struct Ranked {
    score : int
}

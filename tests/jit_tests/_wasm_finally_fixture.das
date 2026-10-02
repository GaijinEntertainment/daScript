options gen2

// Returns, breaks and continues that leave through a `finally`: the shape whose wasm64 object
// emit crashed, and the counts a native run prints to prove each exit ran its finally once.

struct Node {
    v : int
}

var g_finally_runs = 0

def make_node(v : int) : Node? {
    return new Node(v = v)
}

def pick(xs : array<int>) : Node? {
    if (xs[0] == 1) {
        var arr : array<Node?>
        {
            for (x in xs) {
                if (x < 0) return null
                if (x == 7) return make_node(7)
                arr |> push(make_node(x))
            }
            return null
        } finally {
            g_finally_runs++
            unsafe {
                delete arr
            }
        }
    } elif (xs[0] == 2) {
        var tab : table<int; Node?>
        {
            for (x in xs) {
                if (x == 9) return make_node(9)
                tab |> insert(x, make_node(x))
            }
            return null
        } finally {
            g_finally_runs++
            unsafe {
                delete tab
            }
        }
    }
    return make_node(xs[0])
}

def loop_exits(n : int) : int {
    var sum = 0
    for (i in range(n)) {
        {
            if (i == 1) continue
            if (i == 4) break
            sum += i
        } finally {
            g_finally_runs++
        }
    }
    return sum
}

[export]
def main() {
    let a = pick([1, 2, 7])
    let b = pick([2, 3, 9])
    let c = pick([3])
    let s = loop_exits(10)
    print("values {a.v} {b.v} {c.v} {s} finally {g_finally_runs}\n")
}

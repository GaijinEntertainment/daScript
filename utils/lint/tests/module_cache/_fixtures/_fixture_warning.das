options gen2

require hidden_mod

[export]
def main {
    var xs : array<int>
    for (i in range(3)) {
        xs |> push(i + hidden_value())
    }
    print("{length(xs)}\n")
}

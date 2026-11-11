options gen2

require mc_tu_facade

[export]
def main {
    let made = TargetAl()
    var maybe : MaybeText
    print("{made.x} {maybe |> is_some()}\n")
}

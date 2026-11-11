options gen2

require _fixture_typedef_used_facade
require _fixture_typedef_used_inferred

struct Own {
    x : int
}

typedef OwnAl = Own

[export]
def use_it() : int {
    var own : OwnAl
    var other : TargetAl
    var maybe : MaybeText
    return own.x + other.x + (maybe |> is_some() ? 1 : 0)
}

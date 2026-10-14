options gen2
require daslib/match

variant Tags {
    nada
    nothing
    anything : int
}

[export]
def case_label(v : Tags) : string {
    return match (v) {
        Tags(nada) => "nada"
        Tags(anything = x) => "anything {x}"
    }
}

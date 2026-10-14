// A bare case is one value of the variant: a match that names every
// other case still misses it.
options gen2
require daslib/match
expect 50503

variant Tags {
    nada
    nothing
    anything : int
}

def case_label(v : Tags) : string {
    return match (v) {
        Tags(nada) => "nada"
        Tags(anything = x) => "anything {x}"
    }
}

[export]
def main {
    case_label(Tags(nothing))
}

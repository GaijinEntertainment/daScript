// A variant case declared without a type has no value: `as`, `?as`
// and the unsafe field read all refuse it.
options gen2
expect 30113, 30223, 30839:3

variant Tags {
    nada
    anything : int
}

[export]
def main {
    var v = Tags(nada)
    let a = v as nada
    let b = unsafe(v ?as nada)
    unsafe {
        let c = v.nada
        var p : Tags? = addr(v)
        let d = p.nada
        let e = p?.nada
    }
}

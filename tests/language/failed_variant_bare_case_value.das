// A case with no value takes none, and a case with a value is not
// spelled bare.
options gen2
expect 30951:2, 30284

variant Tags {
    nada
    anything : int
}

[export]
def main {
    var v = Tags(nada = 1)
    var w = Tags(anything)
    var x = 1
    var y = Tags(x)
}

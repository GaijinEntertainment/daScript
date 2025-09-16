require daslib/linq_boost

[export]
def main() {
    var t = ([1,2,3,4,5].to_sequence()
        ._where( _ <= 3)
        .to_array()
        ._fold())   // this one folds into comprehension
    debug(t)

    var tt = ([1,2,3,4,5].to_sequence()
        .min()  // this one does not fold for now
        ._fold())
    debug(tt)
}

options log




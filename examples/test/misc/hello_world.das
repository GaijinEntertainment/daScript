require daslib/linq_boost

[export]
def main() {
    var t1 <- ([1, 2, 3, 4, 5].to_sequence()
        ._where(_ <= 3)
        .to_array()
        ._fold())
    debug(t1)

    var t2 <- ([1, 2, 3, 4, 5].to_sequence()
        ._where(_ <= 3)
        ._select(_ * 2)
        ._fold())
    debug(t2.to_array())

    var t3 <- ([1, 2, 3, 4, 5].to_sequence()
        ._select(_ * 2)
        .to_array()
        ._fold())
    debug(t3)
}

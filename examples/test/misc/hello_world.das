require daslib/linq_boost

[export]
def main() {
    var t2 <- ([1, 2, 3, 4, 5].to_sequence()
        .reverse()
        ._where(_ <= 3)
        ._select(_ * 2)
        .to_array()
        ._fold()
        )
    debug(t2)

}


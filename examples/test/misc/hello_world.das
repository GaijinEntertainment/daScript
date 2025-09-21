require daslib/linq_boost

[export]
def main() {
    var t2 <- ([1, 2, 3, 4, 5].to_sequence()
        ._where_to_array(_ <= 3)
        .reverse()
        ._select_to_sequence(_ * 2)
        ._order_by_descending(_)
        .to_array()
        ._fold()
        )
    debug(t2)

}

options log

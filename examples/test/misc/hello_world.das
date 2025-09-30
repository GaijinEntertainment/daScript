require daslib/linq_boost

[export]
def main() {
    let arr = [1, 2, 3, 4, 5]
    var qarray = to_table(
        arr,
        $(x : int) => x,
        $(x : int) => x * 10
    )

}

options log

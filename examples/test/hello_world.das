options log = true

[export, unsafe]
def test
    var it <- each("123")
    var ahead : int
    while next(it, ahead)
        print("val {ahead}\n")
    var iaa = [[int 1;2;3]]
    var it2 <- each(iaa)
    while next(it2, ahead)
        print("ref {ahead}\n")
    return true

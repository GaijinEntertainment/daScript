// options log_infer_passes = true
// options log_generics = true

[export]
def test
    let t <- [{for x in range(5); x}]
    print("t = {t}\n")
    let z = iter_range(t)
    print("length(t) = {z}\n")
    return true


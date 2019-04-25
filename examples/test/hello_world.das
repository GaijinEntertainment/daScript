// options log = true, logInferPasses = true

def take_arr(a:int[])
    static_assert(typeinfo(is_dim a))
    static_assert(typeinfo(dim a)==4)
    var summ = 0
    for x in a
        summ += x
    return summ

[export]
def test
    let t = [[int[4] 1;2;3;4]]
    let t_summ = take_arr(t)
    print("t_summ={t_summ}\n")
    return true

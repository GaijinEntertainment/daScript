options log = true
// options log_infer_passes = true
// options print_c_style = true

[export]
def test
    let gen = generator() <| $(var a:int &) : bool
        var count = 0
        var total = 0
        while count < 10
            yield count ++
            if count > 5
                break
            if count > 4
                continue
            var j = 0
            while j < 3
                yield 1000 + j++
                var k = 0
                while k < 2
                    yield 2000 + j*10 + k++
            total ++
        finally
            yield total
        return false
    print("gen = {gen}\n")
    for x in gen
        print("x = {x}\n")
    print("gen = {gen}\n")
    return true

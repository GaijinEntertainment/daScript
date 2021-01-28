
[export]
def test
    var gen <- generator<bool>() <|
        for x in range(10)
            print("coroutine: step {x}\n")
            yield true
        print("almost done\n")
        yield true
        return false
    return gen
require _moduleEnum

[export]
def test
    let a = _moduleEnum::Numbers zero
    assert(int(a) == 0)
    return true

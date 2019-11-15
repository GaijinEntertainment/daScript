require moduleEnum

[export]
def test
    let a = moduleEnum::Numbers zero
    assert(int(a) == 0)

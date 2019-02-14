require math
require foo_bar

[export]
def test
    assert(foo_bar::getValue()==13)
    return true



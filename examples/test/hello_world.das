options log_nodes = true

enum Bar
    none
    yes
    no

[export]
def foo ( a : int )
    return Bar yes

[export]
def test
    return true

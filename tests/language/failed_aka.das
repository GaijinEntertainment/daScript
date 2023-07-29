expect 30126:2

options persistent_heap = true
options gc

require dastest/testing_boost public

var foo aka bar : int   // 30126: global variable foo can't have an aka

typedef kaboom = function<(a aka b:int):void>   // 30126: type declaration can't have an aka

[test]
def test_fail_aka( t: T? )
    pass
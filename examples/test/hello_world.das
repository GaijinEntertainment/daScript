// options log

require jobque
require daslib/jobque_boost

[export]
def test
    for x in range(5)
        var y <- [{for t in range(3); t+x}]
        pfork <| @ [[<-y]]
            print("x = {x}, y = {y}, context = {this_context()}\n")
    return true

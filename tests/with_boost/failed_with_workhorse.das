options gen2
expect 50503

require daslib/with_boost

[export]
def main {
    var ints <- [10, 20, 30]
    with_(ints[0]) {  // refuse: workhorse element (mutation wouldn't propagate)
        print("body")
    }
    delete ints
}

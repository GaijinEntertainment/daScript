// a for-loop variable stays const inside a generator body, range sources included:
// the generated counter is written through the iterator's void? launder, not by
// relaxing the variable the body sees
options gen2
expect 30952

def g() : iterator<int> {
    return <- generator<int> {
        for (i in range(3)) {
            i = 5
            yield i
        }
        return false
    }
}

[export]
def main() {
    for (x in g()) {
        print("{x}\n")
    }
}

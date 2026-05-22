// Sort benchmark — ported from dasProfile/tests/table-sort.das.
// 100,000-element array sort, descending. Renamed from `table-sort.das`
// (hyphen) to `table_sort.das` to match the wasm cross-compile target naming.
options gen2
options persistent_heap

require math

def makeRandomSequence(var src : array<int>) {
    let n = 100000
    let seed = 1u
    resize(src, n)
    for (i in range(n)) {
        src[i] = int(uint_noise_1D(i, seed))
    }
}

def sort_table(var tab : array<int>) {
    sort(tab, $(a, b) => a > b)
}

[export]
def main {
    var tab : array<int>
    makeRandomSequence(tab)
    profile(10, "sort") {
        var inscope tabb := tab
        sort_table(tabb)
    }
}

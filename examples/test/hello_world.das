def report(extra="")
    print("\n{int(string_heap_bytes_allocated())} bytes in string heap{extra}\n")
    string_heap_report()

var
    s : array<string>

[export, unsafe]
def test
    report(" initially")
    let TOTAL = 200
    let TOTAL_BIG = 5
    for t in range(0,TOTAL)
        push(s,repeat("{t} ",50))
    for t in range(0,TOTAL_BIG)
        push(s,repeat("big{t} ",1000))
    report(" after allocating")
    for t in range(0,TOTAL)
        if t % 100 <= 97
            s[t] = ""
    for t in range(0,TOTAL_BIG)
        if t%2==0
            s[t+TOTAL] = ""
    report(" after removing")
    string_heap_collect()
    report(" after collecting")
	return true

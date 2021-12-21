[export]
def main
    var q <- [[for x in range(5); x]]
    for i in flatten(q)
        print("q[{i}] = {i}\n")

    var qq <- [[for x in range(3); [[for y in range(4); x*4+y ]]  ]]
    for i in flatten(qq)
        print("qq[{i}] = {i}\n")

    var qqq <- [[for x in range(3); [[for y in range(2); [[for z in range(2); x*2*2 + y*2 + z ]] ]]  ]]
    for i in flatten(qqq)
        print("qqq[{i}] = {i}\n")

options log=false, logStack = false

def makeSequence(base:auto(numT)!&;max)
    let a : array<numT>
    resize(a, max+1-base)
    for t in a
        t = base++
    return <- a

def joinSequences(l:array<auto(numT)>;r:array<numT>)
    let a : array<numT>
    reserve(a, length(l) + length(r))
    for t in l
        push(a, t)
    for t in r
        push(a, t)
    return <- a

def makeSequenceB(b:block< array<auto> >)
    return <- invoke(b)

def test
    let t <- makeSequenceB({ return <- makeSequence(0,4); })
    let q <- makeSequenceB({ return <- makeSequence(5,9); })
    let z <- joinSequences(makeSequenceB({ return <- makeSequence(0,4); }), makeSequenceB({ return <- makeSequence(5,9); }))
    print("z={z}\n")
    return true

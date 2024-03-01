// from https://spiiin.github.io/blog/2385889062/

[sideeffects]
def popcount(x)
    var temp = x
    var count = 0u
    while temp != 0u
        temp &= temp - 1u
        count++
    return count

[sideeffects]
def maxFrom1s(x)
    let count1s = popcount(x)
    var res = 0u
    for i in range(count1s)
        res++
        res<<=1u
    for i in urange(31u - count1s) //assume 32 bits
        res<<=1u
    return res

[sideeffects]
def maxFrom1s_popcnt(x)
    let count1s = popcnt(x)
    var res = 0u
    for i in range(count1s)
        res++
        res<<=1u
    for i in urange(31u - count1s) //assume 32 bits
        res<<=1u
    return res

[jit,sideeffects]
def popcount_jit(x)
    var temp = x
    var count = 0u
    while temp != 0u
        temp &= temp - 1u
        count++
    return count

[jit,sideeffects]
def maxFrom1s_jit(x)
    let count1s = popcount_jit(x)
    var res = 0u
    for i in range(count1s)
        res++
        res<<=1u
    for i in urange(31u - count1s) //assume 32 bits
        res<<=1u
    return res

[jit,sideeffects]
def maxFrom1s_popcnt_jit(x)
    let count1s = popcnt(x)
    var res = 0u
    for i in range(count1s)
        res++
        res<<=1u
    for i in urange(31u - count1s) //assume 32 bits
        res<<=1u
    return res

[export,no_jit]
def main
    verify(maxFrom1s(256u-1u)==0xff000000)
    verify(maxFrom1s_popcnt(256u-1u)==0xff000000)
    verify(maxFrom1s_jit(256u-1u)==0xff000000)
    verify(maxFrom1s_popcnt_jit(256u-1u)==0xff000000)
    profile(10, "max from 1s")  <|
        for i in range(100000)
            maxFrom1s(256u-1u)  //'0xff000000'
    profile(10, "max from 1s, popcnt")  <|
        for i in range(100000)
            maxFrom1s_popcnt(256u-1u)  //'0xff000000'
    profile(10, "max from 1s, jit")  <|
        for i in range(100000)
            maxFrom1s_jit(256u-1u)  //'0xff000000'
    profile(10, "max from 1s, popcnt, jit")  <|
        for i in range(100000)
            maxFrom1s_popcnt_jit(256u-1u)  //'0xff000000'


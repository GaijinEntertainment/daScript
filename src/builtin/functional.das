options indenting=4

require functional

// filter with lambda
def filter(var src:iterator<auto(TT)>; blk:lambda<(what:TT):bool> )
    return <- generator<TT> () <| $ ()
        for w in src
            if invoke(blk,w)
                yield w
        return false

// filter with function
def filter(var src:iterator<auto(TT)>; blk:function<(what:TT):bool> )
    return <- generator<TT> () <| $ ()
        for w in src
            if invoke(blk,w)
                yield w
        return false

// map with lambda
def map(var src:iterator<auto(TT)>; blk:lambda<(what:TT):auto(QQ)>) : iterator<QQ>
    return <- generator<QQ> () <| $ ()
        for w in src
            yield invoke(blk,w)
        return false

// map with function
def map(var src:iterator<auto(TT)>; blk:function<(what:TT):auto(QQ)>) : iterator<QQ>
    return <- generator<QQ> () <| $ ()
        for w in src
            yield invoke(blk,w)
        return false

// reduce value, any invokable
[private]
def reduce_any(it:iterator<auto(TT)>;functor)
    var left : TT delete &
    var first = true
    for right in it
        if first
            first = false
            left = right
        else
            left = invoke(functor,left,right)
    if first
        panic("can't reduce empty sequence")
    return left

 // reduce with lambda
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:lambda<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

 // reduce with function
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:function<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

 // reduce with block
[unsafe]
def reduce(it:iterator<auto(TT)>;blk:block<(left,right:TT delete &):TT const delete &>)
    return reduce_any(it,blk)

// summ of all elements
//  same as reduce(it, @(a,b) => a + b)
def sum(it:iterator<auto(TT)>)
    var temp : TT delete &
    for x in it
        temp += x
    return temp

// any
def any ( it )
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't ANY non-iterable type")
        return false
    else
        for x in it
            if x
                return true
        return false

// all
def all ( it )
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't ALL non-iterable type")
        return false
    else
        for x in it
            if !x
                return false
        return true

// islice
def islice( var src : iterator<auto(TT)>; start,stop : int ) : iterator<TT>
    static_if typeinfo(can_copy type TT delete &)
        return <- generator<TT> () <| $ ()
            for x,i in src,range(0,stop)
                if i>=start
                    yield x
            return false
    static_if typeinfo(can_move type TT delete &) & typeinfo(can_clone type TT delete &)
        return <- generator<TT> () <| $ ()
            for x,i in src,range(0,stop)
                if i>=start
                    var t := x
                    yield <- t
            return false
    else
        concept_assert(false,"can't islice on this iterator; subtype needs to by copiable, or moveable and cloneable")
        var dummy : iterator<TT delete &>
        return <- dummy

// [[ start; start+step; start+2*step ]]
def count(var start,step:int) : iterator<int>
    return <- each() <| @(var result:int&) : bool
        result = start
        start += step
        return true

// [[ value; value; value; .... count times ]]
def repeat(value : auto(TT); var count : int = 0)
    return <- generator<TT> () <| $ ()
        if count == 0
            while true
                yield value
        for x in range(0,count)
            yield value
        return false

def is_equal(a,b)
    return a == b

def is_not_equal(a,b)
    return a == b

def not(x)
    return !x

def echo(var x;extra : string = "\n")
    print("{x}{extra}")
    return <- x

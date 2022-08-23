options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module regex_gen shared private

require daslib/regex public

require strings

var private log_gen_enabled = false

typedef
    ReGenRandom = iterator<uint>
    ReGenFun = function< ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void >
    ReGenTab = table < ReNode?; ReGenFun >

var private g_re_gen_current : ReGenTab?
var private g_re_gen_rep_limit = 8u

def private rnd_set_character ( cset:uint[8]; var rnd:ReGenRandom ) : int
    // TODO: there is a lot faster way to count number of set bits
    //  there is also das_clz for getting first set bit
    //  finally this may just belong to C++, maybe?
    var total = 0u
    for t in range(8)
        let bits = cset[t]
        if bits != 0u
            for b in urange(32)
                if (bits & (1u << b)) != 0u
                    total ++
    var idx = 0u
    next(rnd,idx)
    idx = idx % total
    var index = 0u
    for t in range(8)
        let bits = cset[t]
        if bits != 0u
            for b in urange(32)
                if (bits & (1u << b)) != 0u
                    if index == idx
                        return t*32 + int(b)
                    index ++
    assert(false,"we should not be here. mismatch generated character")
    return 0

def private re_get_gen_func ( node:ReNode? ) : ReGenFun
    assert(g_re_gen_current!=null, "not in a re_gen_current")
    if (*g_re_gen_current) |> key_exists(node)
        return (*g_re_gen_current)[node]
    else
        return @@re_gen2_fail

def private re_gen2_fail ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen fail\n")
    assert(false, "not running generated regex")

def private re_gen2_char ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen char `{node.text}`\n")
    str |> write(node.text)
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_union ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen union\n")
    for sub in node.all
        invoke(re_get_gen_func(sub),sub,rnd,str)

def private re_gen2_set ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen set {node.cset}\n")
    str |> write_char(rnd_set_character(node.cset,rnd))
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_any ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen any\n")
    var ch : uint
    next(rnd, ch)
    str |> write_char( int((ch%255u)+1u) )
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_concat ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen concat\n")
    var left = node.left
    invoke(re_get_gen_func(left),left,rnd,str)

def private re_gen2_eos ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen eos\n")
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_question ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen question\n")
    var a = 0u
    next(rnd,a)
    if (a & 1u) != 0u               // zero or one?
        var nsub = node.subexpr
        if nsub!=null
            invoke(re_get_gen_func(nsub),nsub,rnd,str)
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_plus ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen plus\n")
    var cnt = 0u
    next(rnd,cnt)
    cnt = (cnt % g_re_gen_rep_limit) + 1u
    var nsub = node.subexpr
    if nsub!=null
        for i in urange(cnt)
            invoke(re_get_gen_func(nsub),nsub,rnd,str)
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_star ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("gen star\n")
    var cnt = 0u
    next(rnd,cnt)
    cnt = cnt % g_re_gen_rep_limit
    var nsub = node.subexpr
    if nsub!=null
        for i in urange(cnt)
            invoke(re_get_gen_func(nsub),nsub,rnd,str)
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)

def private re_gen2_group ( var node:ReNode?; var rnd:ReGenRandom; var str:StringBuilderWriter ) : void
    if log_gen_enabled
        print("match2 group\n")
    var nsub = node.subexpr
    if nsub!=null
        invoke(re_get_gen_func(nsub),nsub,rnd,str)
    var node2 = node.next
    if node2!=null
        invoke(re_get_gen_func(node2),node2,rnd,str)


def private re_assign_gen_functions ( var re:Regex )
    visit_top_down(re.root) <| $ ( var node )
        var fun2 : ReGenFun
        if node.op==ReOp Char
            fun2 = @@re_gen2_char
        elif node.op==ReOp Union
            fun2 = @@re_gen2_union
        elif node.op==ReOp Set
            fun2 = @@re_gen2_set
        elif node.op==ReOp Any
            fun2 = @@re_gen2_any
        elif node.op==ReOp Eos
            fun2 = @@re_gen2_eos
        elif node.op==ReOp Concat
            fun2 = @@re_gen2_concat
        elif node.op==ReOp Plus
            fun2 = @@re_gen2_plus
        elif node.op==ReOp Star
            fun2 = @@re_gen2_star
        elif node.op==ReOp Question
            fun2 = @@re_gen2_question
        elif node.op==ReOp Group
            fun2 = @@re_gen2_group
        else
            panic("unsupported {node.op}")
        (*g_re_gen_current)[node] = fun2


def public re_gen_compile ( var re:Regex )
    var tab:ReGenTab
    g_re_gen_current = unsafe(addr(tab))
    re_assign_gen_functions(re)
    g_re_gen_current = null
    return <- tab

def public re_gen ( var re:Regex; var tab:ReGenTab; var rnd:ReGenRandom ) : string
    return build_string <| $ ( writer )
        g_re_gen_current = unsafe(addr(tab))
        invoke(re_get_gen_func(re.root),re.root,rnd,writer)
        g_re_gen_current = null

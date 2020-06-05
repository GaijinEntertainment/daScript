
//options log = true
//options log_stack = true
//options log_nodes = true
//options fusion = false

// options debugger = true

typedef
    ReText = string

enum ReOp
    Char
    Set
    NegativeSet
    Any
    Eos
    Group
    Plus
    Star
    Question
    Concat
    Union

typedef
    CharSet = uint[8]

struct ReNode
    op      : ReOp
    at      : range
    text    : string
    all     : array<ReNode?>
    left    : ReNode?
    right   : ReNode?
    cset    : CharSet

variant MaybeReNode
    value   : ReNode?
    nothing : void?

/*
    Parser
*/

let
    log_enabled = false
    range_check_enabled = false

var
    trace_tab = 0

def trace(msg:string; offset:int)
    static_if log_enabled
        print("{repeat(" ",trace_tab)}{offset}: {msg}\n")
        trace_tab ++

def note(msg:string; offset:int)
    static_if log_enabled
        print("{repeat(" ",trace_tab)}{offset}: {msg}\n")

def trace_backtrack()
    static_if log_enabled
        trace_tab --
        print("{repeat(" ",trace_tab)}backtrack\n")

def trace_value(value : ReNode?)
    static_if log_enabled
        if value==null
            panic("trace_value null?")
        trace_tab --
        print("{repeat(" ",trace_tab)}matched {value.op}, next {value.at.y}\n")

[unsafe]
def nada()
    trace_backtrack()
    return [[MaybeReNode nothing=null]]

def maybe( value : ReNode? )
    trace_value(value)
    return [[MaybeReNode value=value]]

def eos(expr:ReText; offset:int)
    return offset >= length(expr)

[unsafe]
def at(expr:ReText; offset:int) : int
    static_if range_check_enabled
        let len = length(expr)
        if offset<len
            return character_at(expr, offset)
        elif offset==len
            return 0
        else
            stackwalk()
            panic("out of range {offset} of {len}")
            return 0
    else
        return character_uat(expr, offset)

def next(re:MaybeReNode)
    if !(re is value)
        stackwalk()
        panic("expecting value")
    return (re as value).at.y

def set_or_char(var cset:CharSet; ch:int)
    cset[ch>>5] |= uint(1 << (ch & 31))

def set_or_range(var cset:CharSet; bits:range)
    for ch in range(bits.x,bits.y+1)
        cset[ch>>5] |= uint(1 << (ch & 31))

def set_invert(var cset:CharSet)
    for x in cset
        x ^= 0xffffffff

def set_or_set(var cset:CharSet; eset:CharSet)
    for x,y in cset,eset
        x |= y

def set_meta(var cset:CharSet; che:int)
    if che=='w'
        set_or_range(cset,range('a','z'))
        set_or_range(cset,range('A','Z'))
        set_or_range(cset,range('0','9'))
        set_or_char(cset,'_')
    elif che=='W'
        var eset:CharSet
        set_meta(eset,'w')
        set_or_set(cset,eset)
    else
        set_or_char(cset,che)

let
    meta = "\\+-*.()[]|^"
    meta_set = "wW"

def is_meta_character(ch:int) : bool
    return find_first_of(meta,ch) != -1

def is_set_character(ch:int) : bool
    return find_first_of(meta_set,ch) != -1

// <char>	::=	any non metacharacter | "\" metacharacter
def re_char (expr : ReText; offset:int) : MaybeReNode
    trace("re_char",offset)
    if eos(expr,offset)
        return nada()
    let ch = at(expr,offset)
    if ch == '\\'
        let ech = at(expr,offset+1)
        if ech==0
            note("syntax error, expecting meta", offset+1)
            return nada()
        if is_set_character(ech)
            var cset : CharSet
            set_meta(cset,ech)
            return maybe(new [[ReNode op=ReOp Set, cset=cset, at=range(offset,offset+2)]])
        return maybe(new [[ReNode op=ReOp Char, text=to_char(ech), at=range(offset,offset+2)]])
    elif is_meta_character(ch)
        return nada()
    return maybe(new [[ReNode op=ReOp Char, text=to_char(ch), at=range(offset,offset+1)]])

// <set-items>	::=	<set-item> | <set-item> <set-items>
// <set-item>	::=	<range> | <char>
// <range>	::=	<char> "-" <char>
// <char>	::=	any non metacharacter | "\" metacharacter
def re_set_items(expr:ReText; offset:int) : MaybeReNode
    trace("re_set_items",offset)
    if eos(expr,offset)
        return nada()
    var cset : CharSet
    var iofs = offset
    while !eos(expr,iofs)
        let ch = at(expr,iofs)
        if ch == ']'
            break
        elif ch=='\\'
            let che = at(expr,iofs+1)
            if che==0
                note("syntax error, expecting meta", iofs)
                return nada()
            set_meta(cset,che)
            iofs += 2
        elif at(expr,iofs+1)=='-'
            let che = at(expr,iofs+2)
            if che==0
                note("syntax error, expecting range", iofs+1)
                return nada()
            elif ch > che
                note("syntax error, character range out of order", iofs)
                return nada()
            set_or_range(cset,range(ch,che))
            iofs += 3
        else
            set_or_char(cset,ch)
            iofs += 1
    return maybe(new [[ReNode op=ReOp Set, cset=cset, at=range(offset,iofs)]])

// <positive-set>	::=	"[" <set-items> "]"
def re_positive_set(expr:ReText; offset:int) : MaybeReNode
    trace("re_positive_set",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset) != '['
        return nada()
    var oset = re_set_items(expr,offset+1)
    if oset is nothing
        return nada()
    if at(expr,next(oset)) != ']'
        delete oset
        return nada()
    var oval = oset as value
    oval.at.x = offset
    oval.at.y ++
    trace_value(oval)
    return oset

// <negative-set>	::=	"[^" <set-items> "]"
def re_negative_set(expr:ReText; offset:int) : MaybeReNode
    trace("re_negative_set",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)!='[' | at(expr,offset+1)!='^'
        return nada()
    var oset = re_set_items(expr,offset+2)
    if oset is nothing
        return nada()
    if at(expr,next(oset)) != ']'
        delete oset
        return nada()
    var oval = oset as value
    oval.at.x = offset
    oval.at.y ++
    oval.op = ReOp NegativeSet
    trace_value(oval)
    return oset

// <set>	::=	<positive-set> | <negative-set>
// todo: optimize
def re_set(expr:ReText; offset:int) : MaybeReNode
    trace("re_set",offset)
    if eos(expr,offset)
        return nada()
    var oneg = re_negative_set(expr,offset)
    if oneg is value
        return oneg
    var opos = re_positive_set(expr,offset)
    if opos is value
        return opos
    return nada()

// <any>	::=	"."
def re_any(expr:ReText; offset:int) : MaybeReNode
    trace("re_any",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)=='.'
        return maybe(new [[ReNode op=ReOp Any, at=range(offset,offset+1)]])
    return nada()

// <eos>	::=	"$"
def re_eos(expr:ReText; offset:int) : MaybeReNode
    trace("re_eos",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)=='$'
        return maybe(new [[ReNode op=ReOp Eos, at=range(offset,offset+1)]])
    return nada()

// <group>	::=	"(" <RE> ")"
def re_group(expr:ReText; offset:int) : MaybeReNode
    trace("re_group",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)!='('
        return nada()
    var ore = re_re(expr,offset+1)
    if ore is nothing
        return nada()
    if at(expr,next(ore))!=')'
        delete ore
        return nada()
    return maybe(new [[ReNode op=ReOp Group, left=ore as value, at=range(offset,next(ore)+1)]])

// <elementary-RE>	::=	<group> | <any> | <eos> | <set> | <char>
def re_elementary(expr:ReText; offset:int) : MaybeReNode
    trace("re_elementary",offset)
    if eos(expr,offset)
        return nada()
    var ogr = re_group(expr,offset)
    if ogr is value
        return ogr
    var oany = re_any(expr,offset)
    if oany is value
        return oany
    var oeos = re_eos(expr,offset)
    if oeos is value
        return oeos
    var oset = re_set(expr,offset)
    if oset is value
        return oset
    var ochr = re_char(expr,offset)
    if ochr is value
        return ochr
    return nada()

def concat(var left,right:ReNode?)
    if left == null
        return right
    elif left.op==ReOp Char & right.op==ReOp Char
        left.text += right.text
        left.at.y = right.at.y
        delete right
        return left
    else
        return new [[ReNode op=ReOp Concat, left=left, right=right, at=range(left.at.x,right.at.y)]]

// <RE>	::=	<union> | <simple-RE>
// <union>	::=	<RE> "|" <simple-RE>
// <simple-RE>	::=	<concatenation> | <basic-RE>
// <concatenation>	::=	<simple-RE> <basic-RE>
def re_re(expr:ReText; offset:int) : MaybeReNode
    trace("re_re",offset)
    if eos(expr,offset)
        return nada()
    var cofs = offset
    var last : ReNode?
    while !eos(expr,cofs)
        if at(expr,cofs)=='|'
            if last == null
                note("syntax error, nada | union", cofs)
                return nada()
            let oright = re_re(expr,cofs+1)
            if oright is nothing
                note("syntax error, re | nada", cofs+1)
                return nada()
            last = new [[ReNode op=ReOp Union, left=last, right=oright as value, at=range(last.at.x,next(oright))]]
            cofs = next(oright)
        else
            var onext = re_basic(expr, cofs)
            if onext is nothing
                return last==null? nada() : maybe(last)
            last = concat(last,onext as value)
            cofs = last.at.y
    return maybe(last)

// <basic-RE>	::=	<star> | <plus> | <elementary-RE>
// <star>	::=	<elementary-RE> "*"
// <plus>	::=	<elementary-RE> "+"
def re_basic(expr:ReText; offset:int) : MaybeReNode
    trace("re_basic",offset)
    if eos(expr,offset)
        return nada()
    var oelem = re_elementary(expr,offset)
    if oelem is nothing
        return nada()
    let ch = at(expr,next(oelem))
    if ch=='*'
        return maybe(new [[ReNode op=ReOp Star, left=oelem as value, at=range(offset,next(oelem)+1)]])
    elif ch=='+'
        return maybe(new [[ReNode op=ReOp Plus, left=oelem as value, at=range(offset,next(oelem)+1)]])
    elif ch=='?'
        return maybe(new [[ReNode op=ReOp Question, left=oelem as value, at=range(offset,next(oelem)+1)]])
    return oelem

def re_parse ( expr:ReText ) : ReNode?
    var ore = re_re(expr, 0)
    if ore is nothing
        return null
    let at = next(ore)
    if at != length(expr)
        note("syntax error",at)
        return null
    return ore as value

/*
    matching
*/

def re_match_char ( str:ReText; offset:int; node:ReNode? ) : int
    let len = length(node.text)
    for Ch,Ofs in node.text,range(offset,offset+len)
        if at(str,Ofs) != Ch
            return -1
    return offset + len

def re_match_union ( str:ReText; offset:int; node:ReNode? ) : int
    let left = re_match(str,offset,node.left)
    if left != -1
        return left
    let right = re_match(str,offset,node.right)
    if right != -1
        return right
    return -1

def re_match_set ( str:ReText; offset:int; node:ReNode? ) : int
    let ch = at(str,offset)
    if (node.cset[ch>>5] & uint(1 << (ch & 31))) == 0u
        return -1
    return offset + 1

def re_match_negative_set ( str:ReText; offset:int; node:ReNode? ) : int
    let ch = at(str,offset)
    if (node.cset[ch>>5] & uint(1 << (ch & 31))) != 0u
        return -1
    return offset + 1

def re_match_any ( str:ReText; offset:int ) : int
    if eos(str,offset)
        return -1
    return offset + 1

def re_match_concat ( str:ReText; offset:int; node:ReNode? ) : int
    if eos(str,offset)
        return -1
    let oleft = re_match(str,offset,node.left)
    if oleft == -1
        return -1
    let oright = re_match(str,oleft,node.right)
    if oright == -1
        return -1
    return oright

def re_match_eos ( str:ReText; offset:int ) : int
    if eos(str,offset)
        return offset
    return -1

def re_match_question ( str:ReText; offset:int; node:ReNode? ) : int
    var osym = re_match(str, offset, node.left)
    if osym == -1
        return offset
    return osym

def re_match_plus ( str:ReText; offset:int; node:ReNode? ) : int
    var osym = re_match(str, offset, node.left)
    if osym == -1
        return -1
    var ofs = osym
    while osym != -1
        ofs = osym
        osym = re_match(str, ofs, node.left)
    return ofs

def re_match_star ( str:ReText; offset:int; node:ReNode? ) : int
    var ofs = offset
    var osym = re_match(str, offset, node.left)
    while osym != -1
        ofs = osym
        osym = re_match(str, ofs, node.left)
    return ofs

def re_match_group ( str:ReText; offset:int; node:ReNode? ) : int
    let osub = re_match(str,offset,node.left)
    if osub==-1
        return -1
    // todo: return group last match
    return osub

def re_match ( str:ReText; offset:int; node:ReNode? ) : int
    if node.op==ReOp Char
        return re_match_char(str,offset,node)
    elif node.op==ReOp Union
        return re_match_union(str,offset,node)
    elif node.op==ReOp Set
        return re_match_set(str,offset,node)
    elif node.op==ReOp NegativeSet
        return re_match_negative_set(str,offset,node)
    elif node.op==ReOp Any
        return re_match_any(str,offset)
    elif node.op==ReOp Eos
        return re_match_eos(str,offset)
    elif node.op==ReOp Concat
        return re_match_concat(str,offset,node)
    elif node.op==ReOp Plus
        return re_match_plus(str,offset,node)
    elif node.op==ReOp Star
        return re_match_star(str,offset,node)
    elif node.op==ReOp Question
        return re_match_question(str,offset,node)
    elif node.op==ReOp Group
        return re_match_group(str,offset,node)
    else
        panic("unsupported {node.op}")
    return -1

def regex_match ( str:ReText; node:ReNode? ) : int
    if empty(str)
        return -1
    return re_match(str,0,node)

/*
    printer
*/

def debug_re ( node: ReNode?; tab:int )
    if node==null
        print("null")
    elif node.op==ReOp Char
        print("(Char `{node.text}`)")
    elif node.op==ReOp Set
        print("(Set ")
        for x in range(0,256)
            if (node.cset[x>>5] & uint(1<<(x & 31))) != 0u
                print("{to_char(x)}")
        print(")")
    elif node.op==ReOp NegativeSet
        print("(Negative-Set ")
        for x in range(0,256)
            if (node.cset[x>>5] & uint(1<<(x & 31))) != 0u
                print("{to_char(x)}")
        print(")")
    elif node.op==ReOp Any
        print("(Any)")
    elif node.op==ReOp Eos
        print("(Eos)")
    elif node.op==ReOp Group
        print("(Group ")
        debug_re(node.left,tab+1)
        print(")")
    elif node.op==ReOp Plus
        print("(Plus ")
        debug_re(node.left,tab)
        print(")")
    elif node.op==ReOp Star
        print("(Star ")
        debug_re(node.left,tab)
        print(")")
    elif node.op==ReOp Question
        print("(Question ")
        debug_re(node.left,tab)
        print(")")
    elif node.op==ReOp Concat
        print("(Contact\n")
        print("{repeat("\t",tab)}")
        debug_re(node.left,tab+1)
        print("\n{repeat("\t",tab)}")
        debug_re(node.right,tab+1)
        print("\n{repeat("\t",tab-1)})")
    elif node.op==ReOp Union
        print("(Union\n")
        print("{repeat("\t",tab)}")
        debug_re(node.left,tab+1)
        print("\n")
        print("{repeat("\t",tab)}")
        debug_re(node.right,tab+1)
        print("\n{repeat("\t",tab-1)})")
    else
        panic("unsupported op")


def test_re ( reg:string; matches:array<string>; nonmatches:array<string> )
    print("testing {reg}\n")
    let res = re_parse(reg)
    if res==null
        print("failed to compile\n")
    print("re is:\n")
    debug_re(res,1)
    print("\n")
    var failed = false
    for m in matches
        let ma = regex_match(m, res)
        print("matching `{m}` -> {ma} {ma==-1 ? "failed" : "ok"}\n")
        failed |= ma==-1
    for m in nonmatches
        let ma = regex_match(m, res)
        print("matching `{m}` -> {ma} {ma!=-1 ? "failed" : "ok"}\n")
        failed |= ma!=-1
    print("{failed ? "failed" : "ok"}\n\n")
    assert(!failed)

[export,unsafe]
def test
     // character
    test_re("a", [{string[] "a"}], [{string ""; "b"; "ba"}])
    // union
    test_re("cat|dog", [{string "cat";"dog";"cats"}], [{string "at";" cat";"doog"}])
    // set & negatvie set
    test_re("[0-9a-zA-Z_]",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("[\\w]",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("\\w",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("[^0-9a-zA-Z_]",[{string "#";"*"}],[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}])
    // any, concat
    test_re("cat.", [{string "catt";"cats"}], [{string "cat";" cat"}])
    // eos
    test_re("cat$", [{string[] "cat"}], [{string "cattt";" cat"}])
    // +
    test_re("ab+", [{string "ab";"abb";"abbbc"}], [{string "a";" ab"; "bbb"}])
    test_re("(cat)+",[{string "cat";"catcat"}], [{string "dog";"caat"}])
    // *
    test_re("ab*", [{string "a";"ab";"abb";"abbbc"}], [{string " ab"; "bbb"}])
    test_re("a(cat)*",[{string "a";"acat";"acatcat"}], [{string "dog";"caat";"cat"}])
    // ?
    test_re("ab?", [{string "a";"ab";"abb"}], [{string "b";" ab"; "bbb"}])
    test_re("(cat)?x",[{string "catx";"x"}], [{string "dog";"caat"}])
    return true

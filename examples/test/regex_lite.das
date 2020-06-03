
//options log = true
//options log_stack = true
//options log_nodes = true
//options fusion = false

// options debugger = true

typedef
    ReText = string

enum ReOp
    Char
    Range
    Set
    NegativeSet
    Any
    Eos
    Group
    Plus
    Star
    Concat
    Union

struct ReNode
    op      : ReOp
    at      : range
    text    : string
    all     : array<ReNode?>
    left    : ReNode?
    right   : ReNode?

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

let
    meta = "\\+-*.()[]|^"

def is_meta_character(ch:int) : bool
    return find_first_of(meta,ch) != -1

// <char>	::=	any non metacharacter | "\" metacharacter
def re_char (expr : ReText; offset:int) : MaybeReNode
    trace("re_char",offset)
    if eos(expr,offset)
        return nada()
    let ch = at(expr,offset)
    if !is_meta_character(ch)
        return maybe(new [[ReNode op=ReOp Char, text=to_char(ch), at=range(offset,offset+1)]])
    if ch != '\\'
        return nada()
    if eos(expr,offset+1)
        return nada()
    let ech = at(expr,offset+1)
    return maybe(new [[ReNode op=ReOp Char, text=to_char(ech), at=range(offset,offset+2)]])

// <range>	::=	<char> "-" <char>
def re_range(expr:ReText; offset:int) : MaybeReNode
    trace("re_range",offset)
    if eos(expr,offset)
        return nada()
    var omin = re_char(expr,offset)
    if omin is nothing
        return nada()
    if at(expr,next(omin))!='-'
        delete omin
        return nada()
    let omax = re_char(expr,next(omin)+1)
    if omax is nothing
        delete omin
        return nada()
    return maybe(new [[ReNode op=ReOp Range, left=omin as value, right=omax as value, at=range(offset,next(omax))]])

// <set-item>	::=	<range> | <char>
// todo: optimize
def re_set_item(expr:ReText; offset:int) : MaybeReNode
    trace("re_set_item",offset)
    if eos(expr,offset)
        return nada()
    var orange = re_range(expr,offset)
    if orange is value
        return orange
    return re_char(expr, offset)

// <set-items>	::=	<set-item> | <set-item> <set-items>
def re_set_items(expr:ReText; offset:int) : MaybeReNode
    trace("re_set_items",offset)
    if eos(expr,offset)
        return nada()
    var items : array<ReNode?>
    var iofs = offset
    while !eos(expr,iofs)
        note("trying next set item",iofs)
        let oitem = re_set_item(expr,iofs)
        if oitem is nothing
            break
        push(items,oitem as value)
        iofs = next(oitem)
    note("finished with set items [{items}]",iofs)
    if length(items)==0
        delete items
        return nada()
    return maybe(new [[ReNode op=ReOp Set, all<-items, at=range(offset,iofs)]])

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
    var opos = re_positive_set(expr,offset)
    if opos is value
        return opos
    var oneg = re_negative_set(expr,offset)
    if oneg is value
        return oneg
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
    note("-----> in_group",offset)
    var ore = re_re(expr,offset+1)
    if ore is nothing
        return nada()
    note("-----> before )",offset)
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
        note("?",cofs)
        if at(expr,cofs)=='|'
            note("union",offset)
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
// todo: optimize
def re_basic(expr:ReText; offset:int) : MaybeReNode
    trace("re_basic",offset)
    if eos(expr,offset)
        return nada()
    var ostar = re_star(expr,offset)
    if ostar is value
        return ostar
    var oplus = re_plus(expr,offset)
    if oplus is value
        return oplus
    var oelem = re_elementary(expr,offset)
    if oelem is value
        return oelem
    return nada()

// <star>	::=	<elementary-RE> "*"
def re_star(expr:ReText; offset:int) : MaybeReNode
    trace("re_star",offset)
    if eos(expr,offset)
        return nada()
    var oel = re_elementary(expr,offset)
    if oel is nothing
        return nada()
    if at(expr,next(oel))!='*'
        delete oel
        return nada()
    return maybe(new [[ReNode op=ReOp Star, left=oel as value, at=range(offset,next(oel)+1)]])

// <plus>	::=	<elementary-RE> "+"
def re_plus(expr:ReText; offset:int) : MaybeReNode
    trace("re_plus",offset)
    if eos(expr,offset)
        return nada()
    var oel = re_elementary(expr,offset)
    if oel is nothing
        return nada()
    if at(expr,next(oel))!='+'
        delete oel
        return nada()
    return maybe(new [[ReNode op=ReOp Plus, left=oel as value, at=range(offset,next(oel)+1)]])

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

def re_match_char ( str:ReText; offset:int; text:string; len:int ) : int
    for Ch,Ofs in text,range(offset,offset+len)
        if at(str,Ofs) != Ch
            return -1
    return offset + len

def re_match_single_char ( str:ReText; offset:int; sc:int ) : int
    let Ch = at(str,offset)
    if Ch != sc
        return -1
    return offset + 1

def re_match_range ( str:ReText; offset:int; sub:range ) : int
    let Ch = at(str,offset)
    if Ch<sub.x | Ch>sub.y
        return -1
    return offset + 1

def re_match_union ( str:ReText; offset:int; node:ReNode? ) : int
    let left = re_match(str,offset,node.left)
    if left != -1
        return left
    let right = re_match(str,offset,node.right)
    if right != -1
        return right
    return -1

def re_match_set ( str:ReText; offset:int; node:ReNode? ) : int
    if eos(str,offset)
        return -1
    for sub in node.all
        let any = re_match(str, offset, sub)
        if any != -1
            return any
    return -1

def re_match_negative_set ( str:ReText; offset:int; node:ReNode? ) : int
    if eos(str,offset)
        return -1
    for sub in node.all
        if re_match(str, offset, sub) != -1
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
        let len = length(node.text)
        if len == 1
            let sc = at(node.text,0)
            return re_match_single_char(str,offset,sc)
        else
            return re_match_char(str,offset,node.text,len)
    elif node.op==ReOp Range
        var sub : range
        sub.x = at(node.left.text,0)
        sub.y = at(node.right.text,0)
        return re_match_range(str,offset,sub)
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
    elif node.op==ReOp Range
        print("(Range ")
        debug_re(node.left,tab)
        print(" ")
        debug_re(node.right,tab)
        print(")")
    elif node.op==ReOp Set
        print("(Set")
        for subs in node.all
            print("\n{repeat("\t",tab)}")
            debug_re(subs,tab+1)
        print("\n{repeat("\t",tab-1)})")
    elif node.op==ReOp NegativeSet
        print("(Negative-Set")
        for subs in node.all
            print("\n{repeat("\t",tab)}")
            debug_re(subs,tab+1)
        print("\n{repeat("\t",tab-1)})")
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
    return true

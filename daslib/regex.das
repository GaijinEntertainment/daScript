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
    id      : int
    fun2    : function< (var regex:Regex; var node,node2:ReNode?; str:string; offset:int) : int >
    at      : range
    text    : string
    all     : array<ReNode?>
    left    : ReNode?
    right   : ReNode?
    subexpr : ReNode?
    next    : ReNode?
    cset    : CharSet
    index   : int
    tail    : int

struct Regex
    root    : ReNode?
    groups  : array<tuple<range;string>>

variant MaybeReNode
    value   : ReNode?
    nothing : void?

/*
    Parser
*/

let
    log_parse_enabled = false
    log_match_enabled = false
    range_check_enabled = false

var
    trace_tab = 0

[private]
def trace(msg:string; offset:int)
    static_if log_parse_enabled
        print("{repeat(" ",trace_tab)}{offset}: {msg}\n")
        trace_tab ++

[private]
def note(msg:string; offset:int)
    static_if log_parse_enabled
        print("{repeat(" ",trace_tab)}{offset}: {msg}\n")

[private]
def trace_backtrack()
    static_if log_parse_enabled
        trace_tab --
        print("{repeat(" ",trace_tab)}backtrack\n")

[private]
def trace_value(value : ReNode?)
    static_if log_parse_enabled
        if value==null
            panic("trace_value null?")
        trace_tab --
        print("{repeat(" ",trace_tab)}matched {value.op}, next {value.at.y}\n")

[private]
def nada()
    trace_backtrack()
    return [[MaybeReNode nothing=null]]

[private]
def maybe( value : ReNode? )
    trace_value(value)
    return [[MaybeReNode value=value]]

[private]
def eos(expr:string; offset:int)
    return offset >= length(expr)

[private,unsafe]
def at(expr:string; offset:int) : int
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

[private]
def next(re:MaybeReNode)
    if !(re is value)
        stackwalk()
        panic("expecting value")
    return (re as value).at.y

[private]
def set_or_char(var cset:CharSet; ch:int)
    cset[ch>>5] |= uint(1 << (ch & 31))

[private]
def set_or_range(var cset:CharSet; bits:range)
    for ch in range(bits.x,bits.y+1)
        cset[ch>>5] |= uint(1 << (ch & 31))

[private]
def set_invert(var cset:CharSet)
    for x in cset
        x ^= 0xffffffff

[private]
def set_or_set(var cset:CharSet; eset:CharSet)
    for x,y in cset,eset
        x |= y

[private]
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

[private]
def is_meta_character(ch:int) : bool
    return find_first_of(meta,ch) != -1

[private]
def is_set_character(ch:int) : bool
    return find_first_of(meta_set,ch) != -1

// <char>	::=	any non metacharacter | "\" metacharacter
[private]
def re_char (expr : string; offset:int) : MaybeReNode
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
[private]
def re_set_items(expr:string; offset:int) : MaybeReNode
    trace("re_set_items",offset)
    if eos(expr,offset)
        return nada()
    var cset : CharSet
    var iofs = offset
    while !eos(expr,iofs)
        let ch = at(expr,iofs)
        if ch == ']'
            note("set - break - ]",iofs)
            break
        elif ch=='\\'
            let che = at(expr,iofs+1)
            if che==0
                note("syntax error, expecting meta", iofs)
                return nada()
            set_meta(cset,che)
            note("set - meta",iofs)
            iofs += 2
        elif at(expr,iofs+1)=='-' & at(expr,iofs+2)!=']'
            let che = at(expr,iofs+2)
            if che==0
                note("syntax error, expecting range", iofs+1)
                return nada()
            elif ch > che
                note("syntax error, character range out of order", iofs)
                return nada()
            set_or_range(cset,range(ch,che))
            note("set - range",iofs)
            iofs += 3
        else
            set_or_char(cset,ch)
            note("set - character",iofs)
            iofs += 1
    return maybe(new [[ReNode op=ReOp Set, cset=cset, at=range(offset,iofs)]])

// <positive-set>	::=	"[" <set-items> "]"
[private]
def re_positive_set(expr:string; offset:int) : MaybeReNode
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
[private]
def re_negative_set(expr:string; offset:int) : MaybeReNode
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
[private]
def re_set(expr:string; offset:int) : MaybeReNode
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
[private]
def re_any(expr:string; offset:int) : MaybeReNode
    trace("re_any",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)=='.'
        return maybe(new [[ReNode op=ReOp Any, at=range(offset,offset+1)]])
    return nada()

// <eos>	::=	"$"
[private]
def re_eos(expr:string; offset:int) : MaybeReNode
    trace("re_eos",offset)
    if eos(expr,offset)
        return nada()
    if at(expr,offset)=='$'
        return maybe(new [[ReNode op=ReOp Eos, at=range(offset,offset+1)]])
    return nada()

// <group>	::=	"(" <RE> ")"
[private]
def re_group(expr:string; offset:int) : MaybeReNode
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
    return maybe(new [[ReNode op=ReOp Group, subexpr=ore as value, at=range(offset,next(ore)+1)]])

// <elementary-RE>	::=	<group> | <any> | <eos> | <set> | <char>
[private]
def re_elementary(expr:string; offset:int) : MaybeReNode
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

[private]
def mk_concat(var left,right:ReNode?)
    if left == null
        return right
    elif left.op==ReOp Char & right.op==ReOp Char
        // mk_concat(char(x),char(y)) = char(xy)
        left.at.y = right.at.y
        left.text += right.text
        delete right
        return left
    elif left.op==ReOp Concat & left.right.op==ReOp Char & right.op==ReOp Char
        // mk_concat(concat(z,char(x)),char(y)) = concat(z,char(xy))
        var lor = left.right
        left.at.y = right.at.y
        lor.at.y = right.at.y
        lor.text += right.text
        delete right
        return left
    else
        return new [[ReNode op=ReOp Concat, left = left, right = right, at=range(left.at.x,right.at.y)]]

[private]
def mk_union(var left,right:ReNode?) : ReNode?
    if left.op==ReOp Union
        left.at.y = right.at.y
        if right.op==ReOp Union                    // union(union(x),union(y)) = union(xy)
            for x in right.all
                push(left.all,x)
            delete right
        else                                       // union(union(x),y) = union(xy)
            push(left.all,right)
        return left
    elif right.op==ReOp Union
        right.at.x = left.at.x
        push(right.all,left)                      // union(x,union(y)) = union(xy)
        return right
    else
        return new [[ReNode op=ReOp Union, all <- [{ReNode? left; right}], at=range(left.at.x,right.at.y)]]

// <RE>	::=	<union> | <simple-RE>
// <union>	::=	<RE> "|" <simple-RE>
// <simple-RE>	::=	<concatenation> | <basic-RE>
// <concatenation>	::=	<simple-RE> <basic-RE>
[private]
def re_re(expr:string; offset:int) : MaybeReNode
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
            var oright = re_re(expr,cofs+1)
            if oright is nothing
                note("syntax error, re | nada", cofs+1)
                return nada()
            last = mk_union(last,oright as value)
            cofs = last.at.y
        else
            var onext = re_basic(expr, cofs)
            if onext is nothing
                return last==null? nada() : maybe(last)
            last = mk_concat(last,onext as value)
            cofs = last.at.y
    return maybe(last)

// <basic-RE>	::=	<star> | <plus> | <elementary-RE>
// <star>	::=	<elementary-RE> "*"
// <plus>	::=	<elementary-RE> "+"
[private]
def re_basic(expr:string; offset:int) : MaybeReNode
    trace("re_basic",offset)
    if eos(expr,offset)
        return nada()
    var oelem = re_elementary(expr,offset)
    if oelem is nothing
        return nada()
    let ch = at(expr,next(oelem))
    if ch=='*'
        return maybe(new [[ReNode op=ReOp Star, subexpr=oelem as value, at=range(offset,next(oelem)+1)]])
    elif ch=='+'
        return maybe(new [[ReNode op=ReOp Plus, subexpr=oelem as value, at=range(offset,next(oelem)+1)]])
    elif ch=='?'
        return maybe(new [[ReNode op=ReOp Question, subexpr=oelem as value, at=range(offset,next(oelem)+1)]])
    return oelem

[private]
def re_parse ( expr:string ) : ReNode?
    var ore = re_re(expr, 0)
    if ore is nothing
        return null
    let at = next(ore)
    if at != length(expr)
        note("syntax error",at)
        return null
    return ore as value

[private]
def visit_top_down ( var node:ReNode?; blk : block<(var n:ReNode?):void> )
    invoke(blk,node)
    for x in node.all
        visit_top_down(x, blk)
    if node.subexpr!=null
        visit_top_down(node.subexpr, blk)
    if node.left!=null
        visit_top_down(node.left, blk)
    if node.right!=null
        visit_top_down(node.right, blk)

[private]
def re_assign_next ( var re:Regex )
    var id = 0
    visit_top_down(re.root) <| $ ( var node )
        node.id = id ++
        if node.op == ReOp Concat
            node.left.next = node.right
            node.right.next = node.next
        elif node.op == ReOp Group
            node.subexpr.next = node.next
        elif node.op == ReOp Union
            for sub in node.all
                sub.next = node.next

[private]
def re_assign_groups ( var re:Regex )
    push(re.groups, [[auto range(0,0),""]])
    visit_top_down(re.root) <| $ ( var node )
        if node.op == ReOp Group
            let index = length(re.groups)
            node.index = index
            push(re.groups, [[auto range(0,0),"{index}"]])

/*
    matching
*/

[private]
def re_assign_match_functions ( var re:Regex )
    visit_top_down(re.root) <| $ ( var node )
        if node.op==ReOp Char
            if length(node.text)==1
                node.fun2 = @@re_match2_single_char
            else
                node.fun2 = @@re_match2_char
        elif node.op==ReOp Union
            node.fun2 = @@re_match2_union
        elif node.op==ReOp Set
            node.fun2 = @@re_match2_set
        elif node.op==ReOp NegativeSet
            node.fun2 = @@re_match2_negative_set
        elif node.op==ReOp Any
            node.fun2 = @@re_match2_any
        elif node.op==ReOp Eos
            node.fun2 = @@re_match2_eos
        elif node.op==ReOp Concat
            node.fun2 = @@re_match2_concat
        elif node.op==ReOp Plus
            node.fun2 = @@re_match2_plus
        elif node.op==ReOp Star
            node.fun2 = @@re_match2_star
        elif node.op==ReOp Question
            node.fun2 = @@re_match2_question
        elif node.op==ReOp Group
            node.fun2 = @@re_match2_group
        else
            panic("unsupported {node.op}")

[private]
def re_match2_single_char ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match single char `{node.text}` {offset}\n")
    if at(node.text,0) != at(str,offset)
        return -1
    node.tail = offset + 1
    if node2!=null
        return re_match(regex,node2,str,offset+1)
    else
        return offset+1

[private]
def re_match2_char ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match char `{node.text}` {offset}\n")
    let len = length(node.text)
    for Ch,Ofs in node.text,range(offset,offset+len)
        if at(str,Ofs) != Ch
            return -1
    node.tail = offset + len
    if node2!=null
        return re_match(regex,node2,str,offset+len)
    else
        return offset+len

[private]
def re_match2_union ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match union {offset}\n")
    for sub in node.all
        let osub = re_match(regex,sub,str,offset)
        if osub != -1
            node.tail = sub.tail
            return osub
    return -1

[private]
def re_match2_set ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 set {offset}\n")
    let ch = at(str,offset)
    if (node.cset[ch>>5] & uint(1 << (ch & 31))) == 0u
        return -1
    node.tail = offset + 1
    if node2!=null
        return re_match(regex, node2, str, offset+1)
    else
        return offset + 1


[private]
def re_match2_negative_set ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match negative set {offset}\n")
    let ch = at(str,offset)
    if (node.cset[ch>>5] & uint(1 << (ch & 31))) != 0u
        return -1
    node.tail = offset + 1
    if node2!=null
        return re_match(regex, node2, str, offset+1)
    else
        return offset + 1

[private]
def re_match2_any ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 any {offset}\n")
    if at(str,offset)==0
        return -1
    node.tail = offset + 1
    if node2!=null
        return re_match(regex, node2, str, offset+1)
    else
        return offset + 1

[private]
def re_match2_concat ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 concat {offset}\n")
    let oleft = re_match(regex, node.left, str, offset)
    if oleft == -1
        return -1
    node.tail = node.right.tail
    return oleft


[private]
def re_match2_eos ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match eos {offset}\n")
    if at(str,offset)!=0
        return -1
    node.tail = offset
    if node2!=null
        return re_match(regex, node2, str, offset)
    else
        return offset

[private]
def re_match2_question ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match question {offset}\n")
    var osym = re_match(regex,node.subexpr,str,offset)
    if osym == -1
        osym = offset
    node.tail = osym
    if node2!=null
        return re_match(regex,node2,str,osym)
    else
        return osym

[private]
def re_match2_plus ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 plus {offset}\n")
    var osym = re_match(regex,node.subexpr,str,offset)
    if osym == -1
        return -1
    var ofs = osym
    if node2!=null
        while osym != -1
            node.tail = osym
            var otail = re_match(regex,node2,str,osym)
            if otail != -1
                return otail
            osym = re_match(regex,node.subexpr,str,osym)
        return -1
    else
        while osym != -1
            ofs = osym
            osym = re_match(regex,node.subexpr,str,ofs)
        node.tail = ofs
        return ofs

[private]
def re_match2_star ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 plus {offset}\n")
    var ofs = offset
    if node2!=null
        while ofs != -1
            node.tail = ofs
            let oany = re_match(regex,node2,str,ofs)
            if oany != -1
                return oany
            ofs = re_match(regex,node.subexpr,str,ofs)
        return -1
    else
        var osym = ofs
        while osym != -1
            ofs = osym
            osym = re_match(regex,node.subexpr,str,ofs)
        node.tail = ofs
        return ofs

[private]
def re_match2_group ( var regex:Regex; var node,node2:ReNode?; str:string; offset:int ) : int
    if log_match_enabled
        print("match2 group {offset}\n")
    let osub = re_match(regex,node.subexpr,str,offset)
    if osub==-1
        return -1
    regex.groups[node.index]._0 = range(offset,node.subexpr.tail)
    return osub

[private]
def re_match ( var regex:Regex; var node:ReNode?;  str:string; offset:int ) : int
    return invoke(node.fun2, regex, node, node.next, str, offset)

/*
    top level API
*/

def regex_compile( var re:Regex; expr:string ) : bool
    re.root = re_parse(expr)
    if re.root != null
        re_assign_next(re)
        re_assign_groups(re)
        re_assign_match_functions(re)
    return re.root != null

def regex_match ( var regex:Regex; str:string; offset:int=0 ) : int
    if empty(str)
        return -1
    if log_match_enabled
        print("matching with `{str}` at {offset}\n")
    return re_match(regex,regex.root,str,offset)

def regex_group ( regex:Regex; index:int; match:string )
    let sub_range = regex.groups[index]._0
    return slice(match, sub_range.x, sub_range.y)

def regex_foreach ( var regex:Regex; str:string; blk : block<(at:range):bool> )
    if empty(str)
        return
    var offset = 0
    while at(str,offset) != 0
        let om = re_match(regex,regex.root,str,offset)
        if om != -1
            if !invoke(blk,range(offset,om))
                break
            offset = om
        else
            offset ++

/*
    printer
*/

def regex_debug ( regex:Regex )
    debug_re(regex.root,1)
    print("\n")
    if length(regex.groups) != 0
        print("groups:")
        for g in regex.groups
            print("\t{g._1}\n")

def debug_set(cset:CharSet)
    for x in range(0,256)
        if (cset[x>>5] & uint(1<<(x & 31))) != 0u
            print("{to_char(x)}")

[private]
def debug_re ( node: ReNode?; tab:int )
    print("(")
    if node != null
        print("#{node.id}")
        if node.next!=null
            print("->{node.next.id}")
        else
            print("->!")
        if node.tail != 0
            print(" [G{node.tail}]")
        print(" ")
    if node==null
        print("null)")
    elif node.op==ReOp Char
        print("Char `{node.text}`)")
    elif node.op==ReOp Set
        print("Set ")
        debug_set(node.cset)
        print(")")
    elif node.op==ReOp NegativeSet
        print("Negative-Set ")
        debug_set(node.cset)
        print(")")
    elif node.op==ReOp Any
        print("Any)")
    elif node.op==ReOp Eos
        print("Eos)")
    elif node.op==ReOp Group
        print("Group ")
        debug_re(node.subexpr,tab+1)
        print(")")
    elif node.op==ReOp Plus
        print("Plus ")
        debug_re(node.subexpr,tab)
        print(")")
    elif node.op==ReOp Star
        print("Star ")
        debug_re(node.subexpr,tab)
        print(")")
    elif node.op==ReOp Question
        print("Question ")
        debug_re(node.subexpr,tab)
        print(")")
    elif node.op==ReOp Concat
        print("Contact")
        print("\n{repeat("\t",tab)}")
        debug_re(node.left,tab+1)
        print("\n{repeat("\t",tab)}")
        debug_re(node.right,tab+1)
        print("\n{repeat("\t",tab-1)})")
    elif node.op==ReOp Union
        print("Union\n")
        for sub in node.all
            print("{repeat("\t",tab)}")
            debug_re(sub,tab+1)
            print("\n")
        print("{repeat("\t",tab-1)})")
    else
        panic("unsupported op")

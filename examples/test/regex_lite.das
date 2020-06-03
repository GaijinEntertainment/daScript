
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

[unsafe]
def nada()
    return [[MaybeReNode nothing=null]]

def maybe( value : ReNode? )
    return [[MaybeReNode value=value]]

[unsafe]
def at(expr:ReText; offset:int) : int
    return character_uat(expr,offset)

def next(re:MaybeReNode)
    if !(re is value)
        stackwalk()
        panic("expecting value")
    return (re as value).at.y

let
    meta = "+-*.()[]|^"

def is_meta_character(ch:int) : bool
    return find_first_of(meta,ch) != -1

// <char>	::=	any non metacharacter | "\" metacharacter
def re_char (expr : ReText; offset:int)
    let ch = at(expr,offset)
    if !is_meta_character(ch)
        return maybe(new [[ReNode op=ReOp Char, text=to_char(ch), at=range(offset,offset+1)]])
    let ech = at(expr,offset+1)
    if ech!=0
        return maybe(new [[ReNode op=ReOp Char, text=to_char(ech), at=range(offset,offset+2)]])
    return nada()

// <range>	::=	<char> "-" <char>
def re_range(expr:ReText; offset:int)
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
def re_set_item(expr:ReText; offset:int)
    let orange = re_range(expr,offset)
    if orange is value
        return orange
    return re_char(expr, offset)

// <set-items>	::=	<set-item> | <set-item> <set-items>
def re_set_items(expr:ReText; offset:int)
    var items : array<ReNode?>
    var iofs = offset
    while true
        let oitem = re_set_item(expr,iofs)
        if oitem is nothing
            break
        push(items,oitem as value)
        iofs = next(oitem)
    if length(items)==0
        delete items
        return nada()
    return maybe(new [[ReNode op=ReOp Set, all<-items, at=range(offset,iofs)]])

// <positive-set>	::=	"[" <set-items> "]"
def re_positive_set(expr:ReText; offset:int)
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
    return oset

// <negative-set>	::=	"[^" <set-items> "]"
def re_negative_set(expr:ReText; offset:int)
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
    return oset

// <set>	::=	<positive-set> | <negative-set>
// todo: optimize
def re_set(expr:ReText; offset:int)
    let opos = re_positive_set(expr,offset)
    if opos is value
        return opos
    return re_negative_set(expr,offset)

// <any>	::=	"."
def re_any(expr:ReText; offset:int)
    if at(expr,offset)=='.'
        return maybe(new [[ReNode op=ReOp Any, at=range(offset,offset+1)]])
    return nada()

// <eos>	::=	"$"
def re_eos(expr:ReText; offset:int)
    if at(expr,offset)=='$'
        return maybe(new [[ReNode op=ReOp Eos, at=range(offset,offset+1)]])
    return nada()

// <group>	::=	"(" <RE> ")"
def re_group(expr:ReText; offset:int)
    if at(expr,offset)!='('
        return nada()
    var ore = re_re(expr,offset+1)
    if ore is nothing
        return nada()
    if at(expr,next(ore))!=')'
        delete ore
        return nada()
    return maybe(new [[ReNode op=ReOp Group, left=ore as value, at=range(offset,next(ore)+1)]])

// <elementary-RE>	::=	<group> | <any> | <eos> | <char> | <set>
def re_elementary(expr:ReText; offset:int)
    let ogr = re_group(expr,offset)
    if ogr is value
        return ogr
    let oany = re_any(expr,offset)
    if oany is value
        return oany
    let oeos = re_eos(expr,offset)
    if oeos is value
        return oeos
    let oset = re_set(expr,offset)
    if oset is value
        return oset
    let ochr = re_char(expr,offset)
    if ochr is value
        debug(ochr,"ochr=")
        return ochr
    return nada()

// <RE>	::=	<union> | <simple-RE>
// <union>	::=	<RE> "|" <simple-RE>
// <simple-RE>	::=	<concatenation> | <basic-RE>
// <concatenation>	::=	<simple-RE> <basic-RE>
def re_re(expr:ReText; offset:int)
    var cofs = offset
    var last : ReNode?
    while at(expr,cofs)!= 0
        let oleft = re_basic(expr, cofs)
        if oleft is nothing
            // TODO: syntax error
            return nada()
        if at(expr,next(oleft))=='|'
            let oright = re_basic(expr,next(oleft)+1)
            if oright is nothing
                let uni = new [[ReNode op=ReOp Union, left=oleft as value, right=oright as value, at=range(cofs,next(oright))]]
                return nada()
            cofs = next(oright)
        else
            if last != null
                last = new [[ReNode op=ReOp Concat, left=last, right=oleft as value, at=range(cofs,next(oleft))]]
            else
                last = oleft as value
            cofs = next(oleft)
    return maybe(last)

// <basic-RE>	::=	<star> | <plus> | <elementary-RE>
// todo: optimize
def re_basic(expr:ReText; offset:int)
    let ostar = re_star(expr,offset)
    if ostar is value
        return ostar
    let oplus = re_plus(expr,offset)
    if oplus is value
        return oplus
    return re_elementary(expr,offset)

// <star>	::=	<elementary-RE> "*"
def re_star(expr:ReText; offset:int)
    var oel = re_elementary(expr,offset)
    debug(oel,"oel=")
    if oel is nothing
        return nada()
    if at(expr,next(oel))!='*'
        delete oel
        return nada()
    return maybe(new [[ReNode op=ReOp Star, left=oel as value, at=range(offset,next(oel)+1)]])

// <plus>	::=	<elementary-RE> "+"
def re_plus(expr:ReText; offset:int)
    var oel = re_elementary(expr,offset)
    if oel is nothing
        return nada()
    if at(expr,next(oel))!='+'
        delete oel
        return nada()
    return maybe(new [[ReNode op=ReOp Plus, left=oel as value, at=range(offset,next(oel)+1)]])

def debug_re ( expr: MaybeReNode )
    if expr is nothing
        print("nothing\n")

[export,unsafe]
def test
    let sample = "(cat|dog)([0-9]*)"
    let res = re_re(sample, 0)
    // print("res = {res}\n")
    debug_re(res)
    return true

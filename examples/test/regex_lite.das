// options log = true

options debugger = true

typedef
    ReText = string

[unsafe]
def at(expr:ReText; offset:int) : int
    return character_uat(expr,offset)

let
    meta = "+-*.()[]|^"

def is_meta_character(ch:int) : bool
    return find_first_of(meta,ch) != -1

// <char>	::=	any non metacharacter | "\" metacharacter
def re_char(expr:ReText; offset:int) : int
    let ch = at(expr,offset)
    if !is_meta_character(ch)
        return offset + 1
    elif ch=='\\' & at(expr,offset+1)!=0
        return offset + 2
    else
        return -1

// <range>	::=	<char> "-" <char>
def re_range(expr:ReText; offset:int) : int
    let omin = re_char(expr, offset)
    if omin==-1
        return -1
    if at(expr,omin)!='-'
        return -1
    return re_char(expr, omin + 1)

// <set-item>	::=	<range> | <char>
def re_set_item(expr:ReText; offset:int) : int
    let orange = re_range(expr,offset)
    if orange != -1
        return orange
    return re_char(expr, offset)

// <set-items>	::=	<set-item> | <set-item> <set-items>
def re_set_items(expr:ReText; offset:int) : int
    var oitem = offset
    var litem : int
    while oitem != -1
        litem = oitem
        oitem = re_set_item(expr,oitem)
    return offset!=litem ? litem : -1

// <positive-set>	::=	"[" <set-items> "]"
def re_positive_set(expr:ReText; offset:int) : int
    if at(expr,offset) != '['
        return -1
    let oset = re_set_items(expr,offset+1)
    if oset==-1
        return -1
    if at(expr,oset) != ']'
        return -1
    return oset + 1

// <negative-set>	::=	"[^" <set-items> "]"
def re_negative_set(expr:ReText; offset:int) : int
    if at(expr,offset)!='[' | at(expr,offset+1)!='^'
        return -1
    let oset = re_set_items(expr,offset+2)
    if oset==-1
        return -1
    if at(expr,oset) != ']'
        return -1
    return oset + 1

// <set>	::=	<positive-set> | <negative-set>
def re_set(expr:ReText; offset:int) : int
    let opos = re_positive_set(expr,offset)
    if opos != -1
        return opos
    return re_negative_set(expr,offset)

// <any>	::=	"."
def re_any(expr:ReText; offset:int) : int
    return at(expr,offset)=='.' ? offset + 1 : -1

// <eos>	::=	"$"
def re_eos(expr:ReText; offset:int) : int
    return at(expr,offset)=='$' ? offset + 1 : -1

// <group>	::=	"(" <RE> ")"
def re_group(expr:ReText; offset:int) : int
    if at(expr,offset)!='('
        return -1
    let ore = re_re(expr,offset+1)
    if ore==-1
        return -1
    if at(expr,ore)!=')'
        return -1
    return ore + 1

// <elementary-RE>	::=	<group> | <any> | <eos> | <char> | <set>
def re_elementary(expr:ReText; offset:int) : int
    let ogr = re_group(expr,offset)
    if ogr!=-1
        return ogr
    let oany = re_any(expr,offset)
    if oany!=-1
        return oany
    let oeos = re_eos(expr,offset)
    if oeos!=-1
        return oeos
    let oset = re_set(expr,offset)
    if oset!=-1
        return oset
    let ochr = re_char(expr,offset)
    if ochr!=-1
        return ochr
    return -1

// <RE>	::=	<union> | <simple-RE>
// <union>	::=	<RE> "|" <simple-RE>
// <simple-RE>	::=	<concatenation> | <basic-RE>
// <concatenation>	::=	<simple-RE> <basic-RE>
def re_re(expr:ReText; offset:int) : int
    var cofs = offset
    while at(expr,cofs)!= 0
        let bofs = re_basic(expr, cofs)
        if bofs == -1
            return cofs     // syntax error
        if at(expr,bofs)=='|'
            let brofs = re_basic(expr,bofs+1)
            if brofs==-1
                return -1   // syntax error
            // append union
            cofs = brofs
        else
            // append concatination
            cofs = bofs
    return cofs

// <basic-RE>	::=	<star> | <plus> | <elementary-RE>
def re_basic(expr:ReText; offset:int) : int
    let ostar = re_star(expr,offset)
    if ostar!=-1
        return ostar
    let oplus = re_plus(expr,offset)
    if oplus!=-1
        return oplus
    return re_elementary(expr,offset)

// <star>	::=	<elementary-RE> "*"
def re_star(expr:ReText; offset:int) : int
    let oel = re_elementary(expr,offset)
    if oel==-1
        return -1
    return at(expr,oel)=='*' ? oel+1 : -1

// <plus>	::=	<elementary-RE> "+"
def re_plus(expr:ReText; offset:int) : int
    let oel = re_elementary(expr,offset)
    if oel==-1
        return -1
    return at(expr,oel)=='+' ? oel+1 : -1


[export,unsafe]
def test

    let sample = "[a-zA-Z]+side"

    let res = re_re(sample, 0)
    print("res = {res}\n")

    return true

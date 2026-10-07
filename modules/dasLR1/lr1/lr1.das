options gen2
options indenting = 4
options strict_smart_pointers = true

module lr1 shared public

require daslib/strings_boost

// An LALR(1) parser generator over BYTE terminals, built at run time from a grammar a program assembles
// (`GrammarBuilder`) or reads from text (`lr1/gbnf`), and an incremental acceptor over its tables: feed one
// byte, ask which bytes may come next, probe a whole byte string on a copy of the stack. The acceptor has
// the viable-prefix property - a byte is refused the moment no sentence of the grammar extends the prefix -
// so a sampler masking tokens through it never emits a prefix it must later undo.

//! A set of bytes: one flag per byte value.
typedef ByteSet = bool[256]

//! A symbol on a production's right-hand side before the grammar is finalized: a byte set, or a rule.
struct GrammarSym {
    is_rule : bool
    rule : int          //!< the rule id when is_rule
    bytes : ByteSet     //!< the terminal's byte set otherwise
}

//! One production over finalized symbols: terminals are byte-class ids `0 .. nclasses-1`, rules are
//! encoded `-(1 + rule)`.
struct Production {
    lhs : int
    rhs : array<int>
}

//! A finalized grammar with its LALR(1) tables. `action[state * (nclasses + 1) + cls]`: 0 = error,
//! `1 + s` = shift to state s, `-(1 + p)` = reduce by production p; the EOF column is `cls == nclasses`,
//! where `ACCEPT` marks acceptance. `goto_nt[state * nrules + rule]` = the state after a reduce, or -1. A
//! cell with more than one action keeps the others in `alt` (keyed by the cell index): the grammar is not
//! LALR(1) there and the acceptor forks its stack, so an ambiguous grammar still accepts exactly its
//! language; `conflicts` counts such cells and `first_conflict` names the first.
struct Grammar {
    rule_names : array<string>
    prods : array<Production>
    byte_class : array<int>     //!< 256 entries: byte -> terminal class
    nclasses : int
    nstates : int
    action : array<int>
    alt : table<int; array<int>>
    goto_nt : array<int>
    root : int                  //!< the rule the augmented start produces
    conflicts : int
    first_conflict : string
    err : string                //!< "" when the build succeeded, else why not (a reachable rule that derives no terminal string)
}

let ACCEPT = 2147483647   //!< the EOF-column action that marks acceptance

//! The acceptor's state: the set of live LR stacks - one for a deterministic grammar, several while an
//! ambiguous grammar's alternatives are all still open. A copy for a probe clones the set.
struct LrStack {
    stacks : array<array<int>>
}

//! The most stacks the acceptor keeps alive; past it the acceptor refuses, so a pathological grammar
//! cannot grow without bound.
let LR_MAX_STACKS = 256

// ===== building =====

//! Assembles a grammar: rules by name, productions as sequences of byte sets and rule references.
struct GrammarBuilder {
    rule_names : array<string>
    rule_index : table<string; int>
    prods : array<tuple<lhs : int; rhs : array<GrammarSym>>>
}

//! The id of rule `name`, declared on first mention.
def rule_id(var gb : GrammarBuilder; name : string) : int {
    if (gb.rule_index |> key_exists(name)) {
        return gb.rule_index[name]
    }
    let id = length(gb.rule_names)
    gb.rule_names |> push(name)
    gb.rule_index[name] = id
    return id
}

def sym_rule(rule : int) : GrammarSym => GrammarSym(is_rule = true, rule = rule)

def sym_byte(b : int) : GrammarSym {
    var s : GrammarSym
    s.bytes[b] = true
    return s
}

def sym_bytes(bytes : ByteSet) : GrammarSym {
    var s : GrammarSym
    s.bytes = bytes
    return s
}

//! Add `lhs -> rhs`; an empty `rhs` is the epsilon production.
def add_production(var gb : GrammarBuilder; lhs : int; var rhs : array<GrammarSym>) {
    gb.prods |> emplace((lhs = lhs, rhs <- rhs))
}

def byteset_empty(bs : ByteSet) : bool {
    for (i in range(256)) {
        if (bs[i]) {
            return false
        }
    }
    return true
}

// the finest partition of the byte alphabet that every terminal set of the grammar is a union of:
// refine a single class by each set in turn
def private byte_classes(gb : GrammarBuilder; var cls : array<int>) : int {
    cls |> resize(256)
    for (i in range(256)) {
        cls[i] = 0
    }
    var n = 1
    for (p in gb.prods) {
        for (s in p.rhs) {
            continue if (s.is_rule)
            // every class split by this set: members inside keep a fresh id per old class
            var remap : table<int; int>
            for (b in range(256)) {
                continue if (!s.bytes[b])
                let old = cls[b]
                if (!(remap |> key_exists(old))) {
                    // does the old class have members outside the set? if not, no split
                    var outside = false
                    for (c in range(256)) {
                        if (cls[c] == old && !s.bytes[c]) {
                            outside = true
                            break
                        }
                    }
                    remap[old] = outside ? n : old
                    if (outside) {
                        n++
                    }
                }
                cls[b] = remap[old]
            }
        }
    }
    // the classes are numbered by first appearance; compact them to 0..n-1 in byte order
    var seen : table<int; int>
    var next = 0
    for (b in range(256)) {
        if (!(seen |> key_exists(cls[b]))) {
            seen[cls[b]] = next
            next++
        }
        cls[b] = seen[cls[b]]
    }
    return next
}

// ----- LR(0) items and the canonical collection -----

struct private Item {
    prod : int
    dot : int
}

def private item_key(it : Item) : int64 => int64(it.prod) * 4096l + int64(it.dot)   // 64-bit: a grammar past 500k productions still keys apart

struct private LrState {
    kernel : array<Item>
    la : array<table<int>>          //!< per kernel item: lookahead terminal classes (EOF = nclasses)
    trans : table<int; int>         //!< symbol -> state
}

struct private Builder {
    prods : array<Production>
    nrules : int
    nclasses : int
    nullable : array<bool>
    first : array<table<int>>       //!< per rule: terminal classes that can start it
    by_lhs : array<array<int>>      //!< per rule: its productions
    states : array<LrState>
    state_index : table<string; int>
}

def private sym_is_rule(s : int) : bool => s < 0
def private sym_rule_of(s : int) : int => -1 - s

def private compute_first(var b : Builder) {
    b.nullable |> resize(b.nrules)
    b.first |> resize(b.nrules)
    var changed = true
    while (changed) {
        changed = false
        for (p in b.prods) {
            var all_null = true
            for (s in p.rhs) {
                if (sym_is_rule(s)) {
                    let r = sym_rule_of(s)
                    for (c in keys(b.first[r])) {
                        if (!(b.first[p.lhs] |> key_exists(c))) {
                            b.first[p.lhs] |> insert(c)
                            changed = true
                        }
                    }
                    if (!b.nullable[r]) {
                        all_null = false
                        break
                    }
                } else {
                    if (!(b.first[p.lhs] |> key_exists(s))) {
                        b.first[p.lhs] |> insert(s)
                        changed = true
                    }
                    all_null = false
                    break
                }
            }
            if (all_null && !b.nullable[p.lhs]) {
                b.nullable[p.lhs] = true
                changed = true
            }
        }
    }
}

// FIRST of the symbol string `syms[from..]` followed by lookahead `la`, added into `out`
def private first_of_tail(b : Builder; syms : array<int>; from : int; la : int; var out : table<int>) {
    for (i in range(from, length(syms))) {
        let s = syms[i]
        if (!sym_is_rule(s)) {
            out |> insert(s)
            return
        }
        let r = sym_rule_of(s)
        for (c in keys(b.first[r])) {
            out |> insert(c)
        }
        if (!b.nullable[r]) {
            return
        }
    }
    out |> insert(la)
}

// LR(0) closure of a kernel: the kernel items plus every `B -> .gamma` for a rule B after a dot
def private closure0(b : Builder; kernel : array<Item>) : array<Item> {
    var items := kernel
    var seen : table<int64>
    for (it in items) {
        seen |> insert(item_key(it))
    }
    var i = 0
    while (i < length(items)) {
        let it = items[i]
        let p & = unsafe(b.prods[it.prod])
        if (it.dot < length(p.rhs) && sym_is_rule(p.rhs[it.dot])) {
            for (q in b.by_lhs[sym_rule_of(p.rhs[it.dot])]) {
                let ni = Item(prod = q, dot = 0)
                if (!(seen |> key_exists(item_key(ni)))) {
                    seen |> insert(item_key(ni))
                    items |> push(ni)
                }
            }
        }
        i++
    }
    return <- items
}

def private kernel_key(kernel : array<Item>) : string {
    var keys : array<int64>
    keys |> reserve(length(kernel))
    for (it in kernel) {
        keys |> push(item_key(it))
    }
    sort(keys)
    return join(keys, ",")
}

// the state reached from `kernel`'s closure on `sym`: its kernel items, or none
def private goto_kernel(b : Builder; closure : array<Item>; sym : int) : array<Item> {
    var out : array<Item>
    for (it in closure) {
        let p & = unsafe(b.prods[it.prod])
        if (it.dot < length(p.rhs) && p.rhs[it.dot] == sym) {
            out |> push(Item(prod = it.prod, dot = it.dot + 1))
        }
    }
    return <- out
}

def private state_of(var b : Builder; var kernel : array<Item>) : int {
    let key = kernel_key(kernel)
    if (b.state_index |> key_exists(key)) {
        return b.state_index[key]
    }
    let id = length(b.states)
    var st : LrState
    st.kernel <- kernel
    st.la |> resize(length(st.kernel))
    b.states |> emplace(st)
    b.state_index[key] = id
    return id
}

def private build_lr0(var b : Builder) {
    var start : array<Item>
    start |> push(Item(prod = 0, dot = 0))
    state_of(b, start)
    var i = 0
    while (i < length(b.states)) {
        var closure <- closure0(b, b.states[i].kernel)
        // every symbol after a dot, each once, in closure order
        var syms : array<int>
        var seen : table<int>
        for (it in closure) {
            let p & = unsafe(b.prods[it.prod])
            if (it.dot < length(p.rhs)) {
                let s = p.rhs[it.dot]
                if (!(seen |> key_exists(s))) {
                    seen |> insert(s)
                    syms |> push(s)
                }
            }
        }
        for (s in syms) {
            var k <- goto_kernel(b, closure, s)
            let to = state_of(b, k)
            b.states[i].trans[s] = to
        }
        i++
    }
}

// ----- LALR(1) lookaheads: spontaneous generation and propagation (the dragon book's 4.7.5) -----

struct private Prop {
    from_state : int
    from_item : int
    to_state : int
    to_item : int
}

def private kernel_item_index(st : LrState; it : Item) : int {
    for (i, k in count(), st.kernel) {
        if (k.prod == it.prod && k.dot == it.dot) {
            return i
        }
    }
    return -1
}

// LR(1) closure of one item with lookahead `la`: (item, lookahead) pairs
def private closure1(b : Builder; it0 : Item; la0 : int) : array<tuple<it : Item; la : int>> {
    var items : array<tuple<it : Item; la : int>>
    var seen : table<int64>
    items |> push((it = it0, la = la0))
    seen |> insert(item_key(it0) * 1024l + int64(la0 + 1))
    var i = 0
    while (i < length(items)) {
        let cur = items[i]
        let p & = unsafe(b.prods[cur.it.prod])
        if (cur.it.dot < length(p.rhs) && sym_is_rule(p.rhs[cur.it.dot])) {
            var las : table<int>
            first_of_tail(b, p.rhs, cur.it.dot + 1, cur.la, las)
            for (q in b.by_lhs[sym_rule_of(p.rhs[cur.it.dot])]) {
                for (la in keys(las)) {
                    let key = item_key(Item(prod = q, dot = 0)) * 1024l + int64(la + 1)
                    if (!(seen |> key_exists(key))) {
                        seen |> insert(key)
                        items |> push((it = Item(prod = q, dot = 0), la = la))
                    }
                }
            }
        }
        i++
    }
    return <- items
}

let private HASH_LA = -2   //!< the propagation marker lookahead (the dragon book's #)

def private compute_lookaheads(var b : Builder) {
    var props : array<Prop>
    let eof = b.nclasses
    b.states[0].la[0] |> insert(eof)
    for (si in range(length(b.states))) {
        for (ki in range(length(b.states[si].kernel))) {
            var cl <- closure1(b, b.states[si].kernel[ki], HASH_LA)
            for (c in cl) {
                let p & = unsafe(b.prods[c.it.prod])
                continue if (c.it.dot >= length(p.rhs))
                let sym = p.rhs[c.it.dot]
                let to = b.states[si].trans[sym]
                let ti = kernel_item_index(b.states[to], Item(prod = c.it.prod, dot = c.it.dot + 1))
                if (c.la == HASH_LA) {
                    props |> push(Prop(from_state = si, from_item = ki, to_state = to, to_item = ti))
                } else {
                    b.states[to].la[ti] |> insert(c.la)
                }
            }
        }
    }
    var changed = true
    while (changed) {
        changed = false
        for (pr in props) {
            for (la in keys(b.states[pr.from_state].la[pr.from_item])) {
                if (!(b.states[pr.to_state].la[pr.to_item] |> key_exists(la))) {
                    b.states[pr.to_state].la[pr.to_item] |> insert(la)
                    changed = true
                }
            }
        }
    }
}

// ----- the tables -----

def private describe_prod(b : Builder; g : Grammar; p : int) : string {
    let pr & = unsafe(b.prods[p])
    return build_string() $(var w) {
        w |> write(g.rule_names[pr.lhs])
        w |> write(" ::=")
        for (s in pr.rhs) {
            w |> write(" ")
            if (sym_is_rule(s)) {
                w |> write(g.rule_names[sym_rule_of(s)])
            } else {
                w |> write("[class {s}]")
            }
        }
    }
}

def private build_tables(var b : Builder; var g : Grammar) {
    let width = b.nclasses + 1
    g.nstates = length(b.states)
    g.action |> resize(g.nstates * width)
    g.goto_nt |> resize(g.nstates * b.nrules)
    for (i in range(length(g.goto_nt))) {
        g.goto_nt[i] = -1
    }
    for (si in range(length(b.states))) {
        for (s, to in keys(b.states[si].trans), values(b.states[si].trans)) {
            if (sym_is_rule(s)) {
                g.goto_nt[si * b.nrules + sym_rule_of(s)] = to
            } else {
                g.action[si * width + s] = 1 + to
            }
        }
        // reductions: every completed item of the LR(1) closure of the kernel with its lookaheads
        for (ki in range(length(b.states[si].kernel))) {
            for (la0 in keys(b.states[si].la[ki])) {
                var cl <- closure1(b, b.states[si].kernel[ki], la0)
                for (c in cl) {
                    let p & = unsafe(b.prods[c.it.prod])
                    continue if (c.it.dot < length(p.rhs))
                    let cell = si * width + c.la
                    let want = c.it.prod == 0 ? ACCEPT : -(1 + c.it.prod)
                    let have = g.action[cell]
                    if (have != 0 && have != want) {
                        var dup = false
                        if (g.alt |> key_exists(cell)) {
                            for (a in g.alt[cell]) {
                                dup ||= a == want
                            }
                        }
                        continue if (dup)
                        if (g.conflicts == 0) {
                            let kind = have > 0 && have != ACCEPT ? "shift/reduce" : "reduce/reduce"
                            let on = c.la == b.nclasses ? "end of input" : "class {c.la}"
                            g.first_conflict = "{kind} conflict in state {si} on {on}: {describe_prod(b, g, c.it.prod)}"
                        }
                        g.conflicts++
                        g.alt[cell] |> push(want)
                        continue
                    }
                    g.action[cell] = want
                }
            }
        }
    }
}

// every rule the start reaches derives some terminal string - the condition under which the
// viable-prefix property means "every accepted prefix completes"
def private check_well_formed(b : Builder; var g : Grammar) {
    var productive : array<bool>
    productive |> resize(b.nrules)
    var changed = true
    while (changed) {
        changed = false
        for (p in b.prods) {
            continue if (productive[p.lhs])
            var ok = true
            for (s in p.rhs) {
                if (sym_is_rule(s) && !productive[sym_rule_of(s)]) {
                    ok = false
                    break
                }
            }
            if (ok) {
                productive[p.lhs] = true
                changed = true
            }
        }
    }
    var reach : array<bool>
    reach |> resize(b.nrules)
    var todo : array<int>
    todo |> push(b.prods[0].lhs)
    reach[b.prods[0].lhs] = true
    while (!empty(todo)) {
        let r = todo[length(todo) - 1]
        todo |> pop()
        for (q in b.by_lhs[r]) {
            for (s in b.prods[q].rhs) {
                if (sym_is_rule(s) && !reach[sym_rule_of(s)]) {
                    reach[sym_rule_of(s)] = true
                    todo |> push(sym_rule_of(s))
                }
            }
        }
    }
    for (r in range(b.nrules)) {
        if (reach[r] && !productive[r] && empty(g.err)) {
            g.err = "rule '{g.rule_names[r]}' derives no terminal string"
        }
    }
    // a rule the root never reaches is dead text, not a defect: a grammar file keeps its helpers
}

//! Finalize `gb` with `root` as the start rule: byte classes, productions, LALR(1) tables. The
//! result's `err` is non-empty for a grammar with a reachable rule that derives no terminal string; such a
//! grammar accepts nothing. A table conflict is not an error: the acceptor forks its stack there.
def build_grammar(var gb : GrammarBuilder; root : int) : Grammar {
    var g : Grammar
    g.rule_names := gb.rule_names
    g.root = root
    g.nclasses = byte_classes(gb, g.byte_class)
    // the augmented start `S' -> root` is production 0; its rule takes the last id
    let start_rule = length(g.rule_names)
    g.rule_names |> push("<start>")
    var b = Builder(nclasses = g.nclasses, nrules = length(g.rule_names))
    var set_rules : table<string; int>   // one rule per distinct class set
    var p0 : Production
    p0.lhs = start_rule
    p0.rhs |> push(-(1 + root))
    b.prods |> reserve(length(gb.prods) + 1)
    b.prods |> emplace(p0)
    for (gp in gb.prods) {
        var p : Production
        p.lhs = gp.lhs
        for (s in gp.rhs) {
            if (s.is_rule) {
                p.rhs |> push(-(1 + s.rule))
            } else {
                // the set is a union of whole classes: one class is a terminal in place, several a fresh
                // rule with one production per class
                var classes : array<int>
                var seen : table<int>
                for (bt in range(256)) {
                    if (s.bytes[bt] && !(seen |> key_exists(g.byte_class[bt]))) {
                        seen |> insert(g.byte_class[bt])
                        classes |> push(g.byte_class[bt])
                    }
                }
                if (length(classes) == 1) {
                    p.rhs |> push(classes[0])
                } else {
                    sort(classes)
                    let key = join(classes, ",")
                    if (!(set_rules |> key_exists(key))) {
                        let alt = length(g.rule_names)
                        g.rule_names |> push("<set{alt}>")
                        b.prods |> reserve(length(b.prods) + length(classes))
                        for (c in classes) {
                            var q : Production
                            q.lhs = alt
                            q.rhs |> push(c)
                            b.prods |> emplace(q)
                        }
                        set_rules[key] = alt
                    }
                    p.rhs |> push(-(1 + set_rules[key]))
                }
            }
        }
        b.prods |> emplace(p)
    }
    b.nrules = length(g.rule_names)
    b.by_lhs |> resize(b.nrules)
    for (i, p in count(), b.prods) {
        b.by_lhs[p.lhs] |> push(i)
    }
    check_well_formed(b, g)
    if (!empty(g.err)) {
        g.prods <- b.prods
        return <- g
    }
    compute_first(b)
    build_lr0(b)
    compute_lookaheads(b)
    build_tables(b, g)
    g.prods <- b.prods
    return <- g
}

// ===== the acceptor =====

//! A fresh stack set at the start state.
[unused_argument(g)]
def lr_start(g : Grammar) : LrStack {
    var st : LrStack
    var one : array<int>
    one |> push(0)
    st.stacks |> emplace(one)
    return <- st
}

// every action of a cell: the primary one, then the alternatives of a conflicting cell
def private actions_at(g : Grammar; state, cls : int; var out : array<int>) {
    out |> clear()
    let cell = state * (g.nclasses + 1) + cls
    let a = g.action[cell]
    if (a != 0) {
        out |> push(a)
        g.alt |> get(cell) $(extra) {
            for (x in extra) {
                out |> push(x)
            }
        }
    }
}

def private stack_key(st : array<int>) : string => join(st, ",")

// follow every reduce chain of `st` on lookahead `cls`; each stack whose chain ends in a shift (or in
// ACCEPT on the EOF column) lands in `out`, the shift target at its top for a terminal class; false when
// the set would grow past LR_MAX_STACKS (the chains left unwalked)
def private resolve(g : Grammar; st : array<int>; cls : int; var out : array<array<int>>; var seen : table<string>) : bool {
    var work : array<array<int>>
    var first := st
    work |> emplace(first)
    var acts : array<int>
    while (!empty(work)) {
        return false if (length(out) > LR_MAX_STACKS)
        var cur <- work[length(work) - 1]
        work |> pop()
        let key = stack_key(cur)
        continue if (seen |> key_exists(key))
        seen |> insert(key)
        actions_at(g, cur[length(cur) - 1], cls, acts)
        for (a in acts) {
            if (a == ACCEPT) {
                var done := cur
                done |> push(ACCEPT)
                out |> emplace(done)
            } elif (a > 0) {
                var shifted := cur
                shifted |> push(a - 1)
                out |> emplace(shifted)
            } else {
                let p & = unsafe(g.prods[-1 - a])
                var reduced := cur
                reduced |> resize(length(reduced) - length(p.rhs))
                let to = g.goto_nt[reduced[length(reduced) - 1] * length(g.rule_names) + p.lhs]
                if (to >= 0) {
                    reduced |> push(to)
                    work |> emplace(reduced)
                }
            }
        }
    }
    return length(out) <= LR_MAX_STACKS
}

// the stack set after consuming terminal class `cls`; empty when the class is refused - by the grammar, or
// because the set would grow past LR_MAX_STACKS
def private advance(g : Grammar; st : LrStack; cls : int) : LrStack {
    var next : LrStack
    next.stacks |> reserve(length(st.stacks))
    var seen : table<string>
    for (s in st.stacks) {
        if (!resolve(g, s, cls, next.stacks, seen)) {
            next.stacks |> clear()
            return <- next
        }
    }
    // dedupe the survivors
    var uniq : table<string>
    var kept : array<array<int>>
    kept |> reserve(length(next.stacks))
    for (s in next.stacks) {
        let k = stack_key(s)
        if (!(uniq |> key_exists(k))) {
            uniq |> insert(k)
            kept |> emplace(s)
        }
    }
    next.stacks <- kept
    return <- next
}

//! Feed one byte: true and the state advanced, or false and the state unchanged (the byte is refused).
def lr_step(g : Grammar; var st : LrStack; b : uint8) : bool {
    return false if (!empty(g.err) || empty(st.stacks))
    var next <- advance(g, st, g.byte_class[int(b)])
    if (empty(next.stacks)) {
        return false
    }
    st.stacks <- next.stacks
    return true
}

//! Whether the prefix fed so far is a complete sentence (the end of input is acceptable here).
def lr_at_end(g : Grammar; st : LrStack) : bool {
    return false if (!empty(g.err))
    let next <- advance(g, st, g.nclasses)
    for (s in next.stacks) {
        if (s[length(s) - 1] == ACCEPT) {
            return true
        }
    }
    return false
}

//! Whether `b` may come next, the state untouched.
def lr_accepts(g : Grammar; st : LrStack; b : uint8) : bool {
    return false if (!empty(g.err))
    let next <- advance(g, st, g.byte_class[int(b)])
    return !empty(next.stacks)
}

//! The set of bytes that may come next.
def lr_next_bytes(g : Grammar; st : LrStack) : ByteSet {
    var out : ByteSet
    return out if (!empty(g.err))
    var by_class : table<int; bool>
    for (b in range(256)) {
        let cls = g.byte_class[b]
        if (!(by_class |> key_exists(cls))) {
            let next <- advance(g, st, cls)
            by_class[cls] = !empty(next.stacks)
        }
        out[b] = by_class[cls]
    }
    return out
}

//! Feed every byte of `s`: true when all were accepted (the state advanced past them), false and the
//! state unchanged otherwise - the whole-token probe a sampler asks per candidate.
def lr_step_bytes(g : Grammar; var st : LrStack; s : string) : bool {
    var probe : LrStack
    probe.stacks := st.stacks
    var ok = true
    peek_data(s) $(d : array<uint8> const#) {
        for (b in d) {
            if (!lr_step(g, probe, b)) {
                ok = false
                break
            }
        }
    }
    if (ok) {
        st.stacks <- probe.stacks
    }
    return ok
}

//! Whether the whole string `s` is a sentence of the grammar.
def lr_matches(g : Grammar; s : string) : bool {
    var st <- lr_start(g)
    return lr_step_bytes(g, st, s) && lr_at_end(g, st)
}

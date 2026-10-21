options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_prefix shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // KVPool, Session, kv_pool_release_group
require math   // min

// The prefix cache, page-granular: completed streams donate their full KV pages,
// keyed by a CHAINED page hash over token ids, so a later request with the same prompt prefix
// attaches those pages (pool refcnt++) and prefills only the tail. Whole-page matching keeps a
// live stream's write cursor in its own private pages — no copy-on-write on the serving path.
// Hashes route, tokens verify: every hit compares the stored token ids before attaching.

//! One cached page: the pool group holding P consecutive positions of some evaled token history,
//! keyed by the chained hash of every token up to and including this page. A donation's last,
//! partial page is an entry too - its tokens fewer than P. The pages that followed this one, a
//! donation each, are linked, so a prompt sharing only a following page's leading rows gets a COPY of it.
struct PrefixEntry {
    group : int                 // page-group id in the pool (held: its refcnt includes this entry)
    tokens : array<int64>       // the page's token ids, P of them or a tail's fewer — verified on every hit
    last_use : int64            // LRU clock value of the last attach/insert touch
    chain : int64               // owning PrefixChain id (0 = none; duplicates keep their original)
    nexts : array<uint64>       //! the keys of the pages that followed this one, a donation each (a key the cache dropped misses)
    ring_floor : int64          //! its donor's ring floor (`Session.kv_ring_floor`): an attach ending under it lacks the sliding layers' window
}

//! One donated prefix chain, for dashboards: created when a donation contributes at least one
//! new page; an attach whose deepest matched page belongs to it bumps ``hits``.
struct PrefixChain {
    id : int64
    n_tokens : int64            // tokens the donation contributed as NEW pages (fixed at insert;
                                // already-cached pages belong to their original chains)
    pages_live : int64          // its entries still held (evictions decrement; 0 removes the chain)
    hits : int64
    born_at : int64             // ref_time_ticks at insert
    last_hit_at : int64         // 0 until the first hit
    preview : string            // caller-provided detokenized head ("" when not given)
}

//! What a checkpoint stands at: a prompt's opening a prefill stopped at (a system block, a conversation's earlier
//! turns), which other requests share, or a finished turn's tail, which only that conversation's next turn extends.
enum PrefixStateKind {
    opening
    tail
}

//! One recurrent checkpoint: the tokens a session had evaled, the page groups holding their rows
//! (each held), and the recurrent state after the last of them.
struct PrefixState {
    tokens : array<int64>
    groups : array<int>         // ceil(tokens / page_rows) groups, the last one partial when the count is off a page
    snap : DnSnapshot
    kind : PrefixStateKind = PrefixStateKind.tail
    nbytes : int64              //! the snapshot's and the held pages' bytes, alone (`prefix_state_bytes_`); the budget counts a page shared with another checkpoint once
    last_use : int64
    hits : int64
}

//! Page-granular prefix cache over the sessions of ONE ``KVPool`` (``create_prefix_cache_``).
//! Pure data — every verb takes the pool explicitly, so the cache can live inside any owner
//! struct without interior pointers. Not thread-safe; drive it from the pool's owning thread.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-recurrent-checkpoints")]
struct PrefixCache {
    entries : table<uint64; PrefixEntry>   // chained page hash -> cached page
    chains : table<int64; PrefixChain>     // chain id -> donation summary (dashboards)
    next_chain_id : int64
    max_groups : int64          // held-page budget (LRU-evicts past it); 0 = unbounded
    tick : int64                // LRU clock
    n_hit_pages : int64         // pages attached across all lookups
    n_insert_pages : int64      // pages donated
    n_evicted_pages : int64     // pages LRU-dropped
    n_hit_partial : int64       //! pages copied into a taker for the leading rows it shares
    roots : array<uint64>       //! the keys of the first pages donated: a prompt no whole page serves copies the one it shares the most leading rows with
    states : array<PrefixState> // a recurrent model's checkpoints
    max_states : int64          // checkpoint budget (LRU-drops past it); 0 = a recurrent model caches nothing
    max_state_bytes : int64     //! the checkpoints' byte budget (snapshots and held pages); 0 = unbounded, the count alone bounds them
    state_bytes : int64         //! the standing checkpoints' bytes, each held page counted once (`prefix_state_charge`)
    state_holds : array<int>    //! per pool group, how many standing checkpoints hold it: a page is charged as the first takes it and freed as the last lets it go
    n_state_hits : int64
    n_state_inserts : int64
    n_state_evicted : int64
    spare : array<DnSnapshot>   //! the arrays of the last checkpoints dropped (`PREFIX_SPARE_SNAPS` at most, outside the byte budget), refilled by the next snapshot a device takes (`prefix_take_spare_`)
}

// FNV-1a 64 over the 8 bytes of v, continuing h.
def private fnv1a64(h : uint64; v : uint64) : uint64 {
    var x = h
    var b = v
    for (_i in range(8)) {
        x = (x ^ (b & 0xfful)) * 0x100000001b3ul
        b >>= 8ul
    }
    return x
}

// Chained hash of tokens[lo, hi) continuing `prev` — the chain is what makes a page's key encode
// its FULL history, so equal keys mean equal token prefixes (module hash collisions, which the
// stored tokens verify away).
def private chain_page_hash(prev : uint64; tokens : array<int64>; lo, hi : int64) : uint64 {
    var h = fnv1a64(0xcbf29ce484222325ul, prev)
    for (i in range64(lo, hi)) {
        h = fnv1a64(h, uint64(tokens[i]))
    }
    return h
}

def private page_tokens_match(entry : PrefixEntry; tokens : array<int64>; lo : int64) : bool {
    for (i in range64(long_length(entry.tokens))) {
        if (entry.tokens[i] != tokens[lo + i]) {
            return false
        }
    }
    return true
}

//! The key of page `i` of `tokens` chained after `h`, when the cache holds
//! it whole with matching tokens; 0 otherwise.
def private prefix_page_next(cache : PrefixCache; tokens : array<int64>; i, pr : int64;
                             h : uint64) : uint64 {
    let nh = chain_page_hash(h, tokens, i * pr, (i + 1l) * pr)
    let e = unsafe(cache.entries?[nh])
    let whole = e != null && long_length(e.tokens) == pr
    return whole && page_tokens_match(*e, tokens, i * pr) ? nh : 0ul
}

//! Create a prefix cache for the sessions of one pool. ``max_groups`` caps how many page groups
//! the cache retains (LRU-dropped past it; a dropped page a session still uses stays alive — only
//! the cache's hold is released); 0 = unbounded.
def create_prefix_cache_(max_groups : int64 = 0l; max_states : int64 = DEFAULT_PREFIX_STATES; max_state_bytes : int64 = 0l) : PrefixCache =>
    <- PrefixCache(max_groups = max_groups, max_states = max(max_states, 0l), max_state_bytes = max(max_state_bytes, 0l))

let DEFAULT_PREFIX_STATES = 16l   //! a library caller's checkpoint cap with no byte budget set: a request leaves up to three, so this holds a few interleaved conversations; the server sets its own cap under a byte budget

//! a snapshot's arrays and a token list of `n`
def private snapshot_bytes_(snap : DnSnapshot; n : int64) : int64 =>
    (4l * (long_length(snap.conv) + long_length(snap.state) + long_length(snap.ple_hist) + long_length(snap.mtp_h))
        + 8l * (long_length(snap.ple_prev) + n))

//! the bytes of one page group of `pool`
def private page_bytes_(pool : KVPool) : int64 {
    var row_bytes = 0l
    for (k, v in pool.krow_bytes, pool.vrow_bytes) {
        row_bytes += k + v
    }
    return pool.page_rows * row_bytes
}

//! what a checkpoint of `n` tokens costs alone: its snapshot's arrays, its token list, and the rows of the pages it holds
def private prefix_state_bytes_(pool : KVPool; snap : DnSnapshot; n : int64) : int64 {
    let held = (n + pool.page_rows - 1l) / pool.page_rows
    return snapshot_bytes_(snap, n) + held * page_bytes_(pool)
}

//! a checkpoint's standing (`hold` 1) or dropping (-1) charged to the standing checkpoints' bytes: its snapshot and token list,
//! and each of its pages only as the first checkpoint holds it or the last lets it go - the checkpoints of one conversation share
//! every page under their fork, so their own figures summed would charge those pages twice
def private prefix_state_charge(var cache : PrefixCache; pool : KVPool; st : PrefixState; hold : int) {
    cache.state_bytes += int64(hold) * snapshot_bytes_(st.snap, long_length(st.tokens))
    let page = page_bytes_(pool)
    for (g in st.groups) {
        if (g >= length(cache.state_holds)) {
            cache.state_holds |> resize(max(g + 1, 2 * length(cache.state_holds)))
        }
        if (hold > 0 && cache.state_holds[g] == 0) {
            cache.state_bytes += page
        }
        cache.state_holds[g] += hold
        if (hold < 0 && cache.state_holds[g] == 0) {
            cache.state_bytes -= page
        }
    }
}

//! Where a recurrent session's prefill of `prompt` stops to leave a checkpoint, `matched` tokens attached: the caller's
//! stable opening (`stable_at`, 0 = unknown), else the longest opening an earlier checkpoint's tokens share with the
//! prompt - each only a page or more past the hit, under which a checkpoint saves less than it costs. 0 = nowhere.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-recurrent-checkpoints")]
def prefix_checkpoint_at_(cache : PrefixCache; pool : KVPool; s : Session; prompt : array<int64>; matched, stable_at : int64) : int64 {
    if (empty(s.dn_state_prefix) || cache.max_states <= 0l) {
        return 0l
    }
    let floor_at = matched + pool.page_rows
    if (stable_at >= floor_at && stable_at < long_length(prompt)) {
        return stable_at
    }
    var opening = 0l
    for (st in cache.states) {
        opening = max(opening, common_prefix_len(st.tokens, prompt))
    }
    opening = min(opening, long_length(prompt) - 1l)
    return opening >= floor_at ? opening : 0l
}

let PREFIX_SPARE_SNAPS = 2l   //! dropped checkpoints whose arrays wait for reuse: a full pool drops one a copy, so one would do; the second covers a copy that drops two

//! A dropped checkpoint's snapshot to refill in place of a fresh one (its arrays keep their room), or an empty one where none waits
def prefix_take_spare_(var cache : PrefixCache) : DnSnapshot {
    return <- DnSnapshot() if (empty(cache.spare))
    var snap <- cache.spare[length(cache.spare) - 1]
    cache.spare |> pop()
    return <- snap
}

def private prefix_state_drop(var cache : PrefixCache; var pool : KVPool; at : int) {
    prefix_state_charge(cache, pool, cache.states[at], -1)
    for (g in cache.states[at].groups) {
        kv_pool_release_group(pool, g)
    }
    cache.n_state_evicted ++
    if (long_length(cache.spare) < PREFIX_SPARE_SNAPS) {   //! the next snapshot taken off a device reuses these arrays: no fresh pages to fault in
        cache.spare |> emplace(cache.states[at].snap)
    }
    delete cache.states[at]   //! erase is a raw remove: the owned arrays go first
    cache.states |> erase(at)
}

//! the checkpoint to drop first, the one at `exempt` never (the checkpoint just inserted stands): the tail used longest
//! ago, and only when no other tail stands the opening used longest ago - a tail serves one conversation's next turn,
//! an opening every request that shares it; -1 when nothing but the exempt one stands
def private prefix_state_victim(cache : PrefixCache; exempt : int) : int {
    var lru = -1
    for (st, i in cache.states, count()) {
        if (i != exempt && st.kind == PrefixStateKind.tail && (lru < 0 || st.last_use < cache.states[lru].last_use)) {
            lru = i
        }
    }
    if (lru >= 0) {
        return lru
    }
    for (st, i in cache.states, count()) {
        if (i != exempt && (lru < 0 || st.last_use < cache.states[lru].last_use)) {
            lru = i
        }
    }
    return lru
}

//! the deepest checkpoint whose every token opens `prompt`, short of the prompt's last token; -1 where none does
def private prefix_state_best(cache : PrefixCache; prompt : array<int64>) : int {
    var best = -1
    var best_n = 0l
    for (st, i in cache.states, count()) {
        let n = long_length(st.tokens)
        if (n > best_n && n < long_length(prompt) && common_prefix_len(st.tokens, prompt) == n) {
            best = i
            best_n = n
        }
    }
    return best
}

//! checkpoint `at` counted as attached: its use is the newest, its hits and the cache's one up
def private prefix_state_hit(var cache : PrefixCache; at : int) {
    cache.tick ++
    cache.states[at].last_use = cache.tick
    cache.states[at].hits ++
    cache.n_state_hits ++
}

//! the deepest checkpoint whose every token opens `prompt`, short of the prompt's last token: its
//! whole pages shared, its partial page copied (the session writes on from there), its state restored
def private prefix_attach_state(var cache : PrefixCache; var pool : KVPool; var s : Session; prompt : array<int64>) : int64 {
    let best = prefix_state_best(cache, prompt)
    if (best < 0) {
        return 0l
    }
    let best_n = long_length(cache.states[best].tokens)
    let kept = (best_n - 1l) / pool.page_rows   //! the page of row n - 1 is copied even when whole: the draft head rewrites that row for the token that follows
    s.block_table |> reserve(kept + 1l)
    for (i in range64(kept)) {
        let g = cache.states[best].groups[i]
        s.block_table |> push(g)
        pool.refcnt[g] ++
    }
    s.block_table |> push(kv_pool_copy_group(pool, cache.states[best].groups[kept]))
    dn_snapshot_restore(s, cache.states[best].snap)
    s.n_past = best_n
    prefix_state_hit(cache, best)
    return best_n
}

//! How many leading positions of `prompt` a recurrent checkpoint would attach, touching nothing: the deepest one whose every
//! token opens it, short of its last token; 0 where none does.
def prefix_state_match_len_(cache : PrefixCache; prompt : array<int64>) : int64 {
    let best = prefix_state_best(cache, prompt)
    return best < 0 ? 0l : long_length(cache.states[best].tokens)
}

//! The deepest checkpoint opening `prompt`, attached apart: every page it holds shared into `pages` (a fresh session of the
//! pool that reads them and writes nothing - a device's upload), its state restored into `state`; the count, 0 where none does
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#device-home-pool")]
def prefix_attach_state_split_(var cache : PrefixCache; var pool : KVPool; var pages : Session; var state : Session; prompt : array<int64>) : int64 {
    let best = prefix_state_best(cache, prompt)
    return 0l if (best < 0)
    pages.block_table |> reserve(length(cache.states[best].groups))
    for (g in cache.states[best].groups) {
        pages.block_table |> push(g)
        pool.refcnt[g] ++
    }
    let n = long_length(cache.states[best].tokens)
    pages.n_past = n
    dn_snapshot_restore(state, cache.states[best].snap)
    prefix_state_hit(cache, best)
    return n
}

//! Whether a checkpoint of the first `n` of `tokens` stands. One that does is used now, and a stop at a shared position
//! (`opening`) makes it an opening from here on: a tail another request stops at serves more than its conversation.
def prefix_state_touch_(var cache : PrefixCache; tokens : array<int64>; n : int64; opening : bool) : bool {
    for (st in cache.states) {
        if (long_length(st.tokens) == n && common_prefix_len(st.tokens, tokens) >= n) {
            cache.tick ++
            st.last_use = cache.tick
            if (opening) {
                st.kind = PrefixStateKind.opening
            }
            return true
        }
    }
    return false
}

//! The whole pages of the first `n` of `tokens` the cache already holds, shared into the fresh session `s` of the pool: a
//! checkpoint's pages under the longest opening it shares with them on a recurrent model, the page chain elsewhere. Answers the
//! positions they cover, a whole number of pages - the rows a caller still has to write start there.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#device-home-pool")]
def prefix_share_pages_(var cache : PrefixCache; var pool : KVPool; var s : Session; tokens : array<int64>; n : int64) : int64 {
    let pr = pool.page_rows
    if (!empty(s.dn_state_prefix)) {
        var best = -1
        var best_n = 0l
        for (st, i in cache.states, count()) {
            let common = min(common_prefix_len(st.tokens, tokens), n)
            if (common > best_n) {
                best = i
                best_n = common
            }
        }
        return 0l if (best < 0)
        let kept = best_n / pr
        s.block_table |> reserve(kept)
        for (i in range64(kept)) {
            let g = cache.states[best].groups[i]
            s.block_table |> push(g)
            pool.refcnt[g] ++
        }
        return kept * pr
    }
    var h = 0ul
    var kept = 0l
    for (i in range64(n / pr)) {
        let nh = prefix_page_next(cache, tokens, i, pr, h)
        break if (nh == 0ul)
        let e = unsafe(cache.entries?[nh])
        s.block_table |> push(e.group)
        pool.refcnt[e.group] ++
        s.kv_ring_floor = max(s.kv_ring_floor, e.ring_floor)
        h = nh
        kept ++
    }
    return kept * pr
}

//! a checkpoint of the session where it stands (its first `n_past` tokens, their pages, the recurrent state); answers "" when one stands there after the call, else why not
def private prefix_insert_state(var cache : PrefixCache; var pool : KVPool; var s : Session; tokens : array<int64>; kind : PrefixStateKind) : string {
    let n = min(long_length(tokens), s.n_past)
    if (cache.max_states <= 0l) {
        return "the cache keeps no checkpoints"
    }
    if (n <= 0l) {
        return "no evaled tokens stand ({long_length(tokens)} tokens, n_past {s.n_past})"
    }
    if (prefix_state_touch_(cache, tokens, n, kind == PrefixStateKind.opening)) {
        return ""
    }
    var snap <- dn_snapshot_take(s)
    if (snap.pos != n) {
        let why = "the recurrent state stands at {snap.pos}, the tokens end at {n} ({long_length(tokens)} tokens, n_past {s.n_past})"
        delete snap
        return why
    }
    prefix_insert_snapshot_(cache, pool, s, tokens, snap, kind == PrefixStateKind.opening)
    return ""
}

//! a checkpoint of the first `n_past` (at most `upto`, 0 = all) of `tokens` out of `s`'s pages, its state `snap` moved in from a device's slots or a window's capture; nothing kept unless `snap.pos` stands there
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#device-home-pool")]
def prefix_insert_snapshot_(var cache : PrefixCache; var pool : KVPool; s : Session; tokens : array<int64>; var snap : DnSnapshot; opening : bool; upto : int64 = 0l) {
    var n = min(long_length(tokens), s.n_past)
    if (upto > 0l) {
        n = min(n, upto)
    }
    if (n <= 0l || cache.max_states <= 0l || snap.pos != n || prefix_state_touch_(cache, tokens, n, opening)) {
        delete snap
        return
    }
    let kind = opening ? PrefixStateKind.opening : PrefixStateKind.tail
    cache.tick ++
    let pr = pool.page_rows
    let nbytes = prefix_state_bytes_(pool, snap, n)   // before the move below: a call's arguments evaluate in no fixed order
    var e <- PrefixState(snap <- snap, kind = kind, nbytes = nbytes, last_use = cache.tick)
    e.tokens <- [for (i in range64(n)); tokens[i]]
    let held = (n + pr - 1l) / pr
    e.groups |> reserve(held)
    for (i in range64(held)) {
        e.groups |> push(s.block_table[i])
        pool.refcnt[s.block_table[i]] ++
    }
    prefix_state_charge(cache, pool, e, 1)
    cache.states |> emplace(e)
    cache.n_state_inserts ++
    while (long_length(cache.states) > cache.max_states ||
           (cache.max_state_bytes > 0l && cache.state_bytes > cache.max_state_bytes)) {
        let victim = prefix_state_victim(cache, length(cache.states) - 1)   //! the newest stands, even alone past the budget: a budget under one checkpoint caches the last one
        break if (victim < 0)
        prefix_state_drop(cache, pool, victim)
    }
}

//! Pool page references the cache currently holds: a cached page each, and every page of every recurrent checkpoint.
def prefix_held_groups_(cache : PrefixCache) : int64 {
    var held = long_length(cache.entries)
    for (st in cache.states) {
        held += long_length(st.groups)
    }
    return held
}

//! `key` among the pages `links` names, once; the keys of pages the cache has dropped go first, so a list holds live pages
def private link_page(cache : PrefixCache; var links : array<uint64>; key : uint64) {
    var live <- [for (k in links); k; where k != key && key_exists(cache.entries, k)]
    live |> push(key)
    delete links
    links <- live
}

//! the rows of `prompt` from `at` on that the page under `key` opens with, short of the prompt's last token; 0 for a
//! key the cache does not hold
def private shared_rows(cache : PrefixCache; key : uint64; prompt : array<int64>; at : int64) : int64 {
    return 0l if (key == 0ul || !key_exists(cache.entries, key))
    let e = unsafe(cache.entries?[key])
    let np = long_length(prompt)
    var rows = 0l
    while (rows < long_length(e.tokens) && at + rows < np - 1l && e.tokens[rows] == prompt[at + rows]) {
        rows ++
    }
    return rows
}

//! Attach the longest cached prefix of ``prompt`` to a FRESH paged session of ``pool``, copying matched
//! groups into the block table and setting ``n_past`` accordingly; returns the matched count, capped
//! at one token short of the prompt so the tail eval always produces sampling logits.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page")]
def prefix_attach_(var cache : PrefixCache; var pool : KVPool; var s : Session; prompt : array<int64>) : int64 {
    if (s.kv_pool == null || s.kv_pool != unsafe(addr(pool))) {
        panic("prefix_attach: the session is not paged out of this pool")
    }
    if (s.n_past != 0l || !empty(s.block_table)) {
        panic("prefix_attach: the session already holds context (n_past {s.n_past}, {length(s.block_table)} pages) - attach only to a fresh session")
    }
    if (!empty(s.dn_state_prefix)) {
        return prefix_attach_state(cache, pool, s, prompt)
    }
    let pr = pool.page_rows
    var inscope plan <- prefix_plan(cache, pool, prompt)
    var matched = 0l
    var hit_chain = 0l
    s.block_table |> reserve(length(plan.keys) + 1)
    for (nh in plan.keys) {
        // interior pointer into the table — nothing inserts before the loop exits
        var e = unsafe(cache.entries?[nh])
        cache.tick ++
        e.last_use = cache.tick
        hit_chain = e.chain
        s.block_table |> push(e.group)
        pool.refcnt[e.group] ++
        s.kv_ring_floor = max(s.kv_ring_floor, e.ring_floor)
        matched += pr
        cache.n_hit_pages ++
    }
    if (plan.rows > 0l) {
        var e = unsafe(cache.entries?[plan.take])
        cache.tick ++
        e.last_use = cache.tick
        if (hit_chain == 0l) {
            hit_chain = e.chain
        }
        s.block_table |> push(kv_pool_copy_group(pool, e.group))
        s.kv_ring_floor = max(s.kv_ring_floor, e.ring_floor)
        matched += plan.rows
        cache.n_hit_partial ++
    }
    if (hit_chain != 0l) {
        var c = unsafe(cache.chains?[hit_chain])
        if (c != null) {
            c.hits ++
            c.last_hit_at = ref_time_ticks()   // clock: control — LRU recency, feeds prefix_evict_lru
        }
    }
    s.n_past = matched
    return matched
}

//! How many leading positions of `prompt` `prefix_attach_` would attach right now, with nothing attached and no entry
//! touched: the whole pages the cache holds, then the rows the page past them shares. 0 on a recurrent model's cache,
//! which attaches at checkpoints - a caller that needs the count there treats nothing as held.
def prefix_match_len_(cache : PrefixCache; pool : KVPool; prompt : array<int64>) : int64 {
    return 0l if (!empty(cache.states))
    var inscope plan <- prefix_plan(cache, pool, prompt)
    return long_length(plan.keys) * pool.page_rows + plan.rows
}

//! the pages `prompt` attaches: the whole pages' keys, then the page past them that shares the most leading rows (`take`, `rows`;
//! 0 = none)
struct private PrefixPlan {
    keys : array<uint64>
    take : uint64
    rows : int64
}

//! the pages `prefix_attach_` takes for `prompt`, nothing touched: the whole pages the cache holds, then of the pages that followed
//! the last of them - the first pages when there is none - the one that shares the most leading rows with the prompt; cut to the
//! longest run whose every page's ring floor it reaches (`prefix_plan_floor_cut`)
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page")]
def private prefix_plan(cache : PrefixCache; pool : KVPool; prompt : array<int64>) : PrefixPlan {
    var plan : PrefixPlan
    let np = long_length(prompt)
    let pr = pool.page_rows
    let pages_max = np > 0l ? (np - 1l) / pr : 0l
    var hit_hash = 0ul
    for (i in range64(pages_max)) {
        let nh = prefix_page_next(cache, prompt, i, pr, hit_hash)
        break if (nh == 0ul)
        hit_hash = nh
        plan.keys |> push(nh)
    }
    let matched = long_length(plan.keys) * pr
    for (key in matched > 0l ? unsafe(cache.entries?[hit_hash]).nexts : cache.roots) {
        let opens = shared_rows(cache, key, prompt, matched)
        if (opens > plan.rows) {
            plan.rows = opens
            plan.take = key
        }
    }
    prefix_plan_floor_cut(cache, plan, pr)
    return <- plan
}

//! `plan` cut to the longest prefix that reaches the ring floor of every page it takes: a page donated off a history a GPU mirror
//! rang holds the sliding layers' rows from its donor's floor on, so an attach ending under that floor would continue from a window
//! the page never held
[arch(at="../ARCHITECTURE_GPU_KV_RING.md#kv-ring-floor")]
def private prefix_plan_floor_cut(cache : PrefixCache; var plan : PrefixPlan; pr : int64) {
    var floor = 0l
    var ok = 0
    for (key, i in plan.keys, count()) {
        floor = max(floor, unsafe(cache.entries?[key]).ring_floor)
        if (int64(i + 1) * pr >= floor) {
            ok = i + 1
        }
    }
    let whole = ok == length(plan.keys)
    if (plan.rows > 0l && (!whole || long_length(plan.keys) * pr + plan.rows < max(floor, unsafe(cache.entries?[plan.take]).ring_floor))) {
        plan.rows = 0l
        plan.take = 0ul
    }
    plan.keys |> resize(ok)
}

// Drop the least-recently-used entry (release its group hold + its chain's live count).
def private prefix_evict_lru(var cache : PrefixCache; var pool : KVPool) {
    var lru_key = 0ul
    var lru_use = 0l
    var first = true
    for (k, e in keys(cache.entries), values(cache.entries)) {
        if (first || e.last_use < lru_use) {
            lru_use = e.last_use
            lru_key = k
            first = false
        }
    }
    let ch = cache.entries[lru_key].chain
    kv_pool_release_group(pool, cache.entries[lru_key].group)
    delete cache.entries[lru_key]   // erase is a raw remove — finalize the owned tokens first
    cache.entries |> erase(lru_key)
    let root_at = find_index(cache.roots, lru_key)
    if (root_at >= 0) {
        cache.roots |> erase(root_at)
    }
    cache.n_evicted_pages ++
    if (ch != 0l) {
        var c = unsafe(cache.chains?[ch])
        if (c != null) {
            c.pages_live --
            if (c.pages_live <= 0l) {
                delete cache.chains[ch]
                cache.chains |> erase(ch)
            }
        }
    }
}

//! donate a finished session's pages (tokens up to ``n_past``) right before ``release_kv_pages``, ``preview`` labelling the chain; answers "" when the cache holds them, else why a recurrent checkpoint was not kept
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page")]
def prefix_insert_(var cache : PrefixCache; var pool : KVPool; var s : Session; tokens : array<int64>; preview : string = ""; opening : bool = false) : string {
    if (s.kv_pool == null || s.kv_pool != unsafe(addr(pool))) {
        panic("prefix_insert: the session is not paged out of this pool")
    }
    if (!empty(s.dn_state_prefix)) {
        return prefix_insert_state(cache, pool, s, tokens, opening ? PrefixStateKind.opening : PrefixStateKind.tail)
    }
    let pr = pool.page_rows
    let covered = min(long_length(tokens), s.n_past)
    let pages = (covered + pr - 1l) / pr
    var h = 0ul
    var prev = 0ul
    var chain_id = 0l
    for (i in range64(pages)) {
        let lo = i * pr
        let hi = min(lo + pr, covered)
        h = chain_page_hash(h, tokens, lo, hi)
        cache.tick ++
        if (key_exists(cache.entries, h)) {
            // interior pointer into the table — released before anything inserts
            var e = unsafe(cache.entries?[h])
            e.last_use = cache.tick
        } else {
            if (chain_id == 0l) {   // first NEW page: this donation gets a dashboard chain
                cache.next_chain_id ++
                chain_id = cache.next_chain_id
                cache.chains[chain_id] <- PrefixChain(id = chain_id, born_at = ref_time_ticks(), preview = preview)   // clock: control — chain age, eviction bookkeeping
            }
            var e <- PrefixEntry(group = s.block_table[i], last_use = cache.tick, chain = chain_id, ring_floor = s.kv_ring_floor)
            e.tokens <- [for (j in range64(lo, hi)); tokens[j]]
            cache.entries[h] <- e
            pool.refcnt[s.block_table[i]] ++
            cache.n_insert_pages ++
            cache.chains[chain_id].pages_live ++
            cache.chains[chain_id].n_tokens += hi - lo   // NEW pages only — a partially-cached donation must not over-report
            if (i == 0l) {
                link_page(cache, cache.roots, h)
            }
        }
        if (prev != 0ul) {
            link_page(cache, cache.entries[prev].nexts, h)
        }
        prev = h
    }
    if (cache.max_groups > 0l) {
        while (long_length(cache.entries) > cache.max_groups) {
            prefix_evict_lru(cache, pool)
        }
    }
    return ""
}

//! Release every cached page hold back to ``pool`` and clear the cache (pages still used by live
//! sessions stay alive until those sessions release them). Call before deleting the pool.
def prefix_release_(var cache : PrefixCache; var pool : KVPool) {
    for (e in values(cache.entries)) {
        kv_pool_release_group(pool, e.group)
    }
    delete cache.entries
    delete cache.chains
    delete cache.roots
    while (!empty(cache.states)) {
        prefix_state_drop(cache, pool, length(cache.states) - 1)   // release: the newest goes too
    }
    delete cache.spare
}

//! Snapshot of the donated chains for dashboards (order unspecified).
def prefix_chain_list_(cache : PrefixCache) : array<PrefixChain> {
    var out : array<PrefixChain>
    out |> reserve(length(cache.chains))
    for (c in values(cache.chains)) {
        out |> push(c)
    }
    return <- out
}

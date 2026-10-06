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

//! One recurrent checkpoint: the tokens a session had evaled, the page groups holding their rows
//! (each held), and the recurrent state after the last of them.
struct PrefixState {
    tokens : array<int64>
    groups : array<int>         // ceil(tokens / page_rows) groups, the last one partial when the count is off a page
    snap : DnSnapshot
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
    n_state_hits : int64
    n_state_inserts : int64
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

//! Create a prefix cache for the sessions of one pool. ``max_groups`` caps how many page groups
//! the cache retains (LRU-dropped past it; a dropped page a session still uses stays alive — only
//! the cache's hold is released); 0 = unbounded.
def create_prefix_cache_(max_groups : int64 = 0l; max_states : int64 = DEFAULT_PREFIX_STATES) : PrefixCache =>
    <- PrefixCache(max_groups = max_groups, max_states = max(max_states, 0l))

let DEFAULT_PREFIX_STATES = 4l   //! a recurrent model's checkpoints kept: a reused opening, a conversation's last turn, and two to spare

//! how many of `a`'s leading tokens open `b`
def private shared_opening(a, b : array<int64>) : int64 {
    let cap = min(long_length(a), long_length(b))
    var n = 0l
    while (n < cap && a[n] == b[n]) {
        n ++
    }
    return n
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
        opening = max(opening, shared_opening(st.tokens, prompt))
    }
    opening = min(opening, long_length(prompt) - 1l)
    return opening >= floor_at ? opening : 0l
}

def private prefix_state_drop(var cache : PrefixCache; var pool : KVPool; at : int) {
    for (g in cache.states[at].groups) {
        kv_pool_release_group(pool, g)
    }
    delete cache.states[at]   //! erase is a raw remove: the owned arrays go first
    cache.states |> erase(at)
}

//! the deepest checkpoint whose every token opens `prompt`, short of the prompt's last token: its
//! whole pages shared, its partial page copied (the session writes on from there), its state restored
def private prefix_attach_state(var cache : PrefixCache; var pool : KVPool; var s : Session; prompt : array<int64>) : int64 {
    var best = -1
    var best_n = 0l
    for (st, i in cache.states, count()) {
        let n = long_length(st.tokens)
        if (n > best_n && n < long_length(prompt) && shared_opening(st.tokens, prompt) == n) {
            best = i
            best_n = n
        }
    }
    if (best < 0) {
        return 0l
    }
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
    cache.tick ++
    cache.states[best].last_use = cache.tick
    cache.states[best].hits ++
    cache.n_state_hits ++
    return best_n
}

//! a checkpoint of the session where it stands: its first `n_past` tokens, their pages, the
//! recurrent state. Nothing is kept when the state does not stand at that position.
def private prefix_insert_state(var cache : PrefixCache; var pool : KVPool; var s : Session; tokens : array<int64>) {
    let n = min(long_length(tokens), s.n_past)
    if (n <= 0l || cache.max_states <= 0l) {
        return
    }
    cache.tick ++
    for (st in cache.states) {
        if (long_length(st.tokens) == n && shared_opening(st.tokens, tokens) == n) {
            st.last_use = cache.tick
            return
        }
    }
    var snap <- dn_snapshot_take(s)
    if (snap.pos != n) {
        delete snap
        return
    }
    let pr = pool.page_rows
    var e <- PrefixState(snap <- snap, last_use = cache.tick)
    e.tokens <- [for (i in range64(n)); tokens[i]]
    let held = (n + pr - 1l) / pr
    e.groups |> reserve(held)
    for (i in range64(held)) {
        e.groups |> push(s.block_table[i])
        pool.refcnt[s.block_table[i]] ++
    }
    cache.states |> emplace(e)
    cache.n_state_inserts ++
    while (long_length(cache.states) > cache.max_states) {
        var lru = 0
        for (st, i in cache.states, count()) {
            lru = st.last_use < cache.states[lru].last_use ? i : lru
        }
        prefix_state_drop(cache, pool, lru)
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
    let np = long_length(prompt)
    let pr = pool.page_rows
    let pages_max = np > 0l ? (np - 1l) / pr : 0l
    var hit_hash = 0ul
    var matched = 0l
    var hit_chain = 0l
    for (i in range64(pages_max)) {
        let nh = chain_page_hash(hit_hash, prompt, i * pr, (i + 1l) * pr)
        if (!key_exists(cache.entries, nh)) {
            break
        }
        // interior pointer into the table — nothing inserts before the loop exits
        var e = unsafe(cache.entries?[nh])
        if (e == null || long_length(e.tokens) != pr || !page_tokens_match(*e, prompt, i * pr)) {
            break   // hash collision or corrupted entry: tokens are the truth
        }
        hit_hash = nh
        cache.tick ++
        e.last_use = cache.tick
        hit_chain = e.chain
        s.block_table |> push(e.group)
        pool.refcnt[e.group] ++
        matched += pr
        cache.n_hit_pages ++
    }
    // the page past the whole hits: of the pages that followed the last hit - the first pages when there is none -
    // the one that shares the most leading rows with the prompt
    var take = 0ul
    var rows = 0l
    for (key in matched > 0l ? cache.entries[hit_hash].nexts : cache.roots) {
        let opens = shared_rows(cache, key, prompt, matched)
        if (opens > rows) {
            rows = opens
            take = key
        }
    }
    if (rows > 0l) {
        var e = unsafe(cache.entries?[take])
        cache.tick ++
        e.last_use = cache.tick
        if (hit_chain == 0l) {
            hit_chain = e.chain
        }
        s.block_table |> push(kv_pool_copy_group(pool, e.group))
        matched += rows
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
    let np = long_length(prompt)
    let pr = pool.page_rows
    let pages_max = np > 0l ? (np - 1l) / pr : 0l
    var hit_hash = 0ul
    var matched = 0l
    for (i in range64(pages_max)) {
        let nh = chain_page_hash(hit_hash, prompt, i * pr, (i + 1l) * pr)
        break if (!key_exists(cache.entries, nh))
        let e = unsafe(cache.entries?[nh])
        break if (e == null || long_length(e.tokens) != pr || !page_tokens_match(*e, prompt, i * pr))
        hit_hash = nh
        matched += pr
    }
    var rows = 0l
    if (matched > 0l) {
        for (key in unsafe(cache.entries?[hit_hash]).nexts) {
            rows = max(rows, shared_rows(cache, key, prompt, matched))
        }
    } else {
        for (key in cache.roots) {
            rows = max(rows, shared_rows(cache, key, prompt, matched))
        }
    }
    return matched + rows
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

//! Donate a finished session's pages into the cache, bumping pool refcnt so they survive release;
//! only tokens up to ``n_past`` are eligible, and pages past the budget LRU-drop. Call right before
//! ``release_kv_pages``. ``preview`` labels the donation's chain on the dashboard surface.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page")]
def prefix_insert_(var cache : PrefixCache; var pool : KVPool; var s : Session; tokens : array<int64>; preview : string = "") {
    if (s.kv_pool == null || s.kv_pool != unsafe(addr(pool))) {
        panic("prefix_insert: the session is not paged out of this pool")
    }
    if (!empty(s.dn_state_prefix)) {
        prefix_insert_state(cache, pool, s, tokens)
        return
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
            var e <- PrefixEntry(group = s.block_table[i], last_use = cache.tick, chain = chain_id)
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
        prefix_state_drop(cache, pool, length(cache.states) - 1)
    }
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

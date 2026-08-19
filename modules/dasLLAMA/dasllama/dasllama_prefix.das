options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_prefix shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // KVPool, Session, kv_pool_release_group
require math   // min

// The prefix cache (vLLM-style, page-granular): completed streams donate their full KV pages,
// keyed by a CHAINED page hash over token ids, so a later request with the same prompt prefix
// attaches those pages (pool refcnt++) and prefills only the tail. Whole-page matching keeps a
// live stream's write cursor in its own private pages — no copy-on-write on the serving path.
// Hashes route, tokens verify: every hit compares the stored token ids before attaching.

//! One cached page: the pool group holding P consecutive positions of some evaled token history,
//! keyed by the chained hash of every token up to and including this page.
struct PrefixEntry {
    group : int                 // page-group id in the pool (held: its refcnt includes this entry)
    tokens : array<int64>       // the page's P token ids — verified on every hit
    last_use : int64            // LRU clock value of the last attach/insert touch
    chain : int64               // owning PrefixChain id (0 = none; duplicates keep their original)
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

//! Page-granular prefix cache over the sessions of ONE ``KVPool`` (``create_prefix_cache_``).
//! Pure data — every verb takes the pool explicitly, so the cache can live inside any owner
//! struct without interior pointers. Not thread-safe; drive it from the pool's owning thread.
struct PrefixCache {
    entries : table<uint64; PrefixEntry>   // chained page hash -> cached page
    chains : table<int64; PrefixChain>     // chain id -> donation summary (dashboards)
    next_chain_id : int64
    max_groups : int64          // held-page budget (LRU-evicts past it); 0 = unbounded
    tick : int64                // LRU clock
    n_hit_pages : int64         // pages attached across all lookups
    n_insert_pages : int64      // pages donated
    n_evicted_pages : int64     // pages LRU-dropped
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
def create_prefix_cache_(max_groups : int64 = 0l) : PrefixCache {
    return <- PrefixCache(max_groups = max_groups)
}

//! Pages the cache currently holds (== held pool groups).
def prefix_held_groups_(cache : PrefixCache) : int64 => long_length(cache.entries)

//! Attach the longest cached prefix of ``prompt`` to a FRESH paged session of ``pool``, copying matched
//! groups into the block table and setting ``n_past`` accordingly; returns the matched count, capped
//! at one token short of the prompt so the tail eval always produces sampling logits.
def prefix_attach_(var cache : PrefixCache; var pool : KVPool; var s : Session; prompt : array<int64>) : int64 {
    if (!empty(s.dn_state_prefix)) {
        // hybrid (deltanet): KV pages alone cannot reconstruct recurrent state, so no cached
        // prefix is ever attachable — 0 matched is the correct result, the caller prefills all
        return 0l
    }
    if (s.kv_pool == null || s.kv_pool != unsafe(addr(pool))) {
        panic("prefix_attach: the session is not paged out of this pool")
    }
    if (s.n_past != 0l || !empty(s.block_table)) {
        panic("prefix_attach: the session already holds context (n_past {s.n_past}, {length(s.block_table)} pages) - attach only to a fresh session")
    }
    let np = long_length(prompt)
    let pr = pool.page_rows
    let pages_max = np > 0l ? (np - 1l) / pr : 0l
    var h = 0ul
    var matched = 0l
    var hit_chain = 0l
    for (i in range64(pages_max)) {
        h = chain_page_hash(h, prompt, i * pr, (i + 1l) * pr)
        if (!key_exists(cache.entries, h)) {
            break
        }
        // interior pointer into the table — nothing inserts before the loop exits
        var e = unsafe(cache.entries?[h])
        if (e == null || !page_tokens_match(*e, prompt, i * pr)) {
            break   // hash collision or corrupted entry: tokens are the truth
        }
        cache.tick ++
        e.last_use = cache.tick
        hit_chain = e.chain
        s.block_table |> push(e.group)
        pool.refcnt[e.group] ++
        matched += pr
        cache.n_hit_pages ++
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
def prefix_insert_(var cache : PrefixCache; var pool : KVPool; s : Session; tokens : array<int64>; preview : string = "") {
    if (!empty(s.dn_state_prefix)) {
        return   // hybrid (deltanet): pages without recurrent state can never be attached — don't cache
    }
    if (s.kv_pool == null || s.kv_pool != unsafe(addr(pool))) {
        panic("prefix_insert: the session is not paged out of this pool")
    }
    let pr = pool.page_rows
    let covered = min(long_length(tokens), s.n_past)
    let pages = covered / pr
    var h = 0ul
    var chain_id = 0l
    for (i in range64(pages)) {
        h = chain_page_hash(h, tokens, i * pr, (i + 1l) * pr)
        cache.tick ++
        if (key_exists(cache.entries, h)) {
            // interior pointer into the table — released before anything inserts
            var e = unsafe(cache.entries?[h])
            e.last_use = cache.tick
            continue
        }
        if (chain_id == 0l) {   // first NEW page: this donation gets a dashboard chain
            cache.next_chain_id ++
            chain_id = cache.next_chain_id
            cache.chains[chain_id] <- PrefixChain(id = chain_id, born_at = ref_time_ticks(), preview = preview)   // clock: control — chain age, eviction bookkeeping
        }
        var e <- PrefixEntry(group = s.block_table[i], last_use = cache.tick, chain = chain_id)
        e.tokens <- [for (j in range64(i * pr, (i + 1l) * pr)); tokens[j]]
        cache.entries[h] <- e
        pool.refcnt[s.block_table[i]] ++
        cache.n_insert_pages ++
        cache.chains[chain_id].pages_live ++
        cache.chains[chain_id].n_tokens += pr   // NEW pages only — a partially-cached donation must not over-report
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

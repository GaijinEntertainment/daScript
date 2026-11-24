options gen2
options _dasllama_internal = true

module _kv_prefix_cells shared public

//! the prefix cache's cells on a synthetic Config, and the makers of that Config and of a recurrent session on it; none
//! loads a model

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_prefix
require daslib/fio
require daslib/linq   // sequence_equal
require strings   // find: a refusal's reason by its words

def mk_config(layers, heads, kv_heads, hs, ctx : int64) : Config {
    return Config(dim = heads * hs, hidden_dim = 32l, n_layers = layers, n_heads = heads,
        n_kv_heads = kv_heads, vocab_size = 8l, seq_len = ctx, head_size = hs,
        kv_dim = kv_heads * hs, shared_weights = true, rope_freq_base = 10000.0)
}

def mk_recurrent(c : Config; var pool : KVPool; n : int64) : Session {
    var s <- make_run_state_paged(c, pool)
    s.dn_state_prefix |> push(0l)
    s.dn_conv_prefix |> push(0l)
    s.dn_state <- [for (i in range64(8l)); float(n * 10l + i)]   // one recurrent layer's state, a value a position
    s.dn_conv_state <- [for (i in range64(4l)); float(n * 100l + i)]
    if (n > 0l) {
        kv_ensure(s, n)
    }
    s.n_past = n
    s.dn_pos = n
    return <- s
}

//! positions [0, n) of the paged session `s`, every K and V byte: under `write` set to a value its plane, position and byte
//! name, else compared with it - the first position that differs, -1 where none does
def private rows_walk(var pool : KVPool; s : Session; n : int64; write : bool) : int64 {
    for (p in range64(n)) {
        let at = int64(s.block_table[p / pool.page_rows]) * pool.page_rows + p % pool.page_rows
        for (b in range64(pool.nblobs)) {
            for (i in range64(pool.krow_bytes[b] + pool.vrow_bytes[b])) {
                let k = i < pool.krow_bytes[b]
                let want = uint8((b * 31l + p * 7l + i) & 255l)
                let off = k ? at * pool.krow_bytes[b] + i : at * pool.vrow_bytes[b] + i - pool.krow_bytes[b]
                if (write && k) {
                    pool.kblobs[b][off] = want
                } elif (write) {
                    pool.vblobs[b][off] = want
                } elif ((k ? pool.kblobs[b][off] : pool.vblobs[b][off]) != want) {
                    return p
                }
            }
        }
    }
    return -1l
}

//! the file at `path` copied to `to` with its first `keep` bytes alone (0 = all), byte `at` set to `value` where `at` is not negative
def private file_variant(path, to : string; keep, at : int64; value : uint8) {
    var inscope bytes : array<uint8>
    bytes |> resize(int64(stat(path).size))
    fopen(path, "rb") $(f) {
        long_fread(f, bytes)
    }
    if (keep > 0l) {
        bytes |> resize(keep)
    }
    if (at >= 0l) {
        bytes[at] = value
    }
    fopen(to, "wb") $(f) {
        long_fwrite(f, bytes)
    }
}

//! the context-file test's cell: a dense context: saved off one pool, restored into a pool of another page size, attached whole with the donor's bytes
def context_dense_cell(t : T?; c : Config; dense_path : string) {
    var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
    var cache <- create_prefix_cache_()
    var s <- make_run_state_paged(c, pool)
    kv_ensure(s, 10l)
    s.n_past = 10l
    rows_walk(pool, s, 10l, true)
    var toks <- [for (i in range64(10l)); i % 8l]
    prefix_insert_(cache, pool, s, toks)
    release_kv_pages_(s)
    var carrier <- make_run_state_paged(c, pool)
    var saved <- prefix_file_save_(c, cache, pool, carrier, toks, dense_path)
    t |> equal(saved.why, "", "the save ran")
    t |> equal(saved.tokens, 10l, "it holds every cached token")
    t |> equal(saved.bytes, int64(stat(dense_path).size), "its byte count is the file's")
    t |> equal(length(carrier.block_table), 0, "the carrier leaves holding no page")
    var ask := toks
    ask |> push(1l)
    t |> equal(prefix_match_len_(cache, pool, ask), 10l, "the cache still holds the context")
    var pool2 <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
    var cache2 <- create_prefix_cache_()
    var carrier2 <- make_run_state_paged(c, pool2)
    var got <- prefix_file_restore_(c, cache2, pool2, carrier2, dense_path)
    t |> equal(got.why, "", "the restore ran")
    t |> equal(got.tokens, 10l, "it took every token")
    t |> equal(got.bytes, saved.bytes, "it read the bytes the save wrote")
    t |> success(sequence_equal(got.ids, toks), "its ids are the saved tokens ({got.ids})")
    t |> equal(length(carrier2.block_table), 0, "the carrier leaves holding no page")
    var w <- make_run_state_paged(c, pool2)
    t |> equal(prefix_attach_(cache2, pool2, w, ask), 10l, "a prompt opening with the file's tokens attaches all of them")
    t |> equal(rows_walk(pool2, w, 10l, false), -1l, "every K and V byte of the ten rows is the donor's")
    let flip = int64(w.block_table[1]) * pool2.page_rows * pool2.vrow_bytes[0]
    pool2.vblobs[0][flip] = pool2.vblobs[0][flip] == 0u8 ? 1u8 : 0u8
    t |> equal(rows_walk(pool2, w, 10l, false), 8l, "control: one flipped byte of row 8 is seen")
    release_kv_pages_(w)
    prefix_release_(cache2, pool2)
    prefix_release_(cache, pool)
    t |> equal(long_length(pool2.free_groups), pool2.capacity_groups, "every page of the second pool is back")
    delete w
    delete got
    delete carrier2
    delete cache2
    delete pool2
    delete ask
    delete saved
    delete carrier
    delete toks
    delete s
    delete cache
    delete pool
}

//! the context-file test's cell: a recurrent checkpoint: its rows and its state come back, the state standing at the checkpoint
def context_recurrent_cell(t : T?; c : Config; state_path : string) {
    var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
    var cache <- create_prefix_cache_()
    var s <- mk_recurrent(c, pool, 3l)   // one request's session: a stop its prefill left at 3, its finished turn at 6
    var toks <- [for (i in range64(9l)); i % 8l]
    prefix_insert_(cache, pool, s, toks, "", true)
    var inscope opening_state := s.dn_state
    kv_ensure(s, 6l)
    s.n_past = 6l
    s.dn_pos = 6l
    s.dn_state[0] = 61.0
    rows_walk(pool, s, 6l, true)
    prefix_insert_(cache, pool, s, toks)
    t |> equal(length(cache.states), 2, "the request left a stop and a finished turn")
    var carrier <- mk_recurrent(c, pool, 0l)
    var saved <- prefix_file_save_(c, cache, pool, carrier, toks, state_path)
    t |> equal(saved.why, "", "the save ran")
    t |> equal(saved.tokens, 6l, "it holds the finished turn's tokens, not the whole list")
    t |> equal(saved.bytes, int64(stat(state_path).size), "its byte count is the file's")
    var pool2 <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
    var cache2 <- create_prefix_cache_()
    var carrier2 <- mk_recurrent(c, pool2, 0l)
    var got <- prefix_file_restore_(c, cache2, pool2, carrier2, state_path)
    t |> equal(got.why, "", "the restore ran")
    t |> equal(length(cache2.states), 2, "the cache holds both checkpoints")
    t |> success(cache2.states[0].kind == PrefixStateKind.opening && cache2.states[1].kind == PrefixStateKind.tail, "each of its own kind")
    var early <- [for (i in range64(9l)); i < 4l ? i % 8l : 7l]
    var w0 <- mk_recurrent(c, pool2, 0l)
    t |> equal(prefix_attach_(cache2, pool2, w0, early), 3l, "a prompt that leaves the turn after the stop attaches the stop")
    t |> success(w0.dn_pos == 3l && sequence_equal(w0.dn_state, opening_state), "with the state the stop stood at")
    release_kv_pages_(w0)
    delete w0
    delete early
    var w <- mk_recurrent(c, pool2, 0l)
    t |> equal(prefix_attach_(cache2, pool2, w, toks), 6l, "a prompt past the checkpoint attaches all of it")
    t |> equal(w.dn_pos, 6l, "the recurrent state stands at the checkpoint")
    t |> success(sequence_equal(w.dn_state, s.dn_state) && sequence_equal(w.dn_conv_state, s.dn_conv_state), "and is the donor's")
    var bent := s.dn_state
    bent[0]++
    t |> success(!sequence_equal(w.dn_state, bent), "control: one state float off the donor's is seen")
    delete bent
    t |> equal(rows_walk(pool2, w, 6l, false), -1l, "every K and V byte of the six rows is the donor's")
    var none <- create_prefix_cache_(0l, 0l)
    var carrier3 <- mk_recurrent(c, pool2, 0l)
    var declined <- prefix_file_restore_(c, none, pool2, carrier3, state_path)
    t |> success(!empty(declined.why) && empty(declined.ids) && declined.tokens == 0l, "a cache that keeps no checkpoint declines the file by its own reason: '{declined.why}'")
    release_kv_pages_(w)
    release_kv_pages_(s)
    prefix_release_(cache2, pool2)
    prefix_release_(cache, pool)
    t |> equal(long_length(pool2.free_groups), pool2.capacity_groups, "every page of the second pool is back")
    delete declined
    delete carrier3
    delete none
    delete w
    delete got
    delete carrier2
    delete cache2
    delete pool2
    delete saved
    delete carrier
    delete toks
    delete s
    delete cache
    delete pool
}

//! the context-file test's cell: a recurrent file cut inside a checkpoint, and one whose checkpoint is another model's, refuse by name
def context_cut_cells(t : T?; c : Config; state_path : string) {
    let size = int64(stat(state_path).size)
    let words = 8l * 16l + 16l * 2l + 8l * 6l + 6l * 2l * 2l * 32l * 4l   //! where checkpoint 0's four words begin: past the header, the row sizes, six ids and their rows
    for (k in [(keep = size - 4l, at = -1l, value = 0u8, want = "cut short at checkpoint 1"),
               (keep = words + 8l, at = -1l, value = 0u8, want = "cut short at checkpoint 0"),
               (keep = 0l, at = words, value = 9u8, want = "checkpoint 0 is not this model's"),
               (keep = 0l, at = words + 16l, value = 9u8, want = "with a window of 9 ids")]) {
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var carrier <- mk_recurrent(c, pool, 0l)
        let path = path_join(test_temp_dir(), "dasllama_ctx_state_bad.bin")
        file_variant(state_path, path, k.keep, k.at, k.value)
        var r <- prefix_file_restore_(c, cache, pool, carrier, path)
        t |> success(find(r.why, k.want) >= 0, "the file refuses naming '{k.want}': '{r.why}'")
        t |> success(empty(r.ids) && r.tokens == 0l && empty(cache.states), "and answers no tokens and leaves no checkpoint of the file in the cache")
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "no page is held")
        delete r
        delete carrier
        delete cache
        delete pool
    }
}

//! the context-file test's cell: a save writes the checkpoints whose tokens open the list, shortest first, in whatever order
//! the cache holds them, and no other conversation's
def context_pick_cell(t : T?; c : Config; path : string) {
    var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
    var cache <- create_prefix_cache_()
    var toks <- [for (i in range64(9l)); i % 8l]
    var other <- [for (i in range64(9l)); 7l - i % 8l]
    var late <- mk_recurrent(c, pool, 6l)
    prefix_insert_(cache, pool, late, toks)
    var early <- mk_recurrent(c, pool, 3l)
    prefix_insert_(cache, pool, early, toks, "", true)
    var stray <- mk_recurrent(c, pool, 5l)
    prefix_insert_(cache, pool, stray, other)
    t |> equal(length(cache.states), 3, "the cache holds the finished turn, then the opening inside it, then another conversation's")
    var carrier <- mk_recurrent(c, pool, 0l)
    var saved <- prefix_file_save_(c, cache, pool, carrier, toks, path)
    t |> equal(saved.why, "", "the save ran")
    t |> equal(saved.tokens, 6l, "it holds the longest checkpoint's tokens, though the cache holds that one first")
    var pool2 <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
    var cache2 <- create_prefix_cache_()
    var carrier2 <- mk_recurrent(c, pool2, 0l)
    var got <- prefix_file_restore_(c, cache2, pool2, carrier2, path)
    t |> equal(got.why, "", "the restore ran")
    t |> equal(length(cache2.states), 2, "the other conversation's checkpoint is not in the file")
    var marked <- prefix_file_save_(c, cache, pool, carrier, toks, path, 7l)
    var cache3 <- create_prefix_cache_()
    var other_model <- prefix_file_restore_(c, cache3, pool2, carrier2, path, 9l)
    t |> success(empty(marked.why) && find(other_model.why, "its weights are not this model's") >= 0 && empty(cache3.states), "a file marked by one model's weights refuses a model of the same geometry and another mark: '{other_model.why}'")
    var same_model <- prefix_file_restore_(c, cache3, pool2, carrier2, path, 7l)
    t |> success(empty(same_model.why) && length(cache3.states) == 2, "control: under its own mark the file restores")
    prefix_release_(cache3, pool2)
    delete same_model
    delete other_model
    delete cache3
    delete marked
    t |> success(length(cache2.states) == 2 && long_length(cache2.states[0].tokens) == 3l && long_length(cache2.states[1].tokens) == 6l, "the file's checkpoints stand shortest first")
    release_kv_pages_(late)
    release_kv_pages_(early)
    release_kv_pages_(stray)
    prefix_release_(cache2, pool2)
    prefix_release_(cache, pool)
    delete got
    delete carrier2
    delete cache2
    delete pool2
    delete saved
    delete carrier
    delete stray
    delete early
    delete late
    delete other
    delete toks
    delete cache
    delete pool
}

//! the context-file test's cell: the refusals, each by name, none leaving a page held
def context_refusal_cells(t : T?; c : Config; dense_path, state_path : string) {
    var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
    var cache <- create_prefix_cache_()
    var carrier <- make_run_state_paged(c, pool)
    var absent <- [for (_i in range64(3l)); 7l]
    let no_path = path_join(test_temp_dir(), "dasllama_ctx_none.bin")
    var unheld <- prefix_file_save_(c, cache, pool, carrier, absent, no_path)
    t |> success(find(unheld.why, "holds no context") >= 0 && !stat(no_path).is_valid, "a save of tokens the cache does not hold writes no file: '{unheld.why}'")
    var s <- make_run_state_paged(c, pool)
    kv_ensure(s, 3l)
    s.n_past = 3l
    prefix_insert_(cache, pool, s, absent)
    var blocked <- prefix_file_save_(c, cache, pool, carrier, absent, path_join(path_join(test_temp_dir(), "no_such_folder"), "x.bin"))
    t |> success(find(blocked.why, "does not open for writing") >= 0, "a path that does not open for writing: '{blocked.why}'")
    release_kv_pages_(s)
    prefix_release_(cache, pool)
    let cases = [
        (file = no_path, keep = 0l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "does not open"),
        (file = dense_path, keep = 0l, at = 0l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "is not a context file"),
        (file = dense_path, keep = 0l, at = 8l, value = 9u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "is not a context file of this version"),
        (file = dense_path, keep = 40l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "is not a context file"),
        (file = dense_path, keep = 0l, at = -1l, value = 0u8, ctx = 32l, layers = 3l, dt = KVDtype.f32, want = "K/V planes"),
        (file = dense_path, keep = 0l, at = 8l * 11l, value = 9u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "another model's"),
        (file = state_path, keep = 0l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "recurrent state"),
        (file = dense_path, keep = 0l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f16, want = "K/V codecs"),
        (file = dense_path, keep = 0l, at = -1l, value = 0u8, ctx = 8l, layers = 2l, dt = KVDtype.f32, want = "this server's context 8"),
        (file = dense_path, keep = 400l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "cut short"),
        (file = dense_path, keep = 8l * 17l, at = -1l, value = 0u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "cut short"),
        (file = dense_path, keep = 0l, at = 8l * 16l + 16l * 2l, value = 99u8, ctx = 32l, layers = 2l, dt = KVDtype.f32, want = "names token 99")
    ]
    for (k, ki in cases, count()) {
        let cc = mk_config(k.layers, 4l, 2l, 16l, k.ctx)
        var p <- create_kv_pool_(cc, 4l, k.dt, k.dt)
        var pc <- create_prefix_cache_()
        var carry <- make_run_state_paged(cc, p)
        let path = k.keep > 0l || k.at >= 0l ? path_join(test_temp_dir(), "dasllama_ctx_case{ki}.bin") : k.file
        if (path != k.file) {
            file_variant(k.file, path, k.keep, k.at, k.value)
        }
        var r <- prefix_file_restore_(cc, pc, p, carry, path)
        t |> success(find(r.why, k.want) >= 0, "case {ki} refuses naming '{k.want}': '{r.why}'")
        t |> success(empty(r.ids) && r.tokens == 0l && empty(pc.entries) && empty(pc.states), "case {ki}: the cache takes nothing")
        t |> equal(long_length(p.free_groups), p.capacity_groups, "case {ki}: no page is held")
        delete r
        delete carry
        delete pc
        delete p
    }
    let narrow = mk_config(2l, 4l, 2l, 8l, 32l)
    var narrow_pool <- create_kv_pool_(narrow, 4l, KVDtype.f32, KVDtype.f32)
    var narrow_cache <- create_prefix_cache_()
    var narrow_carrier <- make_run_state_paged(narrow, narrow_pool)
    var other_rows <- prefix_file_restore_(narrow, narrow_cache, narrow_pool, narrow_carrier, dense_path)
    t |> success(find(other_rows.why, "K/V rows of plane 0") >= 0 && empty(narrow_cache.entries), "a model of the same layers, vocabulary and codec but another head size refuses by its rows: '{other_rows.why}'")
    delete other_rows
    delete narrow_carrier
    delete narrow_cache
    delete narrow_pool
    delete blocked
    delete s
    delete unheld
    delete absent
    delete carrier
    delete cache
    delete pool
}

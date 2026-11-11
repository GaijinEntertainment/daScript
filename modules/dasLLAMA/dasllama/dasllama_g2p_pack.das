options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_g2p_pack shared public

require dasllama/dasllama_lint public
require strings
require daslib/fio
require dasllama/dasllama_unicode   // bytes_str, bytes_compare

//! The front-end packs' file shape, shared by every language's reader: little-endian scalars,
//! the byte-sorted string table - an offset column per key and per value over one key blob
//! and one value blob - and the front-coded table - blocks of keys, each key the bytes it adds
//! to the key before it - both searched in place on the file bytes.

//! One byte-sorted string table of a pack: offsets into the file bytes
struct LexTable {
    n : int
    key_off : int64
    val_off : int64
    key_blob : int64
    val_blob : int64
}

def rd_u8(b : array<uint8> | #; o : int64) : int => int(b[o])
def rd_u16(b : array<uint8> | #; o : int64) : int => int(b[o]) | (int(b[o + 1l]) << 8)
def rd_u32(b : array<uint8> | #; o : int64) : uint => uint(b[o]) | (uint(b[o + 1l]) << 8u) | (uint(b[o + 2l]) << 16u) | (uint(b[o + 3l]) << 24u)
def rd_i32(b : array<uint8> | #; o : int64) : int => int(rd_u32(b, o))

//! the whole file as bytes; empty when it cannot be opened or holds nothing, for the caller's panic to name its pack
def pack_read_bytes(path : string) : array<uint8> {
    var inscope data : array<uint8>
    let f = fopen(path, "rb")
    return <- data if (f == null)
    fmap(f) $(var b : array<uint8>#) {
        let n = long_length(b)
        return if (n == 0l)
        data |> reserve(n)
        data |> resize(n)
        unsafe {
            memcpy(addr(data[0]), addr(b[0]), n)
        }
    }
    fclose(f)
    return <- data
}

//! the header every pack opens with - the magic, the version, the source note - checked against the
//! reader's own `magic` and `version`; `o` is left at the first table, and the source note is the
//! answer. A wrong magic or another version panics naming the file, the pack `kind` and its `builder`.
def read_pack_header(b : array<uint8>; path, kind, builder : string; magic : uint; version : int; var o : int64&) : string {
    if (rd_u32(b, o) != magic) {
        panic("dasLLAMA tts: bad magic in '{path}' (not a {kind})")
    }
    o += 4l
    let found = rd_i32(b, o)
    o += 4l
    if (found != version) {
        panic("dasLLAMA tts: '{path}' is a {kind} version {found} pack, and this build reads version {version} - rebuild it with {builder}")
    }
    let src_len = rd_u16(b, o)
    o += 2l
    let source = bytes_str(b, o, src_len)
    o += int64(src_len)
    return source
}

//! one table's header read at `o`, which moves past the table
def read_lex_table(b : array<uint8>; var o : int64&) : LexTable {
    var t = LexTable()
    t.n = rd_i32(b, o)
    o += 4l
    t.key_off = o
    o += int64(t.n + 1) * 4l
    t.val_off = o
    o += int64(t.n + 1) * 4l
    let key_len = rd_i32(b, o)
    o += 4l
    t.key_blob = o
    o += int64(key_len)
    let val_len = rd_i32(b, o)
    o += 4l
    t.val_blob = o
    o += int64(val_len)
    return t
}

//! the entry index of `word` in a table, or -1
def lex_find(data : array<uint8>; t : LexTable; word : string) : int {
    var inscope q <- to_bytes(word)
    var lo = 0
    var hi = t.n - 1
    while (lo <= hi) {
        let mid = (lo + hi) >> 1
        let key = rd_u32(data, t.key_off + int64(mid) * 4l)
        let key_end = rd_u32(data, t.key_off + int64(mid + 1) * 4l)
        let c = bytes_compare(data, t.key_blob + int64(key), int(key_end - key), q, 0l, length(q))
        return mid if (c == 0)
        if (c < 0) {
            lo = mid + 1
        } else {
            hi = mid - 1
        }
    }
    return -1
}

//! the value of entry `i` as the span [a, b) over the file bytes
def lex_value_span(data : array<uint8>; t : LexTable; i : int; var a : int64&; var b : int64&) {
    a = t.val_blob + int64(rd_u32(data, t.val_off + int64(i) * 4l))
    b = t.val_blob + int64(rd_u32(data, t.val_off + int64(i + 1) * 4l))
}

let FRONT_KEY_MAX = 64      //! the longest key a lookup holds in its local buffer; a longer word is no key
let FRONT_VALUE_BITS = 7    //! the value bits of a value-run byte
let FRONT_VALUE_MORE = 1 << FRONT_VALUE_BITS    //! the bit over them, set where another byte of the run follows
let FRONT_LEN_BITS = 4      //! an entry's head byte holds two lengths of this many bits: the prefix's over the suffix's
let FRONT_LEN_ESCAPE = 15   //! the head length that says the length is a byte of its own after the head
let private FRONT_HEAD_BYTES = 20l  //! a table's five u32 counts, ahead of its block offsets

//! One front-coded table of a pack: offsets into the file bytes
struct FrontTable {
    n_keys : int
    keys_per_block : int
    n_blocks : int
    key_max : int       //! the bytes of its longest key
    block_off : int64
    blob : int64
}

//! One entry of a block, read off its head: the key bytes it takes from the key before it, the key bytes of its own, and where those start - its value run follows them
struct private FrontEntry {
    prefix_len : int
    suffix_len : int
    suffix_at : int64
}

//! one table's header read at `o`, which moves past the table
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-stress-pack")]
def read_front_table(b : array<uint8>; var o : int64&) : FrontTable {
    let n_blocks = rd_i32(b, o + 8l)
    let blob_len = int64(rd_u32(b, o + 16l))
    let block_off = o + FRONT_HEAD_BYTES
    let blob = block_off + int64(n_blocks) * 4l
    let n_keys = rd_i32(b, o)
    let keys_per_block = rd_i32(b, o + 4l)
    let key_max = rd_i32(b, o + 12l)
    o = blob + blob_len
    return FrontTable(n_keys = n_keys, keys_per_block = keys_per_block, n_blocks = n_blocks, key_max = key_max, block_off = block_off, blob = blob)
}

def private front_entry(data : array<uint8>; at : int64) : FrontEntry {
    var o = at + 1l
    var prefix_len = int(data[at]) >> FRONT_LEN_BITS
    var suffix_len = int(data[at]) & FRONT_LEN_ESCAPE
    if (prefix_len == FRONT_LEN_ESCAPE) {
        prefix_len = int(data[o])
        o++
    }
    if (suffix_len == FRONT_LEN_ESCAPE) {
        suffix_len = int(data[o])
        o++
    }
    return FrontEntry(prefix_len = prefix_len, suffix_len = suffix_len, suffix_at = o)
}

def private front_block(data : array<uint8>; t : FrontTable; blk : int) : int64 => t.blob + int64(rd_u32(data, t.block_off + int64(blk) * 4l))

//! the value bits of the value-run byte at `o`
def front_value(data : array<uint8>; o : int64) : int => int(data[o]) & (FRONT_VALUE_MORE - 1)

//! whether the value run goes on past its byte at `o`
def front_value_more(data : array<uint8>; o : int64) : bool => int(data[o]) >= FRONT_VALUE_MORE

//! the offset one past the value run that starts at `o`
def front_value_end(data : array<uint8>; o : int64) : int64 {
    var e = o
    while (front_value_more(data, e)) {
        e++
    }
    return e + 1l
}

//! where the value run of the key in `q`'s first `qn` bytes starts in the file bytes, or -1; the key is in the table's own byte alphabet, and the search allocates nothing
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-stress-pack")]
def front_find(data : array<uint8>; t : FrontTable; q : uint8[FRONT_KEY_MAX]; qn : int) : int64 {
    return -1l if (qn <= 0)
    var lo = 0
    var hi = t.n_blocks - 1
    var blk = -1
    while (lo <= hi) {
        let mid = (lo + hi) >> 1
        let first_key = front_entry(data, front_block(data, t, mid))
        if (bytes_compare(q, 0l, qn, data, first_key.suffix_at, first_key.suffix_len) >= 0) {
            blk = mid
            lo = mid + 1
        } else {
            hi = mid - 1
        }
    }
    return -1l if (blk < 0)
    let block_n_keys = blk + 1 < t.n_blocks ? t.keys_per_block : t.n_keys - blk * t.keys_per_block
    var o = front_block(data, t, blk)
    var matched = 0
    for (_i in range(block_n_keys)) {
        let e = front_entry(data, o)
        let value = e.suffix_at + int64(e.suffix_len)
        return -1l if (e.prefix_len < matched)
        if (e.prefix_len == matched) {
            var k = 0
            while (k < e.suffix_len && matched < qn && data[e.suffix_at + int64(k)] == q[matched]) {
                k++
                matched++
            }
            return value if (k == e.suffix_len && matched == qn)
            return -1l if (matched == qn || (k < e.suffix_len && int(data[e.suffix_at + int64(k)]) > int(q[matched])))
        }
        o = front_value_end(data, value)
    }
    return -1l
}

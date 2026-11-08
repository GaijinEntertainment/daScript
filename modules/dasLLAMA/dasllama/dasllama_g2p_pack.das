options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_g2p_pack shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio

//! The front-end packs' file shape, shared by every language's reader: little-endian scalars,
//! and the byte-sorted string table - an offset column per key and per value over one key blob
//! and one value blob - searched in place by binary search on the file bytes.

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

def bytes_str(b : array<uint8> | #; o : int64; n : int) : string {
    var inscope bytes : array<uint8>
    bytes |> resize(n)
    for (i in range(n)) {
        bytes[i] = b[o + int64(i)]
    }
    return string(bytes)
}

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

def private key_compare(data : array<uint8>; t : LexTable; i : int; q : array<uint8>) : int {
    let a = t.key_blob + int64(rd_u32(data, t.key_off + int64(i) * 4l))
    let klen = int(rd_u32(data, t.key_off + int64(i + 1) * 4l) - rd_u32(data, t.key_off + int64(i) * 4l))
    let n = min(klen, length(q))
    for (k in range(n)) {
        let d = int(data[a + int64(k)]) - int(q[k])
        return d if (d != 0)
    }
    return klen - length(q)
}

//! the entry index of `word` in a table, or -1
def lex_find(data : array<uint8>; t : LexTable; word : string) : int {
    var inscope q <- to_bytes(word)
    var lo = 0
    var hi = t.n - 1
    while (lo <= hi) {
        let mid = (lo + hi) >> 1
        let c = key_compare(data, t, mid, q)
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

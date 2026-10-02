options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kv_dtype shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/enum_trait   // the enum's members as a loop: the ordinal and spelling lookups
require strings

//! KV-cache storage codec, chosen per session at create time; K and V carry independent
//! dtypes. f16 = half the KV bytes, near-lossless; f32 = bit-exact reference;
//! q8_0 = block codec (34B/32 elements, needs kv_dim/head_size % 32); tq4 = rotated 4-bit (18B/32, needs head_size a power of two).
enum KVDtype {
    f32
    f16
    q8_0
    tq4
}

//! tq4 cache-pointer dispatch tag: the read seam overloads on the cache pointer's element type, and
//! q8_0 already owns `uint8 const?` - a distinct newtype over uint8 gives tq4 its own overload
//! identity at zero runtime cost (byte-strided arithmetic, ABI identical to uint8).
typedef distinct TQ4B = uint8

//! Bytes per stored element (scalar codecs only; block codecs size via kv_row_bytes directly).
def kv_elem_bytes(dt : KVDtype) : int64 => dt == KVDtype.f16 ? 2l : 4l

//! A block codec stores whole 32-element blocks and addresses in bytes; the scalar codecs address in elements.
def kv_block_codec(dt : KVDtype) : bool => dt == KVDtype.q8_0 || dt == KVDtype.tq4

//! Head-slot offset in the side's own units (elements for scalar codecs, bytes for block codecs) — THE
//! layout seam: q8_0 32-blocks of {f16 d; int8 qs[32]} = 34 B, tq4 32-blocks of {f16 d; uint8 qs[16]} =
//! 18 B (elems % 32 == 0 by create). The _d wrappers prescale with it so the workers' walks stay unit-correct.
def kv_side_off(dt : KVDtype; elems : int64) : int64 {
    if (dt == KVDtype.q8_0) return elems / 32l * 34l
    if (dt == KVDtype.tq4) return elems / 32l * 18l
    return elems
}

//! Byte stride of one position row of width kv_dim under codec dt: the side offset of the whole
//! row, in bytes for a block codec already, scaled by the element size for a scalar one.
def kv_row_bytes(dt : KVDtype; kv_dim : int64) : int64 => kv_side_off(dt, kv_dim) * (kv_block_codec(dt) ? 1l : kv_elem_bytes(dt))

//! The units a device mirror addresses `elems` elements in: 32-element blocks under a block codec, elements under a scalar one.
def kv_row_units(dt : KVDtype; elems : int64) : int64 => kv_block_codec(dt) ? elems / 32l : elems

//! One such unit's bytes.
def kv_unit_bytes(dt : KVDtype) : int64 => kv_row_bytes(dt, kv_block_codec(dt) ? 32l : 1l)

//! The codec an ordinal names: the device tier's seats carry a `KVDtype` as its int.
def kv_dtype_of(ordinal : int) : KVDtype {
    for (dt in type<KVDtype>) {
        return dt if (int(dt) == ordinal)
    }
    panic("no KVDtype has ordinal {ordinal}")
    return KVDtype.f32
}

//! The codec a user's spelling names - `f32`, `f16`, `q8_0`, `tq4`, any case; `known` false (and f16) for any other text.
def kv_dtype_named(name : string) : tuple<dt : KVDtype; known : bool> {
    let want = to_lower(name)
    for (dt in type<KVDtype>) {
        return (dt = dt, known = true) if ("{dt}" == want)
    }
    return (dt = KVDtype.f16, known = false)
}

[call_macro(name = "kv_codec_side")]
class KvCodecSide : AstCallMacro {
    //! ``kv_codec_side(dt, p) <| $(tp) { ... }`` stamps the block once per ``KVDtype`` member behind a
    //! ladder on ``dt``, ``tp`` bound to the byte pointer ``p`` read as that codec's element type (q8_0
    //! ``uint8``, tq4 ``TQ4B``, f16 ``uint16``, f32 ``float``); nested once per side it is the K x V matrix.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(expr.arguments) == 3 && expr.arguments[2] is ExprMakeBlock, prog, expr.at,
            "expecting kv_codec_side(dtype, ptr) followed by a block")
        macro_verify(expr.arguments[0] is ExprVar && expr.arguments[1] is ExprVar, prog, expr.at,
            "kv_codec_side takes plain variables for the dtype and the pointer, got {describe(expr.arguments[0])} and {describe(expr.arguments[1])}")
        let emb = expr.arguments[2] as ExprMakeBlock
        macro_verify(emb._block is ExprBlock, prog, expr.at, "expecting a block body, got {describe(emb._block)}")
        var blk = emb._block as ExprBlock
        macro_verify(length(blk.arguments) == 1 && empty(blk.finalList), prog, expr.at,
            "expecting one block argument, the typed pointer, and no finally section")
        let tp = "{blk.arguments[0].name}"
        var dt = expr.arguments[0]
        var p = expr.arguments[1]
        var arm_q8 = bind_block_argument(blk, tp, qmacro($e(p)), expr.at)
        var arm_tq4 = bind_block_argument(blk, tp, qmacro(unsafe(reinterpret<TQ4B const?>($e(p)))), expr.at)
        var arm_f16 = bind_block_argument(blk, tp, qmacro(unsafe(reinterpret<uint16 const?>($e(p)))), expr.at)
        var arm_f32 = bind_block_argument(blk, tp, qmacro(unsafe(reinterpret<float const?>($e(p)))), expr.at)
        return qmacro_block() {
            if ($e(dt) == KVDtype.q8_0) {
                $e(arm_q8)
            } elif ($e(dt) == KVDtype.tq4) {
                $e(arm_tq4)
            } elif ($e(dt) == KVDtype.f16) {
                $e(arm_f16)
            } else {
                $e(arm_f32)
            }
        }
    }
}

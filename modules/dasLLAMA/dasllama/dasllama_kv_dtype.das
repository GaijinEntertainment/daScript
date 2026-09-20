options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kv_dtype shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

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

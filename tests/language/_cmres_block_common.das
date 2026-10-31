options gen2
module _cmres_block_common shared public

struct BlockReply {
    code : int
    message : string
    delay : int
}

[never_inline]
def call_with_result(cb : block<(value : int) : void>) : BlockReply {
    invoke(cb, 7)
    return BlockReply(code = 19, message = "callee", delay = 23)
}

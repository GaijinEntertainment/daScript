#include "daScript/misc/platform.h"

#include "bytecode.h"

namespace das {

int32_t evalByteCode ( const ByteCode * _ptr, Context * ctx, LineInfoArg * at ) {
    int32_t a = 0, b = 0, c = 0;
    bool cmp = false;
    ByteCode * cur = (ByteCode *)_ptr;
    while ( true ) {
        switch ( cur->op_code ) {
            case OpCode::op_nop:
                break;
            case OpCode::op_mov_a_arg:
                a = cast<int32_t>::to(ctx->abiThisBlockArguments()[cur->high]);
                break;
            case OpCode::op_dec_a:
                --a;
                break;
            case OpCode::op_cmple_a_low_zx:
                cmp = (a <= cur->low);
                break;
            case OpCode::op_cjmp:
                if ( cmp ) {
                    cur += cur->low;
                    continue;
                }
                break;
            case OpCode::op_mov_c_a:
                c = a;
                break;
            case OpCode::op_mov_a_low_zx:
                a = cur->low;
                break;
            case OpCode::op_mov_b_low_zx:
                b = cur->low;
                break;
            case OpCode::op_xchange_a_b:
                das::swap(a,b);
                break;
            case OpCode::op_add_b_a:
                b += a;
                break;
            case OpCode::op_loop:
                if ( --c > 0 ) {
                    cur += cur->low;
                    continue;
                }
                break;
            case OpCode::op_return_b:
                return b;
            default:
                ctx->throw_error_at(at, "invalid op-code %i", (int)cur->op_code);
                return 0;
        }
        ++cur;
    }
}

}

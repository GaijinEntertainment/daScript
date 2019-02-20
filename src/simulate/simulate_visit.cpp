#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"

// this has to be the last included file
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    void SimNode_CallBase::visitCall ( SimVisitor & vis ) {
        vis.arg(fnPtr->name,"fnPtr");
        V_SP(stackTop);
        for ( int32_t i=0; i!=nArguments; ++i ) {
            arguments[i] = arguments[i]->visit(vis);
        }
    }

    SimNode * SimNode_DeleteStructPtr::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(DeleteStructPtr);
        V_ARG(total);
        V_ARG(structSize);
        V_SUB(subexpr);
        V_END();
    }

    SimNode * SimNode_MakeBlock::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(MakeBlock);
        V_SUB(subexpr);
        V_SP(stackTop);
        V_SP_EX(argStackTop);
        V_END();
    }

    SimNode * SimNode_Assert::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Assert);
        V_SUB(subexpr);
        V_ARG(message);
        V_END();
    }

    SimNode * SimNode_Swizzle::visit ( SimVisitor & vis ) {
        const char * xyzw = "xyzw";
        char swizzle[4] = { xyzw[fields[0]], xyzw[fields[1]], xyzw[fields[2]], xyzw[fields[3]] };
        V_BEGIN();
        V_OP(Swizzle);
        V_SUB(value);
        V_ARG(swizzle);
        V_END();
    }

    SimNode * SimNode_FieldDeref::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(FieldDeref);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    SimNode * SimNode_PtrFieldDeref::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(PtrFieldDeref);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    SimNode * SimNode_SafeFieldDeref::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(SafeFieldDeref);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    SimNode * SimNode_SafeFieldDerefPtr::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(SafeFieldDerefPtr);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    SimNode * SimNode_At::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(At);
        V_SUB(value);
        V_SUB(index);
        V_ARG(stride);
        V_ARG(range);
        V_END();
    }

    SimNode * SimNode_StringBuilder::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(StringBuilder);
        // TODO: types?
        V_CALL();
        V_END();
    }

    SimNode * SimNode_Debug::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Debug);
        V_SUB(subexpr);
        // TODO: typeinfo?
        V_ARG(message);
        V_END();
    }

    SimNode * SimNode_GetLocal::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetLocal);
        V_SP(stackTop);
        V_END();
    }

    SimNode * SimNode_GetLocalRef::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetLocalRef);
        V_SP(stackTop);
        V_END();
    }

    SimNode * SimNode_InitLocal::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InitLocal);
        V_SP(stackTop);
        V_ARG(size);
        V_END();
    }

    SimNode * SimNode_GetArgument::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetArgument);
        V_ARG(index);
        V_END();
    }

    SimNode * SimNode_GetArgumentRef::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetArgumentRef);
        V_ARG(index);
        V_END();
    }

    SimNode * SimNode_GetBlockArgument::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetBlockArgument);
        V_SP(stackTop);
        V_ARG(index);
        V_END();
    }

    SimNode * SimNode_GetBlockArgumentRef::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetBlockArgumentRef);
        V_SP(stackTop);
        V_ARG(index);
        V_END();
    }

    SimNode * SimNode_GetGlobal::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(GetGlobal);
        V_ARG(offset);
        V_END();
    }

    SimNode * SimNode_TryCatch::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(TryCatch);
        V_SUB(try_block);
        V_SUB(catch_block);
        V_END();
    }

    SimNode * SimNode_Return::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Return);
        V_SUB_OPT(subexpr);
        V_END();
    }

    SimNode * SimNode_ReturnConst::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnConst);
        V_ARG(value);
        V_END();
    }

    SimNode * SimNode_ReturnAndCopy::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnAndCopy);
        V_SUB_OPT(subexpr);
        V_ARG(size);
        V_END();
    }

    SimNode * SimNode_ReturnAndMove::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnAndMove);
        V_SUB_OPT(subexpr);
        V_ARG(size);
        V_END();
    }

    SimNode * SimNode_ReturnReference::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnReference);
        V_SUB_OPT(subexpr);
        V_END();
    }

    SimNode * SimNode_ReturnAndCopyFromBlock::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnAndCopyFromBlock);
        V_SUB_OPT(subexpr);
        V_ARG(size);
        V_SP_EX(argStackTop);
        V_END();
    }

    SimNode * SimNode_ReturnAndMoveFromBlock::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnAndMoveFromBlock);
        V_SUB_OPT(subexpr);
        V_ARG(size);
        V_SP_EX(argStackTop);
        V_END();
    }

    SimNode * SimNode_ReturnReferenceFromBlock::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ReturnReferenceFromBlock);
        V_SUB_OPT(subexpr);
        V_END();
    }

    SimNode * SimNode_Break::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Break);
        V_END();
    }

    SimNode * SimNode_Ptr2Ref::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Ptr2Ref);
        V_SUB_OPT(subexpr);
        V_END();
    }

    SimNode * SimNode_NullCoalescingRef::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(NullCoalescingRef);
        V_SUB(subexpr);
        V_SUB(value);
        V_END();
    }

    SimNode * SimNode_New::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(New);
        V_ARG(bytes);
        V_END();
    }

    SimNode * SimNode_NewArray::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(NewArray);
        V_SUB(newNode);
        V_SP(stackTop);
        V_ARG(count);
        V_END();
    }

    SimNode * SimNode_ConstValue::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(ConstValue);
        V_ARG(value);
        V_END();
    }

    SimNode * SimNode_Zero::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Zero);
        V_END();
    }

    SimNode * SimNode_CopyReference::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(CopyReference);
        V_SUB(l);
        V_SUB(r);
        V_END();
    }

    SimNode * SimNode_CopyRefValue::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(CopyRefValue);
        V_SUB(l);
        V_SUB(r);
        V_ARG(size);
        V_END();
    }

    SimNode * SimNode_MoveRefValue::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(MoveRefValue);
        V_SUB(l);
        V_SUB(r);
        V_ARG(size);
        V_END();
    }

    SimNode * SimNode_Final::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(Final);
        for ( uint32_t i=0; i!=totalFinal; ++i ) {
            finalList[i] = finalList[i]->visit(vis);
        }
        V_END();
    }

    SimNode * SimNode_Block::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(Block);
        for ( uint32_t i=0; i!=total; ++i ) {
            list[i] = list[i]->visit(vis);
        }
        for ( uint32_t i=0; i!=totalFinal; ++i ) {
            finalList[i] = finalList[i]->visit(vis);
        }
        V_END();
    }
}


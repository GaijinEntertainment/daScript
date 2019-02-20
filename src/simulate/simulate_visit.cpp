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
}


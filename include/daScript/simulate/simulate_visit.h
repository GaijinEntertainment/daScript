#pragma once

namespace das {
    template <typename TT>
    SimNode * SimNode_NewHandle<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(NewHandle);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_DeleteHandlePtr<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(DeleteHandlePtr);
        V_ARG(total);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_FieldDerefR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SimNode_FieldDerefR2V);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_PtrFieldDerefR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(PtrFieldDerefR2V);
        V_SUB(value);
        V_ARG(offset);
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_FastCall<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(FastCall);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_Call<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Call);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_CallAndCopyOrMove<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(CallAndCopyOrMove);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_Invoke<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Invoke);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_InvokeAndCopyOrMove<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InvokeAndCopyOrMove);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_InvokeFn<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InvokeFn);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_InvokeLambda<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InvokeLambda);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_InvokeAndCopyOrMoveFn<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InvokeAndCopyOrMoveFn);
        V_CALL();
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_InvokeAndCopyOrMoveLambda<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(InvokeAndCopyOrMoveLambda);
        V_CALL();
        V_END();
    }

    template <typename CastTo, typename CastFrom>
    SimNode * SimNode_Cast<CastTo,CastFrom>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Cast); // TODO: type name?
        V_CALL();
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_LexicalCast<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(LexicalCast);
        V_CALL();
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetLocalR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetLocalR2V);
        V_SP(stackTop);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetLocalRefR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetLocalRefR2V);
        V_SP(stackTop);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_CopyLocal2LocalT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(CopyLocal2LocalT);
        V_SP_EX(stackTopLeft);
        V_SP_EX(stackTopRight);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_SetLocalRefT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetLocalRefT);
        V_SP(stackTop);
        V_SUB(value);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_SetLocalValueT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetLocalValueT);
        V_SP(stackTop);
        V_SUB(value);
        V_END();
    }
}

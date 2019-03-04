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
        V_OP_TT(FieldDerefR2V);
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
        V_OP(Cast); 
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
    SimNode * SimNode_AtR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(AtR2V);
        V_SUB(value);
        V_SUB(index);
        V_ARG(stride);
        V_ARG(offset);
        V_ARG(range);
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
    SimNode * SimNode_GetLocalRefR2VOff<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetLocalRefR2VOff);
        V_SP(stackTop);
        V_SP(offset);
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

    template <typename TT>
    SimNode * SimNode_SetCMResRefT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetCMResRefT);
        V_SP(offset);
        V_SUB(value);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_SetCMResValueT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetCMResValueT);
        V_SP(offset);
        V_SUB(value);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_SetLocalRefRefOffT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetLocalRefRefOffT);
        V_SP(stackTop);
        V_SP(offset);
        V_SUB(value);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_SetLocalValueRefOffT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(SetLocalValueRefOffT);
        V_SP(stackTop);
        V_SP(offset);
        V_SUB(value);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetArgumentR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetArgumentR2V);
        V_ARG(index);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetArgumentR2VOff<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetArgumentR2VOff);
        V_ARG(index);
        V_ARG(offset);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetBlockArgumentR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetBlockArgument);
        V_SP(stackTop);
        V_ARG(index);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_GetGlobalR2V<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(GetGlobalR2V);
        V_ARG(offset);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_Ref2Value<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(Ref2Value);
        V_SUB(subexpr);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_NullCoalescing<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(NullCoalescing);
        V_SUB(subexpr);
        V_SUB(value);
        V_END();
    }

    template <bool move>
    SimNode * SimNode_Ascend<move>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Ascend);
        V_SUB(subexpr);
        V_ARG(bytes);
        V_END();
    }

    template <bool move>
    SimNode * SimNode_AscendAndRef<move>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(AscendAndRef);
        V_SUB(subexpr);
        V_ARG(bytes);
        V_SP(stackTop);
        V_END();
    }

    template <int argCount>
    SimNode * SimNode_NewWithInitializer<argCount>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(NewWithInitializer);
        V_CALL();
        V_ARG(bytes);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_CopyValue<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(CopyValue);
        V_SUB(l);
        V_SUB(r);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_CopyRefValueT<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP_TT(CopyRefValueT);
        V_SUB(l);
        V_SUB(r);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_IfZeroThenElse<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP_TT(IfZeroThenElse);
        V_SUB(cond);
        V_SUB(if_true);
        V_SUB(if_false);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_IfNotZeroThenElse<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP_TT(IfNotZeroThenElse);
        V_SUB(cond);
        V_SUB(if_true);
        V_SUB(if_false);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_IfZeroThen<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP_TT(IfZeroThen);
        V_SUB(cond);
        V_SUB(if_true);
        V_END();
    }

    template <typename TT>
    SimNode * SimNode_IfNotZeroThen<TT>::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP_TT(IfNotZeroThen);
        V_SUB(cond);
        V_SUB(if_true);
        V_END();
    }

    template <typename OT, typename Fun, Fun PROP, bool SAFE, typename CTYPE>
    SimNode *  SimNode_PropertyImpl<OT,Fun,PROP,SAFE,CTYPE>::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP("PropertyImpl");
        V_SUB_OPT(subexpr);
        V_END();
    }

}

#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_match.h"

#include "daScript/simulate/runtime_array.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/runtime_range.h"
#include "daScript/simulate/hash.h"

namespace das
{
    // common for move and copy

    SimNode * makeLocalCMResMove (const LineInfo & at, Context & context, uint32_t offset, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetCMResOfs>(rE->at, offset);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetCMResOfs>(rE->at, offset);
                return right;
            }
        }
        // now, to the regular move
        auto left = context.code->makeNode<SimNode_GetCMResOfs>(at, offset);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            return context.code->makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else {
            DAS_ASSERTF(0, "we are calling makeLocalCMResMove where expression on a right is not a referece."
                        "we should not be here, script compiler should have caught this during compilation."
                        "compiler later will likely report internal compilation error.");
            return nullptr;
        }
    }

    SimNode * makeLocalCMResCopy(const LineInfo & at, Context & context, uint32_t offset, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() &&
                "we are calling makeLocalCMResCopy on a type, which can't be copied."
                "we should not be here, script compiler should have caught this during compilation."
                "compiler later will likely report internal compilation error.");
        auto right = rE->simulate(context);
        if ( rightType.isWorkhorseType() ) {
            if ( rightType.ref ) {
                return context.code->makeValueNode<SimNode_SetCMResRefT>(rightType.baseType,
                                                                         at, right, offset);
            } else {
                return context.code->makeValueNode<SimNode_SetCMResValueT>(rightType.baseType,
                                                                           at, right, offset);
            }
        }
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * rightC = (SimNode_CallBase *) right;
                rightC->cmresEval = context.code->makeNode<SimNode_GetCMResOfs>(rE->at, offset);
                return rightC;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * rightC = (SimNode_CallBase *) right;
                rightC->cmresEval = context.code->makeNode<SimNode_GetCMResOfs>(rE->at, offset);
                return rightC;
            }
        }
        // wo standard path
        auto left = context.code->makeNode<SimNode_GetCMResOfs>(rE->at, offset);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code->makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code->makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            auto resN = rightType.annotation->simulateCopy(context, at, left, right);
            if ( !resN ) {
                context.thisProgram->error("integration error, simulateCopy returned null",
                                           at, CompilationError::missing_node );
            }
            return resN;
        } else {
            return context.code->makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }

    SimNode * makeLocalRefMove (const LineInfo & at, Context & context, uint32_t stackTop, uint32_t offset, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocalRefOff>(rE->at, stackTop, offset);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocalRefOff>(rE->at, stackTop, offset);
                return right;
            }
        }
        // now, to the regular move
        auto left = context.code->makeNode<SimNode_GetLocalRefOff>(at, stackTop, offset);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            return context.code->makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else {
            DAS_ASSERTF(0, "we are calling makeLocalRefMove where expression on a right is not a referece."
                        "we should not be here, script compiler should have caught this during compilation."
                        "compiler later will likely report internal compilation error.");
            return nullptr;
        }
    }

    SimNode * makeLocalRefCopy(const LineInfo & at, Context & context, uint32_t stackTop, uint32_t offset, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() &&
                "we are calling makeLocalRefCopy on a type, which can't be copied."
                "we should not be here, script compiler should have caught this during compilation."
                "compiler later will likely report internal compilation error.");
        auto right = rE->simulate(context);
        if ( rightType.isWorkhorseType() ) {
            if ( rightType.isRef() ) {
                return context.code->makeValueNode<SimNode_SetLocalRefRefOffT>(rightType.baseType,
                                                                            at, right, stackTop, offset);
            } else {
                return context.code->makeValueNode<SimNode_SetLocalValueRefOffT>(rightType.baseType,
                                                                            at, right, stackTop, offset);
            }
        }
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * rightC = (SimNode_CallBase *) right;
                rightC->cmresEval = context.code->makeNode<SimNode_GetLocalRefOff>(rE->at, stackTop, offset);
                return rightC;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * rightC = (SimNode_CallBase *) right;
                rightC->cmresEval = context.code->makeNode<SimNode_GetLocalRefOff>(rE->at, stackTop, offset);
                return rightC;
            }
        }
        // wo standard path
        auto left = context.code->makeNode<SimNode_GetLocalRefOff>(rE->at, stackTop, offset);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code->makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code->makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            auto resN = rightType.annotation->simulateCopy(context, at, left, right);
            if ( !resN ) {
                context.thisProgram->error("integration error, simulateCopy returned null",
                                           at, CompilationError::missing_node );
            }
            return resN;
        } else {
            return context.code->makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }

    SimNode * makeLocalMove (const LineInfo & at, Context & context, uint32_t stackTop, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocal>(rE->at, stackTop);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocal>(rE->at, stackTop);
                return right;
            }
        }
        // now, to the regular move
        auto left = context.code->makeNode<SimNode_GetLocal>(at, stackTop);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            return context.code->makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else {
            DAS_ASSERTF(0, "we are calling makeLocalMove where expression on a right is not a referece."
                        "we should not be here, script compiler should have caught this during compilation."
                        "compiler later will likely report internal compilation error.");
            return nullptr;
        }
    }

    SimNode * makeLocalCopy(const LineInfo & at, Context & context, uint32_t stackTop, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() &&
                "we are calling makeLocalCopy on a type, which can't be copied."
                "we should not be here, script compiler should have caught this during compilation."
                "compiler later will likely report internal compilation error.");
        // TODO:
        //  expand to (while managing combinatorics explosion, policy?)
        //      1. local ref variables (iterator results etc)
        //      2. global variables
        //      3. arguments
        //      4. blocks
        //      5. common cross-copy scenarios
        if ( rightType.isWorkhorseType() ) {
            if ( rE->rtti_isVar() ) {
                auto rvar = static_pointer_cast<ExprVar>(rE);
                if ( rvar->local && !rvar->variable->type->ref && !rvar->variable->aliasCMRES ) {
                    return context.code->makeValueNode<SimNode_CopyLocal2LocalT>(rightType.baseType,
                                                                                at, stackTop,
                                                                                rvar->variable->stackTop);
                }
            }
            auto right = rE->simulate(context);
            if ( rightType.ref ) {
                return context.code->makeValueNode<SimNode_SetLocalRefT>(rightType.baseType,
                                                                        at, right, stackTop);
            } else {
                return context.code->makeValueNode<SimNode_SetLocalValueT>(rightType.baseType,
                                                                          at, right, stackTop);
            }
        }
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocal>(rE->at, stackTop);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = context.code->makeNode<SimNode_GetLocal>(rE->at, stackTop);
                return right;
            }
        }
        // now, to the regular copy
        auto left = context.code->makeNode<SimNode_GetLocal>(rE->at, stackTop);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code->makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code->makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            auto resN = rightType.annotation->simulateCopy(context, at, left, right);
            if ( !resN ) {
                context.thisProgram->error("integration error, simulateCopy returned null",
                                           at, CompilationError::missing_node );
            }
            return resN;
        } else {
            return context.code->makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }

    SimNode * makeCopy(const LineInfo & at, Context & context, const ExpressionPtr & lE, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() &&
                "we are calling makeCopy on a type, which can't be copied."
                "we should not be here, script compiler should have caught this during compilation."
                "compiler later will likely report internal compilation error.");
        // TODO:
        //  expand to (while managing combinatorics explosion, policy?)
        //      1. local ref variables (iterator results etc)
        //      2. global variables
        //      3. arguments
        //      4. blocks
        //      5. common cross-copy scenarios
        if ( rightType.isWorkhorseType() && lE->rtti_isVar() ) {
            auto var = static_pointer_cast<ExprVar>(lE);
            if ( var->local && !var->variable->type->ref ) {
                if ( var->variable->aliasCMRES ) {
                    auto right = rE->simulate(context);
                    if ( rightType.isWorkhorseType() ) {
                        if ( rightType.ref ) {
                            return context.code->makeValueNode<SimNode_SetCMResRefT>(rightType.baseType,
                                                                                     at, right, 0);
                        } else {
                            return context.code->makeValueNode<SimNode_SetCMResValueT>(rightType.baseType,
                                                                                       at, right, 0);
                        }
                    }
                } else {
                    if ( rE->rtti_isVar() ) {
                        auto rvar = static_pointer_cast<ExprVar>(rE);
                        if ( rvar->local && !rvar->variable->type->ref&& !rvar->variable->aliasCMRES ) {
                            return context.code->makeValueNode<SimNode_CopyLocal2LocalT>(rightType.baseType,
                                    at, var->variable->stackTop, rvar->variable->stackTop);
                        }
                    }
                    auto right = rE->simulate(context);
                    if ( rightType.ref ) {
                        return context.code->makeValueNode<SimNode_SetLocalRefT>(rightType.baseType,
                                    at, right, var->variable->stackTop);
                    } else {
                        return context.code->makeValueNode<SimNode_SetLocalValueT>(rightType.baseType,
                                    at, right, var->variable->stackTop);
                    }
                }
            }
        }
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = lE->simulate(context);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = lE->simulate(context);
                return right;
            }
        }
        // now, to the regular copy
        auto left = lE->simulate(context);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code->makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code->makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            auto resN = rightType.annotation->simulateCopy(context, at, left, right);
            if ( !resN ) {
                context.thisProgram->error("integration error, simulateCopy returned null",
                                           at, CompilationError::missing_node );
            }
            return resN;
        } else {
            return context.code->makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }

    SimNode * makeMove (const LineInfo & at, Context & context, const ExpressionPtr & lE, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        // now, call with CMRES
        if ( rE->rtti_isCall() ) {
            auto cll = static_pointer_cast<ExprCall>(rE);
            if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = lE->simulate(context);
                return right;
            }
        }
        // now, invoke with CMRES
        if ( rE->rtti_isInvoke() ) {
            auto cll = static_pointer_cast<ExprInvoke>(rE);
            if ( cll->isCopyOrMove() ) {
                SimNode_CallBase * right = (SimNode_CallBase *) rE->simulate(context);
                right->cmresEval = lE->simulate(context);
                return right;
            }
        }
        // now to the regular one
        if ( rightType.isRef() ) {
            auto left = lE->simulate(context);
            auto right = rE->simulate(context);
            return context.code->makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else {
            DAS_ASSERTF(0, "we are calling makeMove where expression on a right is not a referece."
                   "we should not be here, script compiler should have caught this during compilation."
                   "compiler later will likely report internal compilation error.");
            return nullptr;
        }
    }

    SimNode * Function::simulate (Context & context) const {
        if ( builtIn ) {
            DAS_ASSERTF(0, "can only simulate non built-in function");
            return nullptr;
        }
        if ( fastCall ) {
            assert(totalStackSize == sizeof(Prologue) && "function can't allocate stack");
            assert((result->isWorkhorseType() || result->isVoid()) && "fastcall can only return a workhorse type");
            assert(body->rtti_isBlock() && "function must contain a block");
            auto block = static_pointer_cast<ExprBlock>(body);
            assert(block->list.size()==1 && "fastcall is only one expr in a function body");
            if ( block->list.back()->rtti_isReturn() ) {
                assert(block->list.back()->rtti_isReturn() && "fastcall body expr is return");
                auto retE = static_pointer_cast<ExprReturn>(block->list.back());
                assert(retE->subexpr && "fastcall must return a value");
                return retE->subexpr->simulate(context);
            } else {
                return block->list.back()->simulate(context);
            }
        } else {
            return body->simulate(context);
        }
    }

    SimNode * Expression::trySimulate (Context &, uint32_t, Type ) const {
        return nullptr;
    }

    void ExprMakeLocal::setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) {
        useStackRef = ref;
        useCMRES = cmres;
        doesNotNeedSp = true;
        doesNotNeedInit = true;
        stackTop = sp;
        extraOffset = off;
    }

    vector<SimNode *> ExprMakeLocal::simulateLocal ( Context & /*context*/ ) const {
        return vector<SimNode *>();
    }

    void ExprMakeStructure::setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) {
        ExprMakeLocal::setRefSp(ref, cmres, sp, off);
        int total = int(structs.size());
        int stride = makeType->getStride();
        // we go through all fields, and if its [[ ]] field
        // we tell it to piggy-back on our current sp, with appropriate offset
        for ( int index=0; index != total; ++index ) {
            auto & fields = structs[index];
            for ( const auto & decl : *fields ) {
                auto field = makeType->structType->findField(decl->name);
                assert(field && "should have failed in type infer otherwise");
                if ( decl->value->rtti_isMakeLocal() ) {
                    uint32_t offset =  extraOffset + index*stride + field->offset;
                    auto mkl = static_pointer_cast<ExprMakeLocal>(decl->value);
                    mkl->setRefSp(ref, cmres, sp, offset);
                } else if ( decl->value->rtti_isCall() ) {
                    auto cll = static_pointer_cast<ExprCall>(decl->value);
                    if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                        cll->doesNotNeedSp = true;
                    }
                } else if ( decl->value->rtti_isInvoke() ) {
                    auto cll = static_pointer_cast<ExprInvoke>(decl->value);
                    if ( cll->isCopyOrMove() ) {
                        cll->doesNotNeedSp = true;
                    }
                }
            }
        }
    }

    vector<SimNode *> ExprMakeStructure::simulateLocal (Context & context) const {
        vector<SimNode *> simlist;
        // init with 0
        int total = int(structs.size());
        int stride = makeType->getStride();
        if ( !doesNotNeedInit ) {
            SimNode * init0;
            if ( useCMRES ) {
                init0 = context.code->makeNode<SimNode_InitLocalCMRes>(at,extraOffset,stride * total);
            } else if ( useStackRef ) {
                init0 = context.code->makeNode<SimNode_InitLocalRef>(at,stackTop,extraOffset,stride * total);
            } else {
                init0 = context.code->makeNode<SimNode_InitLocal>(at,stackTop + extraOffset,stride * total);
            }
            simlist.push_back(init0);
        }
        for ( int index=0; index != total; ++index ) {
            auto & fields = structs[index];
            for ( const auto & decl : *fields ) {
                auto field = makeType->structType->findField(decl->name);
                assert(field && "should have failed in type infer otherwise");
                uint32_t offset =  extraOffset + index*stride + field->offset;
                SimNode * cpy;
                if ( decl->value->rtti_isMakeLocal() ) {
                    // so what happens here, is we ask it for the generated commands and append it to this list only
                    auto mkl = static_pointer_cast<ExprMakeLocal>(decl->value);
                    auto lsim = mkl->simulateLocal(context);
                    simlist.insert(simlist.end(), lsim.begin(), lsim.end());
                    continue;
                } else if ( useCMRES ) {
                    if ( decl->moveSemantic ){
                        cpy = makeLocalCMResMove(at,context,offset,decl->value);
                    } else {
                        cpy = makeLocalCMResCopy(at,context,offset,decl->value);
                    }
                } else if ( useStackRef ) {
                    if ( decl->moveSemantic ){
                        cpy = makeLocalRefMove(at,context,stackTop,offset,decl->value);
                    } else {
                        cpy = makeLocalRefCopy(at,context,stackTop,offset,decl->value);
                    }
                } else {
                    if ( decl->moveSemantic ){
                        cpy = makeLocalMove(at,context,stackTop+offset,decl->value);
                    } else {
                        cpy = makeLocalCopy(at,context,stackTop+offset,decl->value);
                    }
                }
                if ( !cpy ) {
                    context.thisProgram->error("internal compilation error, can't generate structure initialization", at);
                }
                simlist.push_back(cpy);
            }
        }
        return simlist;
    }

    SimNode * ExprMakeStructure::simulate (Context & context) const {
        SimNode_Block * block;
        if ( useCMRES ) {
            block = context.code->makeNode<SimNode_MakeLocalCMRes>(at);
        } else {
            block = context.code->makeNode<SimNode_MakeLocal>(at, stackTop);
        }
        auto simlist = simulateLocal(context);
        block->total = int(simlist.size());
        block->list = (SimNode **) context.code->allocate(sizeof(SimNode *)*block->total);
        for ( uint32_t i = 0; i != block->total; ++i )
            block->list[i] = simlist[i];
        return block;
    }

    void ExprMakeArray::setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) {
        ExprMakeLocal::setRefSp(ref, cmres, sp, off);
        int total = int(values.size());
        uint32_t stride = recordType->getSizeOf();
        for ( int index=0; index != total; ++index ) {
            auto & val = values[index];
            if ( val->rtti_isMakeLocal() ) {
                uint32_t offset =  extraOffset + index*stride;
                auto mkl = static_pointer_cast<ExprMakeLocal>(val);
                mkl->setRefSp(ref, cmres, sp, offset);
            } else if ( val->rtti_isCall() ) {
                auto cll = static_pointer_cast<ExprCall>(val);
                if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                    cll->doesNotNeedSp = true;
                }
            } else if ( val->rtti_isInvoke() ) {
                auto cll = static_pointer_cast<ExprInvoke>(val);
                if ( cll->isCopyOrMove() ) {
                    cll->doesNotNeedSp = true;
                }
            }
        }
    }

    vector<SimNode *> ExprMakeArray::simulateLocal (Context & context) const {
        vector<SimNode *> simlist;
        // init with 0
        int total = int(values.size());
        uint32_t stride = recordType->getSizeOf();
        if ( !doesNotNeedInit ) {
            SimNode * init0;
            if ( useCMRES ) {
                init0 = context.code->makeNode<SimNode_InitLocalCMRes>(at,extraOffset,stride * total);
            } else if ( useStackRef ) {
                init0 = context.code->makeNode<SimNode_InitLocalRef>(at,stackTop,extraOffset,stride * total);
            } else {
                init0 = context.code->makeNode<SimNode_InitLocal>(at,stackTop + extraOffset,stride * total);
            }
            simlist.push_back(init0);
        }
        for ( int index=0; index != total; ++index ) {
            auto & val = values[index];
            uint32_t offset = extraOffset + index*stride;
            SimNode * cpy;
            if ( val->rtti_isMakeLocal() ) {
                // so what happens here, is we ask it for the generated commands and append it to this list only
                auto mkl = static_pointer_cast<ExprMakeLocal>(val);
                auto lsim = mkl->simulateLocal(context);
                simlist.insert(simlist.end(), lsim.begin(), lsim.end());
                continue;
            } else if ( useCMRES ) {
                cpy = makeLocalCMResCopy(at,context,offset,val);
            } else if ( useStackRef ) {
                cpy = makeLocalRefCopy(at,context,stackTop,offset,val);
            } else {
                cpy = makeLocalCopy(at,context,stackTop+offset,val);
            }
            if ( !cpy ) {
                context.thisProgram->error("internal compilation error, can't generate array initialization", at);
            }
            simlist.push_back(cpy);
        }
        return simlist;
    }

    SimNode * ExprMakeArray::simulate (Context & context) const {
        SimNode_Block * block;
        if ( useCMRES ) {
            block = context.code->makeNode<SimNode_MakeLocalCMRes>(at);
        } else {
            block = context.code->makeNode<SimNode_MakeLocal>(at, stackTop);
        }
        auto simlist = simulateLocal(context);
        block->total = int(simlist.size());
        block->list = (SimNode **) context.code->allocate(sizeof(SimNode *)*block->total);
        for ( uint32_t i = 0; i != block->total; ++i )
            block->list[i] = simlist[i];
        return block;
    }

    SimNode * ExprRef2Value::simulate (Context & context) const {
        return GetR2V(context, at, type, subexpr->simulate(context));
    }

    SimNode * ExprAddr::simulate (Context & context) const {
        assert(func->index>=0 && "how, we specified in the unused");
        Func fn; fn.index = func->index + 1;
        return context.code->makeNode<SimNode_ConstValue>(at,cast<Func>::from(fn));
    }

    SimNode * ExprPtr2Ref::simulate (Context & context) const {
        return context.code->makeNode<SimNode_Ptr2Ref>(at,subexpr->simulate(context));
    }

    SimNode * ExprNullCoalescing::simulate (Context & context) const {
        if ( type->ref ) {
            return context.code->makeNode<SimNode_NullCoalescingRef>(at,subexpr->simulate(context),defaultValue->simulate(context));
        } else {
            return context.code->makeValueNode<SimNode_NullCoalescing>(type->baseType,at,subexpr->simulate(context),defaultValue->simulate(context));
        }
    }

    SimNode * ExprConst::simulate (Context & context) const {
        return context.code->makeNode<SimNode_ConstValue>(at,value);
    }

    SimNode * ExprConstString::simulate (Context & context) const {
        char * str = context.code->allocateString(text);
        return context.code->makeNode<SimNode_ConstString>(at,str);
    }

    SimNode * ExprStaticAssert::simulate (Context &) const {
        return nullptr;
    }

    SimNode * ExprAssert::simulate (Context & context) const {
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.code->makeNode<SimNode_Assert>(at,arguments[0]->simulate(context),context.code->allocateString(message));
    }

    SimNode * ExprDebug::simulate (Context & context) const {
        TypeInfo * pTypeInfo = context.thisHelper->makeTypeInfo(nullptr, arguments[0]->type);
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.code->makeNode<SimNode_Debug>(at,
                                               arguments[0]->simulate(context),
                                               pTypeInfo,
                                               context.code->allocateString(message));
    }

    SimNode * ExprMakeLambda::simulate (Context & context) const {
        DAS_ASSERTF(0, "we should not be here ever, ExprMakeLambda should completly fold during type inference.");
        context.thisProgram->error("internal compilation error, generating node for ExprMakeLambda", at);
        return nullptr;
    }

    SimNode * ExprMakeBlock::simulate (Context & context) const {
        uint32_t argSp = static_pointer_cast<ExprBlock>(block)->stackTop;
        return context.code->makeNode<SimNode_MakeBlock>(at,block->simulate(context),argSp,stackTop);
    }

    bool ExprInvoke::isCopyOrMove() const {
        auto blockT = arguments[0]->type;
        return blockT->firstType && blockT->firstType->isRefType() && !blockT->firstType->ref;
    }

    SimNode * ExprInvoke::simulate (Context & context) const {
        auto blockT = arguments[0]->type;
        SimNode_CallBase * pInvoke;
        if ( isCopyOrMove() ) {
            auto getSp = context.code->makeNode<SimNode_GetLocal>(at,stackTop);
            if ( blockT->baseType==Type::tBlock ) {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_InvokeAndCopyOrMove>(
                                                    int(arguments.size()), at, getSp);
            } else if ( blockT->baseType==Type::tFunction ) {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_InvokeAndCopyOrMoveFn>(
                                                    int(arguments.size()), at, getSp);
            } else {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_InvokeAndCopyOrMoveLambda>(
                                                    int(arguments.size()), at, getSp);
            }
        } else {
            if ( blockT->baseType==Type::tBlock ) {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_Invoke>(int(arguments.size()),at);
            } else if ( blockT->baseType==Type::tFunction ) {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_InvokeFn>(int(arguments.size()),at);
            } else {
                pInvoke = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_InvokeLambda>(int(arguments.size()),at);
            }
        }
        pInvoke->debugInfo = at;
        if ( int nArg = (int) arguments.size() ) {
            pInvoke->arguments = (SimNode **) context.code->allocate(nArg * sizeof(SimNode *));
            pInvoke->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pInvoke->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pInvoke->arguments = nullptr;
            pInvoke->nArguments = 0;
        }
        return pInvoke;
    }

    SimNode * ExprErase::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.code->makeValueNode<SimNode_TableErase>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            DAS_ASSERTF(0, "we should not even be here. erase can only accept tables. infer type should have failed.");
            context.thisProgram->error("internal compilation error, generating erase for non-table type", at);
            return nullptr;
        }
    }

    SimNode * ExprFind::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.code->makeValueNode<SimNode_TableFind>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            DAS_ASSERTF(0, "we should not even be here. find can only accept tables. infer type should have failed.");
            context.thisProgram->error("internal compilation error, generating find for non-table type", at);
            return nullptr;
        }
    }

    SimNode * ExprTypeInfo::simulate (Context & context) const {
        DAS_ASSERTF(0, "we should not even be here. typeinfo should resolve to const during infer pass.");
        context.thisProgram->error("internal compilation error, generating typeinfo(...)", at);
        return nullptr;
    }
    
    SimNode * ExprDelete::simulate (Context & context) const {
        uint32_t total = uint32_t(subexpr->type->getCountOf());
        auto sube = subexpr->simulate(context);
        if ( subexpr->type->baseType==Type::tArray ) {
            auto stride = subexpr->type->firstType->getSizeOf();
            return context.code->makeNode<SimNode_DeleteArray>(at, sube, total, stride);
        } else if ( subexpr->type->baseType==Type::tTable ) {
            auto vts_add_kts = subexpr->type->firstType->getSizeOf() +
                subexpr->type->secondType->getSizeOf();
            return context.code->makeNode<SimNode_DeleteTable>(at, sube, total, vts_add_kts);
        } else if ( subexpr->type->baseType==Type::tPointer ) {
            if ( subexpr->type->firstType->baseType==Type::tStructure ) {
                auto structSize = subexpr->type->firstType->getSizeOf();
                return context.code->makeNode<SimNode_DeleteStructPtr>(at, sube, total, structSize);
            } else {
                auto ann = subexpr->type->firstType->annotation;
                assert(ann->canDeletePtr() && "has to be able to delete ptr");
                auto resN = ann->simulateDeletePtr(context, at, sube, total);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateDelete returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            }
        } else if ( subexpr->type->baseType==Type::tHandle ) {
            auto ann = subexpr->type->annotation;
            assert(ann->canDelete() && "has to be able to delete");
            auto resN =  ann->simulateDelete(context, at, sube, total);
            if ( !resN ) {
                context.thisProgram->error("integration error, simulateDelete returned null",
                                           at, CompilationError::missing_node );
            }
            return resN;
        } else if ( subexpr->type->baseType==Type::tString ) {
            return context.code->makeNode<SimNode_DeleteString>(at, sube, total);
        } else {
            DAS_ASSERTF(0, "we should not be here. this is delete for unsupported type. infer types should have failed.");
            context.thisProgram->error("internal compilation error, generating node for unsupported ExprDelete", at);
            return nullptr;
        }
    }
    SimNode * ExprCast::trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const {
        return subexpr->trySimulate(context, extraOffset, r2vType);
    }

    SimNode * ExprCast::simulate (Context & context) const {
        return subexpr->simulate(context);
    }

    SimNode * ExprAscend::simulate (Context & context) const {
        auto se = subexpr->simulate(context);
        auto bytes = subexpr->type->getSizeOf();
        if ( useStackRef ) {
            return context.code->makeNode<SimNode_AscendAndRef<false>>(at, se, bytes, stackTop);
        } else {
            return context.code->makeNode<SimNode_Ascend<false>>(at, se, bytes);
        }
    }

    SimNode * ExprNew::simulate (Context & context) const {
        SimNode * newNode;
        if ( typeexpr->baseType == Type::tHandle ) {
            assert(typeexpr->annotation->canNew() && "how???");
            newNode = typeexpr->annotation->simulateGetNew(context, at);
            if ( !newNode ) {
                context.thisProgram->error("integration error, simulateGetNew returned null",
                                           at, CompilationError::missing_node );
            }
        } else {
            int32_t bytes = type->firstType->getSizeOf();
            if ( initializer ) {
                auto pCall = (SimNode_CallBase *) context.code->makeNodeUnroll<SimNode_NewWithInitializer>(int(arguments.size()),at,bytes);
                pCall->cmresEval = nullptr;
                newNode = ExprCall::simulateCall(func->shared_from_this(), this, context, pCall);
            } else {
                newNode = context.code->makeNode<SimNode_New>(at,bytes);
            }
        }
        if ( type->dim.size() ) {
            uint32_t count = type->getCountOf();
            return context.code->makeNode<SimNode_NewArray>(at,newNode,stackTop,count);
        } else {
            return newNode;
        }
    }

    SimNode * ExprAt::trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const {
        if ( subexpr->type->isVectorType() ) {
            return nullptr;
        } else if ( subexpr->type->isGoodTableType() ) {
            return nullptr;
        } else if ( subexpr->type->isHandle() ) {
            SimNode * result;
            if ( r2vType!=Type::none ) {
                result = subexpr->type->annotation->simulateGetAtR2V(context, at, index->type, subexpr, index, extraOffset);
                if ( !result ) {
                    context.thisProgram->error("integration error, simulateGetAtR2V returned null",
                                               at, CompilationError::missing_node );
                }
            } else {
                result = subexpr->type->annotation->simulateGetAt(context, at, index->type, subexpr, index, extraOffset);
                if ( !result ) {
                    context.thisProgram->error("integration error, simulateGetAt returned null",
                                               at, CompilationError::missing_node );
                }
            }
            return result;
        } else if ( subexpr->type->isGoodArrayType() ) {
            auto prv = subexpr->simulate(context);
            auto pidx = index->simulate(context);
            uint32_t stride = subexpr->type->firstType->getSizeOf();
            if ( r2vType!=Type::none ) {
                return context.code->makeValueNode<SimNode_ArrayAtR2V>(r2vType, at, prv, pidx, stride, extraOffset);
            } else {
                return context.code->makeNode<SimNode_ArrayAt>(at, prv, pidx, stride, extraOffset);
            }
        } else {
            uint32_t range = subexpr->type->dim.back();
            uint32_t stride = subexpr->type->getStride();
            if ( index->rtti_isConstant() ) {
                // if its constant index, like a[3]..., we try to let node bellow simulate
                auto idxCE = static_pointer_cast<ExprConst>(index);
                uint32_t idxC = cast<uint32_t>::to(idxCE->value);
                if ( idxC >= range ) {
                    context.thisProgram->error("index out of range", at, CompilationError::index_out_of_range);
                    return nullptr;
                }
                auto tnode = subexpr->trySimulate(context, extraOffset + idxC*stride, r2vType);
                if ( tnode ) {
                    return tnode;
                }
            }
            // regular scenario
            auto prv = subexpr->simulate(context);
            auto pidx = index->simulate(context);
            if ( r2vType!=Type::none ) {
                return context.code->makeValueNode<SimNode_AtR2V>(r2vType, at, prv, pidx, stride, extraOffset, range);
            } else {
                return context.code->makeNode<SimNode_At>(at, prv, pidx, stride, extraOffset, range);
            }
        }
    }

    SimNode * ExprAt::simulate (Context & context) const {

        if ( subexpr->type->isVectorType() ) {
            auto prv = subexpr->simulate(context);
            auto pidx = index->simulate(context);
            uint32_t range = subexpr->type->getVectorDim();
            uint32_t stride = type->getSizeOf();
            if ( subexpr->type->ref ) {
                auto res = context.code->makeNode<SimNode_At>(at, prv, pidx, stride, 0, range);
                if ( r2v ) {
                    return ExprRef2Value::GetR2V(context, at, type, res);
                } else {
                    return res;
                }
            } else {
                switch ( type->baseType ) {
                    case tInt:      return context.code->makeNode<SimNode_AtVector<int32_t>>(at, prv, pidx, range);
                    case tUInt:     return context.code->makeNode<SimNode_AtVector<uint32_t>>(at, prv, pidx, range);
                    case tFloat:    return context.code->makeNode<SimNode_AtVector<float>>(at, prv, pidx, range);
                    default:
                        DAS_ASSERTF(0, "we should not even be here. infer type should have failed on unsupported_vector[blah]");
                        context.thisProgram->error("internal compilation error, generating vector at for unsupported vector type.", at);
                        return nullptr;
                }
            }
        } else if ( subexpr->type->isGoodTableType() ) {
            auto prv = subexpr->simulate(context);
            auto pidx = index->simulate(context);
            uint32_t valueTypeSize = subexpr->type->secondType->getSizeOf();
            auto res = context.code->makeValueNode<SimNode_TableIndex>(subexpr->type->firstType->baseType, at, prv, pidx, valueTypeSize, 0);
            if ( r2v ) {
                return ExprRef2Value::GetR2V(context, at, type, res);
            } else {
                return res;
            }
        } else {
            if ( r2v ) {
                return trySimulate(context, 0, type->baseType);
            } else {
                return trySimulate(context, 0, Type::none);
            }
        }
    }
    
    vector<SimNode *> ExprBlock::collectExpressions ( Context & context, const vector<ExpressionPtr> & lis ) const {
        vector<SimNode *> simlist;
        for ( auto & node : lis ) {
            if ( node->rtti_isLet()) {
                shared_ptr<ExprLet> pLet = static_pointer_cast<ExprLet>(node);
                auto letInit = ExprLet::simulateInit(context, pLet.get());
                simlist.insert(simlist.end(), letInit.begin(), letInit.end());
                continue;
            }
            if ( auto simE = node->simulate(context) ) {
                simlist.push_back(simE);
            }
        }
        return simlist;
    }
    
    void ExprBlock::simulateFinal ( Context & context, SimNode_Final * block ) const {
        vector<SimNode *> simFList = collectExpressions(context, finalList);
        block->totalFinal = int(simFList.size());
        if ( block->totalFinal ) {
            block->finalList = (SimNode **) context.code->allocate(sizeof(SimNode *)*block->totalFinal);
            for ( uint32_t i = 0; i != block->totalFinal; ++i )
                block->finalList[i] = simFList[i];
        }
    }

    void ExprBlock::simulateBlock ( Context & context, SimNode_Block * block ) const {
        vector<SimNode *> simlist = collectExpressions(context, list);
        block->total = int(simlist.size());
        if ( block->total ) {
            block->list = (SimNode **) context.code->allocate(sizeof(SimNode *)*block->total);
            for ( uint32_t i = 0; i != block->total; ++i )
                block->list[i] = simlist[i];
        }
    }

    SimNode * ExprBlock::simulate (Context & context) const {
        vector<SimNode *> simlist = collectExpressions(context, list);
        // TODO: what if list size is 0?
        if ( simlist.size()!=1 || isClosure || finalList.size() ) {
            auto block = isClosure ? context.code->makeNode<SimNode_ClosureBlock>(at, type!=nullptr && type->baseType!=Type::tVoid, annotationData)
                : context.code->makeNode<SimNode_Block>(at);
            block->total = int(simlist.size());
            if ( block->total ) {
                block->list = (SimNode **) context.code->allocate(sizeof(SimNode *)*block->total);
                for ( uint32_t i = 0; i != block->total; ++i )
                    block->list[i] = simlist[i];
            }
            if ( !inTheLoop ) {
                simulateFinal(context, block);
            }
            return block;
        } else {
            return simlist[0];
        }
    }

    SimNode * ExprSwizzle::trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const {
        if ( !value->type->ref ) {
            return nullptr;
        }
        uint32_t offset = fields[0] * sizeof(float);
        if ( auto chain = value->trySimulate(context, extraOffset + offset, r2vType) ) {
            return chain;
        }
        auto simV = value->simulate(context);
        if ( r2vType!=Type::none ) {
            return context.code->makeValueNode<SimNode_FieldDerefR2V>(r2vType,at,simV,offset + extraOffset);
        } else {
            return context.code->makeNode<SimNode_FieldDeref>(at,simV,offset + extraOffset);
        }
    }

    SimNode * ExprSwizzle::simulate (Context & context) const {
        if ( !type->ref ) {
            bool seq = TypeDecl::isSequencialMask(fields);
            if (seq && value->type->ref) {
                return trySimulate(context, 0, type->baseType);
            } else {
                auto fsz = fields.size();
                uint8_t fs[4];
                fs[0] = fields[0];
                fs[1] = fsz >= 2 ? fields[1] : fields[0];
                fs[2] = fsz >= 3 ? fields[2] : fields[0];
                fs[3] = fsz >= 4 ? fields[3] : fields[0];
                auto simV = value->simulate(context);
                return context.code->makeNode<SimNode_Swizzle>(at, simV, fs);
            }
        } else {
            return trySimulate(context, 0, r2v ? type->baseType : Type::none);
        }
    }

    SimNode * ExprField::simulate (Context & context) const {
        if ( !field ) {
            if ( r2v ) {
                auto resN = annotation->simulateGetFieldR2V(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateGetFieldR2V returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            } else {
                auto resN = annotation->simulateGetField(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateGetField returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            }
        } else {
            return trySimulate(context, 0, r2v ? type->baseType : Type::none);
        }
    }

    SimNode * ExprField::trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const {
        if ( !field ) {
            return nullptr;
        }
        if (value->type->isPointer()) {
            auto simV = value->simulate(context);
            if ( r2vType!=Type::none ) {
                return context.code->makeValueNode<SimNode_PtrFieldDerefR2V>(r2vType, at, simV, field->offset + extraOffset);
            } else {
                return context.code->makeNode<SimNode_PtrFieldDeref>(at, simV, field->offset + extraOffset);
            }
        } else {
            if ( auto chain = value->trySimulate(context, extraOffset + field->offset, r2vType) ) {
                return chain;
            }
            auto simV = value->simulate(context);
            if ( r2vType!=Type::none ) {
                return context.code->makeValueNode<SimNode_FieldDerefR2V>(r2vType, at, simV, extraOffset + field->offset);
            } else {
                return context.code->makeNode<SimNode_FieldDeref>(at, simV, extraOffset + field->offset);
            }
        }
    }

    SimNode * ExprSafeField::simulate (Context & context) const {
        if ( skipQQ ) {
            if ( annotation ) {
                auto resN = annotation->simulateSafeGetFieldPtr(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateSafeGetFieldPtr returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            } else {
                auto resN = context.code->makeNode<SimNode_SafeFieldDerefPtr>(at,value->simulate(context),field->offset);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateSafeFieldDerefPtr returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            }
        } else {
            if ( annotation ) {
                auto resN = annotation->simulateSafeGetField(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateSafeGetField returned null",
                                               at, CompilationError::missing_node );
                }
                return resN;
            } else {
                return context.code->makeNode<SimNode_SafeFieldDeref>(at,value->simulate(context),field->offset);
            }
        }
    }

    SimNode * ExprStringBuilder::simulate (Context & context) const {
        SimNode_StringBuilder * pSB = context.code->makeNode<SimNode_StringBuilder>(at);
        if ( int nArg = (int) elements.size() ) {
            pSB->arguments = (SimNode **) context.code->allocate(nArg * sizeof(SimNode *));
            pSB->types = (TypeInfo **) context.code->allocate(nArg * sizeof(TypeInfo *));
            pSB->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pSB->arguments[a] = elements[a]->simulate(context);
                pSB->types[a] = context.thisHelper->makeTypeInfo(nullptr, elements[a]->type);
            }
        } else {
            pSB->arguments = nullptr;
            pSB->types = nullptr;
            pSB->nArguments = 0;
        }
        return pSB;
    }

    SimNode * ExprVar::trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const {
        if ( block ) {
        } else if ( local ) {
            if ( variable->type->ref ) {
                if ( r2vType!=Type::none ) {
                    return context.code->makeValueNode<SimNode_GetLocalRefR2VOff>(r2vType, at,
                                                    variable->stackTop, extraOffset);
                } else {
                    return context.code->makeNode<SimNode_GetLocalRefOff>(at,
                                                    variable->stackTop, extraOffset);
                }
            } else if ( variable->aliasCMRES ) {
                if ( r2vType!=Type::none ) {
                    return context.code->makeValueNode<SimNode_GetCMResOfsR2V>(r2vType, at,extraOffset);
                } else {
                    return context.code->makeNode<SimNode_GetCMResOfs>(at, extraOffset);
                }
            } else {
                if ( r2vType!=Type::none ) {
                    return context.code->makeValueNode<SimNode_GetLocalR2V>(r2vType, at,
                                                                            variable->stackTop + extraOffset);
                } else {
                    return context.code->makeNode<SimNode_GetLocal>(at, variable->stackTop + extraOffset);
                }
            }
        } else if ( argument ) {
            if ( variable->type->isRef() ) {
                if ( r2vType!=Type::none ) {
                    return context.code->makeValueNode<SimNode_GetArgumentR2VOff>(r2vType, at, argumentIndex, extraOffset);
                } else {
                    return context.code->makeNode<SimNode_GetArgumentOff>(at, argumentIndex, extraOffset);
                }
            }
        } else { // global

        }
        return nullptr;
    }

    SimNode * ExprVar::simulate (Context & context) const {
        if ( block ) {
            auto blk = pBlock.lock();
            if (variable->type->isRef()) {
                if (r2v && !type->isRefType()) {
                    return context.code->makeValueNode<SimNode_GetBlockArgumentR2V>(type->baseType, at, argumentIndex, blk->stackTop);
                } else {
                    return context.code->makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
            } else {
                if (r2v && !type->isRefType()) {
                    return context.code->makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
                else {
                    return context.code->makeNode<SimNode_GetBlockArgumentRef>(at, argumentIndex, blk->stackTop);
                }
            }
        } else if ( local ) {
            if ( r2v ) {
                return trySimulate(context, 0, type->baseType);
            } else {
                return trySimulate(context, 0, Type::none);
            }
        } else if ( argument) {
            if (variable->type->isRef()) {
                if (r2v && !type->isRefType()) {
                    return context.code->makeValueNode<SimNode_GetArgumentR2V>(type->baseType, at, argumentIndex);
                } else {
                    return context.code->makeNode<SimNode_GetArgument>(at, argumentIndex);
                }
            } else {
                if (r2v && !type->isRefType()) {
                    return context.code->makeNode<SimNode_GetArgument>(at, argumentIndex);
                }
                else {
                    return context.code->makeNode<SimNode_GetArgumentRef>(at, argumentIndex);
                }
            }
        } else {
            assert(variable->index >= 0 && "using variable which is not used. how?");
            if ( r2v ) {
                return context.code->makeValueNode<SimNode_GetGlobalR2V>(type->baseType, at, variable->stackTop);
            } else {
                return context.code->makeNode<SimNode_GetGlobal>(at, variable->stackTop);
            }
        }
    }

    SimNode * ExprOp1::simulate (Context & context) const {
        if ( func->builtIn && !func->callBased ) {
            auto pSimOp1 = static_cast<SimNode_Op1 *>(func->makeSimNode(context));
            pSimOp1->x = subexpr->simulate(context);
            return pSimOp1;
        } else {
            auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
            pCall->debugInfo = at;
            pCall->fnPtr = context.getFunction(func->index);
            pCall->arguments = (SimNode **) context.code->allocate(1 * sizeof(SimNode *));
            pCall->nArguments = 1;
            pCall->arguments[0] = subexpr->simulate(context);
            pCall->cmresEval = context.code->makeNode<SimNode_GetLocal>(at,stackTop);
            return pCall;
        }
    }

    SimNode * ExprOp2::simulate (Context & context) const {
        if ( func->builtIn && !func->callBased ) {
            auto pSimOp2 = static_cast<SimNode_Op2 *>(func->makeSimNode(context));
            pSimOp2->l = left->simulate(context);
            pSimOp2->r = right->simulate(context);
            return pSimOp2;
        } else {
            auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
            pCall->debugInfo = at;
            pCall->fnPtr = context.getFunction(func->index);
            pCall->arguments = (SimNode **) context.code->allocate(2 * sizeof(SimNode *));
            pCall->nArguments = 2;
            pCall->arguments[0] = left->simulate(context);
            pCall->arguments[1] = right->simulate(context);
            pCall->cmresEval = context.code->makeNode<SimNode_GetLocal>(at,stackTop);
            return pCall;
        }
    }

    SimNode * ExprOp3::simulate (Context & context) const {
        return context.code->makeNode<SimNode_IfThenElse>(at,
                                                    subexpr->simulate(context),
                                                    left->simulate(context),
                                                    right->simulate(context));
    }

    SimNode * ExprMove::simulate (Context & context) const {
        auto retN = makeMove(at,context,left,right);
        if ( !retN ) {
            context.thisProgram->error("internal compilation error, can't generate move", at);
        }
        return retN;
    }

    SimNode * ExprCopy::simulate (Context & context) const {
        if ( takeOverRightStack ) {
            auto sl = left->simulate(context);
            auto sr = right->simulate(context);
            return context.code->makeNode<SimNode_SetLocalRefAndEval>(at, sl, sr, stackTop);
        } else {
            auto retN = makeCopy(at, context, left, right);
            if ( !retN ) {
                context.thisProgram->error("internal compilation error, can't generate copy", at);
            }
            return retN;
        }
    }

    SimNode * ExprTryCatch::simulate (Context & context) const {
        return context.code->makeNode<SimNode_TryCatch>(at,
                                                  try_block->simulate(context),
                                                  catch_block->simulate(context));
    }

    SimNode * ExprReturn::simulate (Context & context) const {
        SimNode * simSubE = subexpr ? subexpr->simulate(context) : nullptr;
        if (subexpr && subexpr->rtti_isConstant()) {
            if (subexpr->type->isSimpleType(Type::tString)) {
                auto cVal = static_pointer_cast<ExprConstString>(subexpr);
                char * str = context.code->allocateString(cVal->text);
                return context.code->makeNode<SimNode_ReturnConstString>(at, str);
            } else {
                auto cVal = static_pointer_cast<ExprConst>(subexpr);
                return context.code->makeNode<SimNode_ReturnConst>(at, cVal->value);
            }
        }
        if ( returnReference ) {
            if ( returnInBlock ) {
                return context.code->makeNode<SimNode_ReturnReferenceFromBlock>(at, simSubE);
            } else {
                return context.code->makeNode<SimNode_ReturnReference>(at, simSubE);
            }
        } else if ( returnInBlock ) {
            if ( returnCallCMRES ) {
                SimNode_CallBase * simRet = (SimNode_CallBase *) simSubE;
                simRet->cmresEval = context.code->makeNode<SimNode_GetBlockCMResOfs>(at,0,stackTop);
                return context.code->makeNode<SimNode_Return>(at, simSubE);
            } else if ( takeOverRightStack ) {
                return context.code->makeNode<SimNode_ReturnRefAndEvalFromBlock>(at,
                            simSubE, refStackTop, stackTop);
            } else if ( block->copyOnReturn  ) {
                return context.code->makeNode<SimNode_ReturnAndCopyFromBlock>(at,
                            simSubE, subexpr->type->getSizeOf(), stackTop);
            } else if ( block->moveOnReturn ) {
                return context.code->makeNode<SimNode_ReturnAndMoveFromBlock>(at,
                    simSubE, subexpr->type->getSizeOf(), stackTop);
            }
        } else if ( subexpr ) {
            if ( returnCallCMRES ) {
                SimNode_CallBase * simRet = (SimNode_CallBase *) simSubE;
                simRet->cmresEval = context.code->makeNode<SimNode_GetCMResOfs>(at,0);
                return context.code->makeNode<SimNode_Return>(at, simSubE);
            } else if ( returnCMRES ) {
                return context.code->makeNode<SimNode_Return>(at, simSubE);
            } else if ( takeOverRightStack ) {
                return context.code->makeNode<SimNode_ReturnRefAndEval>(at, simSubE, refStackTop);
            } else if ( func->copyOnReturn ) {
                return context.code->makeNode<SimNode_ReturnAndCopy>(at, simSubE, subexpr->type->getSizeOf());
            } else if ( func->moveOnReturn ) {
                return context.code->makeNode<SimNode_ReturnAndMove>(at, simSubE, subexpr->type->getSizeOf());
            }
        }
        return context.code->makeNode<SimNode_Return>(at, simSubE);
    }

    SimNode * ExprBreak::simulate (Context & context) const {
        return context.code->makeNode<SimNode_Break>(at);
    }

    SimNode * ExprIfThenElse::simulate (Context & context) const {
        ExpressionPtr zeroCond;
        bool condIfZero = false;
        if ( matchEquNequZero(cond, zeroCond, condIfZero) ) {
            if ( condIfZero ) {
                if ( if_false ) {
                    return context.code->makeNumericValueNode<SimNode_IfZeroThenElse>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context),
                                            if_false->simulate(context));

                } else {
                    return context.code->makeNumericValueNode<SimNode_IfZeroThen>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context));
                }
            } else {
                if ( if_false ) {
                    return context.code->makeNumericValueNode<SimNode_IfNotZeroThenElse>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context),
                                            if_false->simulate(context));

                } else {
                    return context.code->makeNumericValueNode<SimNode_IfNotZeroThen>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context));
                }
            }
        } else {
            // good old if
            if ( if_false ) {
                return context.code->makeNode<SimNode_IfThenElse>(at, cond->simulate(context),
                                    if_true->simulate(context), if_false->simulate(context));
            } else {
                return context.code->makeNode<SimNode_IfThen>(at, cond->simulate(context),
                                    if_true->simulate(context));
            }
        }
    }

    SimNode * ExprWith::simulate (Context & context) const {
        return body->simulate(context);
    }
    
    void ExprWhile::simulateFinal ( Context & context, const ExpressionPtr & bod, SimNode_Block * blk ) {
        if ( bod->rtti_isBlock() ) {
            auto pBlock = static_pointer_cast<ExprBlock>(bod);
            pBlock->simulateBlock(context, blk);
            pBlock->simulateFinal(context, blk);
        } else {
            context.thisProgram->error("internal error, expecting block", bod->at);
        }
    }

    SimNode * ExprWhile::simulate (Context & context) const {
        auto node = context.code->makeNode<SimNode_While>(at, cond->simulate(context));
        simulateFinal(context, body, node);
        return node;
    }

    SimNode * ExprFor::simulate (Context & context) const {
        // determine iteration types
        bool nativeIterators = false;
        bool fixedArrays = false;
        bool dynamicArrays = false;
        bool rangeBase = false;
        uint32_t fixedSize = UINT16_MAX;
        for ( auto & src : sources ) {
            if ( !src->type ) continue;
            if ( src->type->isArray() ) {
                fixedSize = das::min(fixedSize, src->type->dim.back());
                fixedArrays = true;
            } else if ( src->type->isGoodArrayType() ) {
                dynamicArrays = true;
            } else if ( src->type->isGoodIteratorType() ) {
                nativeIterators = true;
            } else if ( src->type->isHandle() ) {
                nativeIterators = true;
            } else if ( src->type->isRange() ) {
                rangeBase = true;
            }
        }
        // create loops based on
        int  total = int(sources.size());
        int  sourceTypes = int(dynamicArrays) + int(fixedArrays) + int(rangeBase);
        bool hybridRange = rangeBase && (total>1);
        if ( (sourceTypes>1) || hybridRange || nativeIterators ) {
            SimNode_ForWithIteratorBase * result = (SimNode_ForWithIteratorBase *)
                context.code->makeNodeUnroll<SimNode_ForWithIterator>(total, at);
            for ( int t=0; t!=total; ++t ) {
                if ( sources[t]->type->isGoodIteratorType() ) {
                    result->source_iterators[t] = sources[t]->simulate(context);
                } else if ( sources[t]->type->isGoodArrayType() ) {
                    result->source_iterators[t] = context.code->makeNode<SimNode_GoodArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->firstType->getStride());
                } else if ( sources[t]->type->isRange() ) {
                    result->source_iterators[t] = context.code->makeNode<SimNode_RangeIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context));
                } else if ( sources[t]->type->isHandle() ) {
                    result->source_iterators[t] = sources[t]->type->annotation->simulateGetIterator(
                         context,
                         sources[t]->at,
                         sources[t]->simulate(context)
                    );
                    if ( !result ) {
                        context.thisProgram->error("integration error, simulateGetIterator returned null",
                                                   at, CompilationError::missing_node );
                        return nullptr;
                    }
                } else if ( sources[t]->type->dim.size() ) {
                    result->source_iterators[t] = context.code->makeNode<SimNode_FixedArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->dim.back(),
                        sources[t]->type->getStride());
                } else {
                    DAS_ASSERTF(0, "we should not be here. we are doing iterator for on an unsupported type.");
                    context.thisProgram->error("internal compilation error, generating for-with-iterator", at);
                    return nullptr;
                }
                result->stackTop[t] = iteratorVariables[t]->stackTop;
            }
            ExprWhile::simulateFinal(context, subexpr, result);
            return result;
        } else {
            auto flagsE = subexpr->getEvalFlags();
            bool NF = flagsE == 0;
            SimNode_ForBase * result;
            assert(subexpr->rtti_isBlock() && "there would be internal error otherwise");
            auto subB = static_pointer_cast<ExprBlock>(subexpr);
            bool loop1 = (subB->list.size() == 1);
            if ( dynamicArrays ) {
                if (loop1) {
                    result = (SimNode_ForBase *) context.code->makeNodeUnroll<SimNode_ForGoodArray1>(total, at);
                } else {
                    result = (SimNode_ForBase *) context.code->makeNodeUnroll<SimNode_ForGoodArray>(total, at);
                }
            } else if ( fixedArrays ) {
                if (loop1) {
                    result = (SimNode_ForBase *)context.code->makeNodeUnroll<SimNode_ForFixedArray1>(total, at);
                } else {
                    result = (SimNode_ForBase *)context.code->makeNodeUnroll<SimNode_ForFixedArray>(total, at);
                }
            } else if ( rangeBase ) {
                assert(total==1 && "simple range on 1 loop only");
                if ( NF ) {
                    if (loop1) {
                        result = context.code->makeNode<SimNode_ForRangeNF1>(at);
                    } else {
                        result = context.code->makeNode<SimNode_ForRangeNF>(at);
                    }
                } else {
                    if (loop1) {
                        result = context.code->makeNode<SimNode_ForRange1>(at);
                    } else {
                        result = context.code->makeNode<SimNode_ForRange>(at);
                    }
                }
            } else {
                DAS_ASSERTF(0, "we should not be here yet. logic above assumes optimized for path of some kind.");
                context.thisProgram->error("internal compilation error, generating for", at);
                return nullptr;
            }
            for ( int t=0; t!=total; ++t ) {
                result->sources[t] = sources[t]->simulate(context);
                if ( sources[t]->type->isGoodArrayType() ) {
                    result->strides[t] = sources[t]->type->firstType->getStride();
                } else {
                    result->strides[t] = sources[t]->type->getStride();
                }
                result->stackTop[t] = iteratorVariables[t]->stackTop;
            }
            result->size = fixedSize;
            ExprWhile::simulateFinal(context, subexpr, result);
            return result;
        }
    }

    vector<SimNode *> ExprLet::simulateInit(Context & context, const ExprLet * pLet) {
        vector<SimNode *> simlist;
        simlist.reserve(pLet->variables.size());
        for (auto & var : pLet->variables) {
            SimNode * init;
            if (var->init) {
                init = ExprLet::simulateInit(context, var, true);
            } else if (var->aliasCMRES ) {
                init = context.code->makeNode<SimNode_InitLocalCMRes>(pLet->at, 0, var->type->getSizeOf());
            } else {
                init = context.code->makeNode<SimNode_InitLocal>(pLet->at, var->stackTop, var->type->getSizeOf());
            }
            if (init)
                simlist.push_back(init);
        }
        return simlist;
    }

    SimNode * ExprLet::simulateInit(Context & context, const VariablePtr & var, bool local) {
        SimNode * get;
        if ( local ) {
            if ( var->init && var->init->rtti_isMakeLocal() ) {
                return var->init->simulate(context);
            } else {
                get = context.code->makeNode<SimNode_GetLocal>(var->init->at, var->stackTop);
            }
        } else {
            get = context.code->makeNode<SimNode_GetGlobal>(var->init->at, var->index);
        }
        if ( var->type->ref ) {
            return context.code->makeNode<SimNode_CopyReference>(var->init->at, get,
                                                                 var->init->simulate(context));
        } else if ( var->type->canCopy() ) {
            auto varExpr = make_shared<ExprVar>(var->at, var->name);
            varExpr->variable = var;
            varExpr->local = local;
            varExpr->type = make_shared<TypeDecl>(*var->type);
            auto retN = makeCopy(var->init->at, context, varExpr, var->init);
            if ( !retN ) {
                context.thisProgram->error("internal compilation error, can't generate copy", var->at);
            }
            return retN;
        } else if ( var->type->canMove() ) {
            auto varExpr = make_shared<ExprVar>(var->at, var->name);
            varExpr->variable = var;
            varExpr->local = local;
            varExpr->type = make_shared<TypeDecl>(*var->type);
            auto retN = makeMove(var->init->at, context, varExpr, var->init);
            if ( !retN ) {
                context.thisProgram->error("internal compilation error, can't generate move", var->at);
            }
            return retN;
        } else {
            context.thisProgram->error("internal compilation error, initializing variable which can't be copied or moved", var->at);
            return nullptr;
        }
    }

    SimNode * ExprLet::simulate (Context & context) const {
        auto let = context.code->makeNode<SimNode_Let>(at);
        let->total = (uint32_t) variables.size();
        let->list = (SimNode **) context.code->allocate(let->total * sizeof(SimNode*));
        auto simList = ExprLet::simulateInit(context, this);
        copy(simList.data(), simList.data() + simList.size(), let->list);
        return let;
    }

    SimNode_CallBase * ExprCall::simulateCall (const FunctionPtr & func,
                                               const ExprLooksLikeCall * expr,
                                               Context & context,
                                               SimNode_CallBase * pCall) {
        bool needTypeInfo = false;
        for ( auto & arg : func->arguments ) {
            if ( arg->type->baseType==Type::anyArgument )
                needTypeInfo = true;
        }
        pCall->debugInfo = expr->at;
        DAS_ASSERTF((func->builtIn || func->index>=0), "calling function which is not used. how?");
        pCall->fnPtr = context.getFunction(func->index);
        if ( int nArg = (int) expr->arguments.size() ) {
            pCall->arguments = (SimNode **) context.code->allocate(nArg * sizeof(SimNode *));
            if ( needTypeInfo ) {
                pCall->types = (TypeInfo **) context.code->allocate(nArg * sizeof(TypeInfo *));
            } else {
                pCall->types = nullptr;
            }
            pCall->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pCall->arguments[a] = expr->arguments[a]->simulate(context);
                if ( pCall->types ) {
                    if ( func->arguments[a]->type->baseType==Type::anyArgument ) {
                        pCall->types[a] = context.thisHelper->makeTypeInfo(nullptr, expr->arguments[a]->type);
                    } else {
                        pCall->types[a] = nullptr;
                    }
                }
            }
        } else {
            pCall->arguments = nullptr;
            pCall->nArguments = 0;
        }
        return pCall;

    }

    SimNode * ExprCall::simulate (Context & context) const {
        auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
        simulateCall(func->shared_from_this(), this, context, pCall);
        if ( !doesNotNeedSp ) {
            pCall->cmresEval = context.code->makeNode<SimNode_GetLocal>(at,stackTop);
        }
        return pCall;
    }

    bool Program::simulate ( Context & context, TextWriter & logs ) {
        context.thisProgram = this;
        if ( auto optHeap = options.find("heap",Type::tInt) ) {
            context.heap.setInitialSize( uint32_t(optHeap->iValue) );
        }
        DebugInfoHelper helper(context.debugInfo);
        context.thisHelper = &helper;
        context.globalVariables = (GlobalVariable *) context.code->allocate( totalVariables*sizeof(GlobalVariable) );
        context.globalsSize = 0;
        for (auto & pm : library.modules ) {
            for (auto & it : pm->globals) {
                auto pvar = it.second;
                if (!pvar->used)
                    continue;
                DAS_ASSERTF(pvar->index >= 0, "we are simulating variable, which is not used");
                auto & gvar = context.globalVariables[pvar->index];
                gvar.name = context.code->allocateName(pvar->name);
                gvar.size = pvar->type->getSizeOf();
                gvar.debugInfo = helper.makeVariableDebugInfo(*it.second);
                gvar.offset = pvar->stackTop = context.globalsSize;
                context.globalsSize = (context.globalsSize + gvar.size + 0xf) & ~0xf;
            }
        }
        context.globals = (char *) das_aligned_alloc16(context.globalsSize);
        context.totalVariables = totalVariables;
        context.functions = (SimFunction *) context.code->allocate( totalFunctions*sizeof(SimFunction) );
        context.totalFunctions = totalFunctions;
        for (auto & pm : library.modules) {
            for (auto & it : pm->functions) {
                auto pfun = it.second;
                if (pfun->index < 0 || !pfun->used)
                    continue;
                auto & gfun = context.functions[pfun->index];
                gfun.name = context.code->allocateName(pfun->name);
                gfun.code = pfun->simulate(context);
                gfun.stackSize = pfun->totalStackSize;
                gfun.debugInfo = helper.makeFunctionDebugInfo(*pfun);
            }
        }
        for (auto & pm : library.modules ) {
            for (auto & it : pm->globals) {
                auto pvar = it.second;
                if (!pvar->used)
                    continue;
                auto & gvar = context.globalVariables[pvar->index];
                gvar.init = pvar->init ? ExprLet::simulateInit(context, pvar, false) : nullptr;
            }
        }
        if ( annotationData.size() ) {
            uint32_t ads = (uint32_t) (annotationData.size() * sizeof(uint64_t));
            uint64_t * pads = context.annotationData = (uint64_t*) context.code->allocate(ads);
            for ( auto ann : annotationData ) {
                *pads++ = ann;
            }
        }
        context.globalInitStackSize = globalInitStackSize;
        context.simEnd();
        context.restart();
        context.runInitScript();
        context.restart();
        if (options.getOption("logMem")) {
            logs << "code  " << context.code->bytesAllocated() << "\n";
            logs << "debug " << context.debugInfo->bytesAllocated() << "\n";
            logs << "heap  " << context.heap.bytesAllocated() << "\n";
        }
        // log all functions
        if ( options.getOption("logNodes",false) ) {
            for ( int i=0; i!=context.totalFunctions; ++i ) {
                SimFunction * fn = context.getFunction(i);
                logs << "// " << fn->name << "\n";
                printSimNode(logs, fn->code);
                logs << "\n\n";
            }
        }
        if (options.getOption("logCpp")) {
            aotCpp(context,logs);
            registerAotCpp(logs,context);
        }
        return errors.size() == 0;
    }

    void Program::linkCppAot ( Context & context, AotLibrary & aotLib, TextWriter & logs ) {
        bool logIt = options.getOption("logAot",false);

        // make list of functions
        vector<Function *> fnn; fnn.reserve(totalFunctions);
        for (auto & pm : library.modules) {
            for (auto & it : pm->functions) {
                auto pfun = it.second;
                if (pfun->index < 0 || !pfun->used)
                    continue;
                fnn.push_back(pfun.get());
            }
        }

        for ( int fni=0; fni!=context.totalFunctions; ++fni ) {
            if ( !fnn[fni]->noAot ) {
                SimFunction & fn = context.functions[fni];
                uint64_t semHash = getFunctionHash(fnn[fni], fn.code);
                auto it = aotLib.find(semHash);
                if ( it != aotLib.end() ) {
                    fn.code = (it->second)(context);
                    if ( logIt ) logs << fn.name << " AOT=0x" << HEX << semHash << DEC << "\n";
                } else {
                    if ( logIt ) logs << "NOT FOUND " << fn.name << " AOT=0x" << HEX << semHash << DEC << "\n";
                }
            }
        }
        if ( context.totalVariables ) {
            uint64_t semHash = context.getInitSemanticHash();
            auto it = aotLib.find(semHash);
            if ( it != aotLib.end() ) {
                context.aotInitScript = (it->second)(context);
                if ( logIt ) logs << "INIT SCRIPT AOT=0x" << HEX << semHash << DEC << "\n";
            } else {
                if ( logIt ) logs << "INIT SCRIPT NOT FOUND, AOT=0x" << HEX << semHash << DEC << "\n";
            }
        }
    }
}

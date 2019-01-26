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
    
    SimNode * makeLocalCopy(const LineInfo & at, Context & context, uint32_t stackTop, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() && "should check above" );
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
                if ( rvar->local && !rvar->variable->type->ref ) {
                    return context.code.makeValueNode<SimNode_CopyLocal2LocalT>(rightType.baseType,
                                                                                at, stackTop,
                                                                                rvar->variable->stackTop);
                }
            }
            auto right = rE->simulate(context);
            if ( rightType.ref ) {
                return context.code.makeValueNode<SimNode_SetLocalRefT>(rightType.baseType,
                                                                        at, right, stackTop);
            } else {
                return context.code.makeValueNode<SimNode_SetLocalValueT>(rightType.baseType,
                                                                          at, right, stackTop);
            }
        }
        // now, to the regular copy
        auto left = context.code.makeNode<SimNode_GetLocal>(rE->at, stackTop);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code.makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code.makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            return rightType.annotation->simulateCopy(context, at, left, right);
        } else {
            return context.code.makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }
    
    SimNode * makeCopy(const LineInfo & at, Context & context, const ExpressionPtr & lE, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() && "should check above" );
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
                if ( rE->rtti_isVar() ) {
                    auto rvar = static_pointer_cast<ExprVar>(rE);
                    if ( rvar->local && !rvar->variable->type->ref ) {
                        return context.code.makeValueNode<SimNode_CopyLocal2LocalT>(rightType.baseType,
                                at, var->variable->stackTop, rvar->variable->stackTop);
                    }
                }
                auto right = rE->simulate(context);
                if ( rightType.ref ) {
                    return context.code.makeValueNode<SimNode_SetLocalRefT>(rightType.baseType,
                                at, right, var->variable->stackTop);
                } else {
                    return context.code.makeValueNode<SimNode_SetLocalValueT>(rightType.baseType,
                                at, right, var->variable->stackTop);
                }
            }
        }
        // now, to the regular copy
        auto left = lE->simulate(context);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.code.makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.code.makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            return rightType.annotation->simulateCopy(context, at, left, right);
        } else {
            return context.code.makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }
    
    SimNode * makeMove (const LineInfo & at, Context & context, const TypeDecl & rightType, SimNode * left, SimNode * right ) {
        if ( rightType.isRef() ) {
            return context.code.makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else {
            assert(0 && "we should not be here");
            return nullptr;
        }
    }
    
    SimNode * Function::simulate (Context & context) const {
        if ( builtIn ) {
            assert(0 && "can only simulate non built-in function");
            return nullptr;
        }
        if ( fastCall ) {
			assert(totalStackSize == sizeof(Prologue) && "function can't allocate stack");
            assert(result->isWorkhorseType() && "fastcall can only return a workhoree type");
            assert(body->rtti_isBlock() && "function must contain a block");
            auto block = static_pointer_cast<ExprBlock>(body);
            assert(block->list.size()==1 && "fastcall is only one expr in a function body");
            assert(block->list.back()->rtti_isReturn() && "fastcall body expr is return");
            auto retE = static_pointer_cast<ExprReturn>(block->list.back());
            assert(retE->subexpr && "fastcall must return a value");
            return retE->subexpr->simulate(context);
        } else {
            return body->simulate(context);
        }
    }
    
    SimNode * ExprMakeStructure::simulate (Context & context) const {
        vector<SimNode *> simlist;
        // init with 0
        int total = int(structs.size());
        int stride = makeType->getStride();
        auto init0 = context.code.makeNode<SimNode_InitLocal>(at,stackTop,stride * total);
        simlist.push_back(init0);
        for ( int index=0; index != total; ++index ) {
            auto & fields = structs[index];
            for ( const auto & decl : *fields ) {
                auto field = makeType->structType->findField(decl->name);
                assert(field && "should have failed in type infer otherwise");
                uint32_t fieldStackTop = stackTop + index*stride + field->offset;
                auto cpy = makeLocalCopy(at,context,fieldStackTop,decl->value);
                simlist.push_back(cpy);
            }
        }
        // we make a block with all those things in it
        auto block = context.code.makeNode<SimNode_MakeLocal>(at, stackTop);
        block->total = int(simlist.size());
        block->list = (SimNode **) context.code.allocate(sizeof(SimNode *)*block->total);
        for ( uint32_t i = 0; i != block->total; ++i )
            block->list[i] = simlist[i];
        return block;
    }
    
    SimNode * ExprMakeArray::simulate (Context & context) const {
        vector<SimNode *> simlist;
        // init with 0
        int total = int(values.size());
        uint32_t stride = recordType->getSizeOf();
        auto init0 = context.code.makeNode<SimNode_InitLocal>(at,stackTop,stride * total);
        simlist.push_back(init0);
        for ( int index=0; index != total; ++index ) {
            auto & val = values[index];
            uint32_t indexStackTop = stackTop + index*stride;
            auto cpy = makeLocalCopy(at,context,indexStackTop,val);
            simlist.push_back(cpy);
        }
        // we make a block with all those things in it
        auto block = context.code.makeNode<SimNode_MakeLocal>(at, stackTop);
        block->total = int(simlist.size());
        block->list = (SimNode **) context.code.allocate(sizeof(SimNode *)*block->total);
        for ( uint32_t i = 0; i != block->total; ++i )
            block->list[i] = simlist[i];
        return block;
    }
    
    SimNode * ExprRef2Value::simulate (Context & context) const {
        return GetR2V(context, at, type, subexpr->simulate(context));
    }
    
    SimNode * ExprPtr2Ref::simulate (Context & context) const {
        return context.code.makeNode<SimNode_Ptr2Ref>(at,subexpr->simulate(context));
    }
    
    SimNode * ExprNullCoalescing::simulate (Context & context) const {
        if ( type->ref ) {
            return context.code.makeNode<SimNode_NullCoalescingRef>(at,subexpr->simulate(context),defaultValue->simulate(context));
        } else {
            return context.code.makeValueNode<SimNode_NullCoalescing>(type->baseType,at,subexpr->simulate(context),defaultValue->simulate(context));
        }
    }
    
    SimNode * ExprConst::simulate (Context & context) const {
        return context.code.makeNode<SimNode_ConstValue>(at,value);
    }
    
    SimNode * ExprConstString::simulate (Context & context) const {
        char * str = context.code.allocateString(text);
        return context.code.makeNode<SimNode_ConstValue>(at,cast<char *>::from(str));
    }
    
    SimNode * ExprStaticAssert::simulate (Context &) const {
        return nullptr;
    }

    SimNode * ExprAssert::simulate (Context & context) const {
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.code.makeNode<SimNode_Assert>(at,arguments[0]->simulate(context),context.code.allocateString(message));
    }
    
    SimNode * ExprDebug::simulate (Context & context) const {
        TypeInfo * pTypeInfo = context.thisHelper->makeTypeInfo(nullptr, arguments[0]->type);
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.code.makeNode<SimNode_Debug>(at,
                                               arguments[0]->simulate(context),
                                               pTypeInfo,
                                               context.code.allocateString(message));
    }
    
    SimNode * ExprMakeBlock::simulate (Context & context) const {
        uint32_t argSp = static_pointer_cast<ExprBlock>(block)->stackTop;
        return context.code.makeNode<SimNode_MakeBlock>(at,block->simulate(context),argSp);
    }
    
    SimNode * ExprInvoke::simulate (Context & context) const {
        auto blockT = arguments[0]->type;
        SimNode_CallBase * pInvoke;
        if ( blockT->firstType && blockT->firstType->isRefType() ) {
            pInvoke = (SimNode_CallBase *) context.code.makeNodeUnroll<SimNode_InvokeAndCopyOrMove>(
                                                int(arguments.size()), at, stackTop);
        } else {
            pInvoke = (SimNode_CallBase *) context.code.makeNodeUnroll<SimNode_Invoke>(int(arguments.size()),at);
        }
        pInvoke->debug = at;
        if ( int nArg = (int) arguments.size() ) {
            pInvoke->arguments = (SimNode **) context.code.allocate(nArg * sizeof(SimNode *));
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
    
    SimNode * ExprHash::simulate (Context & context) const {
        auto val = arguments[0]->simulate(context);
        if ( !arguments[0]->type->isRef() ) {
            return context.code.makeValueNode<SimNode_HashOfValue>(arguments[0]->type->baseType, at, val);
        } else if ( arguments[0]->type->isPod() ) {
            return context.code.makeNode<SimNode_HashOfRef>(at, val, arguments[0]->type->getSizeOf());
        } else {
            auto typeInfo = context.thisHelper->makeTypeInfo(nullptr, arguments[0]->type);
            return context.code.makeNode<SimNode_HashOfMixedType>(at, val, typeInfo);
        }
    }
    
    SimNode * ExprErase::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.code.makeValueNode<SimNode_TableErase>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            assert(0 && "we should not even be here");
            return nullptr;
        }
    }
    
    SimNode * ExprFind::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.code.makeValueNode<SimNode_TableFind>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            assert(0 && "we should not even be here");
            return nullptr;
        }
    }
    
    SimNode * ExprSizeOf::simulate (Context & context) const {
        uint32_t size = typeexpr->getSizeOf();
        return context.code.makeNode<SimNode_ConstValue>(at,cast<uint32_t>::from(size));
    }
    
    SimNode * ExprTypeName::simulate (Context & context) const {
        auto pName = context.code.allocateString(typeexpr->describe(false));
        return context.code.makeNode<SimNode_ConstValue>(at,cast<char *>::from(pName));
    }
    
    SimNode * ExprNew::simulate (Context & context) const {
        if ( typeexpr->isHandle() ) {
            return typeexpr->annotation->simulateGetNew(context, at);
        } else {
            int32_t bytes = typeexpr->getSizeOf();
            return context.code.makeNode<SimNode_New>(at,bytes);
        }
    }

    SimNode * ExprAt::simulate (Context & context) const {
        auto prv = subexpr->simulate(context);
        auto pidx = index->simulate(context);
        SimNode * result = nullptr;
        if ( subexpr->type->isVectorType() ) {
            uint32_t range = subexpr->type->getVectorDim();
            uint32_t stride = type->getSizeOf();
            if ( subexpr->type->ref ) {
                result = context.code.makeNode<SimNode_At>(at, prv, pidx, stride, range);
            } else {
                switch ( type->baseType ) {
                    case tInt:      return context.code.makeNode<SimNode_AtVector<int32_t>>(at, prv, pidx, range);
                    case tUInt:     return context.code.makeNode<SimNode_AtVector<uint32_t>>(at, prv, pidx, range);
                    case tFloat:    return context.code.makeNode<SimNode_AtVector<float>>(at, prv, pidx, range);
                    default:
                        assert(0 && "we should not even be here");
                }
            }
        } else if ( subexpr->type->isGoodTableType() ) {
            uint32_t valueTypeSize = subexpr->type->secondType->getSizeOf();
            result = context.code.makeValueNode<SimNode_TableIndex>(subexpr->type->firstType->baseType, at, prv, pidx, valueTypeSize);
        } else if ( subexpr->type->isGoodArrayType() ) {
            uint32_t stride = subexpr->type->firstType->getSizeOf();
            result = context.code.makeNode<SimNode_ArrayAt>(at, prv, pidx, stride);
        } else if ( subexpr->type->isHandle() ) {
            result = subexpr->type->annotation->simulateGetAt(context, at, index->type, prv, pidx);
        } else {
            uint32_t stride = subexpr->type->getStride();
            uint32_t range = subexpr->type->dim.back();
            result = context.code.makeNode<SimNode_At>(at, prv, pidx, stride, range);
        }
        if ( r2v ) {
            return ExprRef2Value::GetR2V(context, at, type, result);
        } else {
            return result;
        }
    }
    
    SimNode * ExprBlock::simulate (Context & context) const {
        vector<SimNode *> simlist;
        for ( auto & node : list ) {
			if ( node->rtti_isLet()) {
				shared_ptr<ExprLet> pLet = static_pointer_cast<ExprLet>(node);
				if (!pLet->subexpr) {
					auto letInit = ExprLet::simulateInit(context, pLet.get());
					simlist.insert(simlist.end(), letInit.begin(), letInit.end());
					continue;
				}
			}
            if ( auto simE = node->simulate(context) ) {
                simlist.push_back(simE);
            }
        }
        // TODO: what if list size is 0?
        if ( simlist.size()!=1 || isClosure ) {
            auto block = isClosure ? context.code.makeNode<SimNode_ClosureBlock>(at, type!=nullptr && type->baseType!=Type::tVoid, annotationData)
                : context.code.makeNode<SimNode_Block>(at);
            block->total = int(simlist.size());
            block->list = (SimNode **) context.code.allocate(sizeof(SimNode *)*block->total);
            for ( uint32_t i = 0; i != block->total; ++i )
                block->list[i] = simlist[i];
            return block;
        } else {
            return simlist[0];
        }
    }
    
    SimNode * ExprSwizzle::simulate (Context & context) const {
        uint32_t offset = fields[0] * sizeof(float);
        auto simV = value->simulate(context);
        if ( type->ref ) {
            if ( r2v ) {
                return context.code.makeValueNode<SimNode_FieldDerefR2V>(type->baseType,at,simV,offset);
            } else {
                return context.code.makeNode<SimNode_FieldDeref>(at,simV,offset);
            }
        } else {
            assert(!r2v && "how did it ever become value");
            if ( fields.size()==1 ) {
                return context.code.makeValueNode<SimNode_FieldDerefR2V>(type->baseType,at,simV,offset);
            } else {
                auto fsz = fields.size();
                uint8_t fs[4];
                fs[0] = fields[0];
                fs[1] = fields[1];
                fs[2] = fsz>=3 ? fields[2] : fields[0];
                fs[3] = fsz>=4 ? fields[3] : fields[0];
                return context.code.makeNode<SimNode_Swizzle>(at,simV,fs);
            }
        }
    }
    
    SimNode * ExprField::simulate (Context & context) const {
        auto simV = value->simulate(context);
        if ( !field ) {
            if ( r2v ) {
                return annotation->simulateGetFieldR2V(name, context, at, simV);
            } else {
                return annotation->simulateGetField(name, context, at, simV);
            }
        } else {
			if (type->isPointer()) {
				if (r2v)
					return context.code.makeValueNode<SimNode_PtrFieldDerefR2V>(type->baseType, at, simV, field->offset);
				else
					return context.code.makeNode<SimNode_PtrFieldDeref>(at, simV, field->offset);
			} else {
				if (r2v)
					return context.code.makeValueNode<SimNode_FieldDerefR2V>(type->baseType, at, simV, field->offset);
				else
					return context.code.makeNode<SimNode_FieldDeref>(at, simV, field->offset);
			}
        }
    }
    
    SimNode * ExprSafeField::simulate (Context & context) const {
        if ( skipQQ ) {
            if ( annotation ) {
                return annotation->simulateSafeGetFieldPtr(name, context, at, value->simulate(context));
            } else {
                return context.code.makeNode<SimNode_SafeFieldDerefPtr>(at,value->simulate(context),field->offset);
            }
        } else {
            if ( annotation ) {
                return annotation->simulateSafeGetField(name, context, at, value->simulate(context));
            } else {
                return context.code.makeNode<SimNode_SafeFieldDeref>(at,value->simulate(context),field->offset);
            }
        }
    }
    
    SimNode * ExprStringBuilder::simulate (Context & context) const {
        SimNode_StringBuilder * pSB = context.code.makeNode<SimNode_StringBuilder>(at);
        if ( int nArg = (int) elements.size() ) {
            pSB->arguments = (SimNode **) context.code.allocate(nArg * sizeof(SimNode *));
            pSB->types = (TypeInfo **) context.code.allocate(nArg * sizeof(TypeInfo *));
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
    
    SimNode * ExprVar::simulate (Context & context) const {
        if ( block ) {
            auto blk = pBlock.lock();
            if (variable->type->isRef()) {
                if (r2v && !type->isRefType()) {
                    return context.code.makeValueNode<SimNode_GetBlockArgumentR2V>(type->baseType, at, argumentIndex, blk->stackTop);
                } else {
                    return context.code.makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
            } else {
                if (r2v && !type->isRefType()) {
                    return context.code.makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
                else {
                    return context.code.makeNode<SimNode_GetBlockArgumentRef>(at, argumentIndex, blk->stackTop);
                }
            }
        } else if ( local ) {
            if ( variable->type->ref ) {
                if ( r2v && !type->isRefType() ) {
                    return context.code.makeValueNode<SimNode_GetLocalRefR2V>(type->baseType, at, variable->stackTop);
                } else {
                    return context.code.makeNode<SimNode_GetLocalRef>(at, variable->stackTop);
                }
            } else {
                if ( r2v && !type->isRefType()) {
                    return context.code.makeValueNode<SimNode_GetLocalR2V>(type->baseType, at, variable->stackTop);
                } else {
                    return context.code.makeNode<SimNode_GetLocal>(at, variable->stackTop);
                }
            }
        } else if ( argument) {
			if (variable->type->isRef()) {
				if (r2v && !type->isRefType()) {
					return context.code.makeValueNode<SimNode_GetArgumentR2V>(type->baseType, at, argumentIndex);
				} else {
					return context.code.makeNode<SimNode_GetArgument>(at, argumentIndex);
				}
			} else {
				if (r2v && !type->isRefType()) {
					return context.code.makeNode<SimNode_GetArgument>(at, argumentIndex);
				}
				else {
					return context.code.makeNode<SimNode_GetArgumentRef>(at, argumentIndex);
				}
            }
        } else {
			assert(variable->index >= 0 && "using variable which is not used. how?");
            if ( r2v ) {
                return context.code.makeValueNode<SimNode_GetGlobalR2V>(type->baseType, at, variable->index);
            } else {
                return context.code.makeNode<SimNode_GetGlobal>(at, variable->index);
            }
        }
    }
    
    SimNode * ExprOp1::simulate (Context & context) const {
        if ( func->builtIn ) {
            auto pSimOp1 = static_cast<SimNode_Op1 *>(func->makeSimNode(context));
            pSimOp1->x = subexpr->simulate(context);
            return pSimOp1;
        } else {
            auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
            pCall->debug = at;
            pCall->fnPtr = context.getFunction(func->index);
            pCall->arguments = (SimNode **) context.code.allocate(1 * sizeof(SimNode *));
            pCall->nArguments = 1;
            pCall->arguments[0] = subexpr->simulate(context);
            return pCall;
        }
    }
    
    SimNode * ExprOp2::simulate (Context & context) const {
        if ( func->builtIn ) {
            auto pSimOp2 = static_cast<SimNode_Op2 *>(func->makeSimNode(context));
            pSimOp2->l = left->simulate(context);
            pSimOp2->r = right->simulate(context);
            return pSimOp2;
        } else {
            auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
            pCall->debug = at;
            pCall->fnPtr = context.getFunction(func->index);
            pCall->arguments = (SimNode **) context.code.allocate(2 * sizeof(SimNode *));
            pCall->nArguments = 2;
            pCall->arguments[0] = left->simulate(context);
            pCall->arguments[1] = right->simulate(context);
            return pCall;
        }
    }
    
    SimNode * ExprOp3::simulate (Context & context) const {
        return context.code.makeNode<SimNode_IfThenElse>(at,
                                                    subexpr->simulate(context),
                                                    left->simulate(context),
                                                    right->simulate(context));
    }

    SimNode * ExprMove::simulate (Context & context) const {
        return makeMove(at,
                        context,
                        *right->type,
                        left->simulate(context),
                        right->simulate(context));
    }
    
    SimNode * ExprCopy::simulate (Context & context) const {
        return makeCopy(at, context, left, right);
    }

    SimNode * ExprTryCatch::simulate (Context & context) const {
        return context.code.makeNode<SimNode_TryCatch>(at,
                                                  try_block->simulate(context),
                                                  catch_block->simulate(context));
    }

    SimNode * ExprReturn::simulate (Context & context) const {
        SimNode * simSubE = subexpr ? subexpr->simulate(context) : nullptr;
		if (subexpr && subexpr->rtti_isConstant()) {
			if (subexpr->type->isSimpleType(Type::tString)) {
				auto cVal = static_pointer_cast<ExprConstString>(subexpr);
				char * str = context.code.allocateString(cVal->text);
				return context.code.makeNode<SimNode_ReturnConst>(at, cast<char *>::from(str));
			} else {
				auto cVal = static_pointer_cast<ExprConst>(subexpr);
				return context.code.makeNode<SimNode_ReturnConst>(at, cVal->value);
			}
		}
        if ( returnReference ) {
            if ( returnInBlock ) {
                return context.code.makeNode<SimNode_ReturnReferenceFromBlock>(at, simSubE);
            } else {
                return context.code.makeNode<SimNode_ReturnReference>(at, simSubE);
            }
        } else if ( copyOnReturn ) {
            if ( returnInBlock ) {
                return context.code.makeNode<SimNode_ReturnAndCopyFromBlock>(at,
                            simSubE, subexpr->type->getSizeOf(), stackTop);
            } else {
                return context.code.makeNode<SimNode_ReturnAndCopy>(at, simSubE, subexpr->type->getSizeOf());
            }
        } else if ( moveOnReturn ) {
            if ( returnInBlock ) {
                return context.code.makeNode<SimNode_ReturnAndMoveFromBlock>(at,
                            simSubE, subexpr->type->getSizeOf(), stackTop);
            } else {
                return context.code.makeNode<SimNode_ReturnAndMove>(at, simSubE, subexpr->type->getSizeOf());
            }
        } else {
            return context.code.makeNode<SimNode_Return>(at, simSubE);
        }
    }
    
    SimNode * ExprBreak::simulate (Context & context) const {
        return context.code.makeNode<SimNode_Break>(at);
    }
    
    SimNode * ExprIfThenElse::simulate (Context & context) const {
        ExpressionPtr zeroCond;
        bool condIfZero = false;
        if ( matchEquNequZero(cond, zeroCond, condIfZero) ) {
            if ( condIfZero ) {
                if ( if_false ) {
                    return context.code.makeNumericValueNode<SimNode_IfZeroThenElse>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context),
                                            if_false->simulate(context));
                
                } else {
                    return context.code.makeNumericValueNode<SimNode_IfZeroThen>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context));
                }
            } else {
                if ( if_false ) {
                    return context.code.makeNumericValueNode<SimNode_IfNotZeroThenElse>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context),
                                            if_false->simulate(context));
                
                } else {
                    return context.code.makeNumericValueNode<SimNode_IfNotZeroThen>(zeroCond->type->baseType,
                                    at, zeroCond->simulate(context), if_true->simulate(context));
                }
            }
        } else {
            // good old if
            if ( if_false ) {
                return context.code.makeNode<SimNode_IfThenElse>(at, cond->simulate(context),
                                    if_true->simulate(context), if_false->simulate(context));
            } else {
                return context.code.makeNode<SimNode_IfThen>(at, cond->simulate(context),
                                    if_true->simulate(context));
            }
        }
    }

    SimNode * ExprWhile::simulate (Context & context) const {
        return context.code.makeNode<SimNode_While>(at, cond->simulate(context),body->simulate(context));
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
                fixedSize = min(fixedSize, src->type->dim.back());
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
                context.code.makeNodeUnroll<SimNode_ForWithIterator>(total, at);
            for ( int t=0; t!=total; ++t ) {
                if ( sources[t]->type->isGoodIteratorType() ) {
                    result->source_iterators[t] = sources[t]->simulate(context);
                } else if ( sources[t]->type->isGoodArrayType() ) {
                    result->source_iterators[t] = context.code.makeNode<SimNode_GoodArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->firstType->getStride());
                } else if ( sources[t]->type->isRange() ) {
                    result->source_iterators[t] = context.code.makeNode<SimNode_RangeIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context));
                } else if ( sources[t]->type->isHandle() ) {
                    result->source_iterators[t] = sources[t]->type->annotation->simulateGetIterator(
                         context,
                         sources[t]->at,
                         sources[t]->simulate(context)
                    );
                } else if ( sources[t]->type->dim.size() ) {
                    result->source_iterators[t] = context.code.makeNode<SimNode_FixedArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->dim.back(),
                        sources[t]->type->getStride());
                } else {
                    assert(0 && "we should not be here yet");
                    return nullptr;
                }
                result->stackTop[t] = iteratorVariables[t]->stackTop;
            }
            result->body = subexpr->simulate(context);
            return result;
        } else {
            SimNode_ForBase * result;
            if ( dynamicArrays ) {
                result = (SimNode_ForBase *) context.code.makeNodeUnroll<SimNode_ForGoodArray>(total, at);
            } else if ( fixedArrays ) {
                result = (SimNode_ForBase *) context.code.makeNodeUnroll<SimNode_ForFixedArray>(total, at);
            } else if ( rangeBase ) {
                assert(total==1 && "simple range on 1 loop only");
                result = context.code.makeNode<SimNode_ForRange>(at);
            } else {
                assert(0 && "we should not be here yet");
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
            result->body = subexpr->simulate(context);
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
			} else {
				init = context.code.makeNode<SimNode_InitLocal>(pLet->at, var->stackTop, var->type->getSizeOf());
			}
			if (init)
				simlist.push_back(init);
		}
		return simlist;
	}

    SimNode * ExprLet::simulateInit(Context & context, const VariablePtr & var, bool local) {
        SimNode * init = var->init->simulate(context);
        SimNode * get;
        if ( local )
            get = context.code.makeNode<SimNode_GetLocal>(var->init->at, var->stackTop);
        else
            get = context.code.makeNode<SimNode_GetGlobal>(var->init->at, var->index);
        if ( var->type->ref ) {
            return context.code.makeNode<SimNode_CopyReference>(var->init->at, get, init);
        } else if ( var->type->canCopy() ) {
            auto varExpr = make_shared<ExprVar>(var->at, var->name);
            varExpr->variable = var;
            varExpr->local = local;
            varExpr->type = make_shared<TypeDecl>(*var->type);
            return makeCopy(var->init->at, context, varExpr, var->init);
        }
        else if ( var->type->canMove() )
            return makeMove(var->init->at, context, *var->init->type, get, init);
        else {
            assert(0 && "we should not be here");
            return nullptr;
        }
    }
    
    SimNode * ExprLet::simulate (Context & context) const {
        auto let = context.code.makeNode<SimNode_Let>(at);
        let->total = (uint32_t) variables.size();
        let->list = (SimNode **) context.code.allocate(let->total * sizeof(SimNode*));
		auto simList = ExprLet::simulateInit(context, this);
		std::copy(simList.data(), simList.data() + simList.size(), let->list);
        let->subexpr = subexpr ? subexpr->simulate(context) : nullptr;
        return let;
    }
    
    SimNode * ExprCall::simulate (Context & context) const {
        auto pCall = static_cast<SimNode_CallBase *>(func->makeSimNode(context));
        pCall->debug = at;
		assert((func->builtIn || func->index>=0) && "calling function which is not used. how?");
        pCall->fnPtr = context.getFunction(func->index);
        pCall->stackTop = stackTop;
        if ( int nArg = (int) arguments.size() ) {
            pCall->arguments = (SimNode **) context.code.allocate(nArg * sizeof(SimNode *));
            pCall->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pCall->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pCall->arguments = nullptr;
            pCall->nArguments = 0;
        }
        return pCall;
    }

    bool Program::simulate ( Context & context, ostream & logs ) {
        context.thisProgram = this;
        DebugInfoHelper helper(context.debugInfo);
        context.thisHelper = &helper;
        context.globalVariables = (GlobalVariable *) context.code.allocate( totalVariables*sizeof(GlobalVariable) );
		for (auto & pm : library.modules ) {
			for (auto & it : pm->globals) {
				auto pvar = it.second;
				if (!pvar->used)
					continue;
				assert(pvar->index >= 0 && "we are simulating variable, which is not used");
				auto & gvar = context.globalVariables[pvar->index];
				gvar.name = context.code.allocateName(pvar->name);
				gvar.size = pvar->type->getSizeOf();
				gvar.debug = helper.makeVariableDebugInfo(*it.second);
                auto varData = context.heap.allocate(gvar.size);
				gvar.value = cast<char *>::from((char *)varData);
                if ( !varData ) {
                    context.throw_error("can't allocate variable data, out of heap");
                    return false;
                }
			}
		}
        context.totalVariables = totalVariables;
        context.functions = (SimFunction *) context.code.allocate( totalFunctions*sizeof(SimFunction) );
        context.totalFunctions = totalFunctions;
		for (auto & pm : library.modules) {
			for (auto & it : pm->functions) {
				auto pfun = it.second;
				if (pfun->index < 0 || !pfun->used)
					continue;
				auto & gfun = context.functions[pfun->index];
				gfun.name = context.code.allocateName(pfun->name);
				gfun.code = pfun->simulate(context);
				gfun.stackSize = pfun->totalStackSize;
				gfun.debug = helper.makeFunctionDebugInfo(*pfun);
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
		context.globalInitStackSize = globalInitStackSize;
        context.simEnd();
        context.restart();
        context.runInitScript();
        context.restart();
        if (options.getOption("logMem")) {
            logs << "code  " << context.code.bytesAllocated()       << " of " << context.code.bytesTotal() << "\n";
            logs << "debug " << context.debugInfo.bytesAllocated()  << " of " << context.debugInfo.bytesTotal() << "\n";
            logs << "heap  " << context.heap.bytesAllocated()       << " of " << context.heap.bytesTotal() << "\n";
        }
        return errors.size() == 0;
    }
}

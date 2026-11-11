#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_simulate.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/simulate/debug_print.h"
#include "misc/include_fmt.h"

namespace das
{
    GcRootLambda::GcRootLambda( const Lambda & that, Context * _context ) : Lambda(that.capture) {
        context = _context;
        context->addGcRoot( (void *)capture, nullptr );
    }

    GcRootLambda::~GcRootLambda() {
        if ( capture && context && (context->category.value & uint32_t(das::ContextCategory::dead)) == 0u ) {
            context->removeGcRoot( (void *)capture );
        }
    }

    // this is here to occasionally investigate untyped evaluation paths
    #define WARN_SLOW_CAST(TYPE)
    // #define WARN_SLOW_CAST(TYPE)    DAS_ASSERTF(0, "internal perofrmance issue, casting eval to eval##TYPE" );

    SimNode * SimNode::copyNode ( Context &, NodeAllocator * code ) {
        auto prefix = ((NodePrefix *)this) - 1;
#ifndef DAS_NO_ASSERTIONS
        DAS_ASSERTF(prefix->magic==0xdeadc0de,"node was allocated on the heap without prefix");
#endif
        char * newNode;
        if ( code->prefixWithHeader ) {
            newNode = code->allocate(prefix->size + sizeof(NodePrefix));
            memcpy ( newNode, ((char *)this) - sizeof(NodePrefix), sizeof(NodePrefix));
            newNode += sizeof(NodePrefix);
        } else {
            newNode = code->allocate(prefix->size);
        }
        memcpy ( newNode, (char *)this, prefix->size );
        return (SimNode *) newNode;
    }

    bool SimNode::evalBool ( Context & context ) {
        WARN_SLOW_CAST(Bool);
        return cast<bool>::to(eval(context));
    }

    float SimNode::evalFloat ( Context & context ) {
        WARN_SLOW_CAST(Float);
        return cast<float>::to(eval(context));
    }

    double SimNode::evalDouble(Context & context) {
        WARN_SLOW_CAST(Double);
        return cast<double>::to(eval(context));
    }

    int32_t SimNode::evalInt ( Context & context ) {
        WARN_SLOW_CAST(Int);
        return cast<int32_t>::to(eval(context));
    }

    uint32_t SimNode::evalUInt ( Context & context ) {
        WARN_SLOW_CAST(UInt);
        return cast<uint32_t>::to(eval(context));
    }

    int64_t SimNode::evalInt64 ( Context & context ) {
        WARN_SLOW_CAST(Int64);
        return cast<int64_t>::to(eval(context));
    }

    uint64_t SimNode::evalUInt64 ( Context & context ) {
        WARN_SLOW_CAST(UInt64);
        return cast<uint64_t>::to(eval(context));
    }

    char * SimNode::evalPtr ( Context & context ) {
        WARN_SLOW_CAST(Ptr);
        return cast<char *>::to(eval(context));
    }

    SimNode * SimNode_WithErrorMessage::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_WithErrorMessage * that = (SimNode_WithErrorMessage *) SimNode::copyNode(context, code);
        if ( errorMessage ) {
            that->errorMessage = errorMessage[0]==0 ? "" : code->allocateName(errorMessage);
        }
        return that;
    }

    SimNode * SimNode_CallBase::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_CallBase * that = (SimNode_CallBase *) SimNode_WithErrorMessage::copyNode(context, code);
        if ( nArguments ) {
            that->arguments = copyNodeArray(code, that->arguments, nArguments);
            if ( that->types ) {
                that->types = copyNodeArray(code, that->types, nArguments);
            }
        }
        if ( fnPtr ) {
            that->fnPtr = context.fnByMangledName(fnPtr->mangledNameHash);
            // printf("CALL %p -> %p\n", fnPtr, fnPtr );
        }
        return that;
    }

    SimNode * SimNode_Final::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_Final * that = (SimNode_Final *) SimNode::copyNode(context, code);
        if ( totalFinal ) {
            that->finalList = copyNodeArray(code, that->finalList, totalFinal);
        }
        return that;
    }

    SimNode * SimNode_Block::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_Block * that = (SimNode_Block *) SimNode_Final::copyNode(context, code);
        if ( total ) {
            that->list = copyNodeArray(code, that->list, total);
        }
        if ( totalLabels ) {
            that->labels = copyNodeArray(code, that->labels, totalLabels);
        }
        return that;
    }

    const LineInfo * SimFunction::getLineInfo() const { return &code->debugInfo; }
    SimNode * SimNode::visit ( SimVisitor & vis ) {
        V_BEGIN();
        vis.op("??");
        V_END();
    }

    void SimVisitor::sub ( SimNode ** nodes, uint32_t count, const char * ) {
        for ( uint32_t t=0; t!=count; ++t ) {
            nodes[t] = nodes[t]->visit(*this);
        }
    }

    void SimNode_CallBase::visitCall ( SimVisitor & vis ) {
        if ( fnPtr ) {
            vis.arg(fnPtr->name,"fnPtr");
            vis.arg(Func(), fnPtr->mangledName, "fnIndex");
        }
        if ( cmresEval ) {
            V_SUB(cmresEval);
        }
        vis.sub(arguments, nArguments, "arguments");
    }

    SimNode * SimNode_At::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(At);
        V_SUB(value);
        V_SUB(index);
        V_ARG(stride);
        V_ARG(offset);
        V_ARG(range);
        V_END();
    }

    SimNode * SimNode_TypeInfo::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(TypeInfo);
        string dt = debug_type(typeInfo);
        V_ARG(dt.c_str());
        V_END();
    }

    SimNode * Function::makeSimNode ( Context & context, const vector<ExpressionPtr> & ) {
        if ( copyOnReturn || moveOnReturn ) {
            if ( context.maxFramedCallDepth ) {
                return context.code->makeNodeUnrollAny<SimNode_CallAndCopyOrMoveChecked>(int(arguments.size()), at);
            }
            return context.code->makeNodeUnrollAny<SimNode_CallAndCopyOrMove>(int(arguments.size()), at);
        } else if ( fastCall ) {
            // include/daScript/simulate/ARCHITECTURE.md#fastcall-depth-guard
            if ( context.maxFastCallDepth ) {
                return context.code->makeNodeUnrollAny<SimNode_FastCallChecked>(int(arguments.size()), at);
            }
            return context.code->makeNodeUnrollAny<SimNode_FastCall>(int(arguments.size()), at);
        } else {
            if ( context.maxFramedCallDepth ) {
                return context.code->makeNodeUnrollAny<SimNode_CallChecked>(int(arguments.size()), at);
            }
            return context.code->makeNodeUnrollAny<SimNode_Call>(int(arguments.size()), at);
        }
    }

    SimNode * GetR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & type, SimNode * expr ) {
        if ( type->isRefType() ) {
            return expr;
        } else {
            return context.code->makeValueNode<SimNode_Ref2Value>(type->getR2VType(), at, expr);
        }
    }

    SimNode* SimNode_FastCallAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(FastCall);
        V_CALL();
        V_END();
    }

    SimNode* SimNode_FastCallCheckedAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(FastCallChecked);
        V_CALL();
        V_END();
    }

    SimNode * SimNode_CallAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(Call);
        V_CALL();
        V_END();
    }

    SimNode* SimNode_CallAndCopyOrMoveAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(CallAndCopyOrMove);
        V_CALL();
        V_END();
    }

    SimNode * SimNode_CallCheckedAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(CallChecked);
        V_CALL();
        V_END();
    }

    SimNode* SimNode_CallAndCopyOrMoveCheckedAny::visit(SimVisitor& vis) {
        V_BEGIN();
        V_OP(CallAndCopyOrMoveChecked);
        V_CALL();
        V_END();
    }

    SimNode * SimNode_Ptr2Ref::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Ptr2Ref);
        V_SUB_OPT(subexpr);
        V_END();
    }

    SimNode * SimNode_Delete::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Delete);
        V_ARG(total);
        V_SUB(subexpr);
        V_END();
    }

    SimNode * SimNode_Zero::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Zero);
        V_END();
    }

    SimNode * SimNode_Op1::visitOp1 ( SimVisitor & vis, const char * op, int typeSize, const string & typeName ) {
        V_BEGIN();
        vis.op(op, typeSize, typeName);
        V_SUB(x);
        V_END();
    }

    SimNode * SimNode_Op2::visitOp2 ( SimVisitor & vis, const char * op, int typeSize, const string & typeName ) {
        V_BEGIN();
        vis.op(op, typeSize, typeName);
        V_SUB(l);
        V_SUB(r);
        V_END();
    }

    SimNode * SimNode_CallBase::visitOp1 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName ) {
        V_BEGIN();
        vis.op(op, typeSize, typeName);
        V_SUB(arguments[0]);
        V_END();
    }

    SimNode * SimNode_CallBase::visitOp2 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName ) {
        V_BEGIN();
        vis.op(op, typeSize, typeName);
        V_SUB(arguments[0]);
        V_SUB(arguments[1]);
        V_END();
    }

    SimNode * SimNode_CallBase::visitOp3 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName ) {
        V_BEGIN();
        vis.op(op, typeSize, typeName);
        V_SUB(arguments[0]);
        V_SUB(arguments[1]);
        V_SUB(arguments[2]);
        V_END();
    }

    vec4f SimNode_Block::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        return v_zero();
    }

    vec4f SimNode_ClosureBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        if ( context.stopFlags & EvalFlags::stopForReturn ) {
            context.stopFlags &= ~EvalFlags::stopForReturn;
            return context.abiResult();
        } else {
            if ( needResult ) context.throw_error_at(debugInfo,"end of block without return");
            return v_zero();
        }
    }

    void SimNode_Final::visitFinal ( SimVisitor & vis ) {
        vis.sub(finalList, totalFinal, "final");
    }

    SimNode * SimNode_Final::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(Final);
        V_FINAL()
        V_END();
    }

    void SimNode_Block::visitBlock ( SimVisitor & vis ) {
        vis.sub(list, total, "block");
    }

    void SimNode_Block::visitLabels ( SimVisitor & vis ) {
        if ( labels ) {
            V_ARG(totalLabels);
            for ( uint32_t i=0, is=totalLabels; i!=is; ++i ) {
                if ( labels[i]!=-1U ) {
                    char name[32];
                    auto result = fmt::format_to(name, FMT_STRING("label_{}"), i);
                    *result = 0;
                    vis.arg(labels[i], name);
                }
            }
        }
    }

    SimNode * SimNode_Block::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(Block);
        V_BLOCK();
        V_FINAL();
        V_END();
    }

    SimNode * SimNode_ClosureBlock::visit ( SimVisitor & vis ) {
        V_BEGIN_CR();
        V_OP(Block);
        V_BLOCK();
        V_FINAL();
        V_ARG(needResult);
        V_ARG(annotationDataSid);
        V_END();
    }

#if DAS_DEBUGGER
    SimNode * SimNodeDebug_Instrument::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Instrument);
        V_SUB(subexpr);
        V_END();
    }

    SimNode * SimNodeDebug_InstrumentFunction::visit ( SimVisitor & vis ) {
        V_BEGIN();
        V_OP(Instrument);
        vis.arg(func->name,"fnPtr");
        vis.arg(Func(), func->mangledName, "fnIndex");
        V_SUB(subexpr);
        V_END();
    }
#endif
}

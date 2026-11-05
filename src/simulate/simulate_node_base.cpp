#include "daScript/misc/platform.h"

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
            SimNode ** newArguments = (SimNode **) code->allocate(nArguments * sizeof(SimNode *));
            memcpy ( newArguments, that->arguments, nArguments * sizeof(SimNode *));
            that->arguments = newArguments;
            if ( that->types ) {
                TypeInfo ** newTypes = (TypeInfo **) code->allocate(nArguments * sizeof(TypeInfo **));
                memcpy ( newTypes, that->types, nArguments * sizeof(TypeInfo **));
                that->types = newTypes;
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
            SimNode ** newList = (SimNode **) code->allocate(totalFinal * sizeof(SimNode *));
            memcpy ( newList, that->finalList, totalFinal*sizeof(SimNode *));
            that->finalList = newList;
        }
        return that;
    }

    SimNode * SimNode_Block::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_Block * that = (SimNode_Block *) SimNode_Final::copyNode(context, code);
        if ( total ) {
            SimNode ** newList = (SimNode **) code->allocate(total * sizeof(SimNode *));
            memcpy ( newList, that->list, total*sizeof(SimNode *));
            that->list = newList;
        }
        if ( totalLabels ) {
            uint32_t * newLabels = (uint32_t *) code->allocate(totalLabels * sizeof(uint32_t));
            memcpy ( newLabels, that->labels, totalLabels*sizeof(uint32_t));
            that->labels = newLabels;
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

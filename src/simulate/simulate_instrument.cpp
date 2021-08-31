#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"

namespace das {

#if DAS_DEBUGGER

    struct SimInstVisitor : SimVisitor {
        bool isCorrectFileAndLine ( const LineInfo & info ) {
            if ( lineNumber>=info.line && lineNumber<=info.last_line ) {
                if ( info.fileInfo && info.fileInfo->name==fileName ) {
                    return true;
                }
            }
            return false;
        }
        SimNode * instrumentNode ( SimNode * expr ) {
            if ( !expr->rtti_node_isInstrument() ) {
                return context->code->makeNode<SimNodeDebug_Instrument>(expr->debugInfo, expr);
            } else {
                return expr;
            }
        }
        SimNode * clearNode ( SimNode * expr ) {
            if ( expr->rtti_node_isInstrument() ) {
                auto si = (SimNodeDebug_Instrument *) expr;
                return si->subexpr;
            } else {
                return expr;
            }
        }
        virtual SimNode * visit ( SimNode * node ) override {
            if ( node->rtti_node_isBlock() ) {
                SimNode_Block * blk = (SimNode_Block *) node;
                for ( uint32_t i=0; i!=blk->total; ++i ) {
                    auto & expr = blk->list[i];
                    if ( anyLine || isCorrectFileAndLine(expr->debugInfo) ) {
                        expr = isInstrumenting ? instrumentNode(expr) : clearNode(expr);
                    }
                }
                for ( uint32_t i=0; i!=blk->totalFinal; ++i ) {
                    auto & expr = blk->finalList[i];
                    if ( anyLine || isCorrectFileAndLine(expr->debugInfo) ) {
                        expr = isInstrumenting ? instrumentNode(expr) : clearNode(expr);
                    }
                }
            }
            return node;
        }
        Context * context = nullptr;
        const char * fileName = nullptr;
        uint32_t lineNumber = 0;
        bool isInstrumenting = true;
        bool anyLine = false;
    };

    void Context::instrumentContextNode ( const char * fileName, int32_t lineNumber, bool isInstrumenting ) {
        SimInstVisitor instrument;
        instrument.context = this;
        instrument.fileName = fileName;
        instrument.lineNumber = uint32_t(lineNumber);
        instrument.isInstrumenting = isInstrumenting;
        runVisitor(&instrument);
    }
    void Context::clearInstruments() {
        SimInstVisitor instrument;
        instrument.context = this;
        instrument.isInstrumenting = false;
        instrument.anyLine = true;
        runVisitor(&instrument);
    }
#else
    void Context::instrumentContextNode ( const char *, int32_t, bool ) {}
    void Context::clearInstruments() {}
#endif

}

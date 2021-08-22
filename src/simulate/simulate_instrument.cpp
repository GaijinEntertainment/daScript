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
            return context->code->makeNode<SimNodeDebug_Instrument>(expr->debugInfo, expr);
        }

        virtual SimNode * visit ( SimNode * node ) override {
            if ( node->rtti_node_isBlock() ) {
                SimNode_Block * blk = (SimNode_Block *) node;
                for ( uint32_t i=0; i!=blk->total; ++i ) {
                    auto & expr = blk->list[i];
                    if ( !expr->rtti_node_isInstrument() && isCorrectFileAndLine(expr->debugInfo) ) {
                        expr = instrumentNode(expr);
                    }
                }
                for ( uint32_t i=0; i!=blk->totalFinal; ++i ) {
                    auto & expr = blk->finalList[i];
                    if ( !expr->rtti_node_isInstrument() && isCorrectFileAndLine(expr->debugInfo) ) {
                        expr = instrumentNode(expr);
                    }
                }
            }
            return node;
        }
        Context * context = nullptr;
        const char * fileName = nullptr;
        int32_t lineNumber = 0;
    };

    void Context::InstrumentContext ( const char * fileName, int32_t lineNumber ) {
        SimInstVisitor instrument;
        instrument.context = this;
        instrument.fileName = fileName;
        instrument.lineNumber = lineNumber;
        for ( int gvi=0; gvi!=totalVariables; ++gvi ) {
            const auto & gv = globalVariables[gvi];
            if ( gv.init ) gv.init->visit(instrument);
        }
        for ( int fni=0; fni!=totalFunctions; ++fni ) {
            const auto & fn = functions[fni];
            if ( fn.code ) fn.code->visit(instrument);
        }
    }

#else
    void Context::InstrumentContext ( const char *, int32_t ) {
    }
#endif

}

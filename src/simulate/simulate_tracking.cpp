#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"

namespace das
{
#if DAS_ENABLE_SMART_PTR_TRACKING
    vector<smart_ptr<ptr_ref_count>> Context::sptrAllocations;
#endif

    string around ( const LineInfo & info ) {
        TextWriter tw;
        tw << ":" << info.line << ":" << info.column;
        return tw.str();
    }

    void dumpTrackingLeaks ( ) {
        TextPrinter tp;
#if DAS_ENABLE_SMART_PTR_TRACKING
        int total = 0;
        for ( auto & ptr : Context::sptrAllocations ) {
            if ( ptr->use_count()>1 ) {
                tp << "0x" << HEX << intptr_t(ptr.get()) << DEC << " (" << (ptr->use_count()-1) << ")";
#if DAS_SMART_PTR_ID
                tp << " id=" << HEX << ptr->ref_count_id << DEC;
#endif
                if ( auto td = dynamic_cast<TypeDecl *>(ptr.get()) ) {
                    tp << " TypeDecl at " << around(td->at);
                } else if ( auto ex = dynamic_cast<Expression *>(ptr.get()) ) {
                    tp << " " <<  ex->__rtti << " at " << around(ex->at);
                    if ( strcmp(ex->__rtti,"ExprConstString")==0 ) {
                        auto excs = static_cast<ExprConstString *>(ex);
                        tp << " \"" << excs->text << "\"";
                    }
                } else if ( auto vp = dynamic_cast<Variable *>(ptr.get()) ) {
                    tp << " Variable " << vp->name << " at " << around(vp->at);
                }
                tp << "\n";
                total ++;
            }
        }
        if ( total ) tp << "total " << total << " leaks\n";
#else
        tp << "smart_ptr tracking is disabled\n";
#endif
    }
}


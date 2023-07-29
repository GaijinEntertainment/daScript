#pragma once

namespace das {

char * from_CXString_to_string ( const CXString & cxs, Context * context );
void peek_CXString ( const CXString & cxs, const TBlock<void,TTemporary<const char *>> & blk, Context * context, LineInfoArg * arg );
void das_clangVisitChildren ( CXCursor cursor, const TBlock<CXChildVisitResult,CXCursor,CXCursor> & block, Context * ctx, LineInfoArg * at );

}

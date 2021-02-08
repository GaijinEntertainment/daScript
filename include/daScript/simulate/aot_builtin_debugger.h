#pragma once

namespace das {

    DebugAgentPtr makeDebugAgent ( const void * pClass, const StructInfo * info, Context * context );
    void debuggerStackWalk ( Context & context, const LineInfo & lineInfo );
    void debuggerSetContextSingleStep ( Context & context, bool step );

    DataWalkerPtr makeDataWalker ( const void * pClass, const StructInfo * info, Context * context );
    void dapiWalkData ( DataWalkerPtr walker, void * data, const TypeInfo & info );
    void dapiWalkDataV ( DataWalkerPtr walker, float4 data, const TypeInfo & info );

    StackWalkerPtr makeStackWalker ( const void * pClass, const StructInfo * info, Context * context );

    __forceinline Context  & thisContext ( Context * context ) { return *context; }
}

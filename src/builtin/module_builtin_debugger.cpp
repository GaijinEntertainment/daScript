#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/aot_builtin_debugger.h"
#include "module_builtin_rtti.h"
#include "daScript/misc/performance_time.h"

using namespace das;

MAKE_TYPE_FACTORY(DebugAgent,DebugAgent)
MAKE_TYPE_FACTORY(DataWalker,DataWalker)

namespace das
{
    struct DebugAgentAdapter : DebugAgent {
        DebugAgentAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : classPtr(pClass), context(ctx) {
            fnOnInstall = adapt("onInstall",pClass,info);
            fnOnUninstall = adapt("onUninstall",pClass,info);
            fnOnCreateContext = adapt("onCreateContext",pClass,info);
            fnOnDestroyContext = adapt("onDestroyContext",pClass,info);
            fnOnSingleStep = adapt("onSingleStep",pClass,info);
            fnOnBreakpoint = adapt("onBreakpoint",pClass,info);
            fnOnTick = adapt("onTick",pClass,info);
        }
        virtual void onInstall ( DebugAgent * agent ) override {
            if ( fnOnInstall ) {
                das_invoke_function<void>::invoke<void *,DebugAgentPtr>(context,fnOnInstall,classPtr,agent);
            }
        }
        virtual void onUninstall ( DebugAgent * agent ) override {
            if ( fnOnInstall ) {
                das_invoke_function<void>::invoke<void *,DebugAgentPtr>(context,fnOnUninstall,classPtr,agent);
            }
        }
        virtual void onCreateContext ( Context * ctx ) override {
            if ( fnOnCreateContext ) {
                das_invoke_function<void>::invoke<void *,Context *>(context,fnOnCreateContext,classPtr,ctx);
            }
        }
        virtual void onDestroyContext ( Context * ctx ) override {
            if ( fnOnDestroyContext ) {
                das_invoke_function<void>::invoke<void *,Context *>(context,fnOnDestroyContext,classPtr,ctx);
            }
        }
        virtual void onSingleStep ( Context * ctx, const LineInfo & at ) override {
            if ( fnOnSingleStep ) {
                das_invoke_function<void>::invoke<void *,Context *,const LineInfo &>(context,fnOnSingleStep,classPtr,ctx,at);
            }
        }
        virtual void onBreakpoint ( Context * ctx, const LineInfo & at ) override {
            if ( fnOnBreakpoint ) {
                das_invoke_function<void>::invoke<void *,Context *,const LineInfo &>(context,fnOnBreakpoint,classPtr,ctx,at);
            }
        }
        virtual void onTick () override {
            if ( fnOnTick ) {
                das_invoke_function<void>::invoke<void *>(context,fnOnTick,classPtr);
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnOnInstall;
        Func    fnOnUninstall;
        Func    fnOnCreateContext;
        Func    fnOnDestroyContext;
        Func    fnOnSingleStep;
        Func    fnOnBreakpoint;
        Func    fnOnTick;
    };

    struct AstDebugAgentAnnotation : ManagedStructureAnnotation<DebugAgent,false,true> {
        AstDebugAgentAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("DebugAgent", ml) {
        }
    };

    struct DataWalkerAdapter : DataWalker {
        DataWalkerAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : classPtr(pClass) {
            context = ctx;
            // data structures
            fn_canVisitHandle = adapt("canVisitHandle",pClass,info);
            fn_canVisitStructure = adapt("canVisitStructure",pClass,info);
            fn_beforeStructure = adapt("beforeStructure",pClass,info);
            fn_afterStructure = adapt("afterStructure",pClass,info);
            fn_afterStructureCancel = adapt("afterStructureCancel",pClass,info);
            fn_beforeStructureField = adapt("beforeStructureField",pClass,info);
            fn_afterStructureField = adapt("afterStructureField",pClass,info);
            fn_beforeTuple = adapt("beforeTuple",pClass,info);
            fn_afterTuple = adapt("afterTuple",pClass,info);
            fn_beforeTupleEntry = adapt("beforeTupleEntry",pClass,info);
            fn_afterTupleEntry = adapt("afterTupleEntry",pClass,info);
            fn_beforeVariant = adapt("beforeVariant",pClass,info);
            fn_afterVariant = adapt("afterVariant",pClass,info);
            fn_beforeArrayData = adapt("beforeArrayData",pClass,info);
            fn_afterArrayData = adapt("afterArrayData",pClass,info);
            fn_beforeArrayElement = adapt("beforeArrayElement",pClass,info);
            fn_afterArrayElement = adapt("afterArrayElement",pClass,info);
            fn_beforeDim = adapt("beforeDim",pClass,info);
            fn_afterDim = adapt("afterDim",pClass,info);
            fn_beforeArray = adapt("beforeArray",pClass,info);
            fn_afterArray = adapt("afterArray",pClass,info);
            fn_beforeTable = adapt("beforeTable",pClass,info);
            fn_beforeTableKey = adapt("beforeTableKey",pClass,info);
            fn_afterTableKey = adapt("afterTableKey",pClass,info);
            fn_beforeTableValue = adapt("beforeTableValue",pClass,info);
            fn_afterTableValue = adapt("afterTableValue",pClass,info);
            fn_afterTable = adapt("afterTable",pClass,info);
            fn_beforeRef = adapt("beforeRef",pClass,info);
            fn_afterRef = adapt("afterRef",pClass,info);
            fn_beforePtr = adapt("beforePtr",pClass,info);
            fn_afterPtr = adapt("afterPtr",pClass,info);
            fn_beforeHandle = adapt("beforeHandle",pClass,info);
            fn_afterHandle = adapt("afterHandle",pClass,info);
            fn_beforeLambda = adapt("beforeLambda",pClass,info);
            fn_afterLambda = adapt("afterLambda",pClass,info);
            fn_beforeIterator = adapt("beforeIterator",pClass,info);
            fn_afterIterator = adapt("afterIterator",pClass,info);
            fn_Null = adapt("Null",pClass,info);
            fn_Bool = adapt("Bool",pClass,info);
            fn_Int8 = adapt("Int8",pClass,info);
            fn_UInt8 = adapt("UInt8",pClass,info);
            fn_Int16 = adapt("Int16",pClass,info);
            fn_UInt16 = adapt("UInt16",pClass,info);
            fn_Int64 = adapt("Int64",pClass,info);
            fn_UInt64 = adapt("UInt64",pClass,info);
            fn_String = adapt("String",pClass,info);
            fn_Double = adapt("Double",pClass,info);
            fn_Float = adapt("Float",pClass,info);
            fn_Int = adapt("Int",pClass,info);
            fn_UInt = adapt("UInt",pClass,info);
            fn_Bitfield = adapt("Bitfield",pClass,info);
            fn_Int2 = adapt("Int2",pClass,info);
            fn_Int3 = adapt("Int3",pClass,info);
            fn_Int4 = adapt("Int4",pClass,info);
            fn_UInt2 = adapt("UInt2",pClass,info);
            fn_UInt3 = adapt("UInt3",pClass,info);
            fn_UInt4 = adapt("UInt4",pClass,info);
            fn_Float2 = adapt("Float2",pClass,info);
            fn_Float3 = adapt("Float3",pClass,info);
            fn_Float4 = adapt("Float4",pClass,info);
            fn_Range = adapt("Range",pClass,info);
            fn_URange = adapt("URange",pClass,info);
            fn_WalkBlock = adapt("WalkBlock",pClass,info);
            fn_WalkFunction = adapt("WalkFunction",pClass,info);
            fn_WalkEnumeration = adapt("WalkEnumeration",pClass,info);
            fn_WalkEnumeration8 = adapt("WalkEnumeration8",pClass,info);
            fn_WalkEnumeration16 = adapt("WalkEnumeration16",pClass,info);
            fn_FakeContext = adapt("FakeContext",pClass,info);
        }
    // data structures
        virtual bool canVisitHandle ( char * ps, TypeInfo * ti ) override {
            if ( fn_canVisitHandle ) {
                return das_invoke_function<bool>::invoke<void *,char *,TypeInfo *>
                    (context,fn_canVisitHandle,classPtr,ps,ti);
            } else {
                return true;
            }
        }
        virtual bool canVisitStructure ( char * ps, StructInfo * si ) override {
            if ( fn_canVisitStructure ) {
                return das_invoke_function<bool>::invoke<void *,char *,StructInfo *>
                    (context,fn_canVisitStructure,classPtr,ps,si);
            } else {
                return true;
            }
        }
        virtual void beforeStructure ( char * ps, StructInfo * si ) override {
            if ( fn_beforeStructure ) {
                das_invoke_function<void>::invoke<void *,char *,StructInfo *>
                    (context,fn_beforeStructure,classPtr,ps,si);
            }
        }
        virtual void afterStructure ( char * ps, StructInfo * si ) override {
            if ( fn_afterStructure ) {
                das_invoke_function<void>::invoke<void *,char *,StructInfo *>
                    (context,fn_afterStructure,classPtr,ps,si);
            }
        }
        virtual void afterStructureCancel ( char * ps, StructInfo * si ) override {
            if ( fn_afterStructureCancel ) {
                das_invoke_function<void>::invoke<void *,char *,StructInfo *>
                    (context,fn_afterStructureCancel,classPtr,ps,si);
            }
        }
        virtual void beforeStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            if ( fn_beforeStructureField ) {
                das_invoke_function<void>::invoke<void *,char *,StructInfo *, char *, VarInfo *, bool>
                    (context,fn_beforeStructureField,classPtr,ps,si,pv,vi,last);
            }
        }
        virtual void afterStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            if ( fn_afterStructureField ) {
                das_invoke_function<void>::invoke<void *,char *,StructInfo *, char *, VarInfo *, bool>
                    (context,fn_afterStructureField,classPtr,ps,si,pv,vi,last);
            }
        }
        virtual void beforeTuple ( char * ps, TypeInfo * ti ) override {
            if ( fn_beforeTuple ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforeTuple,classPtr,ps,ti);
            }
        }
        virtual void afterTuple ( char * ps, TypeInfo * ti ) override {
            if ( fn_afterTuple ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterTuple,classPtr,ps,ti);
            }
        }
        virtual void beforeTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            if ( fn_beforeTupleEntry ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *, char *, TypeInfo *, bool>
                    (context,fn_beforeTupleEntry,classPtr,ps,ti,pv,vi,last);
            }
        }
        virtual void afterTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            if ( fn_afterTupleEntry ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *, char *, TypeInfo *, bool>
                    (context,fn_afterTupleEntry,classPtr,ps,ti,pv,vi,last);
            }
        }
        virtual void beforeVariant ( char * ps, TypeInfo * ti ) override {
            if ( fn_beforeVariant ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforeVariant,classPtr,ps,ti);
            }
        }
        virtual void afterVariant ( char * ps, TypeInfo * ti ) override {
            if ( fn_afterVariant ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterVariant,classPtr,ps,ti);
            }
        }
        virtual void beforeArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) override {
            if ( fn_beforeArrayData ) {
                das_invoke_function<void>::invoke<void *,char *,uint32_t,uint32_t,TypeInfo *>
                    (context,fn_beforeArrayData,classPtr,pa,stride,count,ti);
            }
        }
        virtual void afterArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) override {
            if ( fn_afterArrayData ) {
                das_invoke_function<void>::invoke<void *,char *,uint32_t,uint32_t,TypeInfo *>
                    (context,fn_afterArrayData,classPtr,pa,stride,count,ti);
            }
        }
        virtual void beforeArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            if ( fn_beforeArrayElement ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *,char *,uint32_t,bool>
                    (context,fn_beforeArrayElement,classPtr,pa,ti,pe,index,last);
            }
        }
        virtual void afterArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            if ( fn_afterArrayElement ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *,char *,uint32_t,bool>
                    (context,fn_afterArrayElement,classPtr,pa,ti,pe,index,last);
            }
        }
        virtual void beforeDim ( char * pa, TypeInfo * ti ) override {
            if ( fn_beforeDim ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforeDim,classPtr,pa,ti);
            }
        }
        virtual void afterDim ( char * pa, TypeInfo * ti ) override {
            if ( fn_afterDim ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterDim,classPtr,pa,ti);
            }
        }
        virtual void beforeArray ( Array * pa, TypeInfo * ti ) override {
            if ( fn_beforeArray ) {
                das_invoke_function<void>::invoke<void *,Array *,TypeInfo *>
                    (context,fn_beforeArray,classPtr,pa,ti);
            }
        }
        virtual void afterArray ( Array * pa, TypeInfo * ti ) override {
            if ( fn_afterArray ) {
                das_invoke_function<void>::invoke<void *,Array *,TypeInfo *>
                    (context,fn_afterArray,classPtr,pa,ti);
            }
        }
        virtual void beforeTable ( Table * pa, TypeInfo * ti ) override {
           if ( fn_beforeTable ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *>
                    (context,fn_beforeTable,classPtr,pa,ti);
            }
        }
        virtual void beforeTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
           if ( fn_beforeTableKey ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *,char *,TypeInfo *,uint32_t,bool>
                    (context,fn_beforeTableKey,classPtr,pa,ti,pk,ki,index,last);
            }
        }
        virtual void afterTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
           if ( fn_afterTableKey ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *,char *,TypeInfo *,uint32_t,bool>
                    (context,fn_afterTableKey,classPtr,pa,ti,pk,ki,index,last);
            }
        }
        virtual void beforeTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
           if ( fn_beforeTableValue ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *,char *,TypeInfo *,uint32_t,bool>
                    (context,fn_beforeTableValue,classPtr,pa,ti,pv,kv,index,last);
            }
        }
        virtual void afterTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
           if ( fn_afterTableValue ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *,char *,TypeInfo *,uint32_t,bool>
                    (context,fn_afterTableValue,classPtr,pa,ti,pv,kv,index,last);
            }
        }
        virtual void afterTable ( Table * pa, TypeInfo * ti ) override {
           if ( fn_afterTable ) {
                das_invoke_function<void>::invoke<void *,Table *,TypeInfo *>
                    (context,fn_afterTable,classPtr,pa,ti);
            }
        }
        virtual void beforeRef ( char * pa, TypeInfo * ti ) override {
           if ( fn_beforeRef ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforeRef,classPtr,pa,ti);
            }
        }
        virtual void afterRef ( char * pa, TypeInfo * ti ) override {
           if ( fn_afterRef ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterRef,classPtr,pa,ti);
            }
        }
        virtual void beforePtr ( char * pa, TypeInfo * ti ) override {
           if ( fn_beforePtr ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforePtr,classPtr,pa,ti);
            }
        }
        virtual void afterPtr ( char * pa, TypeInfo * ti ) override {
           if ( fn_afterPtr ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterPtr,classPtr,pa,ti);
            }
        }
        virtual void beforeHandle ( char * pa, TypeInfo * ti ) override {
           if ( fn_beforeHandle ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_beforeHandle,classPtr,pa,ti);
            }
        }
        virtual void afterHandle ( char * pa, TypeInfo * ti ) override {
           if ( fn_afterHandle ) {
                das_invoke_function<void>::invoke<void *,char *,TypeInfo *>
                    (context,fn_afterHandle,classPtr,pa,ti);
            }
        }
        virtual void beforeLambda ( Lambda * pa, TypeInfo * ti ) override {
           if ( fn_beforeLambda ) {
                das_invoke_function<void>::invoke<void *,Lambda *,TypeInfo *>
                    (context,fn_beforeLambda,classPtr,pa,ti);
            }
        }
        virtual void afterLambda ( Lambda * pa, TypeInfo * ti ) override {
           if ( fn_afterLambda ) {
                das_invoke_function<void>::invoke<void *,Lambda *,TypeInfo *>
                    (context,fn_afterLambda,classPtr,pa,ti);
            }
        }
        virtual void beforeIterator ( Sequence * pa, TypeInfo * ti ) override {
           if ( fn_beforeIterator ) {
                das_invoke_function<void>::invoke<void *,Sequence *,TypeInfo *>
                    (context,fn_beforeIterator,classPtr,pa,ti);
            }
        }
        virtual void afterIterator ( Sequence * pa, TypeInfo * ti ) override {
           if ( fn_afterIterator ) {
                das_invoke_function<void>::invoke<void *,Sequence *,TypeInfo *>
                    (context,fn_afterIterator,classPtr,pa,ti);
            }
        }
    // types
        virtual void Null ( TypeInfo * ti ) override {
           if ( fn_Null ) {
                das_invoke_function<void>::invoke<void *,TypeInfo *>
                    (context,fn_Null,classPtr,ti);
            }
        }
        virtual void Bool ( bool & value ) override {
           if ( fn_Bool ) {
                das_invoke_function<void>::invoke<void *,bool *>
                    (context,fn_Bool,classPtr,&value);
            }
        }
        virtual void Int8 ( int8_t & value ) override {
           if ( fn_Int8 ) {
                das_invoke_function<void>::invoke<void *,int8_t *>
                    (context,fn_Int8,classPtr,&value);
            }
        }
        virtual void UInt8 ( uint8_t & value ) override {
           if ( fn_UInt8 ) {
                das_invoke_function<void>::invoke<void *,uint8_t *>
                    (context,fn_UInt8,classPtr,&value);
            }
        }
        virtual void Int16 ( int16_t & value ) override {
           if ( fn_Int16 ) {
                das_invoke_function<void>::invoke<void *,int16_t *>
                    (context,fn_Int16,classPtr,&value);
            }
        }
        virtual void UInt16 ( uint16_t & value ) override {
           if ( fn_UInt16 ) {
                das_invoke_function<void>::invoke<void *,uint16_t *>
                    (context,fn_UInt16,classPtr,&value);
            }
        }
        virtual void Int64 ( int64_t & value ) override {
           if ( fn_Int64 ) {
                das_invoke_function<void>::invoke<void *,int64_t *>
                    (context,fn_Int64,classPtr,&value);
            }
        }
        virtual void UInt64 ( uint64_t & value ) override {
           if ( fn_UInt64 ) {
                das_invoke_function<void>::invoke<void *,uint64_t *>
                    (context,fn_UInt64,classPtr,&value);
            }
        }
        virtual void String ( char * & value ) override {
           if ( fn_String ) {
                das_invoke_function<void>::invoke<void *,char **>
                    (context,fn_String,classPtr,&value);
            }
        }
        virtual void Double ( double & value ) override {
           if ( fn_Double ) {
                das_invoke_function<void>::invoke<void *,double *>
                    (context,fn_Double,classPtr,&value);
            }
        }
        virtual void Float ( float & value ) override {
           if ( fn_Float ) {
                das_invoke_function<void>::invoke<void *,float *>
                    (context,fn_Float,classPtr,&value);
            }
        }
        virtual void Int ( int32_t & value ) override {
           if ( fn_Int ) {
                das_invoke_function<void>::invoke<void *,int32_t *>
                    (context,fn_Int,classPtr,&value);
            }
        }
        virtual void UInt ( uint32_t & value ) override {
           if ( fn_UInt ) {
                das_invoke_function<void>::invoke<void *,uint32_t *>
                    (context,fn_UInt,classPtr,&value);
            }
        }
        virtual void Bitfield ( uint32_t & value, TypeInfo * ti ) override {
           if ( fn_Bitfield ) {
                das_invoke_function<void>::invoke<void *,uint32_t *,TypeInfo *>
                    (context,fn_Bitfield,classPtr,&value,ti);
            }
        }
        virtual void Int2 ( int2 & value ) override {
           if ( fn_Int2 ) {
                das_invoke_function<void>::invoke<void *,int2 *>
                    (context,fn_Int2,classPtr,&value);
            }
        }
        virtual void Int3 ( int3 & value ) override {
           if ( fn_Int3 ) {
                das_invoke_function<void>::invoke<void *,int3 *>
                    (context,fn_Int3,classPtr,&value);
            }
        }
        virtual void Int4 ( int4 & value ) override {
           if ( fn_Int4 ) {
                das_invoke_function<void>::invoke<void *,int4 *>
                    (context,fn_Int4,classPtr,&value);
            }
        }
        virtual void UInt2 ( uint2 & value ) override {
           if ( fn_UInt2 ) {
                das_invoke_function<void>::invoke<void *,uint2 *>
                    (context,fn_UInt2,classPtr,&value);
            }
        }
        virtual void UInt3 ( uint3 & value ) override {
           if ( fn_UInt3 ) {
                das_invoke_function<void>::invoke<void *,uint3 *>
                    (context,fn_UInt3,classPtr,&value);
            }
        }
        virtual void UInt4 ( uint4 & value ) override {
           if ( fn_UInt4 ) {
                das_invoke_function<void>::invoke<void *,uint4 *>
                    (context,fn_UInt4,classPtr,&value);
            }
        }
        virtual void Float2 ( float2 & value ) override {
           if ( fn_Float2 ) {
                das_invoke_function<void>::invoke<void *,float2 *>
                    (context,fn_Float2,classPtr,&value);
            }
        }
        virtual void Float3 ( float3 & value ) override {
           if ( fn_Float3 ) {
                das_invoke_function<void>::invoke<void *,float3 *>
                    (context,fn_Float3,classPtr,&value);
            }
        }
        virtual void Float4 ( float4 & value ) override {
           if ( fn_Float4 ) {
                das_invoke_function<void>::invoke<void *,float4 *>
                    (context,fn_Float4,classPtr,&value);
            }
        }
        virtual void Range ( range & value ) override {
           if ( fn_Range ) {
                das_invoke_function<void>::invoke<void *,range *>
                    (context,fn_Range,classPtr,&value);
            }
        }
        virtual void URange ( urange & value ) override {
           if ( fn_URange ) {
                das_invoke_function<void>::invoke<void *,urange *>
                    (context,fn_URange,classPtr,&value);
            }
        }
        virtual void WalkBlock ( Block * value ) override {
           if ( fn_WalkBlock ) {
                das_invoke_function<void>::invoke<void *,Block *>
                    (context,fn_WalkBlock,classPtr,value);
            }
        }
        virtual void WalkFunction ( Func * value ) override {
           if ( fn_WalkFunction ) {
                das_invoke_function<void>::invoke<void *,Func *>
                    (context,fn_WalkFunction,classPtr,value);
            }
        }
        virtual void WalkEnumeration ( int32_t & value, EnumInfo * ei ) override {
           if ( fn_WalkEnumeration ) {
                das_invoke_function<void>::invoke<void *,int32_t *,EnumInfo *>
                    (context,fn_WalkEnumeration,classPtr,&value,ei);
            }
        }
        virtual void WalkEnumeration8  ( int8_t & value, EnumInfo * ei ) override {
           if ( fn_WalkEnumeration8 ) {
                das_invoke_function<void>::invoke<void *,int8_t *,EnumInfo *>
                    (context,fn_WalkEnumeration8,classPtr,&value,ei);
            }
        }
        virtual void WalkEnumeration16 ( int16_t & value, EnumInfo * ei ) override {
           if ( fn_WalkEnumeration16 ) {
                das_invoke_function<void>::invoke<void *,int16_t *,EnumInfo *>
                    (context,fn_WalkEnumeration16,classPtr,&value,ei);
            }
        }
        virtual void FakeContext ( Context * value ) override {
           if ( fn_FakeContext ) {
                das_invoke_function<void>::invoke<void *,Context *>
                    (context,fn_FakeContext,classPtr,value);
            }
        }
    protected:
        void *      classPtr;
    protected:
    // data structures
        Func fn_canVisitHandle;
        Func fn_canVisitStructure;
        Func fn_beforeStructure;
        Func fn_afterStructure;
        Func fn_afterStructureCancel;
        Func fn_beforeStructureField;
        Func fn_afterStructureField;
        Func fn_beforeTuple;
        Func fn_afterTuple;
        Func fn_beforeTupleEntry;
        Func fn_afterTupleEntry;
        Func fn_beforeVariant;
        Func fn_afterVariant;
        Func fn_beforeArrayData;
        Func fn_afterArrayData;
        Func fn_beforeArrayElement;
        Func fn_afterArrayElement;
        Func fn_beforeDim;
        Func fn_afterDim;
        Func fn_beforeArray;
        Func fn_afterArray;
        Func fn_beforeTable;
        Func fn_beforeTableKey;
        Func fn_afterTableKey;
        Func fn_beforeTableValue;
        Func fn_afterTableValue;
        Func fn_afterTable;
        Func fn_beforeRef;
        Func fn_afterRef;
        Func fn_beforePtr;
        Func fn_afterPtr;
        Func fn_beforeHandle;
        Func fn_afterHandle;
        Func fn_beforeLambda;
        Func fn_afterLambda;
        Func fn_beforeIterator;
        Func fn_afterIterator;
    // types
        Func fn_Null;
        Func fn_Bool;
        Func fn_Int8;
        Func fn_UInt8;
        Func fn_Int16;
        Func fn_UInt16;
        Func fn_Int64;
        Func fn_UInt64;
        Func fn_String;
        Func fn_Double;
        Func fn_Float;
        Func fn_Int;
        Func fn_UInt;
        Func fn_Bitfield;
        Func fn_Int2;
        Func fn_Int3;
        Func fn_Int4;
        Func fn_UInt2;
        Func fn_UInt3;
        Func fn_UInt4;
        Func fn_Float2;
        Func fn_Float3;
        Func fn_Float4;
        Func fn_Range;
        Func fn_URange;
        Func fn_WalkBlock;
        Func fn_WalkFunction;
        Func fn_WalkEnumeration;
        Func fn_WalkEnumeration8;
        Func fn_WalkEnumeration16;
        Func fn_FakeContext;
    };

    struct AstDataWalkerAnnotation : ManagedStructureAnnotation<DataWalker,false,true> {
        AstDataWalkerAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("DataWalker", ml) {
        }
    };

    #include "debugger.das.inc"

    DebugAgentPtr makeDebugAgent ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<DebugAgentAdapter>((char *)pClass,info,context);
    }

    void debuggerSetContextSingleStep ( Context & context, bool step ) {
        context.setSingleStep(step);
    }

    void debuggerStackWalk ( Context & context, const LineInfo & lineInfo ) {
        context.stackWalk(&lineInfo, true, true);
    }

    DataWalkerPtr makeDataWalker ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<DataWalkerAdapter>((char *)pClass,info,context);
    }

    void dapiWalkData ( DataWalkerPtr walker, void * data, const TypeInfo & info ) {
        walker->walk((char *)data,(TypeInfo*)&info);
    }

    void dapiWalkDataV ( DataWalkerPtr walker, float4 data, const TypeInfo & info ) {
        walker->walk((vec4f)data,(TypeInfo*)&info);
    }

    class Module_Debugger : public Module {
    public:
        Module_Debugger() : Module("debugapi") {
            DAS_PROFILE_SECTION("Module_Debugger");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            lib.addModule(Module::require("rtti"));
            // annotations
            addAnnotation(make_smart<AstDebugAgentAnnotation>(lib));
            addAnnotation(make_smart<AstDataWalkerAnnotation>(lib));
            // debug agent
            addExtern<DAS_BIND_FUN(makeDebugAgent)>(*this, lib,  "make_debug_agent",
                SideEffects::modifyExternal, "makeDebugAgent");
            addExtern<DAS_BIND_FUN(tickDebugAgent)>(*this, lib,  "tick_debug_agent",
                SideEffects::modifyExternal, "tickDebugAgent");
            addExtern<DAS_BIND_FUN(installDebugAgent)>(*this, lib,  "install_debug_agent",
                SideEffects::modifyExternal, "installDebugAgent");
            addExtern<DAS_BIND_FUN(forkDebugAgentContext)>(*this, lib,  "fork_debug_agent_context",
                SideEffects::modifyExternal, "forkDebugAgentContext");
            addExtern<DAS_BIND_FUN(debuggerSetContextSingleStep)>(*this, lib,  "set_single_step",
                SideEffects::modifyExternal, "debuggerSetContextSingleStep");
            addExtern<DAS_BIND_FUN(debuggerStackWalk)>(*this, lib, "stackwalk",
                SideEffects::modifyExternal, "debuggerStackWalk");
            // data walker
            addExtern<DAS_BIND_FUN(makeDataWalker)>(*this, lib,  "make_data_walker",
                SideEffects::modifyExternal, "makeDataWalker");
            addExtern<DAS_BIND_FUN(dapiWalkData)>(*this, lib,  "walk_data",
                SideEffects::modifyExternal, "dapiWalkData");
            addExtern<DAS_BIND_FUN(dapiWalkDataV)>(*this, lib,  "walk_data",
                SideEffects::modifyExternal, "dapiWalkDataV");
            // add builtin module
            compileBuiltinModule("debugger.das",debugger_das,sizeof(debugger_das));
            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_debugger.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Debugger,das);

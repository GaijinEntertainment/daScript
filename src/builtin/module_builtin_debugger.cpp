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
MAKE_TYPE_FACTORY(StackWalker,StackWalker)
MAKE_TYPE_FACTORY(Prologue,Prologue)

namespace das
{
    struct PrologueAnnotation : ManagedStructureAnnotation<Prologue,false> {
        PrologueAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Prologue", ml) {
            addField<DAS_BIND_MANAGED_FIELD(info)>("info");
            addField<DAS_BIND_MANAGED_FIELD(block)>("_block");
            addField<DAS_BIND_MANAGED_FIELD(fileName)>("fileName");
            addField<DAS_BIND_MANAGED_FIELD(stackSize)>("stackSize");
            addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            addField<DAS_BIND_MANAGED_FIELD(cmres)>("cmres");
            addField<DAS_BIND_MANAGED_FIELD(line)>("line");
        }
    };

// we declare Dapi version of each structure in debugger.das
// to fake the API we need C++ version, which we now make locally
namespace debugapi {
    typedef Array DapiArray;
    typedef Table DapiTable;
    typedef Lambda DapiLambda;
    typedef Sequence DapiSequence;
    typedef Block DapiBlock;
    typedef Func DapiFunc;
}

#include "debugapi_gen.inc"

    struct DebugAgentAdapter : DebugAgent, DapiDebugAgent_Adapter {
        DebugAgentAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : DapiDebugAgent_Adapter(info), classPtr(pClass), context(ctx) {
       }
        virtual void onInstall ( DebugAgent * agent ) override {
            if ( auto fnOnInstall = get_onInstall(classPtr) ) {
                context->lock();
                invoke_onInstall(context,fnOnInstall,classPtr,agent);
                context->unlock();
            }
        }
        virtual void onUninstall ( DebugAgent * agent ) override {
            if ( auto fnOnUninstall = get_onUninstall(classPtr) ) {
                context->lock();
                invoke_onUninstall(context,fnOnUninstall,classPtr,agent);
                context->unlock();
            }
        }
        virtual void onCreateContext ( Context * ctx ) override {
            if ( auto fnOnCreateContext = get_onCreateContext(classPtr)) {
                context->lock();
                invoke_onCreateContext(context,fnOnCreateContext,classPtr,*ctx);
                context->unlock();
            }
        }
        virtual void onDestroyContext ( Context * ctx ) override {
            if ( auto fnOnDestroyContext = get_onDestroyContext(classPtr) ) {
                context->lock();
                invoke_onDestroyContext(context,fnOnDestroyContext,classPtr,*ctx);
                context->unlock();
            }
        }
        virtual void onSingleStep ( Context * ctx, const LineInfo & at ) override {
            if ( auto fnOnSingleStep = get_onSingleStep(classPtr) ) {
                context->lock();
                invoke_onSingleStep(context,fnOnSingleStep,classPtr,*ctx,at);
                context->unlock();
            }
        }
        virtual void onInstrument ( Context * ctx, const LineInfo & at ) override {
            if ( ctx==context ) return; // do not step into the same context
            if ( auto fnOnInstrument = get_onInstrument(classPtr) ) {
                context->lock();
                invoke_onInstrument(context,fnOnInstrument,classPtr,*ctx,at);
                context->unlock();
            }
        }
        virtual void onInstrumentFunction ( Context * ctx, SimFunction * sim, bool entering ) override {
            if ( ctx==context ) return;  // do not step into the same context
            if ( auto fnOnInstrumentFunction = get_onInstrumentFunction(classPtr) ) {
                context->lock();
                invoke_onInstrumentFunction(context,fnOnInstrumentFunction,classPtr,*ctx,sim,entering);
                context->unlock();
            }
        }
        virtual void onBreakpoint ( Context * ctx, const LineInfo & at, const char * reason, const char * text ) override {
            if ( auto fnOnBreakpoint = get_onBreakpoint(classPtr) ) {
                context->lock();
                invoke_onBreakpoint(context,fnOnBreakpoint,classPtr,*ctx,at,(char *)reason,(char *)text);
                context->unlock();
            }
        }
        virtual void onVariable ( Context * ctx, const char * category, const char * name, TypeInfo * info, void * data ) override {
            if ( auto fnOnVariable = get_onVariable(classPtr) ) {
                context->lock();
                invoke_onVariable(context,fnOnVariable,classPtr,*ctx,(char *)category,(char *)name,*info,data);
                context->unlock();
            }
        }
        virtual void onTick () override {
            if ( auto fnOnTick = get_onTick(classPtr) ) {
                context->lock();
                invoke_onTick(context,fnOnTick,classPtr);
                context->unlock();
            }
        }
        virtual void onCollect ( Context * ctx ) override {
            if ( auto fnOnCollect = get_onCollect(classPtr) ) {
                context->lock();
                invoke_onCollect(context,fnOnCollect,classPtr,*ctx);
                context->unlock();
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstDebugAgentAnnotation : ManagedStructureAnnotation<DebugAgent,false,true> {
        AstDebugAgentAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("DebugAgent", ml) {
        }
    };

    struct DataWalkerAdapter : DataWalker, DapiDataWalker_Adapter {
        DataWalkerAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : DapiDataWalker_Adapter(info), classPtr(pClass) {
            context = ctx;
        }
    // data structures
        virtual bool canVisitHandle ( char * ps, TypeInfo * ti ) override {
            if ( auto fn_canVisitHandle = get_canVisitHandle(classPtr) ) {
                return invoke_canVisitHandle(context,fn_canVisitHandle,classPtr,ps,*ti);
            } else {
                return true;
            }
        }
        virtual bool canVisitStructure ( char * ps, StructInfo * si ) override {
            if ( auto fn_canVisitStructure = get_canVisitStructure(classPtr) ) {
                return invoke_canVisitStructure(context,fn_canVisitStructure,classPtr,ps,*si);
            } else {
                return true;
            }
        }
        virtual void beforeStructure ( char * ps, StructInfo * si ) override {
            if ( auto fn_beforeStructure = get_beforeStructure(classPtr) ) {
                invoke_beforeStructure(context,fn_beforeStructure,classPtr,ps,*si);
            }
        }
        virtual void afterStructure ( char * ps, StructInfo * si ) override {
            if ( auto fn_afterStructure = get_afterStructure(classPtr) ) {
                invoke_afterStructure(context,fn_afterStructure,classPtr,ps,*si);
            }
        }
        virtual void afterStructureCancel ( char * ps, StructInfo * si ) override {
            if ( auto fn_afterStructureCancel = get_afterStructureCancel(classPtr) ) {
                invoke_afterStructureCancel(context,fn_afterStructureCancel,classPtr,ps,*si);
            }
        }
        virtual void beforeStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            if ( auto fn_beforeStructureField = get_beforeStructureField(classPtr) ) {
                invoke_beforeStructureField(context,fn_beforeStructureField,classPtr,ps,*si,pv,*vi,last);
            }
        }
        virtual void afterStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            if ( auto fn_afterStructureField = get_afterStructureField(classPtr) ) {
                invoke_afterStructureField(context,fn_afterStructureField,classPtr,ps,*si,pv,*vi,last);
            }
        }
        virtual void beforeTuple ( char * ps, TypeInfo * ti ) override {
            if ( auto fn_beforeTuple = get_beforeTuple(classPtr) ) {
                invoke_beforeTuple(context,fn_beforeTuple,classPtr,ps,*ti);
            }
        }
        virtual void afterTuple ( char * ps, TypeInfo * ti ) override {
            if ( auto fn_afterTuple = get_afterTuple(classPtr) ) {
                invoke_afterTuple(context,fn_afterTuple,classPtr,ps,*ti);
            }
        }
        virtual void beforeTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            if ( auto fn_beforeTupleEntry = get_beforeTupleEntry(classPtr) ) {
                invoke_beforeTupleEntry(context,fn_beforeTupleEntry,classPtr,ps,*ti,pv,*vi,last);
            }
        }
        virtual void afterTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            if ( auto fn_afterTupleEntry = get_afterTupleEntry(classPtr) ) {
                invoke_afterTupleEntry(context,fn_afterTupleEntry,classPtr,ps,*ti,pv,*vi,last);
            }
        }
        virtual void beforeVariant ( char * ps, TypeInfo * ti ) override {
            if ( auto fn_beforeVariant = get_beforeVariant(classPtr) ) {
                invoke_beforeVariant(context,fn_beforeVariant,classPtr,ps,*ti);
            }
        }
        virtual void afterVariant ( char * ps, TypeInfo * ti ) override {
            if ( auto fn_afterVariant = get_afterVariant(classPtr) ) {
                invoke_afterVariant(context,fn_afterVariant,classPtr,ps,*ti);
            }
        }
        virtual void beforeArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) override {
            if ( auto fn_beforeArrayData = get_beforeArrayData(classPtr) ) {
                invoke_beforeArrayData(context,fn_beforeArrayData,classPtr,pa,stride,count,*ti);
            }
        }
        virtual void afterArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) override {
            if ( auto fn_afterArrayData = get_afterArrayData(classPtr) ) {
                invoke_afterArrayData(context,fn_afterArrayData,classPtr,pa,stride,count,*ti);
            }
        }
        virtual void beforeArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            if ( auto fn_beforeArrayElement = get_beforeArrayElement(classPtr) ) {
                invoke_beforeArrayElement(context,fn_beforeArrayElement,classPtr,pa,*ti,pe,index,last);
            }
        }
        virtual void afterArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            if ( auto fn_afterArrayElement = get_afterArrayElement(classPtr) ) {
                invoke_afterArrayElement(context,fn_afterArrayElement,classPtr,pa,*ti,pe,index,last);
            }
        }
        virtual void beforeDim ( char * pa, TypeInfo * ti ) override {
            if ( auto fn_beforeDim = get_beforeDim(classPtr) ) {
                invoke_beforeDim(context,fn_beforeDim,classPtr,pa,*ti);
            }
        }
        virtual void afterDim ( char * pa, TypeInfo * ti ) override {
            if ( auto fn_afterDim = get_afterDim(classPtr) ) {
                invoke_afterDim(context,fn_afterDim,classPtr,pa,*ti);
            }
        }
        virtual void beforeArray ( Array * pa, TypeInfo * ti ) override {
            if ( auto fn_beforeArray = get_beforeArray(classPtr) ) {
                invoke_beforeArray(context,fn_beforeArray,classPtr,*pa,*ti);
            }
        }
        virtual void afterArray ( Array * pa, TypeInfo * ti ) override {
            if ( auto fn_afterArray = get_afterArray(classPtr) ) {
                invoke_afterArray(context,fn_afterArray,classPtr,*pa,*ti);
            }
        }
        virtual void beforeTable ( Table * pa, TypeInfo * ti ) override {
           if ( auto fn_beforeTable = get_beforeTable(classPtr) ) {
                invoke_beforeTable(context,fn_beforeTable,classPtr,*pa,*ti);
            }
        }
        virtual void beforeTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
           if ( auto fn_beforeTableKey = get_beforeTableKey(classPtr) ) {
                invoke_beforeTableKey(context,fn_beforeTableKey,classPtr,*pa,*ti,pk,*ki,index,last);
            }
        }
        virtual void afterTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
           if ( auto fn_afterTableKey = get_afterTableKey(classPtr) ) {
                invoke_afterTableKey(context,fn_afterTableKey,classPtr,*pa,*ti,pk,*ki,index,last);
            }
        }
        virtual void beforeTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
           if ( auto fn_beforeTableValue = get_beforeTableValue(classPtr) ) {
                invoke_beforeTableValue(context,fn_beforeTableValue,classPtr,*pa,*ti,pv,*kv,index,last);
            }
        }
        virtual void afterTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
           if ( auto fn_afterTableValue = get_afterTableValue(classPtr) ) {
                invoke_afterTableValue(context,fn_afterTableValue,classPtr,*pa,*ti,pv,*kv,index,last);
            }
        }
        virtual void afterTable ( Table * pa, TypeInfo * ti ) override {
           if ( auto fn_afterTable = get_afterTable(classPtr) ) {
                invoke_afterTable(context,fn_afterTable,classPtr,*pa,*ti);
            }
        }
        virtual void beforeRef ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_beforeRef = get_beforeRef(classPtr) ) {
                invoke_beforeRef(context,fn_beforeRef,classPtr,pa,*ti);
            }
        }
        virtual void afterRef ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_afterRef = get_afterRef(classPtr) ) {
                invoke_afterRef(context,fn_afterRef,classPtr,pa,*ti);
            }
        }
        virtual void beforePtr ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_beforePtr = get_beforePtr(classPtr) ) {
                invoke_beforePtr(context,fn_beforePtr,classPtr,pa,*ti);
            }
        }
        virtual void afterPtr ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_afterPtr = get_afterPtr(classPtr) ) {
                invoke_afterPtr(context,fn_afterPtr,classPtr,pa,*ti);
            }
        }
        virtual void beforeHandle ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_beforeHandle = get_beforeHandle(classPtr) ) {
                invoke_beforeHandle(context,fn_beforeHandle,classPtr,pa,*ti);
            }
        }
        virtual void afterHandle ( char * pa, TypeInfo * ti ) override {
           if ( auto fn_afterHandle = get_afterHandle(classPtr) ) {
                invoke_afterHandle(context,fn_afterHandle,classPtr,pa,*ti);
            }
        }
        virtual void beforeLambda ( Lambda * pa, TypeInfo * ti ) override {
           if ( auto fn_beforeLambda = get_beforeLambda(classPtr) ) {
                invoke_beforeLambda(context,fn_beforeLambda,classPtr,*pa,*ti);
            }
        }
        virtual void afterLambda ( Lambda * pa, TypeInfo * ti ) override {
           if ( auto fn_afterLambda = get_afterLambda(classPtr) ) {
                invoke_afterLambda(context,fn_afterLambda,classPtr,*pa,*ti);
            }
        }
        virtual void beforeIterator ( Sequence * pa, TypeInfo * ti ) override {
           if ( auto fn_beforeIterator = get_beforeIterator(classPtr) ) {
                invoke_beforeIterator(context,fn_beforeIterator,classPtr,*pa,*ti);
            }
        }
        virtual void afterIterator ( Sequence * pa, TypeInfo * ti ) override {
           if ( auto fn_afterIterator = get_afterIterator(classPtr) ) {
                invoke_afterIterator(context,fn_afterIterator,classPtr,*pa,*ti);
            }
        }
    // types
        virtual void Null ( TypeInfo * ti ) override {
           if ( auto fn_Null = get_Null(classPtr) ) {
                invoke_Null(context,fn_Null,classPtr,*ti);
            }
        }
        virtual void VoidPtr ( void * & value ) override {
           if ( auto fn_VoidPtr = get_VoidPtr(classPtr) ) {
                invoke_VoidPtr(context,fn_VoidPtr,classPtr,value);
            }
        }
        virtual void Bool ( bool & value ) override {
           if ( auto fn_Bool = get_Bool(classPtr) ) {
                invoke_Bool(context,fn_Bool,classPtr,value);
            }
        }
        virtual void Int8 ( int8_t & value ) override {
           if ( auto fn_Int8 = get_Int8(classPtr) ) {
                invoke_Int8(context,fn_Int8,classPtr,value);
            }
        }
        virtual void UInt8 ( uint8_t & value ) override {
           if ( auto fn_UInt8 = get_UInt8(classPtr) ) {
                invoke_UInt8(context,fn_UInt8,classPtr,value);
            }
        }
        virtual void Int16 ( int16_t & value ) override {
           if ( auto fn_Int16 = get_Int16(classPtr) ) {
                invoke_Int16(context,fn_Int16,classPtr,value);
            }
        }
        virtual void UInt16 ( uint16_t & value ) override {
           if ( auto fn_UInt16 = get_UInt16(classPtr) ) {
                invoke_UInt16(context,fn_UInt16,classPtr,value);
            }
        }
        virtual void Int64 ( int64_t & value ) override {
           if ( auto fn_Int64 = get_Int64(classPtr) ) {
                invoke_Int64(context,fn_Int64,classPtr,value);
            }
        }
        virtual void UInt64 ( uint64_t & value ) override {
           if ( auto fn_UInt64 = get_UInt64(classPtr) ) {
                invoke_UInt64(context,fn_UInt64,classPtr,value);
            }
        }
        virtual void String ( char * & value ) override {
           if ( auto fn_String = get_String(classPtr) ) {
                invoke_String(context,fn_String,classPtr,value);
            }
        }
        virtual void Double ( double & value ) override {
           if ( auto fn_Double = get_Double(classPtr) ) {
                invoke_Double(context,fn_Double,classPtr,value);
            }
        }
        virtual void Float ( float & value ) override {
           if ( auto fn_Float = get_Float(classPtr) ) {
                invoke_Float(context,fn_Float,classPtr,value);
            }
        }
        virtual void Int ( int32_t & value ) override {
           if ( auto fn_Int = get_Int(classPtr) ) {
                invoke_Int(context,fn_Int,classPtr,value);
            }
        }
        virtual void UInt ( uint32_t & value ) override {
           if ( auto fn_UInt = get_UInt(classPtr) ) {
                invoke_UInt(context,fn_UInt,classPtr,value);
            }
        }
        virtual void Bitfield ( uint32_t & value, TypeInfo * ti ) override {
           if ( auto fn_Bitfield = get_Bitfield(classPtr) ) {
                invoke_Bitfield(context,fn_Bitfield,classPtr,value,*ti);
            }
        }
        virtual void Int2 ( int2 & value ) override {
           if ( auto fn_Int2 = get_Int2(classPtr) ) {
                invoke_Int2(context,fn_Int2,classPtr,value);
            }
        }
        virtual void Int3 ( int3 & value ) override {
           if ( auto fn_Int3 = get_Int3(classPtr) ) {
                invoke_Int3(context,fn_Int3,classPtr,value);
            }
        }
        virtual void Int4 ( int4 & value ) override {
           if ( auto fn_Int4 = get_Int4(classPtr) ) {
                invoke_Int4(context,fn_Int4,classPtr,value);
            }
        }
        virtual void UInt2 ( uint2 & value ) override {
           if ( auto fn_UInt2 = get_UInt2(classPtr) ) {
                invoke_UInt2(context,fn_UInt2,classPtr,value);
            }
        }
        virtual void UInt3 ( uint3 & value ) override {
           if ( auto fn_UInt3 = get_UInt3(classPtr) ) {
                invoke_UInt3(context,fn_UInt3,classPtr,value);
            }
        }
        virtual void UInt4 ( uint4 & value ) override {
           if ( auto fn_UInt4 = get_UInt4(classPtr) ) {
                invoke_UInt4(context,fn_UInt4,classPtr,value);
            }
        }
        virtual void Float2 ( float2 & value ) override {
           if ( auto fn_Float2 = get_Float2(classPtr) ) {
                invoke_Float2(context,fn_Float2,classPtr,value);
            }
        }
        virtual void Float3 ( float3 & value ) override {
           if ( auto fn_Float3 = get_Float3(classPtr) ) {
                invoke_Float3(context,fn_Float3,classPtr,value);
            }
        }
        virtual void Float4 ( float4 & value ) override {
           if ( auto fn_Float4 = get_Float4(classPtr) ) {
                invoke_Float4(context,fn_Float4,classPtr,value);
            }
        }
        virtual void Range ( range & value ) override {
           if ( auto fn_Range = get_Range(classPtr) ) {
                invoke_Range(context,fn_Range,classPtr,value);
            }
        }
        virtual void URange ( urange & value ) override {
           if ( auto fn_URange = get_URange(classPtr) ) {
                invoke_URange(context,fn_URange,classPtr,value);
            }
        }
        virtual void WalkBlock ( Block * value ) override {
           if ( auto fn_WalkBlock = get_WalkBlock(classPtr) ) {
                invoke_WalkBlock(context,fn_WalkBlock,classPtr,*value);
            }
        }
        virtual void WalkFunction ( Func * value ) override {
           if ( auto fn_WalkFunction = get_WalkFunction(classPtr) ) {
                invoke_WalkFunction(context,fn_WalkFunction,classPtr,*value);
            }
        }
        virtual void WalkEnumeration ( int32_t & value, EnumInfo * ei ) override {
           if ( auto fn_WalkEnumeration = get_WalkEnumeration(classPtr) ) {
                invoke_WalkEnumeration(context,fn_WalkEnumeration,classPtr,value,*ei);
            }
        }
        virtual void WalkEnumeration8  ( int8_t & value, EnumInfo * ei ) override {
           if ( auto fn_WalkEnumeration8 = get_WalkEnumeration8(classPtr) ) {
                invoke_WalkEnumeration8(context,fn_WalkEnumeration8,classPtr,value,*ei);
            }
        }
        virtual void WalkEnumeration16 ( int16_t & value, EnumInfo * ei ) override {
           if ( auto fn_WalkEnumeration16 = get_WalkEnumeration16(classPtr) ) {
                invoke_WalkEnumeration16(context,fn_WalkEnumeration16,classPtr,value,*ei);
            }
        }
        virtual void FakeContext ( Context * value ) override {
           if ( auto fn_FakeContext = get_FakeContext(classPtr) ) {
                invoke_FakeContext(context,fn_FakeContext,classPtr,*value);
            }
        }
    protected:
        void *      classPtr;
    };

    struct AstDataWalkerAnnotation : ManagedStructureAnnotation<DataWalker,false,true> {
        AstDataWalkerAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("DataWalker", ml) {
        }
    };

    class StackWalkerAdapter : public StackWalker, DapiStackWalker_Adapter {
    public:
        StackWalkerAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : DapiStackWalker_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool canWalkArguments () override {
            if ( auto fnCanWalkArguments = get_canWalkArguments(classPtr) ) {
                return invoke_canWalkArguments(context,fnCanWalkArguments,classPtr);
            } else {
                return true;
            }
        }
        virtual bool canWalkVariables () override {
            if ( auto fnCanWalkVariables = get_canWalkVariables(classPtr) ) {
                return invoke_canWalkArguments(context,fnCanWalkVariables,classPtr);
            } else {
                return true;
            }
        }
        virtual bool canWalkOutOfScopeVariables() override {
            if ( auto fnCanWalkOutOfScopeVariables = get_canWalkOutOfScopeVariables(classPtr) ) {
                return invoke_canWalkOutOfScopeVariables(context,fnCanWalkOutOfScopeVariables,classPtr);
            } else {
                return true;
            }
        }
        virtual void onBeforeCall ( Prologue * pp, char * sp ) override {
            if ( auto fnOnBeforeCall = get_onBeforeCall(classPtr) ) {
                invoke_onBeforeCall(context, fnOnBeforeCall, classPtr, *pp, sp);
            }
        }
        virtual void onCallAOT ( Prologue * pp, const char * fileName ) override {
            if ( auto fnOnCallAOT = get_onCallAOT(classPtr) ) {
                invoke_onCallAOT(context, fnOnCallAOT, classPtr, *pp, (char *)fileName);
            }
        }
        virtual void onCallAt ( Prologue * pp, FuncInfo * info, LineInfo * at ) override {
            if ( auto fnOnCallAt = get_onCallAt(classPtr) ) {
                invoke_onCallAt(context, fnOnCallAt, classPtr, *pp, *info, *at);
            }
        }
        virtual void onCall ( Prologue * pp, FuncInfo * info ) override {
            if ( auto fnOnCall = get_onCall(classPtr) ) {
                invoke_onCall(context, fnOnCall, classPtr, *pp, *info);
            }
        }
        virtual void onAfterPrologue ( Prologue * pp, char * sp ) override {
            if ( auto fnOnAfterPrologue = get_onAfterPrologue(classPtr) ) {
                invoke_onAfterPrologue(context, fnOnAfterPrologue, classPtr, *pp, sp);
            }
        }
        virtual void onArgument ( FuncInfo * info, int index, VarInfo * vinfo, vec4f arg ) override {
            if ( auto fnOnArgument = get_onArgument(classPtr) ) {
                invoke_onArgument(context, fnOnArgument, classPtr, *info, index, *vinfo, arg);
            }
        }
        virtual void onBeforeVariables ( ) override {
            if ( auto fnOnBeforeVariables = get_onBeforeVariables(classPtr) ) {
                invoke_onBeforeVariables(context, fnOnBeforeVariables, classPtr);
            }
        }
        virtual void onVariable ( FuncInfo * info, LocalVariableInfo * vinfo, void * addr, bool inScope ) override {
            if ( auto fnOnVariable = get_onVariable(classPtr) ) {
                invoke_onVariable(context, fnOnVariable, classPtr, *info, *vinfo, addr, inScope);
            }
        }
        virtual bool onAfterCall ( Prologue * pp ) override {
            if ( auto fnOnAfterCall = get_onAfterCall(classPtr) ) {
                return invoke_onAfterCall(context,fnOnAfterCall,classPtr,*pp);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstStackWalkerAnnotation : ManagedStructureAnnotation<StackWalker,false,true> {
        AstStackWalkerAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("StackWalker", ml) {
        }
    };


    #include "debugger.das.inc"

    StackWalkerPtr makeStackWalker ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<StackWalkerAdapter>((char *)pClass,info,context);
    }

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

    int32_t dapiStackDepth ( Context & context ) {
    #if DAS_ENABLE_STACK_WALK
        char * sp = context.stack.ap();
        int32_t depth = 0;
        while (  sp < context.stack.top() ) {
            Prologue * pp = (Prologue *) sp;
            Block * block = nullptr;
            FuncInfo * info = nullptr;
            char * SP = sp;
            if ( pp->info ) {
                intptr_t iblock = intptr_t(pp->block);
                if ( iblock & 1 ) {
                    block = (Block *) (iblock & ~1);
                    info = block->info;
                    SP = context.stack.bottom() + block->stackOffset;
                } else {
                    info = pp->info;
                }
            }
            sp += info ? info->stackSize : pp->stackSize;
            depth ++;
        }
        return depth;
    #else
        context.throw_error("stack walking disabled");
        return 0;
    #endif
    }

    void dapiStackWalk ( StackWalkerPtr walker, Context & context, const LineInfo & at ) {
    #if DAS_ENABLE_STACK_WALK
        char * sp = context.stack.ap();
        const LineInfo * lineAt = &at;
        while (  sp < context.stack.top() ) {
            Prologue * pp = (Prologue *) sp;
            Block * block = nullptr;
            FuncInfo * info = nullptr;
            char * SP = sp;
            if ( pp->info ) {
                intptr_t iblock = intptr_t(pp->block);
                if ( iblock & 1 ) {
                    block = (Block *) (iblock & ~1);
                    info = block->info;
                    SP = context.stack.bottom() + block->stackOffset;
                } else {
                    info = pp->info;
                }
            }
            walker->onBeforeCall(pp,SP);
            if ( !info ) {
                walker->onCallAOT(pp,pp->fileName);
            } else if ( pp->line ) {
                walker->onCallAt(pp,info,pp->line);
            } else {
                walker->onCall(pp,info);
            }
            if ( info ) {
                if ( walker->canWalkArguments() ) {
                    for ( uint32_t i = 0; i != info->count; ++i ) {
                        walker->onArgument(info, i, info->fields[i], pp->arguments[i]);
                    }
                }
            }
            walker->onAfterPrologue(pp,SP);
            if ( info && info->locals ) {
                if ( walker->canWalkVariables() ) {
                    walker->onBeforeVariables();
                    for ( uint32_t i = 0; i != info->localCount; ++i ) {
                        auto lv = info->locals[i];
                        bool inScope = lineAt ? lineAt->inside(lv->visibility) : false;
                        if ( !walker->canWalkOutOfScopeVariables() && !inScope ) {
                            continue;
                        }
                        char * addr = nullptr;
                        if ( !inScope ) {
                            addr = nullptr;
                        } else if ( lv->cmres ) {
                            addr = (char *)pp->cmres;
                        } else if ( lv->isRefValue( ) ) {
                            addr = SP + lv->stackTop;
                        } else {
                            addr = SP + lv->stackTop;
                        }
                        walker->onVariable(info, lv, addr, inScope);
                    }
                }
            }
            lineAt = info ? pp->line : nullptr;
            sp += info ? info->stackSize : pp->stackSize;
            if ( !walker->onAfterCall(pp) ) break;
        }
    #else
        context.throw_error("stack walking disabled");
    #endif
    }

    // pinvoke(context,"function",....)

    vec4f pinvoke_impl ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        auto invCtx = cast<Context *>::to(args[0]);
        if ( !invCtx ) context.throw_error_at(call->debugInfo, "pinvoke with null context");
        auto fn = cast<const char *>::to(args[1]);
        if ( !fn ) context.throw_error_at(call->debugInfo, "can't pinvoke empty string");
        auto simFn = invCtx->findFunction(fn);
        if ( !simFn ) context.throw_error_at(call->debugInfo, "pinvoke can't find %s function", fn);
        if ( simFn->debugInfo->flags & FuncInfo::flag_private ) {
            context.throw_error_at(call->debugInfo, "pinvoke can't invoke private function %s", simFn->mangledName);
        }
        invCtx->lock();
        vec4f res;
        invCtx->exception = nullptr;
        invCtx->runWithCatch([&](){
            if ( !invCtx->ownStack ) {
                StackAllocator sharedStack(8*1024);
                SharedStackGuard guard(*invCtx, sharedStack);
                res = invCtx->callOrFastcall(simFn, args+2, &call->debugInfo);
            } else {
                res = invCtx->callOrFastcall(simFn, args+2, &call->debugInfo);
            }
        });
        invCtx->unlock();
        if ( invCtx->exception ) context.throw_error_at(invCtx->exceptionAt, invCtx->exception);
        return res;
    }

    vec4f pinvoke_impl2 ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        auto invCtx = cast<Context *>::to(args[0]);
        if ( !invCtx ) context.throw_error_at(call->debugInfo, "pinvoke with null context");
        auto fn = cast<Func>::to(args[1]);
        if ( !fn.PTR ) context.throw_error_at(call->debugInfo, "pnvoke can't invoke null function");
        auto simFn = fn.PTR;
        if ( !simFn ) context.throw_error_at(call->debugInfo, "pinvoke can't find function #%p", (void *)simFn);
        if ( simFn->debugInfo->flags & FuncInfo::flag_private ) {
            context.throw_error_at(call->debugInfo, "pinvoke can't invoke private function %s", simFn->mangledName);
        }
        invCtx->lock();
        vec4f res;
        invCtx->exception = nullptr;
        invCtx->runWithCatch([&](){
            if ( !invCtx->ownStack ) {
                StackAllocator sharedStack(8*1024);
                SharedStackGuard guard(*invCtx, sharedStack);
                res = invCtx->callOrFastcall(simFn, args+2, &call->debugInfo);
            } else {
                res = invCtx->callOrFastcall(simFn, args+2, &call->debugInfo);
            }
        });
        invCtx->unlock();
        if ( invCtx->exception ) context.throw_error_at(invCtx->exceptionAt, invCtx->exception);
        return res;
    }

    vec4f pinvoke_impl3 ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        auto invCtx = cast<Context *>::to(args[0]);
        if ( !invCtx ) context.throw_error_at(call->debugInfo, "pinvoke with null context");
        auto fn = cast<Lambda>::to(args[1]);
        SimFunction ** fnMnh = (SimFunction **)fn.capture;
        if (!fnMnh) context.throw_error_at(call->debugInfo, "invoke null lambda");
        SimFunction * simFn = *fnMnh;
        if ( !simFn ) context.throw_error_at(call->debugInfo, "pinvoke can't find function #%p", (void*)simFn);
        if ( simFn->debugInfo->flags & FuncInfo::flag_private ) {
            context.throw_error_at(call->debugInfo, "pinvoke can't invoke private function %s", simFn->mangledName);
        }
        invCtx->lock();
        vec4f res;
        invCtx->exception = nullptr;
        invCtx->runWithCatch([&](){
            if ( !invCtx->ownStack ) {
                StackAllocator sharedStack(8*1024);
                SharedStackGuard guard(*invCtx, sharedStack);
                res = invCtx->callOrFastcall(simFn, args+1, &call->debugInfo);
            } else {
                res = invCtx->callOrFastcall(simFn, args+1, &call->debugInfo);
            }
        });
        invCtx->unlock();
        if ( invCtx->exception ) context.throw_error_at(invCtx->exceptionAt, invCtx->exception);
        return res;
    }

    vec4f get_global_variable ( Context & context, SimNode_CallBase * node, vec4f * args ) {
        auto ctx = cast<Context *>::to(args[0]);
        if ( ctx==nullptr ) context.throw_error_at(node->debugInfo, "expecting context pointer");
        auto name = cast<char *>::to(args[1]);
        auto vidx = ctx->findVariable(name);
        return cast<void *>::from(ctx->getVariable(vidx));
    }

    void instrument_context ( Context & ctx, bool isInstrumenting, const TBlock<bool,LineInfo> & blk, Context * context, LineInfoArg * line ) {
        ctx.instrumentContextNode(blk, isInstrumenting, context, line);
    }

    void instrument_function ( Context & ctx, Func fn, bool isInstrumenting, Context * context, LineInfoArg * arg ) {
        if ( !fn ) context->throw_error_at(*arg, "expecting function");
        ctx.instrumentFunction(fn.PTR, isInstrumenting);
    }

    void instrument_all_functions ( Context & ctx ) {
        ctx.instrumentFunction(0, true);
    }

    void clear_instruments ( Context & ctx ) {
        ctx.clearInstruments();
    }

    bool has_function ( Context & ctx, const char * name ) {
        return ctx.findFunction(name ? name : "") != nullptr;
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
            addAnnotation(make_smart<PrologueAnnotation>(lib));
            addAnnotation(make_smart<AstDebugAgentAnnotation>(lib));
            addAnnotation(make_smart<AstDataWalkerAnnotation>(lib));
            addAnnotation(make_smart<AstStackWalkerAnnotation>(lib));
            // debug agent
            addExtern<DAS_BIND_FUN(makeDebugAgent)>(*this, lib,  "make_debug_agent",
                SideEffects::modifyExternal, "makeDebugAgent")
                    ->args({"class","info","context"});
            addExtern<DAS_BIND_FUN(tickDebugAgent)>(*this, lib,  "tick_debug_agent",
                SideEffects::modifyExternal, "tickDebugAgent");
            addExtern<DAS_BIND_FUN(tickSpecificDebugAgent)>(*this, lib,  "tick_debug_agent",
                SideEffects::modifyExternal, "tickSpecificDebugAgent")
                    ->arg("agent");
            addExtern<DAS_BIND_FUN(collectDebugAgentState)>(*this, lib,  "collect_debug_agent_state",
                SideEffects::modifyExternal, "collectDebugAgentState")
                    ->arg("context");
            addExtern<DAS_BIND_FUN(installDebugAgent)>(*this, lib,  "install_debug_agent",
                SideEffects::modifyExternal, "installDebugAgent")
                    ->args({"agent","category","line","context"});
            addExtern<DAS_BIND_FUN(getDebugAgentContext), SimNode_ExtFuncCallRef>(*this, lib,  "get_debug_agent_context",
                SideEffects::modifyExternal, "getDebugAgentContext")
                    ->args({"category","line","context"});
            addExtern<DAS_BIND_FUN(hasDebugAgentContext)>(*this, lib,  "has_debug_agent_context",
                SideEffects::modifyExternal, "hasDebugAgentContext")
                    ->args({"category","line","context"});;
            addExtern<DAS_BIND_FUN(forkDebugAgentContext)>(*this, lib,  "fork_debug_agent_context",
                SideEffects::modifyExternal, "forkDebugAgentContext")
                    ->args({"function","context","line"});;
            addExtern<DAS_BIND_FUN(isInDebugAgentCreation)>(*this, lib, "is_in_debug_agent_creation",
                SideEffects::accessExternal, "isInDebugAgentCreation");
            addExtern<DAS_BIND_FUN(debuggerSetContextSingleStep)>(*this, lib,  "set_single_step",
                SideEffects::modifyExternal, "debuggerSetContextSingleStep")
                    ->args({"context","enabled"});
            addExtern<DAS_BIND_FUN(debuggerStackWalk)>(*this, lib, "stackwalk",
                SideEffects::modifyExternal, "debuggerStackWalk")
                    ->args({"context","line"});
            addExtern<DAS_BIND_FUN(dapiReportContextState)>(*this, lib, "report_context_state",
                SideEffects::modifyExternal, "dapiReportContextState")
                    ->args({"context","category","name","info","data"});
            // instrumentation
            addExtern<DAS_BIND_FUN(instrument_context)>(*this, lib,  "instrument_node",
                SideEffects::modifyExternal, "instrument_context")
                    ->args({"context","isInstrumenting","block","context","line"});
            addExtern<DAS_BIND_FUN(instrument_function)>(*this, lib,  "instrument_function",
                SideEffects::modifyExternal, "instrument_function")
                    ->args({"context","function","isInstrumenting","context","line"});;
            addExtern<DAS_BIND_FUN(instrument_all_functions)>(*this, lib,  "instrument_all_functions",
                SideEffects::modifyExternal, "instrument_all_functions")
                    ->arg("context");
            addExtern<DAS_BIND_FUN(clear_instruments)>(*this, lib,  "clear_instruments",
                SideEffects::modifyExternal, "clear_instruments")
                    ->arg("context");
            // data walker
            addExtern<DAS_BIND_FUN(makeDataWalker)>(*this, lib,  "make_data_walker",
                SideEffects::modifyExternal, "makeDataWalker")
                    ->args({"class","info","context"});
            addExtern<DAS_BIND_FUN(dapiWalkData)>(*this, lib,  "walk_data",
                SideEffects::modifyExternal, "dapiWalkData")
                    ->args({"walker","data","info"});
            addExtern<DAS_BIND_FUN(dapiWalkDataV)>(*this, lib,  "walk_data",
                SideEffects::modifyExternal, "dapiWalkDataV")
                    ->args({"walker","data","info"});
            // stack walker
            addExtern<DAS_BIND_FUN(makeStackWalker)>(*this, lib,  "make_stack_walker",
                SideEffects::modifyExternal, "makeStackWalker")
                    ->args({"class","info","context"});
            addExtern<DAS_BIND_FUN(dapiStackWalk)>(*this, lib,  "walk_stack",
                SideEffects::modifyExternal, "dapiStackWalk")
                    ->args({"walker","context","line"});
            addExtern<DAS_BIND_FUN(dapiStackDepth)>(*this, lib,  "stack_depth",
                SideEffects::modifyExternal, "dapiStacDepth")
                    ->arg("context");
            // global variable
            addInterop<get_global_variable,void *,vec4f,const char *>(*this,lib,"get_context_global_variable",
                SideEffects::accessExternal,"get_global_variable")
                    ->args({"context","name"})->unsafeOperation = true;
            // has function
            addExtern<DAS_BIND_FUN(has_function)>(*this, lib,  "has_function",
                SideEffects::none, "has_function")
                    ->args({"context","function_name"});
            // pinvoke
            addInterop<pinvoke_impl,void,vec4f,const char *>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            addInterop<pinvoke_impl,void,vec4f,const char *,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl")->unsafeOperation = true;
            // pinvoke2
            addInterop<pinvoke_impl2,void,vec4f,Func>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            addInterop<pinvoke_impl2,void,vec4f,Func,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl2")->unsafeOperation = true;
            // pinvoke3
            addInterop<pinvoke_impl3,void,vec4f,Lambda>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
            addInterop<pinvoke_impl3,void,vec4f,Lambda,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f,vec4f>(*this,lib,"invoke_in_context",
                SideEffects::worstDefault,"pinvoke_impl3")->unsafeOperation = true;
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

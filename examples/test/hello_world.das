options debugger = true

require rtti
require debugapi

class SampleDebugAgent : DapiDebugAgent
    def override onBreakpoint ( var ctx:Context; at:LineInfo ) : void
        print("{ctx} breakpoint at line {int(at.line)}\n")
        set_single_step(ctx,true)
    def override onSingleStep ( var ctx:Context; at:LineInfo ) : void
        print("{ctx} step at {int(at.line)}\n")
        stackwalk(ctx,at)

[export]
def debug_agent ( ctx : Context )
    install_new_debug_agent(new SampleDebugAgent())

///////
// DAPI

// das::Array. should we bind C++ structure?
struct DapiArray
    data : void?
    size : uint
    capacity : uint
    lock : uint
    flags : bitfield<_shared;_hopeless>

// das::Table. should we bind C++ structure?
struct DapiTable : DapiArray
    keys : void?
    hashes : uint?
    maxLookups : uint
    shift : uint

// das::Block
struct DapiBlock
    stackOffset : uint
    argumentsOffset : uint
    body : void?                    // SimNode *
    aotFunction : void?
    functionArguments : float4?
    info: FuncInfo?

// das::Func
struct DapiFunc
    index : int

// das::Lambda
struct DapiLambda
    capture : void?

// das::Sequence
struct DapiSequence
    iter : void?        // Iterator *

class DapiDataWalker
    def abstract canVisitHandle ( ps:void?; ti:TypeInfo ) : bool
    def abstract canVisitStructure ( ps:void?; si:StructInfo ) : bool
    def abstract beforeStructure ( ps:void?; si:StructInfo ) : void
    def abstract afterStructure ( ps:void?; si:StructInfo ) : void
    def abstract afterStructureCancel ( ps:void?; si:StructInfo ) : void
    def abstract beforeStructureField ( ps:void?; si:StructInfo; pv:void?; vi:VarInfo; last:bool ) : void
    def abstract afterStructureField ( ps:void?; si:StructInfo; pv:void?; vi:VarInfo; last:bool ) : void
    def abstract beforeTuple ( ps:void?; ti:TypeInfo ) : void
    def abstract afterTuple ( ps:void?; ti:TypeInfo ) : void
    def abstract beforeTupleEntry ( ps:void?; ti:TypeInfo; pv:void?; vi:TypeInfo; last:bool ) : void
    def abstract afterTupleEntry ( ps:void?; ti:TypeInfo; pv:void?; vi:TypeInfo; last:bool ) : void
    def abstract beforeVariant ( ps:void?; ti:TypeInfo ) : void
    def abstract afterVariant ( ps:void?; ti:TypeInfo ) : void
    def abstract beforeArrayData ( ps:void?; stride:uint; count:uint; ti:TypeInfo ) : void
    def abstract afterArrayData ( ps:void?; stride:uint; count:uint; ti:TypeInfo ) : void
    def abstract beforeArrayElement ( ps:void?; ti:TypeInfo; pe:void?; index:uint; last:bool ) : void
    def abstract afterArrayElement ( ps:void?; ti:TypeInfo; pe:void?; index:uint; last:bool ) : void
    def abstract beforeDim ( ps:void?; ti:TypeInfo ) : void
    def abstract afterDim ( ps:void?; ti:TypeInfo ) : void
    def abstract beforeArray ( pa:DapiArray; ti:TypeInfo ) : void
    def abstract afterArray ( pa:DapiArray; ti:TypeInfo ) : void
    def abstract beforeTable ( pa:DapiTable; ti:TypeInfo ) : void
    def abstract beforeTableKey ( pa:DapiTable; ti:TypeInfo; pk:void?; ki:TypeInfo; index:uint; last:bool ) : void
    def abstract afterTableKey ( pa:DapiTable; ti:TypeInfo; pk:void?; ki:TypeInfo; index:uint; last:bool ) : void
    def abstract beforeTableValue ( pa:DapiTable; ti:TypeInfo; pv:void?; kv:TypeInfo; index:uint; last:bool ) : void
    def abstract afterTableValue ( pa:DapiTable; ti:TypeInfo; pv:void?; kv:TypeInfo; index:uint; last:bool ) : void
    def abstract afterTable ( pa:DapiTable; ti:TypeInfo ) : void
    def abstract beforeRef ( ps:void?; ti:TypeInfo ) : void
    def abstract afterRef ( ps:void?; ti:TypeInfo ) : void
    def abstract beforePtr ( ps:void?; ti:TypeInfo ) : void
    def abstract afterPtr ( ps:void?; ti:TypeInfo ) : void
    def abstract beforeHandle ( ps:void?; ti:TypeInfo ) : void
    def abstract afterHandle ( ps:void?; ti:TypeInfo ) : void
    def abstract beforeLambda ( var value:DapiLambda; ti:TypeInfo ) : void
    def abstract afterLambda ( var value:DapiLambda; ti:TypeInfo ) : void
    def abstract beforeIterator ( var value:DapiSequence; ti:TypeInfo ) : void
    def abstract afterIterator ( var value:DapiSequence; ti:TypeInfo ) : void
    def abstract Null ( ti:TypeInfo ) : void
    def abstract Bool ( var value:bool & ) : void
    def abstract Int8 ( var value:int8 & ) : void
    def abstract UInt8 ( var value:uint8 & ) : void
    def abstract Int16 ( var value:int16 & ) : void
    def abstract UInt16 ( var value:uint16 & ) : void
    def abstract Int64 ( var value:int64 & ) : void
    def abstract UInt64 ( var value:uint64 & ) : void
    def abstract String ( var value:string & ) : void
    def abstract Double ( var value:double & ) : void
    def abstract Float ( var value:float & ) : void
    def abstract Int ( var value:int & ) : void
    def abstract UInt ( var value:uint & ) : void
    def abstract Bitfield ( var value:uint &; ti:TypeInfo ) : void
    def abstract Int2 ( var value:int2 & ) : void
    def abstract Int3 ( var value:int3 & ) : void
    def abstract Int4 ( var value:int4 & ) : void
    def abstract UInt2 ( var value:uint2 & ) : void
    def abstract UInt3 ( var value:uint3 & ) : void
    def abstract UInt4 ( var value:uint4 & ) : void
    def abstract Float2 ( var value:float2 & ) : void
    def abstract Float3 ( var value:float3 & ) : void
    def abstract Float4 ( var value:float4 & ) : void
    def abstract Range ( var value:range & ) : void
    def abstract URange ( var value:urange & ) : void
    def abstract WalkBlock ( var value:DapiBlock ) : void
    def abstract WalkFunction ( var value:DapiFunc ) : void
    def abstract WalkEnumeration ( var value:int &; ei:EnumInfo ) : void
    def abstract WalkEnumeration8  ( var value:int8 &; ei:EnumInfo ) : void
    def abstract WalkEnumeration16 ( var value:int16 &; ei:EnumInfo ) : void
    def abstract FakeContext ( var value:Context ) : void


[export]
def test
    fork_debug_agent_context(@@debug_agent)
	var x = 5
	breakpoint()
	x = 6
	for t in range(0,10)
		print("x={x} t={t}\n")
		x ++
		x += 3
	return true

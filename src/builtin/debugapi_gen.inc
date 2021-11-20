class DapiDataWalker_Adapter {
protected:
  enum {
    __fn_canVisitHandle = 0,
    __fn_canVisitStructure = 1,
    __fn_beforeStructure = 2,
    __fn_afterStructure = 3,
    __fn_afterStructureCancel = 4,
    __fn_beforeStructureField = 5,
    __fn_afterStructureField = 6,
    __fn_beforeTuple = 7,
    __fn_afterTuple = 8,
    __fn_beforeTupleEntry = 9,
    __fn_afterTupleEntry = 10,
    __fn_beforeVariant = 11,
    __fn_afterVariant = 12,
    __fn_beforeArrayData = 13,
    __fn_afterArrayData = 14,
    __fn_beforeArrayElement = 15,
    __fn_afterArrayElement = 16,
    __fn_beforeDim = 17,
    __fn_afterDim = 18,
    __fn_beforeArray = 19,
    __fn_afterArray = 20,
    __fn_beforeTable = 21,
    __fn_beforeTableKey = 22,
    __fn_afterTableKey = 23,
    __fn_beforeTableValue = 24,
    __fn_afterTableValue = 25,
    __fn_afterTable = 26,
    __fn_beforeRef = 27,
    __fn_afterRef = 28,
    __fn_beforePtr = 29,
    __fn_afterPtr = 30,
    __fn_beforeHandle = 31,
    __fn_afterHandle = 32,
    __fn_beforeLambda = 33,
    __fn_afterLambda = 34,
    __fn_beforeIterator = 35,
    __fn_afterIterator = 36,
    __fn_Null = 37,
    __fn_VoidPtr = 38,
    __fn_Bool = 39,
    __fn_Int8 = 40,
    __fn_UInt8 = 41,
    __fn_Int16 = 42,
    __fn_UInt16 = 43,
    __fn_Int64 = 44,
    __fn_UInt64 = 45,
    __fn_String = 46,
    __fn_Double = 47,
    __fn_Float = 48,
    __fn_Int = 49,
    __fn_UInt = 50,
    __fn_Bitfield = 51,
    __fn_Int2 = 52,
    __fn_Int3 = 53,
    __fn_Int4 = 54,
    __fn_UInt2 = 55,
    __fn_UInt3 = 56,
    __fn_UInt4 = 57,
    __fn_Float2 = 58,
    __fn_Float3 = 59,
    __fn_Float4 = 60,
    __fn_Range = 61,
    __fn_URange = 62,
    __fn_WalkBlock = 63,
    __fn_WalkFunction = 64,
    __fn_WalkEnumeration = 65,
    __fn_WalkEnumeration8 = 66,
    __fn_WalkEnumeration16 = 67,
    __fn_FakeContext = 68,
  };
protected:
  static int _das_class_method_offset[69];
public:
  DapiDataWalker_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_canVisitHandle] = adapt_field_offset("canVisitHandle",info);
      _das_class_method_offset[__fn_canVisitStructure] = adapt_field_offset("canVisitStructure",info);
      _das_class_method_offset[__fn_beforeStructure] = adapt_field_offset("beforeStructure",info);
      _das_class_method_offset[__fn_afterStructure] = adapt_field_offset("afterStructure",info);
      _das_class_method_offset[__fn_afterStructureCancel] = adapt_field_offset("afterStructureCancel",info);
      _das_class_method_offset[__fn_beforeStructureField] = adapt_field_offset("beforeStructureField",info);
      _das_class_method_offset[__fn_afterStructureField] = adapt_field_offset("afterStructureField",info);
      _das_class_method_offset[__fn_beforeTuple] = adapt_field_offset("beforeTuple",info);
      _das_class_method_offset[__fn_afterTuple] = adapt_field_offset("afterTuple",info);
      _das_class_method_offset[__fn_beforeTupleEntry] = adapt_field_offset("beforeTupleEntry",info);
      _das_class_method_offset[__fn_afterTupleEntry] = adapt_field_offset("afterTupleEntry",info);
      _das_class_method_offset[__fn_beforeVariant] = adapt_field_offset("beforeVariant",info);
      _das_class_method_offset[__fn_afterVariant] = adapt_field_offset("afterVariant",info);
      _das_class_method_offset[__fn_beforeArrayData] = adapt_field_offset("beforeArrayData",info);
      _das_class_method_offset[__fn_afterArrayData] = adapt_field_offset("afterArrayData",info);
      _das_class_method_offset[__fn_beforeArrayElement] = adapt_field_offset("beforeArrayElement",info);
      _das_class_method_offset[__fn_afterArrayElement] = adapt_field_offset("afterArrayElement",info);
      _das_class_method_offset[__fn_beforeDim] = adapt_field_offset("beforeDim",info);
      _das_class_method_offset[__fn_afterDim] = adapt_field_offset("afterDim",info);
      _das_class_method_offset[__fn_beforeArray] = adapt_field_offset("beforeArray",info);
      _das_class_method_offset[__fn_afterArray] = adapt_field_offset("afterArray",info);
      _das_class_method_offset[__fn_beforeTable] = adapt_field_offset("beforeTable",info);
      _das_class_method_offset[__fn_beforeTableKey] = adapt_field_offset("beforeTableKey",info);
      _das_class_method_offset[__fn_afterTableKey] = adapt_field_offset("afterTableKey",info);
      _das_class_method_offset[__fn_beforeTableValue] = adapt_field_offset("beforeTableValue",info);
      _das_class_method_offset[__fn_afterTableValue] = adapt_field_offset("afterTableValue",info);
      _das_class_method_offset[__fn_afterTable] = adapt_field_offset("afterTable",info);
      _das_class_method_offset[__fn_beforeRef] = adapt_field_offset("beforeRef",info);
      _das_class_method_offset[__fn_afterRef] = adapt_field_offset("afterRef",info);
      _das_class_method_offset[__fn_beforePtr] = adapt_field_offset("beforePtr",info);
      _das_class_method_offset[__fn_afterPtr] = adapt_field_offset("afterPtr",info);
      _das_class_method_offset[__fn_beforeHandle] = adapt_field_offset("beforeHandle",info);
      _das_class_method_offset[__fn_afterHandle] = adapt_field_offset("afterHandle",info);
      _das_class_method_offset[__fn_beforeLambda] = adapt_field_offset("beforeLambda",info);
      _das_class_method_offset[__fn_afterLambda] = adapt_field_offset("afterLambda",info);
      _das_class_method_offset[__fn_beforeIterator] = adapt_field_offset("beforeIterator",info);
      _das_class_method_offset[__fn_afterIterator] = adapt_field_offset("afterIterator",info);
      _das_class_method_offset[__fn_Null] = adapt_field_offset("Null",info);
      _das_class_method_offset[__fn_VoidPtr] = adapt_field_offset("VoidPtr",info);
      _das_class_method_offset[__fn_Bool] = adapt_field_offset("Bool",info);
      _das_class_method_offset[__fn_Int8] = adapt_field_offset("Int8",info);
      _das_class_method_offset[__fn_UInt8] = adapt_field_offset("UInt8",info);
      _das_class_method_offset[__fn_Int16] = adapt_field_offset("Int16",info);
      _das_class_method_offset[__fn_UInt16] = adapt_field_offset("UInt16",info);
      _das_class_method_offset[__fn_Int64] = adapt_field_offset("Int64",info);
      _das_class_method_offset[__fn_UInt64] = adapt_field_offset("UInt64",info);
      _das_class_method_offset[__fn_String] = adapt_field_offset("String",info);
      _das_class_method_offset[__fn_Double] = adapt_field_offset("Double",info);
      _das_class_method_offset[__fn_Float] = adapt_field_offset("Float",info);
      _das_class_method_offset[__fn_Int] = adapt_field_offset("Int",info);
      _das_class_method_offset[__fn_UInt] = adapt_field_offset("UInt",info);
      _das_class_method_offset[__fn_Bitfield] = adapt_field_offset("Bitfield",info);
      _das_class_method_offset[__fn_Int2] = adapt_field_offset("Int2",info);
      _das_class_method_offset[__fn_Int3] = adapt_field_offset("Int3",info);
      _das_class_method_offset[__fn_Int4] = adapt_field_offset("Int4",info);
      _das_class_method_offset[__fn_UInt2] = adapt_field_offset("UInt2",info);
      _das_class_method_offset[__fn_UInt3] = adapt_field_offset("UInt3",info);
      _das_class_method_offset[__fn_UInt4] = adapt_field_offset("UInt4",info);
      _das_class_method_offset[__fn_Float2] = adapt_field_offset("Float2",info);
      _das_class_method_offset[__fn_Float3] = adapt_field_offset("Float3",info);
      _das_class_method_offset[__fn_Float4] = adapt_field_offset("Float4",info);
      _das_class_method_offset[__fn_Range] = adapt_field_offset("Range",info);
      _das_class_method_offset[__fn_URange] = adapt_field_offset("URange",info);
      _das_class_method_offset[__fn_WalkBlock] = adapt_field_offset("WalkBlock",info);
      _das_class_method_offset[__fn_WalkFunction] = adapt_field_offset("WalkFunction",info);
      _das_class_method_offset[__fn_WalkEnumeration] = adapt_field_offset("WalkEnumeration",info);
      _das_class_method_offset[__fn_WalkEnumeration8] = adapt_field_offset("WalkEnumeration8",info);
      _das_class_method_offset[__fn_WalkEnumeration16] = adapt_field_offset("WalkEnumeration16",info);
      _das_class_method_offset[__fn_FakeContext] = adapt_field_offset("FakeContext",info);
    }
  }
  static __forceinline Func get_canVisitHandle ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitHandle]);
  }
  static __forceinline bool invoke_canVisitHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_canVisitStructure ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitStructure]);
  }
  static __forceinline bool invoke_canVisitStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  static __forceinline Func get_beforeStructure ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeStructure]);
  }
  static __forceinline void invoke_beforeStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  static __forceinline Func get_afterStructure ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructure]);
  }
  static __forceinline void invoke_afterStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  static __forceinline Func get_afterStructureCancel ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructureCancel]);
  }
  static __forceinline void invoke_afterStructureCancel ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  static __forceinline Func get_beforeStructureField ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeStructureField]);
  }
  static __forceinline void invoke_beforeStructureField ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si, void * const  pv, VarInfo const  & vi, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &,void * const ,VarInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,si,pv,vi,last);
  }
  static __forceinline Func get_afterStructureField ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructureField]);
  }
  static __forceinline void invoke_afterStructureField ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si, void * const  pv, VarInfo const  & vi, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &,void * const ,VarInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,si,pv,vi,last);
  }
  static __forceinline Func get_beforeTuple ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeTuple]);
  }
  static __forceinline void invoke_beforeTuple ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterTuple ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterTuple]);
  }
  static __forceinline void invoke_afterTuple ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforeTupleEntry ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeTupleEntry]);
  }
  static __forceinline void invoke_beforeTupleEntry ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pv, TypeInfo const  & vi, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,TypeInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pv,vi,last);
  }
  static __forceinline Func get_afterTupleEntry ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterTupleEntry]);
  }
  static __forceinline void invoke_afterTupleEntry ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pv, TypeInfo const  & vi, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,TypeInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pv,vi,last);
  }
  static __forceinline Func get_beforeVariant ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeVariant]);
  }
  static __forceinline void invoke_beforeVariant ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterVariant ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterVariant]);
  }
  static __forceinline void invoke_afterVariant ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforeArrayData ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeArrayData]);
  }
  static __forceinline void invoke_beforeArrayData ( Context * __context__, Func __funcCall__, void * self, void * const  ps, uint32_t stride, uint32_t count, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,uint32_t,uint32_t,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,stride,count,ti);
  }
  static __forceinline Func get_afterArrayData ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterArrayData]);
  }
  static __forceinline void invoke_afterArrayData ( Context * __context__, Func __funcCall__, void * self, void * const  ps, uint32_t stride, uint32_t count, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,uint32_t,uint32_t,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,stride,count,ti);
  }
  static __forceinline Func get_beforeArrayElement ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeArrayElement]);
  }
  static __forceinline void invoke_beforeArrayElement ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pe, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pe,index,last);
  }
  static __forceinline Func get_afterArrayElement ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterArrayElement]);
  }
  static __forceinline void invoke_afterArrayElement ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pe, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pe,index,last);
  }
  static __forceinline Func get_beforeDim ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeDim]);
  }
  static __forceinline void invoke_beforeDim ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterDim ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterDim]);
  }
  static __forceinline void invoke_afterDim ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforeArray ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeArray]);
  }
  static __forceinline void invoke_beforeArray ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiArray const  & pa, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiArray const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  static __forceinline Func get_afterArray ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterArray]);
  }
  static __forceinline void invoke_afterArray ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiArray const  & pa, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiArray const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  static __forceinline Func get_beforeTable ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeTable]);
  }
  static __forceinline void invoke_beforeTable ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  static __forceinline Func get_beforeTableKey ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeTableKey]);
  }
  static __forceinline void invoke_beforeTableKey ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pk, TypeInfo const  & ki, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pk,ki,index,last);
  }
  static __forceinline Func get_afterTableKey ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterTableKey]);
  }
  static __forceinline void invoke_afterTableKey ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pk, TypeInfo const  & ki, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pk,ki,index,last);
  }
  static __forceinline Func get_beforeTableValue ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeTableValue]);
  }
  static __forceinline void invoke_beforeTableValue ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pv, TypeInfo const  & kv, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pv,kv,index,last);
  }
  static __forceinline Func get_afterTableValue ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterTableValue]);
  }
  static __forceinline void invoke_afterTableValue ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pv, TypeInfo const  & kv, uint32_t index, bool last ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pv,kv,index,last);
  }
  static __forceinline Func get_afterTable ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterTable]);
  }
  static __forceinline void invoke_afterTable ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  static __forceinline Func get_beforeRef ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeRef]);
  }
  static __forceinline void invoke_beforeRef ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterRef ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterRef]);
  }
  static __forceinline void invoke_afterRef ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforePtr ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforePtr]);
  }
  static __forceinline void invoke_beforePtr ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterPtr ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterPtr]);
  }
  static __forceinline void invoke_afterPtr ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforeHandle ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeHandle]);
  }
  static __forceinline void invoke_beforeHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_afterHandle ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterHandle]);
  }
  static __forceinline void invoke_afterHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  static __forceinline Func get_beforeLambda ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeLambda]);
  }
  static __forceinline void invoke_beforeLambda ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiLambda & value, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiLambda &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  static __forceinline Func get_afterLambda ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterLambda]);
  }
  static __forceinline void invoke_afterLambda ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiLambda & value, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiLambda &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  static __forceinline Func get_beforeIterator ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeIterator]);
  }
  static __forceinline void invoke_beforeIterator ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiSequence & value, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiSequence &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  static __forceinline Func get_afterIterator ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterIterator]);
  }
  static __forceinline void invoke_afterIterator ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiSequence & value, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiSequence &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  static __forceinline Func get_Null ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Null]);
  }
  static __forceinline void invoke_Null ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  static __forceinline Func get_VoidPtr ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_VoidPtr]);
  }
  static __forceinline void invoke_VoidPtr ( Context * __context__, Func __funcCall__, void * self, void * & value ) {
    das_invoke_function<void>::invoke
      <void *,void * &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Bool ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Bool]);
  }
  static __forceinline void invoke_Bool ( Context * __context__, Func __funcCall__, void * self, bool & value ) {
    das_invoke_function<void>::invoke
      <void *,bool &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int8 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int8]);
  }
  static __forceinline void invoke_Int8 ( Context * __context__, Func __funcCall__, void * self, int8_t & value ) {
    das_invoke_function<void>::invoke
      <void *,int8_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt8 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt8]);
  }
  static __forceinline void invoke_UInt8 ( Context * __context__, Func __funcCall__, void * self, uint8_t & value ) {
    das_invoke_function<void>::invoke
      <void *,uint8_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int16 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int16]);
  }
  static __forceinline void invoke_Int16 ( Context * __context__, Func __funcCall__, void * self, int16_t & value ) {
    das_invoke_function<void>::invoke
      <void *,int16_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt16 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt16]);
  }
  static __forceinline void invoke_UInt16 ( Context * __context__, Func __funcCall__, void * self, uint16_t & value ) {
    das_invoke_function<void>::invoke
      <void *,uint16_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int64 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int64]);
  }
  static __forceinline void invoke_Int64 ( Context * __context__, Func __funcCall__, void * self, int64_t & value ) {
    das_invoke_function<void>::invoke
      <void *,int64_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt64 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt64]);
  }
  static __forceinline void invoke_UInt64 ( Context * __context__, Func __funcCall__, void * self, uint64_t & value ) {
    das_invoke_function<void>::invoke
      <void *,uint64_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_String ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_String]);
  }
  static __forceinline void invoke_String ( Context * __context__, Func __funcCall__, void * self, char * & value ) {
    das_invoke_function<void>::invoke
      <void *,char * &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Double ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Double]);
  }
  static __forceinline void invoke_Double ( Context * __context__, Func __funcCall__, void * self, double & value ) {
    das_invoke_function<void>::invoke
      <void *,double &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Float ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Float]);
  }
  static __forceinline void invoke_Float ( Context * __context__, Func __funcCall__, void * self, float & value ) {
    das_invoke_function<void>::invoke
      <void *,float &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int]);
  }
  static __forceinline void invoke_Int ( Context * __context__, Func __funcCall__, void * self, int32_t & value ) {
    das_invoke_function<void>::invoke
      <void *,int32_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt]);
  }
  static __forceinline void invoke_UInt ( Context * __context__, Func __funcCall__, void * self, uint32_t & value ) {
    das_invoke_function<void>::invoke
      <void *,uint32_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Bitfield ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Bitfield]);
  }
  static __forceinline void invoke_Bitfield ( Context * __context__, Func __funcCall__, void * self, uint32_t & value, TypeInfo const  & ti ) {
    das_invoke_function<void>::invoke
      <void *,uint32_t &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  static __forceinline Func get_Int2 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int2]);
  }
  static __forceinline void invoke_Int2 ( Context * __context__, Func __funcCall__, void * self, int2 & value ) {
    das_invoke_function<void>::invoke
      <void *,int2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int3 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int3]);
  }
  static __forceinline void invoke_Int3 ( Context * __context__, Func __funcCall__, void * self, int3 & value ) {
    das_invoke_function<void>::invoke
      <void *,int3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Int4 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Int4]);
  }
  static __forceinline void invoke_Int4 ( Context * __context__, Func __funcCall__, void * self, int4 & value ) {
    das_invoke_function<void>::invoke
      <void *,int4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt2 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt2]);
  }
  static __forceinline void invoke_UInt2 ( Context * __context__, Func __funcCall__, void * self, uint2 & value ) {
    das_invoke_function<void>::invoke
      <void *,uint2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt3 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt3]);
  }
  static __forceinline void invoke_UInt3 ( Context * __context__, Func __funcCall__, void * self, uint3 & value ) {
    das_invoke_function<void>::invoke
      <void *,uint3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_UInt4 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_UInt4]);
  }
  static __forceinline void invoke_UInt4 ( Context * __context__, Func __funcCall__, void * self, uint4 & value ) {
    das_invoke_function<void>::invoke
      <void *,uint4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Float2 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Float2]);
  }
  static __forceinline void invoke_Float2 ( Context * __context__, Func __funcCall__, void * self, float2 & value ) {
    das_invoke_function<void>::invoke
      <void *,float2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Float3 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Float3]);
  }
  static __forceinline void invoke_Float3 ( Context * __context__, Func __funcCall__, void * self, float3 & value ) {
    das_invoke_function<void>::invoke
      <void *,float3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Float4 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Float4]);
  }
  static __forceinline void invoke_Float4 ( Context * __context__, Func __funcCall__, void * self, float4 & value ) {
    das_invoke_function<void>::invoke
      <void *,float4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_Range ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_Range]);
  }
  static __forceinline void invoke_Range ( Context * __context__, Func __funcCall__, void * self, range & value ) {
    das_invoke_function<void>::invoke
      <void *,range &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_URange ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_URange]);
  }
  static __forceinline void invoke_URange ( Context * __context__, Func __funcCall__, void * self, urange & value ) {
    das_invoke_function<void>::invoke
      <void *,urange &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_WalkBlock ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_WalkBlock]);
  }
  static __forceinline void invoke_WalkBlock ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiBlock & value ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiBlock &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_WalkFunction ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_WalkFunction]);
  }
  static __forceinline void invoke_WalkFunction ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiFunc & value ) {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiFunc &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  static __forceinline Func get_WalkEnumeration ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_WalkEnumeration]);
  }
  static __forceinline void invoke_WalkEnumeration ( Context * __context__, Func __funcCall__, void * self, int32_t & value, EnumInfo const  & ei ) {
    das_invoke_function<void>::invoke
      <void *,int32_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  static __forceinline Func get_WalkEnumeration8 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_WalkEnumeration8]);
  }
  static __forceinline void invoke_WalkEnumeration8 ( Context * __context__, Func __funcCall__, void * self, int8_t & value, EnumInfo const  & ei ) {
    das_invoke_function<void>::invoke
      <void *,int8_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  static __forceinline Func get_WalkEnumeration16 ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_WalkEnumeration16]);
  }
  static __forceinline void invoke_WalkEnumeration16 ( Context * __context__, Func __funcCall__, void * self, int16_t & value, EnumInfo const  & ei ) {
    das_invoke_function<void>::invoke
      <void *,int16_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  static __forceinline Func get_FakeContext ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_FakeContext]);
  }
  static __forceinline void invoke_FakeContext ( Context * __context__, Func __funcCall__, void * self, Context & value ) {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
};
int DapiDataWalker_Adapter::_das_class_method_offset[69];

class DapiDebugAgent_Adapter {
protected:
  enum {
    __fn_onInstall = 0,
    __fn_onUninstall = 1,
    __fn_onCreateContext = 2,
    __fn_onDestroyContext = 3,
    __fn_onSingleStep = 4,
    __fn_onInstrument = 5,
    __fn_onInstrumentFunction = 6,
    __fn_onBreakpoint = 7,
    __fn_onVariable = 8,
    __fn_onTick = 9,
    __fn_onCollect = 10,
    __fn_onLog = 11,
  };
protected:
  static int _das_class_method_offset[12];
public:
  DapiDebugAgent_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_onInstall] = adapt_field_offset("onInstall",info);
      _das_class_method_offset[__fn_onUninstall] = adapt_field_offset("onUninstall",info);
      _das_class_method_offset[__fn_onCreateContext] = adapt_field_offset("onCreateContext",info);
      _das_class_method_offset[__fn_onDestroyContext] = adapt_field_offset("onDestroyContext",info);
      _das_class_method_offset[__fn_onSingleStep] = adapt_field_offset("onSingleStep",info);
      _das_class_method_offset[__fn_onInstrument] = adapt_field_offset("onInstrument",info);
      _das_class_method_offset[__fn_onInstrumentFunction] = adapt_field_offset("onInstrumentFunction",info);
      _das_class_method_offset[__fn_onBreakpoint] = adapt_field_offset("onBreakpoint",info);
      _das_class_method_offset[__fn_onVariable] = adapt_field_offset("onVariable",info);
      _das_class_method_offset[__fn_onTick] = adapt_field_offset("onTick",info);
      _das_class_method_offset[__fn_onCollect] = adapt_field_offset("onCollect",info);
      _das_class_method_offset[__fn_onLog] = adapt_field_offset("onLog",info);
    }
  }
  static __forceinline Func get_onInstall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onInstall]);
  }
  static __forceinline void invoke_onInstall ( Context * __context__, Func __funcCall__, void * self, DebugAgent * const  agent ) {
    das_invoke_function<void>::invoke
      <void *,DebugAgent * const >
        (__context__,nullptr,__funcCall__,
          self,agent);
  }
  static __forceinline Func get_onUninstall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onUninstall]);
  }
  static __forceinline void invoke_onUninstall ( Context * __context__, Func __funcCall__, void * self, DebugAgent * const  agent ) {
    das_invoke_function<void>::invoke
      <void *,DebugAgent * const >
        (__context__,nullptr,__funcCall__,
          self,agent);
  }
  static __forceinline Func get_onCreateContext ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onCreateContext]);
  }
  static __forceinline void invoke_onCreateContext ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  static __forceinline Func get_onDestroyContext ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onDestroyContext]);
  }
  static __forceinline void invoke_onDestroyContext ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  static __forceinline Func get_onSingleStep ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onSingleStep]);
  }
  static __forceinline void invoke_onSingleStep ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at ) {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ctx,at);
  }
  static __forceinline Func get_onInstrument ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onInstrument]);
  }
  static __forceinline void invoke_onInstrument ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at ) {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ctx,at);
  }
  static __forceinline Func get_onInstrumentFunction ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onInstrumentFunction]);
  }
  static __forceinline void invoke_onInstrumentFunction ( Context * __context__, Func __funcCall__, void * self, Context & ctx, SimFunction * const  fn, bool entering ) {
    das_invoke_function<void>::invoke
      <void *,Context &,SimFunction * const ,bool>
        (__context__,nullptr,__funcCall__,
          self,ctx,fn,entering);
  }
  static __forceinline Func get_onBreakpoint ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onBreakpoint]);
  }
  static __forceinline void invoke_onBreakpoint ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at, char * const  reason, char * const  text ) {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &,char * const ,char * const >
        (__context__,nullptr,__funcCall__,
          self,ctx,at,reason,text);
  }
  static __forceinline Func get_onVariable ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onVariable]);
  }
  static __forceinline void invoke_onVariable ( Context * __context__, Func __funcCall__, void * self, Context & ctx, char * const  category, char * const  name, TypeInfo const  & info, void * const  data ) {
    das_invoke_function<void>::invoke
      <void *,Context &,char * const ,char * const ,TypeInfo const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,ctx,category,name,info,data);
  }
  static __forceinline Func get_onTick ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onTick]);
  }
  static __forceinline void invoke_onTick ( Context * __context__, Func __funcCall__, void * self ) {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  static __forceinline Func get_onCollect ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onCollect]);
  }
  static __forceinline void invoke_onCollect ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  static __forceinline Func get_onLog ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onLog]);
  }
  static __forceinline bool invoke_onLog ( Context * __context__, Func __funcCall__, void * self, int32_t level, char * const  text ) {
    return das_invoke_function<bool>::invoke
      <void *,int32_t,char * const >
        (__context__,nullptr,__funcCall__,
          self,level,text);
  }
};
int DapiDebugAgent_Adapter::_das_class_method_offset[12];

class DapiStackWalker_Adapter {
protected:
  enum {
    __fn_canWalkArguments = 0,
    __fn_canWalkVariables = 1,
    __fn_canWalkOutOfScopeVariables = 2,
    __fn_onBeforeCall = 3,
    __fn_onCallAOT = 4,
    __fn_onCallAt = 5,
    __fn_onCall = 6,
    __fn_onAfterPrologue = 7,
    __fn_onArgument = 8,
    __fn_onBeforeVariables = 9,
    __fn_onVariable = 10,
    __fn_onAfterCall = 11,
  };
protected:
  static int _das_class_method_offset[12];
public:
  DapiStackWalker_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_canWalkArguments] = adapt_field_offset("canWalkArguments",info);
      _das_class_method_offset[__fn_canWalkVariables] = adapt_field_offset("canWalkVariables",info);
      _das_class_method_offset[__fn_canWalkOutOfScopeVariables] = adapt_field_offset("canWalkOutOfScopeVariables",info);
      _das_class_method_offset[__fn_onBeforeCall] = adapt_field_offset("onBeforeCall",info);
      _das_class_method_offset[__fn_onCallAOT] = adapt_field_offset("onCallAOT",info);
      _das_class_method_offset[__fn_onCallAt] = adapt_field_offset("onCallAt",info);
      _das_class_method_offset[__fn_onCall] = adapt_field_offset("onCall",info);
      _das_class_method_offset[__fn_onAfterPrologue] = adapt_field_offset("onAfterPrologue",info);
      _das_class_method_offset[__fn_onArgument] = adapt_field_offset("onArgument",info);
      _das_class_method_offset[__fn_onBeforeVariables] = adapt_field_offset("onBeforeVariables",info);
      _das_class_method_offset[__fn_onVariable] = adapt_field_offset("onVariable",info);
      _das_class_method_offset[__fn_onAfterCall] = adapt_field_offset("onAfterCall",info);
    }
  }
  static __forceinline Func get_canWalkArguments ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canWalkArguments]);
  }
  static __forceinline bool invoke_canWalkArguments ( Context * __context__, Func __funcCall__, void * self ) {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  static __forceinline Func get_canWalkVariables ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canWalkVariables]);
  }
  static __forceinline bool invoke_canWalkVariables ( Context * __context__, Func __funcCall__, void * self ) {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  static __forceinline Func get_canWalkOutOfScopeVariables ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canWalkOutOfScopeVariables]);
  }
  static __forceinline bool invoke_canWalkOutOfScopeVariables ( Context * __context__, Func __funcCall__, void * self ) {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  static __forceinline Func get_onBeforeCall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onBeforeCall]);
  }
  static __forceinline void invoke_onBeforeCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, void * const  sp ) {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,pp,sp);
  }
  static __forceinline Func get_onCallAOT ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onCallAOT]);
  }
  static __forceinline void invoke_onCallAOT ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, char * const  fileName ) {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,char * const >
        (__context__,nullptr,__funcCall__,
          self,pp,fileName);
  }
  static __forceinline Func get_onCallAt ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onCallAt]);
  }
  static __forceinline void invoke_onCallAt ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, FuncInfo const  & info, LineInfo const  & at ) {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,FuncInfo const  &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pp,info,at);
  }
  static __forceinline Func get_onCall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onCall]);
  }
  static __forceinline void invoke_onCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, FuncInfo const  & info ) {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,FuncInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pp,info);
  }
  static __forceinline Func get_onAfterPrologue ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onAfterPrologue]);
  }
  static __forceinline void invoke_onAfterPrologue ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, void * const  sp ) {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,pp,sp);
  }
  static __forceinline Func get_onArgument ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onArgument]);
  }
  static __forceinline void invoke_onArgument ( Context * __context__, Func __funcCall__, void * self, FuncInfo const  & info, int32_t index, VarInfo const  & vinfo, float4 arg ) {
    das_invoke_function<void>::invoke
      <void *,FuncInfo const  &,int32_t,VarInfo const  &,float4>
        (__context__,nullptr,__funcCall__,
          self,info,index,vinfo,arg);
  }
  static __forceinline Func get_onBeforeVariables ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onBeforeVariables]);
  }
  static __forceinline void invoke_onBeforeVariables ( Context * __context__, Func __funcCall__, void * self ) {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  static __forceinline Func get_onVariable ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onVariable]);
  }
  static __forceinline void invoke_onVariable ( Context * __context__, Func __funcCall__, void * self, FuncInfo const  & inf, LocalVariableInfo const  & vinfo, void * const  arg, bool inScope ) {
    das_invoke_function<void>::invoke
      <void *,FuncInfo const  &,LocalVariableInfo const  &,void * const ,bool>
        (__context__,nullptr,__funcCall__,
          self,inf,vinfo,arg,inScope);
  }
  static __forceinline Func get_onAfterCall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onAfterCall]);
  }
  static __forceinline bool invoke_onAfterCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp ) {
    return das_invoke_function<bool>::invoke
      <void *,Prologue const  &>
        (__context__,nullptr,__funcCall__,
          self,pp);
  }
};
int DapiStackWalker_Adapter::_das_class_method_offset[12];


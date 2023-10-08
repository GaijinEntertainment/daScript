class DapiDataWalker_Adapter {
public:
  DapiDataWalker_Adapter ( const StructInfo * ) {}
  __forceinline Func get_canVisitHandle ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_canVisitHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_canVisitStructure ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_canVisitStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  __forceinline Func get_canVisitArray ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline bool invoke_canVisitArray ( Context * __context__, Func __funcCall__, void * self, void * const  pa, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  __forceinline Func get_canVisitArrayData ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline bool invoke_canVisitArrayData ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti, uint32_t count ) const {
    return das_invoke_function<bool>::invoke
      <void *,TypeInfo const  &,uint32_t>
        (__context__,nullptr,__funcCall__,
          self,ti,count);
  }
  __forceinline Func get_canVisitTuple ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline bool invoke_canVisitTuple ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_canVisitVariant ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline bool invoke_canVisitVariant ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_canVisitTable ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline bool invoke_canVisitTable ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_canVisitTableData ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline bool invoke_canVisitTableData ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  __forceinline Func get_canVisitPointer ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline bool invoke_canVisitPointer ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  __forceinline Func get_canVisitLambda ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline bool invoke_canVisitLambda ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  __forceinline Func get_canVisitIterator ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline bool invoke_canVisitIterator ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) const {
    return das_invoke_function<bool>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  __forceinline Func get_beforeStructure ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline void invoke_beforeStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  __forceinline Func get_afterStructure ( void * self ) const {
    return getDasClassMethod(self,112);
  }
  __forceinline void invoke_afterStructure ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  __forceinline Func get_afterStructureCancel ( void * self ) const {
    return getDasClassMethod(self,120);
  }
  __forceinline void invoke_afterStructureCancel ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,si);
  }
  __forceinline Func get_beforeStructureField ( void * self ) const {
    return getDasClassMethod(self,128);
  }
  __forceinline void invoke_beforeStructureField ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si, void * const  pv, VarInfo const  & vi, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &,void * const ,VarInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,si,pv,vi,last);
  }
  __forceinline Func get_afterStructureField ( void * self ) const {
    return getDasClassMethod(self,136);
  }
  __forceinline void invoke_afterStructureField ( Context * __context__, Func __funcCall__, void * self, void * const  ps, StructInfo const  & si, void * const  pv, VarInfo const  & vi, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,StructInfo const  &,void * const ,VarInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,si,pv,vi,last);
  }
  __forceinline Func get_beforeTuple ( void * self ) const {
    return getDasClassMethod(self,144);
  }
  __forceinline void invoke_beforeTuple ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterTuple ( void * self ) const {
    return getDasClassMethod(self,152);
  }
  __forceinline void invoke_afterTuple ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforeTupleEntry ( void * self ) const {
    return getDasClassMethod(self,160);
  }
  __forceinline void invoke_beforeTupleEntry ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pv, TypeInfo const  & vi, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,TypeInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pv,vi,last);
  }
  __forceinline Func get_afterTupleEntry ( void * self ) const {
    return getDasClassMethod(self,168);
  }
  __forceinline void invoke_afterTupleEntry ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pv, TypeInfo const  & vi, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,TypeInfo const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pv,vi,last);
  }
  __forceinline Func get_beforeVariant ( void * self ) const {
    return getDasClassMethod(self,176);
  }
  __forceinline void invoke_beforeVariant ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterVariant ( void * self ) const {
    return getDasClassMethod(self,184);
  }
  __forceinline void invoke_afterVariant ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforeArrayData ( void * self ) const {
    return getDasClassMethod(self,192);
  }
  __forceinline void invoke_beforeArrayData ( Context * __context__, Func __funcCall__, void * self, void * const  ps, uint32_t stride, uint32_t count, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,uint32_t,uint32_t,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,stride,count,ti);
  }
  __forceinline Func get_afterArrayData ( void * self ) const {
    return getDasClassMethod(self,200);
  }
  __forceinline void invoke_afterArrayData ( Context * __context__, Func __funcCall__, void * self, void * const  ps, uint32_t stride, uint32_t count, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,uint32_t,uint32_t,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,stride,count,ti);
  }
  __forceinline Func get_beforeArrayElement ( void * self ) const {
    return getDasClassMethod(self,208);
  }
  __forceinline void invoke_beforeArrayElement ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pe, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pe,index,last);
  }
  __forceinline Func get_afterArrayElement ( void * self ) const {
    return getDasClassMethod(self,216);
  }
  __forceinline void invoke_afterArrayElement ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti, void * const  pe, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &,void * const ,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,ps,ti,pe,index,last);
  }
  __forceinline Func get_beforeDim ( void * self ) const {
    return getDasClassMethod(self,224);
  }
  __forceinline void invoke_beforeDim ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterDim ( void * self ) const {
    return getDasClassMethod(self,232);
  }
  __forceinline void invoke_afterDim ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforeArray ( void * self ) const {
    return getDasClassMethod(self,240);
  }
  __forceinline void invoke_beforeArray ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiArray const  & pa, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiArray const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  __forceinline Func get_afterArray ( void * self ) const {
    return getDasClassMethod(self,248);
  }
  __forceinline void invoke_afterArray ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiArray const  & pa, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiArray const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  __forceinline Func get_beforeTable ( void * self ) const {
    return getDasClassMethod(self,256);
  }
  __forceinline void invoke_beforeTable ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  __forceinline Func get_beforeTableKey ( void * self ) const {
    return getDasClassMethod(self,264);
  }
  __forceinline void invoke_beforeTableKey ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pk, TypeInfo const  & ki, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pk,ki,index,last);
  }
  __forceinline Func get_afterTableKey ( void * self ) const {
    return getDasClassMethod(self,272);
  }
  __forceinline void invoke_afterTableKey ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pk, TypeInfo const  & ki, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pk,ki,index,last);
  }
  __forceinline Func get_beforeTableValue ( void * self ) const {
    return getDasClassMethod(self,280);
  }
  __forceinline void invoke_beforeTableValue ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pv, TypeInfo const  & kv, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pv,kv,index,last);
  }
  __forceinline Func get_afterTableValue ( void * self ) const {
    return getDasClassMethod(self,288);
  }
  __forceinline void invoke_afterTableValue ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti, void * const  pv, TypeInfo const  & kv, uint32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &,void * const ,TypeInfo const  &,uint32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,pa,ti,pv,kv,index,last);
  }
  __forceinline Func get_afterTable ( void * self ) const {
    return getDasClassMethod(self,296);
  }
  __forceinline void invoke_afterTable ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiTable const  & pa, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiTable const  &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pa,ti);
  }
  __forceinline Func get_beforeRef ( void * self ) const {
    return getDasClassMethod(self,304);
  }
  __forceinline void invoke_beforeRef ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterRef ( void * self ) const {
    return getDasClassMethod(self,312);
  }
  __forceinline void invoke_afterRef ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforePtr ( void * self ) const {
    return getDasClassMethod(self,320);
  }
  __forceinline void invoke_beforePtr ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterPtr ( void * self ) const {
    return getDasClassMethod(self,328);
  }
  __forceinline void invoke_afterPtr ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforeHandle ( void * self ) const {
    return getDasClassMethod(self,336);
  }
  __forceinline void invoke_beforeHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_afterHandle ( void * self ) const {
    return getDasClassMethod(self,344);
  }
  __forceinline void invoke_afterHandle ( Context * __context__, Func __funcCall__, void * self, void * const  ps, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,void * const ,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ps,ti);
  }
  __forceinline Func get_beforeLambda ( void * self ) const {
    return getDasClassMethod(self,352);
  }
  __forceinline void invoke_beforeLambda ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiLambda & value, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiLambda &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  __forceinline Func get_afterLambda ( void * self ) const {
    return getDasClassMethod(self,360);
  }
  __forceinline void invoke_afterLambda ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiLambda & value, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiLambda &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  __forceinline Func get_beforeIterator ( void * self ) const {
    return getDasClassMethod(self,368);
  }
  __forceinline void invoke_beforeIterator ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiSequence & value, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiSequence &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  __forceinline Func get_afterIterator ( void * self ) const {
    return getDasClassMethod(self,376);
  }
  __forceinline void invoke_afterIterator ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiSequence & value, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiSequence &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  __forceinline Func get_Null ( void * self ) const {
    return getDasClassMethod(self,384);
  }
  __forceinline void invoke_Null ( Context * __context__, Func __funcCall__, void * self, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ti);
  }
  __forceinline Func get_VoidPtr ( void * self ) const {
    return getDasClassMethod(self,392);
  }
  __forceinline void invoke_VoidPtr ( Context * __context__, Func __funcCall__, void * self, void * & value ) const {
    das_invoke_function<void>::invoke
      <void *,void * &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Bool ( void * self ) const {
    return getDasClassMethod(self,400);
  }
  __forceinline void invoke_Bool ( Context * __context__, Func __funcCall__, void * self, bool & value ) const {
    das_invoke_function<void>::invoke
      <void *,bool &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int8 ( void * self ) const {
    return getDasClassMethod(self,408);
  }
  __forceinline void invoke_Int8 ( Context * __context__, Func __funcCall__, void * self, int8_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,int8_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt8 ( void * self ) const {
    return getDasClassMethod(self,416);
  }
  __forceinline void invoke_UInt8 ( Context * __context__, Func __funcCall__, void * self, uint8_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint8_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int16 ( void * self ) const {
    return getDasClassMethod(self,424);
  }
  __forceinline void invoke_Int16 ( Context * __context__, Func __funcCall__, void * self, int16_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,int16_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt16 ( void * self ) const {
    return getDasClassMethod(self,432);
  }
  __forceinline void invoke_UInt16 ( Context * __context__, Func __funcCall__, void * self, uint16_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint16_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int64 ( void * self ) const {
    return getDasClassMethod(self,440);
  }
  __forceinline void invoke_Int64 ( Context * __context__, Func __funcCall__, void * self, int64_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,int64_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt64 ( void * self ) const {
    return getDasClassMethod(self,448);
  }
  __forceinline void invoke_UInt64 ( Context * __context__, Func __funcCall__, void * self, uint64_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint64_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_String ( void * self ) const {
    return getDasClassMethod(self,456);
  }
  __forceinline void invoke_String ( Context * __context__, Func __funcCall__, void * self, char * & value ) const {
    das_invoke_function<void>::invoke
      <void *,char * &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Double ( void * self ) const {
    return getDasClassMethod(self,464);
  }
  __forceinline void invoke_Double ( Context * __context__, Func __funcCall__, void * self, double & value ) const {
    das_invoke_function<void>::invoke
      <void *,double &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Float ( void * self ) const {
    return getDasClassMethod(self,472);
  }
  __forceinline void invoke_Float ( Context * __context__, Func __funcCall__, void * self, float & value ) const {
    das_invoke_function<void>::invoke
      <void *,float &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int ( void * self ) const {
    return getDasClassMethod(self,480);
  }
  __forceinline void invoke_Int ( Context * __context__, Func __funcCall__, void * self, int32_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,int32_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt ( void * self ) const {
    return getDasClassMethod(self,488);
  }
  __forceinline void invoke_UInt ( Context * __context__, Func __funcCall__, void * self, uint32_t & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint32_t &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Bitfield ( void * self ) const {
    return getDasClassMethod(self,496);
  }
  __forceinline void invoke_Bitfield ( Context * __context__, Func __funcCall__, void * self, uint32_t & value, TypeInfo const  & ti ) const {
    das_invoke_function<void>::invoke
      <void *,uint32_t &,TypeInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ti);
  }
  __forceinline Func get_Int2 ( void * self ) const {
    return getDasClassMethod(self,504);
  }
  __forceinline void invoke_Int2 ( Context * __context__, Func __funcCall__, void * self, int2 & value ) const {
    das_invoke_function<void>::invoke
      <void *,int2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int3 ( void * self ) const {
    return getDasClassMethod(self,512);
  }
  __forceinline void invoke_Int3 ( Context * __context__, Func __funcCall__, void * self, int3 & value ) const {
    das_invoke_function<void>::invoke
      <void *,int3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Int4 ( void * self ) const {
    return getDasClassMethod(self,520);
  }
  __forceinline void invoke_Int4 ( Context * __context__, Func __funcCall__, void * self, int4 & value ) const {
    das_invoke_function<void>::invoke
      <void *,int4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt2 ( void * self ) const {
    return getDasClassMethod(self,528);
  }
  __forceinline void invoke_UInt2 ( Context * __context__, Func __funcCall__, void * self, uint2 & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt3 ( void * self ) const {
    return getDasClassMethod(self,536);
  }
  __forceinline void invoke_UInt3 ( Context * __context__, Func __funcCall__, void * self, uint3 & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_UInt4 ( void * self ) const {
    return getDasClassMethod(self,544);
  }
  __forceinline void invoke_UInt4 ( Context * __context__, Func __funcCall__, void * self, uint4 & value ) const {
    das_invoke_function<void>::invoke
      <void *,uint4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Float2 ( void * self ) const {
    return getDasClassMethod(self,552);
  }
  __forceinline void invoke_Float2 ( Context * __context__, Func __funcCall__, void * self, float2 & value ) const {
    das_invoke_function<void>::invoke
      <void *,float2 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Float3 ( void * self ) const {
    return getDasClassMethod(self,560);
  }
  __forceinline void invoke_Float3 ( Context * __context__, Func __funcCall__, void * self, float3 & value ) const {
    das_invoke_function<void>::invoke
      <void *,float3 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Float4 ( void * self ) const {
    return getDasClassMethod(self,568);
  }
  __forceinline void invoke_Float4 ( Context * __context__, Func __funcCall__, void * self, float4 & value ) const {
    das_invoke_function<void>::invoke
      <void *,float4 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Range ( void * self ) const {
    return getDasClassMethod(self,576);
  }
  __forceinline void invoke_Range ( Context * __context__, Func __funcCall__, void * self, range & value ) const {
    das_invoke_function<void>::invoke
      <void *,range &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_URange ( void * self ) const {
    return getDasClassMethod(self,584);
  }
  __forceinline void invoke_URange ( Context * __context__, Func __funcCall__, void * self, urange & value ) const {
    das_invoke_function<void>::invoke
      <void *,urange &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_Range64 ( void * self ) const {
    return getDasClassMethod(self,592);
  }
  __forceinline void invoke_Range64 ( Context * __context__, Func __funcCall__, void * self, range64 & value ) const {
    das_invoke_function<void>::invoke
      <void *,range64 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_URange64 ( void * self ) const {
    return getDasClassMethod(self,600);
  }
  __forceinline void invoke_URange64 ( Context * __context__, Func __funcCall__, void * self, urange64 & value ) const {
    das_invoke_function<void>::invoke
      <void *,urange64 &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_WalkBlock ( void * self ) const {
    return getDasClassMethod(self,608);
  }
  __forceinline void invoke_WalkBlock ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiBlock & value ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiBlock &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_WalkFunction ( void * self ) const {
    return getDasClassMethod(self,616);
  }
  __forceinline void invoke_WalkFunction ( Context * __context__, Func __funcCall__, void * self, debugapi::DapiFunc & value ) const {
    das_invoke_function<void>::invoke
      <void *,debugapi::DapiFunc &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_WalkEnumeration ( void * self ) const {
    return getDasClassMethod(self,624);
  }
  __forceinline void invoke_WalkEnumeration ( Context * __context__, Func __funcCall__, void * self, int32_t & value, EnumInfo const  & ei ) const {
    das_invoke_function<void>::invoke
      <void *,int32_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  __forceinline Func get_WalkEnumeration8 ( void * self ) const {
    return getDasClassMethod(self,632);
  }
  __forceinline void invoke_WalkEnumeration8 ( Context * __context__, Func __funcCall__, void * self, int8_t & value, EnumInfo const  & ei ) const {
    das_invoke_function<void>::invoke
      <void *,int8_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  __forceinline Func get_WalkEnumeration16 ( void * self ) const {
    return getDasClassMethod(self,640);
  }
  __forceinline void invoke_WalkEnumeration16 ( Context * __context__, Func __funcCall__, void * self, int16_t & value, EnumInfo const  & ei ) const {
    das_invoke_function<void>::invoke
      <void *,int16_t &,EnumInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,value,ei);
  }
  __forceinline Func get_FakeContext ( void * self ) const {
    return getDasClassMethod(self,648);
  }
  __forceinline void invoke_FakeContext ( Context * __context__, Func __funcCall__, void * self, Context & value ) const {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,value);
  }
  __forceinline Func get_InvalidData ( void * self ) const {
    return getDasClassMethod(self,656);
  }
  __forceinline void invoke_InvalidData ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
};

class DapiDebugAgent_Adapter {
public:
  DapiDebugAgent_Adapter ( const StructInfo * ) {}
  __forceinline Func get_onInstall ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline void invoke_onInstall ( Context * __context__, Func __funcCall__, void * self, DebugAgent * const  agent ) const {
    das_invoke_function<void>::invoke
      <void *,DebugAgent * const >
        (__context__,nullptr,__funcCall__,
          self,agent);
  }
  __forceinline Func get_onUninstall ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline void invoke_onUninstall ( Context * __context__, Func __funcCall__, void * self, DebugAgent * const  agent ) const {
    das_invoke_function<void>::invoke
      <void *,DebugAgent * const >
        (__context__,nullptr,__funcCall__,
          self,agent);
  }
  __forceinline Func get_onCreateContext ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline void invoke_onCreateContext ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) const {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  __forceinline Func get_onDestroyContext ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline void invoke_onDestroyContext ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) const {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  __forceinline Func get_onSimulateContext ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline void invoke_onSimulateContext ( Context * __context__, Func __funcCall__, void * self, Context & ctx ) const {
    das_invoke_function<void>::invoke
      <void *,Context &>
        (__context__,nullptr,__funcCall__,
          self,ctx);
  }
  __forceinline Func get_onSingleStep ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline void invoke_onSingleStep ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ctx,at);
  }
  __forceinline Func get_onInstrument ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline void invoke_onInstrument ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ctx,at);
  }
  __forceinline Func get_onInstrumentFunction ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline void invoke_onInstrumentFunction ( Context * __context__, Func __funcCall__, void * self, Context & ctx, SimFunction * const  fn, bool entering, uint64_t userData ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,SimFunction * const ,bool,uint64_t>
        (__context__,nullptr,__funcCall__,
          self,ctx,fn,entering,userData);
  }
  __forceinline Func get_onBreakpoint ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline void invoke_onBreakpoint ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at, char * const  reason, char * const  text ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &,char * const ,char * const >
        (__context__,nullptr,__funcCall__,
          self,ctx,at,reason,text);
  }
  __forceinline Func get_onVariable ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline void invoke_onVariable ( Context * __context__, Func __funcCall__, void * self, Context & ctx, char * const  category, char * const  name, TypeInfo const  & info, void * const  data ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,char * const ,char * const ,TypeInfo const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,ctx,category,name,info,data);
  }
  __forceinline Func get_onBreakpointsReset ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline void invoke_onBreakpointsReset ( Context * __context__, Func __funcCall__, void * self, char * const  file, int32_t breakpointsNum ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,int32_t>
        (__context__,nullptr,__funcCall__,
          self,file,breakpointsNum);
  }
  __forceinline Func get_onTick ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline void invoke_onTick ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_onCollect ( void * self ) const {
    return getDasClassMethod(self,112);
  }
  __forceinline void invoke_onCollect ( Context * __context__, Func __funcCall__, void * self, Context & ctx, LineInfo const  & at ) const {
    das_invoke_function<void>::invoke
      <void *,Context &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,ctx,at);
  }
  __forceinline Func get_onLog ( void * self ) const {
    return getDasClassMethod(self,120);
  }
  __forceinline bool invoke_onLog ( Context * __context__, Func __funcCall__, void * self, Context * const  context, LineInfo const  * const  at, int32_t level, char * const  text ) const {
    return das_invoke_function<bool>::invoke
      <void *,Context * const ,LineInfo const  * const ,int32_t,char * const >
        (__context__,nullptr,__funcCall__,
          self,context,at,level,text);
  }
};

class DapiStackWalker_Adapter {
public:
  DapiStackWalker_Adapter ( const StructInfo * ) {}
  __forceinline Func get_canWalkArguments ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_canWalkArguments ( Context * __context__, Func __funcCall__, void * self ) const {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_canWalkVariables ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_canWalkVariables ( Context * __context__, Func __funcCall__, void * self ) const {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_canWalkOutOfScopeVariables ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline bool invoke_canWalkOutOfScopeVariables ( Context * __context__, Func __funcCall__, void * self ) const {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_onBeforeCall ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline void invoke_onBeforeCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, void * const  sp ) const {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,pp,sp);
  }
  __forceinline Func get_onCallAOT ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline void invoke_onCallAOT ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, char * const  fileName ) const {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,char * const >
        (__context__,nullptr,__funcCall__,
          self,pp,fileName);
  }
  __forceinline Func get_onCallAt ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline void invoke_onCallAt ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, FuncInfo const  & info, LineInfo const  & at ) const {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,FuncInfo const  &,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pp,info,at);
  }
  __forceinline Func get_onCall ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline void invoke_onCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, FuncInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,FuncInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,pp,info);
  }
  __forceinline Func get_onAfterPrologue ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline void invoke_onAfterPrologue ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp, void * const  sp ) const {
    das_invoke_function<void>::invoke
      <void *,Prologue const  &,void * const >
        (__context__,nullptr,__funcCall__,
          self,pp,sp);
  }
  __forceinline Func get_onArgument ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline void invoke_onArgument ( Context * __context__, Func __funcCall__, void * self, FuncInfo const  & info, int32_t index, VarInfo const  & vinfo, float4 arg ) const {
    das_invoke_function<void>::invoke
      <void *,FuncInfo const  &,int32_t,VarInfo const  &,float4>
        (__context__,nullptr,__funcCall__,
          self,info,index,vinfo,arg);
  }
  __forceinline Func get_onBeforeVariables ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline void invoke_onBeforeVariables ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_onVariable ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline void invoke_onVariable ( Context * __context__, Func __funcCall__, void * self, FuncInfo const  & inf, LocalVariableInfo const  & vinfo, void * const  arg, bool inScope ) const {
    das_invoke_function<void>::invoke
      <void *,FuncInfo const  &,LocalVariableInfo const  &,void * const ,bool>
        (__context__,nullptr,__funcCall__,
          self,inf,vinfo,arg,inScope);
  }
  __forceinline Func get_onAfterCall ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline bool invoke_onAfterCall ( Context * __context__, Func __funcCall__, void * self, Prologue const  & pp ) const {
    return das_invoke_function<bool>::invoke
      <void *,Prologue const  &>
        (__context__,nullptr,__funcCall__,
          self,pp);
  }
};


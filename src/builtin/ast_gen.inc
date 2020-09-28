
// header file section

class AstFunctionAnnotation_Adapter {
protected:
  enum {
    __fn_transform = 0,
    __fn_apply = 1,
    __fn_finish = 2,
  };
protected:
  static int _das_class_method_offset[3];
public:
  AstFunctionAnnotation_Adapter ( const StructInfo * info );
  smart_ptr_raw<Expression> invoke_transform ( Context * __context__, Func __func__, void * self, smart_ptr_raw<ExprCallFunc> call, das::string & errors );
  __forceinline Func get_transform ( void * self ) { return getDasClassMethod(self,_das_class_method_offset[__fn_transform]); }
  bool invoke_apply ( Context * __context__, Func __func__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors );
  __forceinline Func get_apply ( void * self ) { return getDasClassMethod(self,_das_class_method_offset[__fn_apply]); }
  bool invoke_finish ( Context * __context__, Func __func__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors );
  __forceinline Func get_finish ( void * self ) { return getDasClassMethod(self,_das_class_method_offset[__fn_finish]); }
};

// source file section

int AstFunctionAnnotation_Adapter::_das_class_method_offset[3];

AstFunctionAnnotation_Adapter::AstFunctionAnnotation_Adapter ( const StructInfo * info ) {
  if ( _das_class_method_offset[0]==0 ) {
    _das_class_method_offset[__fn_transform] = adapt_field_offset("transform",info);
    _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
    _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
  }
}

smart_ptr_raw<Expression> AstFunctionAnnotation_Adapter::invoke_transform ( Context * __context__, Func __func__, void * self, smart_ptr_raw<ExprCallFunc> call, das::string & errors ) {
  return das_invoke_function<smart_ptr_raw<Expression>>::invoke
    <void *,smart_ptr_raw<ExprCallFunc>,das::string &>
      (__context__,__func__,
        self,call,errors);
}

bool AstFunctionAnnotation_Adapter::invoke_apply ( Context * __context__, Func __func__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
  return das_invoke_function<bool>::invoke
    <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
      (__context__,__func__,
        self,func,group,args,errors);
}

bool AstFunctionAnnotation_Adapter::invoke_finish ( Context * __context__, Func __func__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
  return das_invoke_function<bool>::invoke
    <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
      (__context__,__func__,
        self,func,group,args,progArgs,errors);
}


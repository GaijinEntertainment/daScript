class AstFunctionAnnotation_Adapter {
protected:
  enum {
    __fn_transform = 0,
    __fn_verifyCall = 1,
    __fn_apply = 2,
    __fn_generic_apply = 3,
    __fn_finish = 4,
    __fn_patch = 5,
    __fn_fixup = 6,
    __fn_lint = 7,
    __fn_complete = 8,
    __fn_isCompatible = 9,
    __fn_isSpecialized = 10,
    __fn_appendToMangledName = 11,
  };
protected:
  int _das_class_method_offset[12];
public:
  AstFunctionAnnotation_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_transform] = adapt_field_offset_ex("transform",info,index);
      _das_class_method_offset[__fn_verifyCall] = adapt_field_offset_ex("verifyCall",info,index);
      _das_class_method_offset[__fn_apply] = adapt_field_offset_ex("apply",info,index);
      _das_class_method_offset[__fn_generic_apply] = adapt_field_offset_ex("generic_apply",info,index);
      _das_class_method_offset[__fn_finish] = adapt_field_offset_ex("finish",info,index);
      _das_class_method_offset[__fn_patch] = adapt_field_offset_ex("patch",info,index);
      _das_class_method_offset[__fn_fixup] = adapt_field_offset_ex("fixup",info,index);
      _das_class_method_offset[__fn_lint] = adapt_field_offset_ex("lint",info,index);
      _das_class_method_offset[__fn_complete] = adapt_field_offset_ex("complete",info,index);
      _das_class_method_offset[__fn_isCompatible] = adapt_field_offset_ex("isCompatible",info,index);
      _das_class_method_offset[__fn_isSpecialized] = adapt_field_offset_ex("isSpecialized",info,index);
      _das_class_method_offset[__fn_appendToMangledName] = adapt_field_offset_ex("appendToMangledName",info,index);
  }
  __forceinline Func get_transform ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_transform]);
  }
  __forceinline smart_ptr<Expression> invoke_transform ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,errors).marshal();
  }
  __forceinline Func get_verifyCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_verifyCall]);
  }
  __forceinline bool invoke_verifyCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,args,progArgs,errors);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,errors);
  }
  __forceinline Func get_generic_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_generic_apply]);
  }
  __forceinline bool invoke_generic_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_patch ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_patch]);
  }
  __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors, bool & astChanged ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors,astChanged);
  }
  __forceinline Func get_fixup ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_fixup]);
  }
  __forceinline bool invoke_fixup ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_lint ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_lint]);
  }
  __forceinline bool invoke_lint ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_complete ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_complete]);
  }
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function>,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,func,ctx);
  }
  __forceinline Func get_isCompatible ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_isCompatible]);
  }
  __forceinline bool invoke_isCompatible ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, das::vector<smart_ptr<TypeDecl>> & types, AnnotationDeclaration const  & decl, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>> &,AnnotationDeclaration const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,types,decl,errors);
  }
  __forceinline Func get_isSpecialized ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_isSpecialized]);
  }
  __forceinline bool invoke_isSpecialized ( Context * __context__, Func __funcCall__, void * self ) const {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_appendToMangledName ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_appendToMangledName]);
  }
  __forceinline void invoke_appendToMangledName ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  func, AnnotationDeclaration const  & decl, das::string & mangledName ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,AnnotationDeclaration const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,decl,mangledName);
  }
};

class AstBlockAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
    __fn_finish = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstBlockAnnotation_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_apply] = adapt_field_offset_ex("apply",info,index);
      _das_class_method_offset[__fn_finish] = adapt_field_offset_ex("finish",info,index);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,progArgs,errors);
  }
};

class AstStructureAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
    __fn_finish = 1,
    __fn_patch = 2,
    __fn_complete = 3,
    __fn_aotPrefix = 4,
    __fn_aotBody = 5,
    __fn_aotSuffix = 6,
  };
protected:
  int _das_class_method_offset[7];
public:
  AstStructureAnnotation_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_apply] = adapt_field_offset_ex("apply",info,index);
      _das_class_method_offset[__fn_finish] = adapt_field_offset_ex("finish",info,index);
      _das_class_method_offset[__fn_patch] = adapt_field_offset_ex("patch",info,index);
      _das_class_method_offset[__fn_complete] = adapt_field_offset_ex("complete",info,index);
      _das_class_method_offset[__fn_aotPrefix] = adapt_field_offset_ex("aotPrefix",info,index);
      _das_class_method_offset[__fn_aotBody] = adapt_field_offset_ex("aotBody",info,index);
      _das_class_method_offset[__fn_aotSuffix] = adapt_field_offset_ex("aotSuffix",info,index);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  __forceinline Func get_patch ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_patch]);
  }
  __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors, bool & astChanged ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors,astChanged);
  }
  __forceinline Func get_complete ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_complete]);
  }
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,st,ctx);
  }
  __forceinline Func get_aotPrefix ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_aotPrefix]);
  }
  __forceinline void invoke_aotPrefix ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
  __forceinline Func get_aotBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_aotBody]);
  }
  __forceinline void invoke_aotBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
  __forceinline Func get_aotSuffix ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_aotSuffix]);
  }
  __forceinline void invoke_aotSuffix ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
};

class AstEnumerationAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
  };
protected:
  int _das_class_method_offset[1];
public:
  AstEnumerationAnnotation_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_apply] = adapt_field_offset_ex("apply",info,index);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Enumeration>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
};

class AstPassMacro_Adapter {
protected:
  enum {
    __fn_apply = 0,
  };
protected:
  int _das_class_method_offset[1];
public:
  AstPassMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_apply] = adapt_field_offset_ex("apply",info,index);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod);
  }
};

class AstVarianMacro_Adapter {
protected:
  enum {
    __fn_visitExprIsVariant = 0,
    __fn_visitExprAsVariant = 1,
    __fn_visitExprSafeAsVariant = 2,
  };
protected:
  int _das_class_method_offset[3];
public:
  AstVarianMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_visitExprIsVariant] = adapt_field_offset_ex("visitExprIsVariant",info,index);
      _das_class_method_offset[__fn_visitExprAsVariant] = adapt_field_offset_ex("visitExprAsVariant",info,index);
      _das_class_method_offset[__fn_visitExprSafeAsVariant] = adapt_field_offset_ex("visitExprSafeAsVariant",info,index);
  }
  __forceinline Func get_visitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprIsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_visitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_visitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSafeAsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstForLoopMacro_Adapter {
protected:
  enum {
    __fn_visitExprFor = 0,
  };
protected:
  int _das_class_method_offset[1];
public:
  AstForLoopMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_visitExprFor] = adapt_field_offset_ex("visitExprFor",info,index);
  }
  __forceinline Func get_visitExprFor ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprFor]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprFor> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstCaptureMacro_Adapter {
protected:
  enum {
    __fn_captureExpression = 0,
    __fn_captureFunction = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstCaptureMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_captureExpression] = adapt_field_offset_ex("captureExpression",info,index);
      _das_class_method_offset[__fn_captureFunction] = adapt_field_offset_ex("captureFunction",info,index);
  }
  __forceinline Func get_captureExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_captureExpression]);
  }
  __forceinline smart_ptr<Expression> invoke_captureExpression ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Module * const  mod, smart_ptr_raw<Expression> const  expr, smart_ptr_raw<TypeDecl> const  etype ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,Program * const ,Module * const ,smart_ptr_raw<Expression> const ,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr,etype).marshal();
  }
  __forceinline Func get_captureFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_captureFunction]);
  }
  __forceinline void invoke_captureFunction ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Module * const  mod, Structure * lcs, smart_ptr_raw<Function> fun ) const {
    das_invoke_function<void>::invoke
      <void *,Program * const ,Module * const ,Structure *,smart_ptr_raw<Function>>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,lcs,fun);
  }
};

class AstSimulateMacro_Adapter {
protected:
  enum {
    __fn_preSimulate = 0,
    __fn_simulate = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstSimulateMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_preSimulate] = adapt_field_offset_ex("preSimulate",info,index);
      _das_class_method_offset[__fn_simulate] = adapt_field_offset_ex("simulate",info,index);
  }
  __forceinline Func get_preSimulate ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preSimulate]);
  }
  __forceinline bool invoke_preSimulate ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Context * const  ctx ) const {
    return das_invoke_function<bool>::invoke
      <void *,Program * const ,Context * const >
        (__context__,nullptr,__funcCall__,
          self,prog,ctx);
  }
  __forceinline Func get_simulate ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_simulate]);
  }
  __forceinline bool invoke_simulate ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Context * const  ctx ) const {
    return das_invoke_function<bool>::invoke
      <void *,Program * const ,Context * const >
        (__context__,nullptr,__funcCall__,
          self,prog,ctx);
  }
};

class AstReaderMacro_Adapter {
protected:
  enum {
    __fn_accept = 0,
    __fn_visit = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstReaderMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_accept] = adapt_field_offset_ex("accept",info,index);
      _das_class_method_offset[__fn_visit] = adapt_field_offset_ex("visit",info,index);
  }
  __forceinline Func get_accept ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_accept]);
  }
  __forceinline bool invoke_accept ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, ExprReader * const  expr, int32_t ch, LineInfo const  & info ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr,ch,info);
  }
  __forceinline Func get_visit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visit]);
  }
  __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprReader> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstCommentReader_Adapter {
protected:
  enum {
    __fn_open = 0,
    __fn_accept = 1,
    __fn_close = 2,
    __fn_beforeStructure = 3,
    __fn_afterStructure = 4,
    __fn_beforeStructureFields = 5,
    __fn_afterStructureField = 6,
    __fn_afterStructureFields = 7,
    __fn_beforeFunction = 8,
    __fn_afterFunction = 9,
    __fn_beforeGlobalVariables = 10,
    __fn_afterGlobalVariable = 11,
    __fn_afterGlobalVariables = 12,
    __fn_beforeVariant = 13,
    __fn_afterVariant = 14,
    __fn_beforeEnumeration = 15,
    __fn_afterEnumeration = 16,
    __fn_beforeAlias = 17,
    __fn_afterAlias = 18,
  };
protected:
  int _das_class_method_offset[19];
public:
  AstCommentReader_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_open] = adapt_field_offset_ex("open",info,index);
      _das_class_method_offset[__fn_accept] = adapt_field_offset_ex("accept",info,index);
      _das_class_method_offset[__fn_close] = adapt_field_offset_ex("close",info,index);
      _das_class_method_offset[__fn_beforeStructure] = adapt_field_offset_ex("beforeStructure",info,index);
      _das_class_method_offset[__fn_afterStructure] = adapt_field_offset_ex("afterStructure",info,index);
      _das_class_method_offset[__fn_beforeStructureFields] = adapt_field_offset_ex("beforeStructureFields",info,index);
      _das_class_method_offset[__fn_afterStructureField] = adapt_field_offset_ex("afterStructureField",info,index);
      _das_class_method_offset[__fn_afterStructureFields] = adapt_field_offset_ex("afterStructureFields",info,index);
      _das_class_method_offset[__fn_beforeFunction] = adapt_field_offset_ex("beforeFunction",info,index);
      _das_class_method_offset[__fn_afterFunction] = adapt_field_offset_ex("afterFunction",info,index);
      _das_class_method_offset[__fn_beforeGlobalVariables] = adapt_field_offset_ex("beforeGlobalVariables",info,index);
      _das_class_method_offset[__fn_afterGlobalVariable] = adapt_field_offset_ex("afterGlobalVariable",info,index);
      _das_class_method_offset[__fn_afterGlobalVariables] = adapt_field_offset_ex("afterGlobalVariables",info,index);
      _das_class_method_offset[__fn_beforeVariant] = adapt_field_offset_ex("beforeVariant",info,index);
      _das_class_method_offset[__fn_afterVariant] = adapt_field_offset_ex("afterVariant",info,index);
      _das_class_method_offset[__fn_beforeEnumeration] = adapt_field_offset_ex("beforeEnumeration",info,index);
      _das_class_method_offset[__fn_afterEnumeration] = adapt_field_offset_ex("afterEnumeration",info,index);
      _das_class_method_offset[__fn_beforeAlias] = adapt_field_offset_ex("beforeAlias",info,index);
      _das_class_method_offset[__fn_afterAlias] = adapt_field_offset_ex("afterAlias",info,index);
  }
  __forceinline Func get_open ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_open]);
  }
  __forceinline void invoke_open ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, bool cpp, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,bool,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,cpp,info);
  }
  __forceinline Func get_accept ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_accept]);
  }
  __forceinline void invoke_accept ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, int32_t ch, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,int32_t,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,ch,info);
  }
  __forceinline Func get_close ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_close]);
  }
  __forceinline void invoke_close ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeStructure ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeStructure]);
  }
  __forceinline void invoke_beforeStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterStructure ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructure]);
  }
  __forceinline void invoke_afterStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  st, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,st,prog,mod,info);
  }
  __forceinline Func get_beforeStructureFields ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeStructureFields]);
  }
  __forceinline void invoke_beforeStructureFields ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterStructureField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructureField]);
  }
  __forceinline void invoke_afterStructureField ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_afterStructureFields ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterStructureFields]);
  }
  __forceinline void invoke_afterStructureFields ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeFunction]);
  }
  __forceinline void invoke_beforeFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterFunction]);
  }
  __forceinline void invoke_afterFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fn, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,fn,prog,mod,info);
  }
  __forceinline Func get_beforeGlobalVariables ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeGlobalVariables]);
  }
  __forceinline void invoke_beforeGlobalVariables ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterGlobalVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterGlobalVariable]);
  }
  __forceinline void invoke_afterGlobalVariable ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_afterGlobalVariables ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterGlobalVariables]);
  }
  __forceinline void invoke_afterGlobalVariables ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeVariant]);
  }
  __forceinline void invoke_beforeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterVariant]);
  }
  __forceinline void invoke_afterVariant ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_beforeEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeEnumeration]);
  }
  __forceinline void invoke_beforeEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterEnumeration]);
  }
  __forceinline void invoke_afterEnumeration ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_beforeAlias ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_beforeAlias]);
  }
  __forceinline void invoke_beforeAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterAlias ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_afterAlias]);
  }
  __forceinline void invoke_afterAlias ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
};

class AstCallMacro_Adapter {
protected:
  enum {
    __fn_preVisit = 0,
    __fn_visit = 1,
    __fn_canVisitArgument = 2,
    __fn_canFoldReturnResult = 3,
  };
protected:
  int _das_class_method_offset[4];
public:
  AstCallMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_preVisit] = adapt_field_offset_ex("preVisit",info,index);
      _das_class_method_offset[__fn_visit] = adapt_field_offset_ex("visit",info,index);
      _das_class_method_offset[__fn_canVisitArgument] = adapt_field_offset_ex("canVisitArgument",info,index);
      _das_class_method_offset[__fn_canFoldReturnResult] = adapt_field_offset_ex("canFoldReturnResult",info,index);
  }
  __forceinline Func get_preVisit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisit]);
  }
  __forceinline void invoke_preVisit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr);
  }
  __forceinline Func get_visit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visit]);
  }
  __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_canVisitArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitArgument]);
  }
  __forceinline bool invoke_canVisitArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr, int32_t argIndex ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const ,int32_t>
        (__context__,nullptr,__funcCall__,
          self,expr,argIndex);
  }
  __forceinline Func get_canFoldReturnResult ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canFoldReturnResult]);
  }
  __forceinline bool invoke_canFoldReturnResult ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
};

class AstTypeInfoMacro_Adapter {
protected:
  enum {
    __fn_getAstChange = 0,
    __fn_getAstType = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstTypeInfoMacro_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_getAstChange] = adapt_field_offset_ex("getAstChange",info,index);
      _das_class_method_offset[__fn_getAstType] = adapt_field_offset_ex("getAstType",info,index);
  }
  __forceinline Func get_getAstChange ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_getAstChange]);
  }
  __forceinline smart_ptr<Expression> invoke_getAstChange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,expr,errors).marshal();
  }
  __forceinline Func get_getAstType ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_getAstType]);
  }
  __forceinline smart_ptr<TypeDecl> invoke_getAstType ( Context * __context__, Func __funcCall__, void * self, ModuleLibrary & lib, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,ModuleLibrary &,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,lib,expr,errors).marshal();
  }
};

class AstVisitor_Adapter {
protected:
  enum {
    __fn_preVisitProgram = 0,
    __fn_visitProgram = 1,
    __fn_preVisitProgramBody = 2,
    __fn_preVisitModule = 3,
    __fn_visitModule = 4,
    __fn_preVisitExprTypeDecl = 5,
    __fn_visitExprTypeDecl = 6,
    __fn_preVisitTypeDecl = 7,
    __fn_visitTypeDecl = 8,
    __fn_preVisitAlias = 9,
    __fn_visitAlias = 10,
    __fn_canVisitEnumeration = 11,
    __fn_preVisitEnumeration = 12,
    __fn_preVisitEnumerationValue = 13,
    __fn_visitEnumerationValue = 14,
    __fn_visitEnumeration = 15,
    __fn_canVisitStructure = 16,
    __fn_preVisitStructure = 17,
    __fn_preVisitStructureField = 18,
    __fn_visitStructureField = 19,
    __fn_visitStructure = 20,
    __fn_canVisitFunction = 21,
    __fn_canVisitFunctionArgumentInit = 22,
    __fn_preVisitFunction = 23,
    __fn_visitFunction = 24,
    __fn_preVisitFunctionArgument = 25,
    __fn_visitFunctionArgument = 26,
    __fn_preVisitFunctionArgumentInit = 27,
    __fn_visitFunctionArgumentInit = 28,
    __fn_preVisitFunctionBody = 29,
    __fn_visitFunctionBody = 30,
    __fn_preVisitExpression = 31,
    __fn_visitExpression = 32,
    __fn_preVisitExprBlock = 33,
    __fn_visitExprBlock = 34,
    __fn_preVisitExprBlockArgument = 35,
    __fn_visitExprBlockArgument = 36,
    __fn_preVisitExprBlockArgumentInit = 37,
    __fn_visitExprBlockArgumentInit = 38,
    __fn_preVisitExprBlockExpression = 39,
    __fn_visitExprBlockExpression = 40,
    __fn_preVisitExprBlockFinal = 41,
    __fn_visitExprBlockFinal = 42,
    __fn_preVisitExprBlockFinalExpression = 43,
    __fn_visitExprBlockFinalExpression = 44,
    __fn_preVisitExprLet = 45,
    __fn_visitExprLet = 46,
    __fn_preVisitExprLetVariable = 47,
    __fn_visitExprLetVariable = 48,
    __fn_preVisitExprLetVariableInit = 49,
    __fn_visitExprLetVariableInit = 50,
    __fn_canVisitGlobalVariable = 51,
    __fn_preVisitGlobalLet = 52,
    __fn_visitGlobalLet = 53,
    __fn_preVisitGlobalLetVariable = 54,
    __fn_visitGlobalLetVariable = 55,
    __fn_preVisitGlobalLetVariableInit = 56,
    __fn_visitGlobalLetVariableInit = 57,
    __fn_preVisitExprStringBuilder = 58,
    __fn_visitExprStringBuilder = 59,
    __fn_preVisitExprStringBuilderElement = 60,
    __fn_visitExprStringBuilderElement = 61,
    __fn_preVisitExprNew = 62,
    __fn_visitExprNew = 63,
    __fn_preVisitExprNewArgument = 64,
    __fn_visitExprNewArgument = 65,
    __fn_preVisitExprNamedCall = 66,
    __fn_visitExprNamedCall = 67,
    __fn_preVisitExprNamedCallArgument = 68,
    __fn_visitExprNamedCallArgument = 69,
    __fn_preVisitExprLooksLikeCall = 70,
    __fn_visitExprLooksLikeCall = 71,
    __fn_preVisitExprLooksLikeCallArgument = 72,
    __fn_visitExprLooksLikeCallArgument = 73,
    __fn_canVisitCall = 74,
    __fn_preVisitExprCall = 75,
    __fn_visitExprCall = 76,
    __fn_preVisitExprCallArgument = 77,
    __fn_visitExprCallArgument = 78,
    __fn_preVisitExprNullCoalescing = 79,
    __fn_visitExprNullCoalescing = 80,
    __fn_preVisitExprNullCoalescingDefault = 81,
    __fn_preVisitExprAt = 82,
    __fn_visitExprAt = 83,
    __fn_preVisitExprAtIndex = 84,
    __fn_preVisitExprSafeAt = 85,
    __fn_visitExprSafeAt = 86,
    __fn_preVisitExprSafeAtIndex = 87,
    __fn_preVisitExprIs = 88,
    __fn_visitExprIs = 89,
    __fn_preVisitExprIsType = 90,
    __fn_preVisitExprOp2 = 91,
    __fn_visitExprOp2 = 92,
    __fn_preVisitExprOp2Right = 93,
    __fn_preVisitExprOp3 = 94,
    __fn_visitExprOp3 = 95,
    __fn_preVisitExprOp3Left = 96,
    __fn_preVisitExprOp3Right = 97,
    __fn_preVisitExprCopy = 98,
    __fn_visitExprCopy = 99,
    __fn_preVisitExprCopyRight = 100,
    __fn_preVisitExprMove = 101,
    __fn_visitExprMove = 102,
    __fn_preVisitExprMoveRight = 103,
    __fn_preVisitExprClone = 104,
    __fn_visitExprClone = 105,
    __fn_preVisitExprCloneRight = 106,
    __fn_canVisitWithAliasSubexpression = 107,
    __fn_preVisitExprAssume = 108,
    __fn_visitExprAssume = 109,
    __fn_preVisitExprWith = 110,
    __fn_visitExprWith = 111,
    __fn_preVisitExprWithBody = 112,
    __fn_preVisitExprWhile = 113,
    __fn_visitExprWhile = 114,
    __fn_preVisitExprWhileBody = 115,
    __fn_preVisitExprTryCatch = 116,
    __fn_visitExprTryCatch = 117,
    __fn_preVisitExprTryCatchCatch = 118,
    __fn_preVisitExprIfThenElse = 119,
    __fn_visitExprIfThenElse = 120,
    __fn_preVisitExprIfThenElseIfBlock = 121,
    __fn_preVisitExprIfThenElseElseBlock = 122,
    __fn_preVisitExprFor = 123,
    __fn_visitExprFor = 124,
    __fn_preVisitExprForVariable = 125,
    __fn_visitExprForVariable = 126,
    __fn_preVisitExprForSource = 127,
    __fn_visitExprForSource = 128,
    __fn_preVisitExprForStack = 129,
    __fn_preVisitExprForBody = 130,
    __fn_preVisitExprMakeVariant = 131,
    __fn_visitExprMakeVariant = 132,
    __fn_preVisitExprMakeVariantField = 133,
    __fn_visitExprMakeVariantField = 134,
    __fn_canVisitMakeStructBody = 135,
    __fn_canVisitMakeStructBlock = 136,
    __fn_preVisitExprMakeStruct = 137,
    __fn_visitExprMakeStruct = 138,
    __fn_preVisitExprMakeStructIndex = 139,
    __fn_visitExprMakeStructIndex = 140,
    __fn_preVisitExprMakeStructField = 141,
    __fn_visitExprMakeStructField = 142,
    __fn_preVisitExprMakeArray = 143,
    __fn_visitExprMakeArray = 144,
    __fn_preVisitExprMakeArrayIndex = 145,
    __fn_visitExprMakeArrayIndex = 146,
    __fn_preVisitExprMakeTuple = 147,
    __fn_visitExprMakeTuple = 148,
    __fn_preVisitExprMakeTupleIndex = 149,
    __fn_visitExprMakeTupleIndex = 150,
    __fn_preVisitExprArrayComprehension = 151,
    __fn_visitExprArrayComprehension = 152,
    __fn_preVisitExprArrayComprehensionSubexpr = 153,
    __fn_preVisitExprArrayComprehensionWhere = 154,
    __fn_preVisitExprTypeInfo = 155,
    __fn_visitExprTypeInfo = 156,
    __fn_preVisitExprPtr2Ref = 157,
    __fn_visitExprPtr2Ref = 158,
    __fn_preVisitExprLabel = 159,
    __fn_visitExprLabel = 160,
    __fn_preVisitExprGoto = 161,
    __fn_visitExprGoto = 162,
    __fn_preVisitExprRef2Value = 163,
    __fn_visitExprRef2Value = 164,
    __fn_preVisitExprRef2Ptr = 165,
    __fn_visitExprRef2Ptr = 166,
    __fn_preVisitExprAddr = 167,
    __fn_visitExprAddr = 168,
    __fn_preVisitExprAssert = 169,
    __fn_visitExprAssert = 170,
    __fn_preVisitExprStaticAssert = 171,
    __fn_visitExprStaticAssert = 172,
    __fn_preVisitExprQuote = 173,
    __fn_visitExprQuote = 174,
    __fn_preVisitExprDebug = 175,
    __fn_visitExprDebug = 176,
    __fn_preVisitExprInvoke = 177,
    __fn_visitExprInvoke = 178,
    __fn_preVisitExprErase = 179,
    __fn_visitExprErase = 180,
    __fn_preVisitExprSetInsert = 181,
    __fn_visitExprSetInsert = 182,
    __fn_preVisitExprFind = 183,
    __fn_visitExprFind = 184,
    __fn_preVisitExprKeyExists = 185,
    __fn_visitExprKeyExists = 186,
    __fn_preVisitExprAscend = 187,
    __fn_visitExprAscend = 188,
    __fn_preVisitExprCast = 189,
    __fn_visitExprCast = 190,
    __fn_preVisitExprDelete = 191,
    __fn_visitExprDelete = 192,
    __fn_preVisitExprVar = 193,
    __fn_visitExprVar = 194,
    __fn_preVisitExprTag = 195,
    __fn_preVisitExprTagValue = 196,
    __fn_visitExprTag = 197,
    __fn_preVisitExprField = 198,
    __fn_visitExprField = 199,
    __fn_preVisitExprSafeField = 200,
    __fn_visitExprSafeField = 201,
    __fn_preVisitExprSwizzle = 202,
    __fn_visitExprSwizzle = 203,
    __fn_preVisitExprIsVariant = 204,
    __fn_visitExprIsVariant = 205,
    __fn_preVisitExprAsVariant = 206,
    __fn_visitExprAsVariant = 207,
    __fn_preVisitExprSafeAsVariant = 208,
    __fn_visitExprSafeAsVariant = 209,
    __fn_preVisitExprOp1 = 210,
    __fn_visitExprOp1 = 211,
    __fn_preVisitExprReturn = 212,
    __fn_visitExprReturn = 213,
    __fn_preVisitExprYield = 214,
    __fn_visitExprYield = 215,
    __fn_preVisitExprBreak = 216,
    __fn_visitExprBreak = 217,
    __fn_preVisitExprContinue = 218,
    __fn_visitExprContinue = 219,
    __fn_canVisitMakeBlockBody = 220,
    __fn_preVisitExprMakeBlock = 221,
    __fn_visitExprMakeBlock = 222,
    __fn_preVisitExprMakeGenerator = 223,
    __fn_visitExprMakeGenerator = 224,
    __fn_preVisitExprMemZero = 225,
    __fn_visitExprMemZero = 226,
    __fn_preVisitExprConst = 227,
    __fn_visitExprConst = 228,
    __fn_preVisitExprConstPtr = 229,
    __fn_visitExprConstPtr = 230,
    __fn_preVisitExprConstEnumeration = 231,
    __fn_visitExprConstEnumeration = 232,
    __fn_preVisitExprConstBitfield = 233,
    __fn_visitExprConstBitfield = 234,
    __fn_preVisitExprConstInt8 = 235,
    __fn_visitExprConstInt8 = 236,
    __fn_preVisitExprConstInt16 = 237,
    __fn_visitExprConstInt16 = 238,
    __fn_preVisitExprConstInt64 = 239,
    __fn_visitExprConstInt64 = 240,
    __fn_preVisitExprConstInt = 241,
    __fn_visitExprConstInt = 242,
    __fn_preVisitExprConstInt2 = 243,
    __fn_visitExprConstInt2 = 244,
    __fn_preVisitExprConstInt3 = 245,
    __fn_visitExprConstInt3 = 246,
    __fn_preVisitExprConstInt4 = 247,
    __fn_visitExprConstInt4 = 248,
    __fn_preVisitExprConstUInt8 = 249,
    __fn_visitExprConstUInt8 = 250,
    __fn_preVisitExprConstUInt16 = 251,
    __fn_visitExprConstUInt16 = 252,
    __fn_preVisitExprConstUInt64 = 253,
    __fn_visitExprConstUInt64 = 254,
    __fn_preVisitExprConstUInt = 255,
    __fn_visitExprConstUInt = 256,
    __fn_preVisitExprConstUInt2 = 257,
    __fn_visitExprConstUInt2 = 258,
    __fn_preVisitExprConstUInt3 = 259,
    __fn_visitExprConstUInt3 = 260,
    __fn_preVisitExprConstUInt4 = 261,
    __fn_visitExprConstUInt4 = 262,
    __fn_preVisitExprConstRange = 263,
    __fn_visitExprConstRange = 264,
    __fn_preVisitExprConstURange = 265,
    __fn_visitExprConstURange = 266,
    __fn_preVisitExprConstRange64 = 267,
    __fn_visitExprConstRange64 = 268,
    __fn_preVisitExprConstURange64 = 269,
    __fn_visitExprConstURange64 = 270,
    __fn_preVisitExprConstBool = 271,
    __fn_visitExprConstBool = 272,
    __fn_preVisitExprConstFloat = 273,
    __fn_visitExprConstFloat = 274,
    __fn_preVisitExprConstFloat2 = 275,
    __fn_visitExprConstFloat2 = 276,
    __fn_preVisitExprConstFloat3 = 277,
    __fn_visitExprConstFloat3 = 278,
    __fn_preVisitExprConstFloat4 = 279,
    __fn_visitExprConstFloat4 = 280,
    __fn_preVisitExprConstString = 281,
    __fn_visitExprConstString = 282,
    __fn_preVisitExprConstDouble = 283,
    __fn_visitExprConstDouble = 284,
    __fn_preVisitExprFakeContext = 285,
    __fn_visitExprFakeContext = 286,
    __fn_preVisitExprFakeLineInfo = 287,
    __fn_visitExprFakeLineInfo = 288,
    __fn_preVisitExprReader = 289,
    __fn_visitExprReader = 290,
    __fn_preVisitExprUnsafe = 291,
    __fn_visitExprUnsafe = 292,
    __fn_preVisitExprCallMacro = 293,
    __fn_visitExprCallMacro = 294,
  };
protected:
  int _das_class_method_offset[295];
public:
  AstVisitor_Adapter ( const StructInfo * info ) {
      uint32_t index = 0;
      _das_class_method_offset[__fn_preVisitProgram] = adapt_field_offset_ex("preVisitProgram",info,index);
      _das_class_method_offset[__fn_visitProgram] = adapt_field_offset_ex("visitProgram",info,index);
      _das_class_method_offset[__fn_preVisitProgramBody] = adapt_field_offset_ex("preVisitProgramBody",info,index);
      _das_class_method_offset[__fn_preVisitModule] = adapt_field_offset_ex("preVisitModule",info,index);
      _das_class_method_offset[__fn_visitModule] = adapt_field_offset_ex("visitModule",info,index);
      _das_class_method_offset[__fn_preVisitExprTypeDecl] = adapt_field_offset_ex("preVisitExprTypeDecl",info,index);
      _das_class_method_offset[__fn_visitExprTypeDecl] = adapt_field_offset_ex("visitExprTypeDecl",info,index);
      _das_class_method_offset[__fn_preVisitTypeDecl] = adapt_field_offset_ex("preVisitTypeDecl",info,index);
      _das_class_method_offset[__fn_visitTypeDecl] = adapt_field_offset_ex("visitTypeDecl",info,index);
      _das_class_method_offset[__fn_preVisitAlias] = adapt_field_offset_ex("preVisitAlias",info,index);
      _das_class_method_offset[__fn_visitAlias] = adapt_field_offset_ex("visitAlias",info,index);
      _das_class_method_offset[__fn_canVisitEnumeration] = adapt_field_offset_ex("canVisitEnumeration",info,index);
      _das_class_method_offset[__fn_preVisitEnumeration] = adapt_field_offset_ex("preVisitEnumeration",info,index);
      _das_class_method_offset[__fn_preVisitEnumerationValue] = adapt_field_offset_ex("preVisitEnumerationValue",info,index);
      _das_class_method_offset[__fn_visitEnumerationValue] = adapt_field_offset_ex("visitEnumerationValue",info,index);
      _das_class_method_offset[__fn_visitEnumeration] = adapt_field_offset_ex("visitEnumeration",info,index);
      _das_class_method_offset[__fn_canVisitStructure] = adapt_field_offset_ex("canVisitStructure",info,index);
      _das_class_method_offset[__fn_preVisitStructure] = adapt_field_offset_ex("preVisitStructure",info,index);
      _das_class_method_offset[__fn_preVisitStructureField] = adapt_field_offset_ex("preVisitStructureField",info,index);
      _das_class_method_offset[__fn_visitStructureField] = adapt_field_offset_ex("visitStructureField",info,index);
      _das_class_method_offset[__fn_visitStructure] = adapt_field_offset_ex("visitStructure",info,index);
      _das_class_method_offset[__fn_canVisitFunction] = adapt_field_offset_ex("canVisitFunction",info,index);
      _das_class_method_offset[__fn_canVisitFunctionArgumentInit] = adapt_field_offset_ex("canVisitFunctionArgumentInit",info,index);
      _das_class_method_offset[__fn_preVisitFunction] = adapt_field_offset_ex("preVisitFunction",info,index);
      _das_class_method_offset[__fn_visitFunction] = adapt_field_offset_ex("visitFunction",info,index);
      _das_class_method_offset[__fn_preVisitFunctionArgument] = adapt_field_offset_ex("preVisitFunctionArgument",info,index);
      _das_class_method_offset[__fn_visitFunctionArgument] = adapt_field_offset_ex("visitFunctionArgument",info,index);
      _das_class_method_offset[__fn_preVisitFunctionArgumentInit] = adapt_field_offset_ex("preVisitFunctionArgumentInit",info,index);
      _das_class_method_offset[__fn_visitFunctionArgumentInit] = adapt_field_offset_ex("visitFunctionArgumentInit",info,index);
      _das_class_method_offset[__fn_preVisitFunctionBody] = adapt_field_offset_ex("preVisitFunctionBody",info,index);
      _das_class_method_offset[__fn_visitFunctionBody] = adapt_field_offset_ex("visitFunctionBody",info,index);
      _das_class_method_offset[__fn_preVisitExpression] = adapt_field_offset_ex("preVisitExpression",info,index);
      _das_class_method_offset[__fn_visitExpression] = adapt_field_offset_ex("visitExpression",info,index);
      _das_class_method_offset[__fn_preVisitExprBlock] = adapt_field_offset_ex("preVisitExprBlock",info,index);
      _das_class_method_offset[__fn_visitExprBlock] = adapt_field_offset_ex("visitExprBlock",info,index);
      _das_class_method_offset[__fn_preVisitExprBlockArgument] = adapt_field_offset_ex("preVisitExprBlockArgument",info,index);
      _das_class_method_offset[__fn_visitExprBlockArgument] = adapt_field_offset_ex("visitExprBlockArgument",info,index);
      _das_class_method_offset[__fn_preVisitExprBlockArgumentInit] = adapt_field_offset_ex("preVisitExprBlockArgumentInit",info,index);
      _das_class_method_offset[__fn_visitExprBlockArgumentInit] = adapt_field_offset_ex("visitExprBlockArgumentInit",info,index);
      _das_class_method_offset[__fn_preVisitExprBlockExpression] = adapt_field_offset_ex("preVisitExprBlockExpression",info,index);
      _das_class_method_offset[__fn_visitExprBlockExpression] = adapt_field_offset_ex("visitExprBlockExpression",info,index);
      _das_class_method_offset[__fn_preVisitExprBlockFinal] = adapt_field_offset_ex("preVisitExprBlockFinal",info,index);
      _das_class_method_offset[__fn_visitExprBlockFinal] = adapt_field_offset_ex("visitExprBlockFinal",info,index);
      _das_class_method_offset[__fn_preVisitExprBlockFinalExpression] = adapt_field_offset_ex("preVisitExprBlockFinalExpression",info,index);
      _das_class_method_offset[__fn_visitExprBlockFinalExpression] = adapt_field_offset_ex("visitExprBlockFinalExpression",info,index);
      _das_class_method_offset[__fn_preVisitExprLet] = adapt_field_offset_ex("preVisitExprLet",info,index);
      _das_class_method_offset[__fn_visitExprLet] = adapt_field_offset_ex("visitExprLet",info,index);
      _das_class_method_offset[__fn_preVisitExprLetVariable] = adapt_field_offset_ex("preVisitExprLetVariable",info,index);
      _das_class_method_offset[__fn_visitExprLetVariable] = adapt_field_offset_ex("visitExprLetVariable",info,index);
      _das_class_method_offset[__fn_preVisitExprLetVariableInit] = adapt_field_offset_ex("preVisitExprLetVariableInit",info,index);
      _das_class_method_offset[__fn_visitExprLetVariableInit] = adapt_field_offset_ex("visitExprLetVariableInit",info,index);
      _das_class_method_offset[__fn_canVisitGlobalVariable] = adapt_field_offset_ex("canVisitGlobalVariable",info,index);
      _das_class_method_offset[__fn_preVisitGlobalLet] = adapt_field_offset_ex("preVisitGlobalLet",info,index);
      _das_class_method_offset[__fn_visitGlobalLet] = adapt_field_offset_ex("visitGlobalLet",info,index);
      _das_class_method_offset[__fn_preVisitGlobalLetVariable] = adapt_field_offset_ex("preVisitGlobalLetVariable",info,index);
      _das_class_method_offset[__fn_visitGlobalLetVariable] = adapt_field_offset_ex("visitGlobalLetVariable",info,index);
      _das_class_method_offset[__fn_preVisitGlobalLetVariableInit] = adapt_field_offset_ex("preVisitGlobalLetVariableInit",info,index);
      _das_class_method_offset[__fn_visitGlobalLetVariableInit] = adapt_field_offset_ex("visitGlobalLetVariableInit",info,index);
      _das_class_method_offset[__fn_preVisitExprStringBuilder] = adapt_field_offset_ex("preVisitExprStringBuilder",info,index);
      _das_class_method_offset[__fn_visitExprStringBuilder] = adapt_field_offset_ex("visitExprStringBuilder",info,index);
      _das_class_method_offset[__fn_preVisitExprStringBuilderElement] = adapt_field_offset_ex("preVisitExprStringBuilderElement",info,index);
      _das_class_method_offset[__fn_visitExprStringBuilderElement] = adapt_field_offset_ex("visitExprStringBuilderElement",info,index);
      _das_class_method_offset[__fn_preVisitExprNew] = adapt_field_offset_ex("preVisitExprNew",info,index);
      _das_class_method_offset[__fn_visitExprNew] = adapt_field_offset_ex("visitExprNew",info,index);
      _das_class_method_offset[__fn_preVisitExprNewArgument] = adapt_field_offset_ex("preVisitExprNewArgument",info,index);
      _das_class_method_offset[__fn_visitExprNewArgument] = adapt_field_offset_ex("visitExprNewArgument",info,index);
      _das_class_method_offset[__fn_preVisitExprNamedCall] = adapt_field_offset_ex("preVisitExprNamedCall",info,index);
      _das_class_method_offset[__fn_visitExprNamedCall] = adapt_field_offset_ex("visitExprNamedCall",info,index);
      _das_class_method_offset[__fn_preVisitExprNamedCallArgument] = adapt_field_offset_ex("preVisitExprNamedCallArgument",info,index);
      _das_class_method_offset[__fn_visitExprNamedCallArgument] = adapt_field_offset_ex("visitExprNamedCallArgument",info,index);
      _das_class_method_offset[__fn_preVisitExprLooksLikeCall] = adapt_field_offset_ex("preVisitExprLooksLikeCall",info,index);
      _das_class_method_offset[__fn_visitExprLooksLikeCall] = adapt_field_offset_ex("visitExprLooksLikeCall",info,index);
      _das_class_method_offset[__fn_preVisitExprLooksLikeCallArgument] = adapt_field_offset_ex("preVisitExprLooksLikeCallArgument",info,index);
      _das_class_method_offset[__fn_visitExprLooksLikeCallArgument] = adapt_field_offset_ex("visitExprLooksLikeCallArgument",info,index);
      _das_class_method_offset[__fn_canVisitCall] = adapt_field_offset_ex("canVisitCall",info,index);
      _das_class_method_offset[__fn_preVisitExprCall] = adapt_field_offset_ex("preVisitExprCall",info,index);
      _das_class_method_offset[__fn_visitExprCall] = adapt_field_offset_ex("visitExprCall",info,index);
      _das_class_method_offset[__fn_preVisitExprCallArgument] = adapt_field_offset_ex("preVisitExprCallArgument",info,index);
      _das_class_method_offset[__fn_visitExprCallArgument] = adapt_field_offset_ex("visitExprCallArgument",info,index);
      _das_class_method_offset[__fn_preVisitExprNullCoalescing] = adapt_field_offset_ex("preVisitExprNullCoalescing",info,index);
      _das_class_method_offset[__fn_visitExprNullCoalescing] = adapt_field_offset_ex("visitExprNullCoalescing",info,index);
      _das_class_method_offset[__fn_preVisitExprNullCoalescingDefault] = adapt_field_offset_ex("preVisitExprNullCoalescingDefault",info,index);
      _das_class_method_offset[__fn_preVisitExprAt] = adapt_field_offset_ex("preVisitExprAt",info,index);
      _das_class_method_offset[__fn_visitExprAt] = adapt_field_offset_ex("visitExprAt",info,index);
      _das_class_method_offset[__fn_preVisitExprAtIndex] = adapt_field_offset_ex("preVisitExprAtIndex",info,index);
      _das_class_method_offset[__fn_preVisitExprSafeAt] = adapt_field_offset_ex("preVisitExprSafeAt",info,index);
      _das_class_method_offset[__fn_visitExprSafeAt] = adapt_field_offset_ex("visitExprSafeAt",info,index);
      _das_class_method_offset[__fn_preVisitExprSafeAtIndex] = adapt_field_offset_ex("preVisitExprSafeAtIndex",info,index);
      _das_class_method_offset[__fn_preVisitExprIs] = adapt_field_offset_ex("preVisitExprIs",info,index);
      _das_class_method_offset[__fn_visitExprIs] = adapt_field_offset_ex("visitExprIs",info,index);
      _das_class_method_offset[__fn_preVisitExprIsType] = adapt_field_offset_ex("preVisitExprIsType",info,index);
      _das_class_method_offset[__fn_preVisitExprOp2] = adapt_field_offset_ex("preVisitExprOp2",info,index);
      _das_class_method_offset[__fn_visitExprOp2] = adapt_field_offset_ex("visitExprOp2",info,index);
      _das_class_method_offset[__fn_preVisitExprOp2Right] = adapt_field_offset_ex("preVisitExprOp2Right",info,index);
      _das_class_method_offset[__fn_preVisitExprOp3] = adapt_field_offset_ex("preVisitExprOp3",info,index);
      _das_class_method_offset[__fn_visitExprOp3] = adapt_field_offset_ex("visitExprOp3",info,index);
      _das_class_method_offset[__fn_preVisitExprOp3Left] = adapt_field_offset_ex("preVisitExprOp3Left",info,index);
      _das_class_method_offset[__fn_preVisitExprOp3Right] = adapt_field_offset_ex("preVisitExprOp3Right",info,index);
      _das_class_method_offset[__fn_preVisitExprCopy] = adapt_field_offset_ex("preVisitExprCopy",info,index);
      _das_class_method_offset[__fn_visitExprCopy] = adapt_field_offset_ex("visitExprCopy",info,index);
      _das_class_method_offset[__fn_preVisitExprCopyRight] = adapt_field_offset_ex("preVisitExprCopyRight",info,index);
      _das_class_method_offset[__fn_preVisitExprMove] = adapt_field_offset_ex("preVisitExprMove",info,index);
      _das_class_method_offset[__fn_visitExprMove] = adapt_field_offset_ex("visitExprMove",info,index);
      _das_class_method_offset[__fn_preVisitExprMoveRight] = adapt_field_offset_ex("preVisitExprMoveRight",info,index);
      _das_class_method_offset[__fn_preVisitExprClone] = adapt_field_offset_ex("preVisitExprClone",info,index);
      _das_class_method_offset[__fn_visitExprClone] = adapt_field_offset_ex("visitExprClone",info,index);
      _das_class_method_offset[__fn_preVisitExprCloneRight] = adapt_field_offset_ex("preVisitExprCloneRight",info,index);
      _das_class_method_offset[__fn_canVisitWithAliasSubexpression] = adapt_field_offset_ex("canVisitWithAliasSubexpression",info,index);
      _das_class_method_offset[__fn_preVisitExprAssume] = adapt_field_offset_ex("preVisitExprAssume",info,index);
      _das_class_method_offset[__fn_visitExprAssume] = adapt_field_offset_ex("visitExprAssume",info,index);
      _das_class_method_offset[__fn_preVisitExprWith] = adapt_field_offset_ex("preVisitExprWith",info,index);
      _das_class_method_offset[__fn_visitExprWith] = adapt_field_offset_ex("visitExprWith",info,index);
      _das_class_method_offset[__fn_preVisitExprWithBody] = adapt_field_offset_ex("preVisitExprWithBody",info,index);
      _das_class_method_offset[__fn_preVisitExprWhile] = adapt_field_offset_ex("preVisitExprWhile",info,index);
      _das_class_method_offset[__fn_visitExprWhile] = adapt_field_offset_ex("visitExprWhile",info,index);
      _das_class_method_offset[__fn_preVisitExprWhileBody] = adapt_field_offset_ex("preVisitExprWhileBody",info,index);
      _das_class_method_offset[__fn_preVisitExprTryCatch] = adapt_field_offset_ex("preVisitExprTryCatch",info,index);
      _das_class_method_offset[__fn_visitExprTryCatch] = adapt_field_offset_ex("visitExprTryCatch",info,index);
      _das_class_method_offset[__fn_preVisitExprTryCatchCatch] = adapt_field_offset_ex("preVisitExprTryCatchCatch",info,index);
      _das_class_method_offset[__fn_preVisitExprIfThenElse] = adapt_field_offset_ex("preVisitExprIfThenElse",info,index);
      _das_class_method_offset[__fn_visitExprIfThenElse] = adapt_field_offset_ex("visitExprIfThenElse",info,index);
      _das_class_method_offset[__fn_preVisitExprIfThenElseIfBlock] = adapt_field_offset_ex("preVisitExprIfThenElseIfBlock",info,index);
      _das_class_method_offset[__fn_preVisitExprIfThenElseElseBlock] = adapt_field_offset_ex("preVisitExprIfThenElseElseBlock",info,index);
      _das_class_method_offset[__fn_preVisitExprFor] = adapt_field_offset_ex("preVisitExprFor",info,index);
      _das_class_method_offset[__fn_visitExprFor] = adapt_field_offset_ex("visitExprFor",info,index);
      _das_class_method_offset[__fn_preVisitExprForVariable] = adapt_field_offset_ex("preVisitExprForVariable",info,index);
      _das_class_method_offset[__fn_visitExprForVariable] = adapt_field_offset_ex("visitExprForVariable",info,index);
      _das_class_method_offset[__fn_preVisitExprForSource] = adapt_field_offset_ex("preVisitExprForSource",info,index);
      _das_class_method_offset[__fn_visitExprForSource] = adapt_field_offset_ex("visitExprForSource",info,index);
      _das_class_method_offset[__fn_preVisitExprForStack] = adapt_field_offset_ex("preVisitExprForStack",info,index);
      _das_class_method_offset[__fn_preVisitExprForBody] = adapt_field_offset_ex("preVisitExprForBody",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeVariant] = adapt_field_offset_ex("preVisitExprMakeVariant",info,index);
      _das_class_method_offset[__fn_visitExprMakeVariant] = adapt_field_offset_ex("visitExprMakeVariant",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeVariantField] = adapt_field_offset_ex("preVisitExprMakeVariantField",info,index);
      _das_class_method_offset[__fn_visitExprMakeVariantField] = adapt_field_offset_ex("visitExprMakeVariantField",info,index);
      _das_class_method_offset[__fn_canVisitMakeStructBody] = adapt_field_offset_ex("canVisitMakeStructBody",info,index);
      _das_class_method_offset[__fn_canVisitMakeStructBlock] = adapt_field_offset_ex("canVisitMakeStructBlock",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeStruct] = adapt_field_offset_ex("preVisitExprMakeStruct",info,index);
      _das_class_method_offset[__fn_visitExprMakeStruct] = adapt_field_offset_ex("visitExprMakeStruct",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeStructIndex] = adapt_field_offset_ex("preVisitExprMakeStructIndex",info,index);
      _das_class_method_offset[__fn_visitExprMakeStructIndex] = adapt_field_offset_ex("visitExprMakeStructIndex",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeStructField] = adapt_field_offset_ex("preVisitExprMakeStructField",info,index);
      _das_class_method_offset[__fn_visitExprMakeStructField] = adapt_field_offset_ex("visitExprMakeStructField",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeArray] = adapt_field_offset_ex("preVisitExprMakeArray",info,index);
      _das_class_method_offset[__fn_visitExprMakeArray] = adapt_field_offset_ex("visitExprMakeArray",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeArrayIndex] = adapt_field_offset_ex("preVisitExprMakeArrayIndex",info,index);
      _das_class_method_offset[__fn_visitExprMakeArrayIndex] = adapt_field_offset_ex("visitExprMakeArrayIndex",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeTuple] = adapt_field_offset_ex("preVisitExprMakeTuple",info,index);
      _das_class_method_offset[__fn_visitExprMakeTuple] = adapt_field_offset_ex("visitExprMakeTuple",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeTupleIndex] = adapt_field_offset_ex("preVisitExprMakeTupleIndex",info,index);
      _das_class_method_offset[__fn_visitExprMakeTupleIndex] = adapt_field_offset_ex("visitExprMakeTupleIndex",info,index);
      _das_class_method_offset[__fn_preVisitExprArrayComprehension] = adapt_field_offset_ex("preVisitExprArrayComprehension",info,index);
      _das_class_method_offset[__fn_visitExprArrayComprehension] = adapt_field_offset_ex("visitExprArrayComprehension",info,index);
      _das_class_method_offset[__fn_preVisitExprArrayComprehensionSubexpr] = adapt_field_offset_ex("preVisitExprArrayComprehensionSubexpr",info,index);
      _das_class_method_offset[__fn_preVisitExprArrayComprehensionWhere] = adapt_field_offset_ex("preVisitExprArrayComprehensionWhere",info,index);
      _das_class_method_offset[__fn_preVisitExprTypeInfo] = adapt_field_offset_ex("preVisitExprTypeInfo",info,index);
      _das_class_method_offset[__fn_visitExprTypeInfo] = adapt_field_offset_ex("visitExprTypeInfo",info,index);
      _das_class_method_offset[__fn_preVisitExprPtr2Ref] = adapt_field_offset_ex("preVisitExprPtr2Ref",info,index);
      _das_class_method_offset[__fn_visitExprPtr2Ref] = adapt_field_offset_ex("visitExprPtr2Ref",info,index);
      _das_class_method_offset[__fn_preVisitExprLabel] = adapt_field_offset_ex("preVisitExprLabel",info,index);
      _das_class_method_offset[__fn_visitExprLabel] = adapt_field_offset_ex("visitExprLabel",info,index);
      _das_class_method_offset[__fn_preVisitExprGoto] = adapt_field_offset_ex("preVisitExprGoto",info,index);
      _das_class_method_offset[__fn_visitExprGoto] = adapt_field_offset_ex("visitExprGoto",info,index);
      _das_class_method_offset[__fn_preVisitExprRef2Value] = adapt_field_offset_ex("preVisitExprRef2Value",info,index);
      _das_class_method_offset[__fn_visitExprRef2Value] = adapt_field_offset_ex("visitExprRef2Value",info,index);
      _das_class_method_offset[__fn_preVisitExprRef2Ptr] = adapt_field_offset_ex("preVisitExprRef2Ptr",info,index);
      _das_class_method_offset[__fn_visitExprRef2Ptr] = adapt_field_offset_ex("visitExprRef2Ptr",info,index);
      _das_class_method_offset[__fn_preVisitExprAddr] = adapt_field_offset_ex("preVisitExprAddr",info,index);
      _das_class_method_offset[__fn_visitExprAddr] = adapt_field_offset_ex("visitExprAddr",info,index);
      _das_class_method_offset[__fn_preVisitExprAssert] = adapt_field_offset_ex("preVisitExprAssert",info,index);
      _das_class_method_offset[__fn_visitExprAssert] = adapt_field_offset_ex("visitExprAssert",info,index);
      _das_class_method_offset[__fn_preVisitExprStaticAssert] = adapt_field_offset_ex("preVisitExprStaticAssert",info,index);
      _das_class_method_offset[__fn_visitExprStaticAssert] = adapt_field_offset_ex("visitExprStaticAssert",info,index);
      _das_class_method_offset[__fn_preVisitExprQuote] = adapt_field_offset_ex("preVisitExprQuote",info,index);
      _das_class_method_offset[__fn_visitExprQuote] = adapt_field_offset_ex("visitExprQuote",info,index);
      _das_class_method_offset[__fn_preVisitExprDebug] = adapt_field_offset_ex("preVisitExprDebug",info,index);
      _das_class_method_offset[__fn_visitExprDebug] = adapt_field_offset_ex("visitExprDebug",info,index);
      _das_class_method_offset[__fn_preVisitExprInvoke] = adapt_field_offset_ex("preVisitExprInvoke",info,index);
      _das_class_method_offset[__fn_visitExprInvoke] = adapt_field_offset_ex("visitExprInvoke",info,index);
      _das_class_method_offset[__fn_preVisitExprErase] = adapt_field_offset_ex("preVisitExprErase",info,index);
      _das_class_method_offset[__fn_visitExprErase] = adapt_field_offset_ex("visitExprErase",info,index);
      _das_class_method_offset[__fn_preVisitExprSetInsert] = adapt_field_offset_ex("preVisitExprSetInsert",info,index);
      _das_class_method_offset[__fn_visitExprSetInsert] = adapt_field_offset_ex("visitExprSetInsert",info,index);
      _das_class_method_offset[__fn_preVisitExprFind] = adapt_field_offset_ex("preVisitExprFind",info,index);
      _das_class_method_offset[__fn_visitExprFind] = adapt_field_offset_ex("visitExprFind",info,index);
      _das_class_method_offset[__fn_preVisitExprKeyExists] = adapt_field_offset_ex("preVisitExprKeyExists",info,index);
      _das_class_method_offset[__fn_visitExprKeyExists] = adapt_field_offset_ex("visitExprKeyExists",info,index);
      _das_class_method_offset[__fn_preVisitExprAscend] = adapt_field_offset_ex("preVisitExprAscend",info,index);
      _das_class_method_offset[__fn_visitExprAscend] = adapt_field_offset_ex("visitExprAscend",info,index);
      _das_class_method_offset[__fn_preVisitExprCast] = adapt_field_offset_ex("preVisitExprCast",info,index);
      _das_class_method_offset[__fn_visitExprCast] = adapt_field_offset_ex("visitExprCast",info,index);
      _das_class_method_offset[__fn_preVisitExprDelete] = adapt_field_offset_ex("preVisitExprDelete",info,index);
      _das_class_method_offset[__fn_visitExprDelete] = adapt_field_offset_ex("visitExprDelete",info,index);
      _das_class_method_offset[__fn_preVisitExprVar] = adapt_field_offset_ex("preVisitExprVar",info,index);
      _das_class_method_offset[__fn_visitExprVar] = adapt_field_offset_ex("visitExprVar",info,index);
      _das_class_method_offset[__fn_preVisitExprTag] = adapt_field_offset_ex("preVisitExprTag",info,index);
      _das_class_method_offset[__fn_preVisitExprTagValue] = adapt_field_offset_ex("preVisitExprTagValue",info,index);
      _das_class_method_offset[__fn_visitExprTag] = adapt_field_offset_ex("visitExprTag",info,index);
      _das_class_method_offset[__fn_preVisitExprField] = adapt_field_offset_ex("preVisitExprField",info,index);
      _das_class_method_offset[__fn_visitExprField] = adapt_field_offset_ex("visitExprField",info,index);
      _das_class_method_offset[__fn_preVisitExprSafeField] = adapt_field_offset_ex("preVisitExprSafeField",info,index);
      _das_class_method_offset[__fn_visitExprSafeField] = adapt_field_offset_ex("visitExprSafeField",info,index);
      _das_class_method_offset[__fn_preVisitExprSwizzle] = adapt_field_offset_ex("preVisitExprSwizzle",info,index);
      _das_class_method_offset[__fn_visitExprSwizzle] = adapt_field_offset_ex("visitExprSwizzle",info,index);
      _das_class_method_offset[__fn_preVisitExprIsVariant] = adapt_field_offset_ex("preVisitExprIsVariant",info,index);
      _das_class_method_offset[__fn_visitExprIsVariant] = adapt_field_offset_ex("visitExprIsVariant",info,index);
      _das_class_method_offset[__fn_preVisitExprAsVariant] = adapt_field_offset_ex("preVisitExprAsVariant",info,index);
      _das_class_method_offset[__fn_visitExprAsVariant] = adapt_field_offset_ex("visitExprAsVariant",info,index);
      _das_class_method_offset[__fn_preVisitExprSafeAsVariant] = adapt_field_offset_ex("preVisitExprSafeAsVariant",info,index);
      _das_class_method_offset[__fn_visitExprSafeAsVariant] = adapt_field_offset_ex("visitExprSafeAsVariant",info,index);
      _das_class_method_offset[__fn_preVisitExprOp1] = adapt_field_offset_ex("preVisitExprOp1",info,index);
      _das_class_method_offset[__fn_visitExprOp1] = adapt_field_offset_ex("visitExprOp1",info,index);
      _das_class_method_offset[__fn_preVisitExprReturn] = adapt_field_offset_ex("preVisitExprReturn",info,index);
      _das_class_method_offset[__fn_visitExprReturn] = adapt_field_offset_ex("visitExprReturn",info,index);
      _das_class_method_offset[__fn_preVisitExprYield] = adapt_field_offset_ex("preVisitExprYield",info,index);
      _das_class_method_offset[__fn_visitExprYield] = adapt_field_offset_ex("visitExprYield",info,index);
      _das_class_method_offset[__fn_preVisitExprBreak] = adapt_field_offset_ex("preVisitExprBreak",info,index);
      _das_class_method_offset[__fn_visitExprBreak] = adapt_field_offset_ex("visitExprBreak",info,index);
      _das_class_method_offset[__fn_preVisitExprContinue] = adapt_field_offset_ex("preVisitExprContinue",info,index);
      _das_class_method_offset[__fn_visitExprContinue] = adapt_field_offset_ex("visitExprContinue",info,index);
      _das_class_method_offset[__fn_canVisitMakeBlockBody] = adapt_field_offset_ex("canVisitMakeBlockBody",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeBlock] = adapt_field_offset_ex("preVisitExprMakeBlock",info,index);
      _das_class_method_offset[__fn_visitExprMakeBlock] = adapt_field_offset_ex("visitExprMakeBlock",info,index);
      _das_class_method_offset[__fn_preVisitExprMakeGenerator] = adapt_field_offset_ex("preVisitExprMakeGenerator",info,index);
      _das_class_method_offset[__fn_visitExprMakeGenerator] = adapt_field_offset_ex("visitExprMakeGenerator",info,index);
      _das_class_method_offset[__fn_preVisitExprMemZero] = adapt_field_offset_ex("preVisitExprMemZero",info,index);
      _das_class_method_offset[__fn_visitExprMemZero] = adapt_field_offset_ex("visitExprMemZero",info,index);
      _das_class_method_offset[__fn_preVisitExprConst] = adapt_field_offset_ex("preVisitExprConst",info,index);
      _das_class_method_offset[__fn_visitExprConst] = adapt_field_offset_ex("visitExprConst",info,index);
      _das_class_method_offset[__fn_preVisitExprConstPtr] = adapt_field_offset_ex("preVisitExprConstPtr",info,index);
      _das_class_method_offset[__fn_visitExprConstPtr] = adapt_field_offset_ex("visitExprConstPtr",info,index);
      _das_class_method_offset[__fn_preVisitExprConstEnumeration] = adapt_field_offset_ex("preVisitExprConstEnumeration",info,index);
      _das_class_method_offset[__fn_visitExprConstEnumeration] = adapt_field_offset_ex("visitExprConstEnumeration",info,index);
      _das_class_method_offset[__fn_preVisitExprConstBitfield] = adapt_field_offset_ex("preVisitExprConstBitfield",info,index);
      _das_class_method_offset[__fn_visitExprConstBitfield] = adapt_field_offset_ex("visitExprConstBitfield",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt8] = adapt_field_offset_ex("preVisitExprConstInt8",info,index);
      _das_class_method_offset[__fn_visitExprConstInt8] = adapt_field_offset_ex("visitExprConstInt8",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt16] = adapt_field_offset_ex("preVisitExprConstInt16",info,index);
      _das_class_method_offset[__fn_visitExprConstInt16] = adapt_field_offset_ex("visitExprConstInt16",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt64] = adapt_field_offset_ex("preVisitExprConstInt64",info,index);
      _das_class_method_offset[__fn_visitExprConstInt64] = adapt_field_offset_ex("visitExprConstInt64",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt] = adapt_field_offset_ex("preVisitExprConstInt",info,index);
      _das_class_method_offset[__fn_visitExprConstInt] = adapt_field_offset_ex("visitExprConstInt",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt2] = adapt_field_offset_ex("preVisitExprConstInt2",info,index);
      _das_class_method_offset[__fn_visitExprConstInt2] = adapt_field_offset_ex("visitExprConstInt2",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt3] = adapt_field_offset_ex("preVisitExprConstInt3",info,index);
      _das_class_method_offset[__fn_visitExprConstInt3] = adapt_field_offset_ex("visitExprConstInt3",info,index);
      _das_class_method_offset[__fn_preVisitExprConstInt4] = adapt_field_offset_ex("preVisitExprConstInt4",info,index);
      _das_class_method_offset[__fn_visitExprConstInt4] = adapt_field_offset_ex("visitExprConstInt4",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt8] = adapt_field_offset_ex("preVisitExprConstUInt8",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt8] = adapt_field_offset_ex("visitExprConstUInt8",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt16] = adapt_field_offset_ex("preVisitExprConstUInt16",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt16] = adapt_field_offset_ex("visitExprConstUInt16",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt64] = adapt_field_offset_ex("preVisitExprConstUInt64",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt64] = adapt_field_offset_ex("visitExprConstUInt64",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt] = adapt_field_offset_ex("preVisitExprConstUInt",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt] = adapt_field_offset_ex("visitExprConstUInt",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt2] = adapt_field_offset_ex("preVisitExprConstUInt2",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt2] = adapt_field_offset_ex("visitExprConstUInt2",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt3] = adapt_field_offset_ex("preVisitExprConstUInt3",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt3] = adapt_field_offset_ex("visitExprConstUInt3",info,index);
      _das_class_method_offset[__fn_preVisitExprConstUInt4] = adapt_field_offset_ex("preVisitExprConstUInt4",info,index);
      _das_class_method_offset[__fn_visitExprConstUInt4] = adapt_field_offset_ex("visitExprConstUInt4",info,index);
      _das_class_method_offset[__fn_preVisitExprConstRange] = adapt_field_offset_ex("preVisitExprConstRange",info,index);
      _das_class_method_offset[__fn_visitExprConstRange] = adapt_field_offset_ex("visitExprConstRange",info,index);
      _das_class_method_offset[__fn_preVisitExprConstURange] = adapt_field_offset_ex("preVisitExprConstURange",info,index);
      _das_class_method_offset[__fn_visitExprConstURange] = adapt_field_offset_ex("visitExprConstURange",info,index);
      _das_class_method_offset[__fn_preVisitExprConstRange64] = adapt_field_offset_ex("preVisitExprConstRange64",info,index);
      _das_class_method_offset[__fn_visitExprConstRange64] = adapt_field_offset_ex("visitExprConstRange64",info,index);
      _das_class_method_offset[__fn_preVisitExprConstURange64] = adapt_field_offset_ex("preVisitExprConstURange64",info,index);
      _das_class_method_offset[__fn_visitExprConstURange64] = adapt_field_offset_ex("visitExprConstURange64",info,index);
      _das_class_method_offset[__fn_preVisitExprConstBool] = adapt_field_offset_ex("preVisitExprConstBool",info,index);
      _das_class_method_offset[__fn_visitExprConstBool] = adapt_field_offset_ex("visitExprConstBool",info,index);
      _das_class_method_offset[__fn_preVisitExprConstFloat] = adapt_field_offset_ex("preVisitExprConstFloat",info,index);
      _das_class_method_offset[__fn_visitExprConstFloat] = adapt_field_offset_ex("visitExprConstFloat",info,index);
      _das_class_method_offset[__fn_preVisitExprConstFloat2] = adapt_field_offset_ex("preVisitExprConstFloat2",info,index);
      _das_class_method_offset[__fn_visitExprConstFloat2] = adapt_field_offset_ex("visitExprConstFloat2",info,index);
      _das_class_method_offset[__fn_preVisitExprConstFloat3] = adapt_field_offset_ex("preVisitExprConstFloat3",info,index);
      _das_class_method_offset[__fn_visitExprConstFloat3] = adapt_field_offset_ex("visitExprConstFloat3",info,index);
      _das_class_method_offset[__fn_preVisitExprConstFloat4] = adapt_field_offset_ex("preVisitExprConstFloat4",info,index);
      _das_class_method_offset[__fn_visitExprConstFloat4] = adapt_field_offset_ex("visitExprConstFloat4",info,index);
      _das_class_method_offset[__fn_preVisitExprConstString] = adapt_field_offset_ex("preVisitExprConstString",info,index);
      _das_class_method_offset[__fn_visitExprConstString] = adapt_field_offset_ex("visitExprConstString",info,index);
      _das_class_method_offset[__fn_preVisitExprConstDouble] = adapt_field_offset_ex("preVisitExprConstDouble",info,index);
      _das_class_method_offset[__fn_visitExprConstDouble] = adapt_field_offset_ex("visitExprConstDouble",info,index);
      _das_class_method_offset[__fn_preVisitExprFakeContext] = adapt_field_offset_ex("preVisitExprFakeContext",info,index);
      _das_class_method_offset[__fn_visitExprFakeContext] = adapt_field_offset_ex("visitExprFakeContext",info,index);
      _das_class_method_offset[__fn_preVisitExprFakeLineInfo] = adapt_field_offset_ex("preVisitExprFakeLineInfo",info,index);
      _das_class_method_offset[__fn_visitExprFakeLineInfo] = adapt_field_offset_ex("visitExprFakeLineInfo",info,index);
      _das_class_method_offset[__fn_preVisitExprReader] = adapt_field_offset_ex("preVisitExprReader",info,index);
      _das_class_method_offset[__fn_visitExprReader] = adapt_field_offset_ex("visitExprReader",info,index);
      _das_class_method_offset[__fn_preVisitExprUnsafe] = adapt_field_offset_ex("preVisitExprUnsafe",info,index);
      _das_class_method_offset[__fn_visitExprUnsafe] = adapt_field_offset_ex("visitExprUnsafe",info,index);
      _das_class_method_offset[__fn_preVisitExprCallMacro] = adapt_field_offset_ex("preVisitExprCallMacro",info,index);
      _das_class_method_offset[__fn_visitExprCallMacro] = adapt_field_offset_ex("visitExprCallMacro",info,index);
  }
  __forceinline Func get_preVisitProgram ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitProgram]);
  }
  __forceinline void invoke_preVisitProgram ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_visitProgram ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitProgram]);
  }
  __forceinline void invoke_visitProgram ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  porg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,porg);
  }
  __forceinline Func get_preVisitProgramBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitProgramBody]);
  }
  __forceinline void invoke_preVisitProgramBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod);
  }
  __forceinline Func get_preVisitModule ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitModule]);
  }
  __forceinline void invoke_preVisitModule ( Context * __context__, Func __funcCall__, void * self, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,Module * const >
        (__context__,nullptr,__funcCall__,
          self,mod);
  }
  __forceinline Func get_visitModule ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitModule]);
  }
  __forceinline void invoke_visitModule ( Context * __context__, Func __funcCall__, void * self, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,Module * const >
        (__context__,nullptr,__funcCall__,
          self,mod);
  }
  __forceinline Func get_preVisitExprTypeDecl ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTypeDecl]);
  }
  __forceinline void invoke_preVisitExprTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeDecl> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTypeDecl ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprTypeDecl]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeDecl> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitTypeDecl ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitTypeDecl]);
  }
  __forceinline void invoke_preVisitTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,typ);
  }
  __forceinline Func get_visitTypeDecl ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitTypeDecl]);
  }
  __forceinline smart_ptr<TypeDecl> invoke_visitTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,typ).marshal();
  }
  __forceinline Func get_preVisitAlias ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitAlias]);
  }
  __forceinline void invoke_preVisitAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ, das::string const  & name ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<TypeDecl> const ,das::string const  &>
        (__context__,nullptr,__funcCall__,
          self,typ,name);
  }
  __forceinline Func get_visitAlias ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitAlias]);
  }
  __forceinline smart_ptr<TypeDecl> invoke_visitAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ, das::string const  & name ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,smart_ptr_raw<TypeDecl> const ,das::string const  &>
        (__context__,nullptr,__funcCall__,
          self,typ,name).marshal();
  }
  __forceinline Func get_canVisitEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitEnumeration]);
  }
  __forceinline bool invoke_canVisitEnumeration ( Context * __context__, Func __funcCall__, void * self, Enumeration * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Enumeration * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitEnumeration]);
  }
  __forceinline void invoke_preVisitEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Enumeration> const >
        (__context__,nullptr,__funcCall__,
          self,enu);
  }
  __forceinline Func get_preVisitEnumerationValue ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitEnumerationValue]);
  }
  __forceinline void invoke_preVisitEnumerationValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu, das::string const  & name, smart_ptr_raw<Expression> const  value, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Enumeration> const ,das::string const  &,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,enu,name,value,last);
  }
  __forceinline Func get_visitEnumerationValue ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitEnumerationValue]);
  }
  __forceinline smart_ptr<Expression> invoke_visitEnumerationValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu, das::string const  & name, smart_ptr_raw<Expression> const  value, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Enumeration> const ,das::string const  &,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,enu,name,value,last).marshal();
  }
  __forceinline Func get_visitEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitEnumeration]);
  }
  __forceinline smart_ptr<Enumeration> invoke_visitEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu ) const {
    return das_invoke_function<smart_ptr_raw<Enumeration>>::invoke
      <void *,smart_ptr_raw<Enumeration> const >
        (__context__,nullptr,__funcCall__,
          self,enu).marshal();
  }
  __forceinline Func get_canVisitStructure ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitStructure]);
  }
  __forceinline bool invoke_canVisitStructure ( Context * __context__, Func __funcCall__, void * self, Structure * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Structure * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitStructure ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitStructure]);
  }
  __forceinline void invoke_preVisitStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const >
        (__context__,nullptr,__funcCall__,
          self,str);
  }
  __forceinline Func get_preVisitStructureField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitStructureField]);
  }
  __forceinline void invoke_preVisitStructureField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str, Structure::FieldDeclaration const  & decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,str,decl,last);
  }
  __forceinline Func get_visitStructureField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitStructureField]);
  }
  __forceinline void invoke_visitStructureField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str, Structure::FieldDeclaration const  & decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,str,decl,last);
  }
  __forceinline Func get_visitStructure ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitStructure]);
  }
  __forceinline smart_ptr<Structure> invoke_visitStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str ) const {
    return das_invoke_function<smart_ptr_raw<Structure>>::invoke
      <void *,smart_ptr_raw<Structure> const >
        (__context__,nullptr,__funcCall__,
          self,str).marshal();
  }
  __forceinline Func get_canVisitFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitFunction]);
  }
  __forceinline bool invoke_canVisitFunction ( Context * __context__, Func __funcCall__, void * self, Function * const  fun ) const {
    return das_invoke_function<bool>::invoke
      <void *,Function * const >
        (__context__,nullptr,__funcCall__,
          self,fun);
  }
  __forceinline Func get_canVisitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitFunctionArgumentInit]);
  }
  __forceinline bool invoke_canVisitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, Function * const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    return das_invoke_function<bool>::invoke
      <void *,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value);
  }
  __forceinline Func get_preVisitFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitFunction]);
  }
  __forceinline void invoke_preVisitFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const >
        (__context__,nullptr,__funcCall__,
          self,fun);
  }
  __forceinline Func get_visitFunction ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitFunction]);
  }
  __forceinline smart_ptr<Function> invoke_visitFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun ) const {
    return das_invoke_function<smart_ptr_raw<Function>>::invoke
      <void *,smart_ptr_raw<Function> const >
        (__context__,nullptr,__funcCall__,
          self,fun).marshal();
  }
  __forceinline Func get_preVisitFunctionArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitFunctionArgument]);
  }
  __forceinline void invoke_preVisitFunctionArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,fun,arg,lastArg);
  }
  __forceinline Func get_visitFunctionArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitFunctionArgument]);
  }
  __forceinline smart_ptr<Variable> invoke_visitFunctionArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,fun,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitFunctionArgumentInit]);
  }
  __forceinline void invoke_preVisitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value);
  }
  __forceinline Func get_visitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitFunctionArgumentInit]);
  }
  __forceinline smart_ptr<Expression> invoke_visitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value).marshal();
  }
  __forceinline Func get_preVisitFunctionBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitFunctionBody]);
  }
  __forceinline void invoke_preVisitFunctionBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,expr);
  }
  __forceinline Func get_visitFunctionBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitFunctionBody]);
  }
  __forceinline smart_ptr<Expression> invoke_visitFunctionBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,expr).marshal();
  }
  __forceinline Func get_preVisitExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExpression]);
  }
  __forceinline void invoke_preVisitExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExpression]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlock]);
  }
  __forceinline void invoke_preVisitExprBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_visitExprBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlock]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk).marshal();
  }
  __forceinline Func get_preVisitExprBlockArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlockArgument]);
  }
  __forceinline void invoke_preVisitExprBlockArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,blk,arg,lastArg);
  }
  __forceinline Func get_visitExprBlockArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlockArgument]);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprBlockArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,blk,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitExprBlockArgumentInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlockArgumentInit]);
  }
  __forceinline void invoke_preVisitExprBlockArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr);
  }
  __forceinline Func get_visitExprBlockArgumentInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlockArgumentInit]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlockExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlockExpression]);
  }
  __forceinline void invoke_preVisitExprBlockExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr);
  }
  __forceinline Func get_visitExprBlockExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlockExpression]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlockFinal ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlockFinal]);
  }
  __forceinline void invoke_preVisitExprBlockFinal ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_visitExprBlockFinal ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlockFinal]);
  }
  __forceinline void invoke_visitExprBlockFinal ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_preVisitExprBlockFinalExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBlockFinalExpression]);
  }
  __forceinline void invoke_preVisitExprBlockFinalExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr);
  }
  __forceinline Func get_visitExprBlockFinalExpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBlockFinalExpression]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockFinalExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr).marshal();
  }
  __forceinline Func get_preVisitExprLet ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLet]);
  }
  __forceinline void invoke_preVisitExprLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLet ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLet]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLet> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLetVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLetVariable]);
  }
  __forceinline void invoke_preVisitExprLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,lastArg);
  }
  __forceinline Func get_visitExprLetVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLetVariable]);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitExprLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLetVariableInit]);
  }
  __forceinline void invoke_preVisitExprLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr);
  }
  __forceinline Func get_visitExprLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLetVariableInit]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr).marshal();
  }
  __forceinline Func get_canVisitGlobalVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitGlobalVariable]);
  }
  __forceinline bool invoke_canVisitGlobalVariable ( Context * __context__, Func __funcCall__, void * self, Variable * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Variable * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitGlobalLet ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitGlobalLet]);
  }
  __forceinline void invoke_preVisitGlobalLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_visitGlobalLet ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitGlobalLet]);
  }
  __forceinline void invoke_visitGlobalLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_preVisitGlobalLetVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitGlobalLetVariable]);
  }
  __forceinline void invoke_preVisitGlobalLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,arg,lastArg);
  }
  __forceinline Func get_visitGlobalLetVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitGlobalLetVariable]);
  }
  __forceinline smart_ptr<Variable> invoke_visitGlobalLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitGlobalLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitGlobalLetVariableInit]);
  }
  __forceinline void invoke_preVisitGlobalLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,arg,expr);
  }
  __forceinline Func get_visitGlobalLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitGlobalLetVariableInit]);
  }
  __forceinline smart_ptr<Expression> invoke_visitGlobalLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,arg,expr).marshal();
  }
  __forceinline Func get_preVisitExprStringBuilder ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprStringBuilder]);
  }
  __forceinline void invoke_preVisitExprStringBuilder ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprStringBuilder ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprStringBuilder]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStringBuilder ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprStringBuilderElement ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprStringBuilderElement]);
  }
  __forceinline void invoke_preVisitExprStringBuilderElement ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr, smart_ptr_raw<Expression> const  elem, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,elem,last);
  }
  __forceinline Func get_visitExprStringBuilderElement ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprStringBuilderElement]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStringBuilderElement ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr, smart_ptr_raw<Expression> const  elem, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,elem,last).marshal();
  }
  __forceinline Func get_preVisitExprNew ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNew]);
  }
  __forceinline void invoke_preVisitExprNew ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNew> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNew ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprNew]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNew ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNew> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNewArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNewArgument]);
  }
  __forceinline void invoke_preVisitExprNewArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprNewArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprNewArgument]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNewArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprNamedCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNamedCall]);
  }
  __forceinline void invoke_preVisitExprNamedCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNamedCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprNamedCall]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNamedCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNamedCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNamedCallArgument]);
  }
  __forceinline void invoke_preVisitExprNamedCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr, smart_ptr_raw<MakeFieldDecl> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprNamedCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprNamedCallArgument]);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprNamedCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr, smart_ptr_raw<MakeFieldDecl> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprLooksLikeCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLooksLikeCall]);
  }
  __forceinline void invoke_preVisitExprLooksLikeCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLooksLikeCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLooksLikeCall]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLooksLikeCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLooksLikeCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLooksLikeCallArgument]);
  }
  __forceinline void invoke_preVisitExprLooksLikeCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprLooksLikeCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLooksLikeCallArgument]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLooksLikeCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_canVisitCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitCall]);
  }
  __forceinline bool invoke_canVisitCall ( Context * __context__, Func __funcCall__, void * self, ExprCall * const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,ExprCall * const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCall]);
  }
  __forceinline void invoke_preVisitExprCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCall ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprCall]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCallArgument]);
  }
  __forceinline void invoke_preVisitExprCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprCallArgument ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprCallArgument]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprNullCoalescing ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNullCoalescing]);
  }
  __forceinline void invoke_preVisitExprNullCoalescing ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNullCoalescing ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprNullCoalescing]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNullCoalescing ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNullCoalescingDefault ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprNullCoalescingDefault]);
  }
  __forceinline void invoke_preVisitExprNullCoalescingDefault ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr, smart_ptr_raw<Expression> const  defval ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,defval);
  }
  __forceinline Func get_preVisitExprAt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAt]);
  }
  __forceinline void invoke_preVisitExprAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAt]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAtIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAtIndex]);
  }
  __forceinline void invoke_preVisitExprAtIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr, smart_ptr_raw<Expression> const  index ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,index);
  }
  __forceinline Func get_preVisitExprSafeAt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSafeAt]);
  }
  __forceinline void invoke_preVisitExprSafeAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeAt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSafeAt]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeAtIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSafeAtIndex]);
  }
  __forceinline void invoke_preVisitExprSafeAtIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr, smart_ptr_raw<Expression> const  index ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,index);
  }
  __forceinline Func get_preVisitExprIs ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIs]);
  }
  __forceinline void invoke_preVisitExprIs ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIs> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIs ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprIs]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIs ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIs> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIsType ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIsType]);
  }
  __forceinline void invoke_preVisitExprIsType ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr, smart_ptr_raw<TypeDecl> const  typeDecl ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr,typeDecl);
  }
  __forceinline Func get_preVisitExprOp2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp2]);
  }
  __forceinline void invoke_preVisitExprOp2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprOp2]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp2Right ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp2Right]);
  }
  __forceinline void invoke_preVisitExprOp2Right ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprOp3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp3]);
  }
  __forceinline void invoke_preVisitExprOp3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprOp3]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp3Left ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp3Left]);
  }
  __forceinline void invoke_preVisitExprOp3Left ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr, smart_ptr_raw<Expression> const  left ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,left);
  }
  __forceinline Func get_preVisitExprOp3Right ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp3Right]);
  }
  __forceinline void invoke_preVisitExprOp3Right ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprCopy ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCopy]);
  }
  __forceinline void invoke_preVisitExprCopy ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCopy> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCopy ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprCopy]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCopy ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCopy> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCopyRight ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCopyRight]);
  }
  __forceinline void invoke_preVisitExprCopyRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprMove ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMove]);
  }
  __forceinline void invoke_preVisitExprMove ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMove> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMove ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMove]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMove ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMove> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMoveRight ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMoveRight]);
  }
  __forceinline void invoke_preVisitExprMoveRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprClone ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprClone]);
  }
  __forceinline void invoke_preVisitExprClone ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprClone> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprClone ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprClone]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprClone ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprClone> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCloneRight ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCloneRight]);
  }
  __forceinline void invoke_preVisitExprCloneRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_canVisitWithAliasSubexpression ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitWithAliasSubexpression]);
  }
  __forceinline bool invoke_canVisitWithAliasSubexpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprAssume ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAssume]);
  }
  __forceinline void invoke_preVisitExprAssume ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAssume ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAssume]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAssume ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWith ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprWith]);
  }
  __forceinline void invoke_preVisitExprWith ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWith> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprWith ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprWith]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprWith ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprWith> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWithBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprWithBody]);
  }
  __forceinline void invoke_preVisitExprWithBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprWhile ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprWhile]);
  }
  __forceinline void invoke_preVisitExprWhile ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWhile> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprWhile ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprWhile]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprWhile ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprWhile> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWhileBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprWhileBody]);
  }
  __forceinline void invoke_preVisitExprWhileBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprTryCatch ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTryCatch]);
  }
  __forceinline void invoke_preVisitExprTryCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTryCatch ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprTryCatch]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTryCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprTryCatchCatch ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTryCatchCatch]);
  }
  __forceinline void invoke_preVisitExprTryCatchCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprIfThenElse ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIfThenElse]);
  }
  __forceinline void invoke_preVisitExprIfThenElse ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIfThenElse ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprIfThenElse]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIfThenElse ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIfThenElseIfBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIfThenElseIfBlock]);
  }
  __forceinline void invoke_preVisitExprIfThenElseIfBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr, smart_ptr_raw<Expression> const  ifBlock ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,ifBlock);
  }
  __forceinline Func get_preVisitExprIfThenElseElseBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIfThenElseElseBlock]);
  }
  __forceinline void invoke_preVisitExprIfThenElseElseBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr, smart_ptr_raw<Expression> const  elseBlock ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,elseBlock);
  }
  __forceinline Func get_preVisitExprFor ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprFor]);
  }
  __forceinline void invoke_preVisitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFor ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprFor]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprForVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprForVariable]);
  }
  __forceinline void invoke_preVisitExprForVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Variable> const  svar, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,svar,last);
  }
  __forceinline Func get_visitExprForVariable ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprForVariable]);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprForVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Variable> const  svar, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,svar,last).marshal();
  }
  __forceinline Func get_preVisitExprForSource ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprForSource]);
  }
  __forceinline void invoke_preVisitExprForSource ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Expression> const  source, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,source,last);
  }
  __forceinline Func get_visitExprForSource ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprForSource]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprForSource ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Expression> const  source, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,source,last).marshal();
  }
  __forceinline Func get_preVisitExprForStack ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprForStack]);
  }
  __forceinline void invoke_preVisitExprForStack ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprForBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprForBody]);
  }
  __forceinline void invoke_preVisitExprForBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprMakeVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeVariant]);
  }
  __forceinline void invoke_preVisitExprMakeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeVariantField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeVariantField]);
  }
  __forceinline void invoke_preVisitExprMakeVariantField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last);
  }
  __forceinline Func get_visitExprMakeVariantField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeVariantField]);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprMakeVariantField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last).marshal();
  }
  __forceinline Func get_canVisitMakeStructBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitMakeStructBody]);
  }
  __forceinline bool invoke_canVisitMakeStructBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_canVisitMakeStructBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitMakeStructBlock]);
  }
  __forceinline bool invoke_canVisitMakeStructBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, smart_ptr_raw<Expression> const  blk ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,blk);
  }
  __forceinline Func get_preVisitExprMakeStruct ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeStruct]);
  }
  __forceinline void invoke_preVisitExprMakeStruct ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeStruct ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeStruct]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeStruct ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeStructIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeStructIndex]);
  }
  __forceinline void invoke_preVisitExprMakeStructIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,last);
  }
  __forceinline Func get_visitExprMakeStructIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeStructIndex]);
  }
  __forceinline void invoke_visitExprMakeStructIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,last);
  }
  __forceinline Func get_preVisitExprMakeStructField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeStructField]);
  }
  __forceinline void invoke_preVisitExprMakeStructField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last);
  }
  __forceinline Func get_visitExprMakeStructField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeStructField]);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprMakeStructField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last).marshal();
  }
  __forceinline Func get_preVisitExprMakeArray ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeArray]);
  }
  __forceinline void invoke_preVisitExprMakeArray ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeArray ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeArray]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeArray ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeArrayIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeArrayIndex]);
  }
  __forceinline void invoke_preVisitExprMakeArrayIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last);
  }
  __forceinline Func get_visitExprMakeArrayIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeArrayIndex]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeArrayIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last).marshal();
  }
  __forceinline Func get_preVisitExprMakeTuple ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeTuple]);
  }
  __forceinline void invoke_preVisitExprMakeTuple ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeTuple ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeTuple]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeTuple ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeTupleIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeTupleIndex]);
  }
  __forceinline void invoke_preVisitExprMakeTupleIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last);
  }
  __forceinline Func get_visitExprMakeTupleIndex ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeTupleIndex]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeTupleIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last).marshal();
  }
  __forceinline Func get_preVisitExprArrayComprehension ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprArrayComprehension]);
  }
  __forceinline void invoke_preVisitExprArrayComprehension ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprArrayComprehension ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprArrayComprehension]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprArrayComprehension ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprArrayComprehensionSubexpr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprArrayComprehensionSubexpr]);
  }
  __forceinline void invoke_preVisitExprArrayComprehensionSubexpr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr, smart_ptr_raw<Expression> const  subexrp ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,subexrp);
  }
  __forceinline Func get_preVisitExprArrayComprehensionWhere ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprArrayComprehensionWhere]);
  }
  __forceinline void invoke_preVisitExprArrayComprehensionWhere ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr, smart_ptr_raw<Expression> const  filter ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,filter);
  }
  __forceinline Func get_preVisitExprTypeInfo ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTypeInfo]);
  }
  __forceinline void invoke_preVisitExprTypeInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTypeInfo ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprTypeInfo]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTypeInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprPtr2Ref ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprPtr2Ref]);
  }
  __forceinline void invoke_preVisitExprPtr2Ref ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprPtr2Ref> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprPtr2Ref> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprPtr2Ref ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprPtr2Ref]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprPtr2Ref ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprPtr2Ref> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprPtr2Ref> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLabel ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprLabel]);
  }
  __forceinline void invoke_preVisitExprLabel ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLabel> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLabel> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLabel ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprLabel]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLabel ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLabel> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLabel> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprGoto ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprGoto]);
  }
  __forceinline void invoke_preVisitExprGoto ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprGoto> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprGoto> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprGoto ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprGoto]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprGoto ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprGoto> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprGoto> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprRef2Value ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprRef2Value]);
  }
  __forceinline void invoke_preVisitExprRef2Value ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Value> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprRef2Value> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprRef2Value ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprRef2Value]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprRef2Value ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Value> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprRef2Value> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprRef2Ptr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprRef2Ptr]);
  }
  __forceinline void invoke_preVisitExprRef2Ptr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Ptr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprRef2Ptr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprRef2Ptr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprRef2Ptr]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprRef2Ptr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Ptr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprRef2Ptr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAddr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAddr]);
  }
  __forceinline void invoke_preVisitExprAddr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAddr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAddr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAddr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAddr]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAddr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAddr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAddr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAssert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAssert]);
  }
  __forceinline void invoke_preVisitExprAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAssert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAssert]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprStaticAssert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprStaticAssert]);
  }
  __forceinline void invoke_preVisitExprStaticAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStaticAssert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStaticAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprStaticAssert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprStaticAssert]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStaticAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStaticAssert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStaticAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprQuote ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprQuote]);
  }
  __forceinline void invoke_preVisitExprQuote ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprQuote> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprQuote> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprQuote ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprQuote]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprQuote ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprQuote> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprQuote> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprDebug ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprDebug]);
  }
  __forceinline void invoke_preVisitExprDebug ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDebug> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprDebug> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprDebug ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprDebug]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprDebug ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDebug> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprDebug> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprInvoke ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprInvoke]);
  }
  __forceinline void invoke_preVisitExprInvoke ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprInvoke> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprInvoke> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprInvoke ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprInvoke]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprInvoke ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprInvoke> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprInvoke> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprErase ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprErase]);
  }
  __forceinline void invoke_preVisitExprErase ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprErase> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprErase> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprErase ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprErase]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprErase ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprErase> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprErase> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSetInsert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSetInsert]);
  }
  __forceinline void invoke_preVisitExprSetInsert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSetInsert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSetInsert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSetInsert ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSetInsert]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSetInsert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSetInsert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSetInsert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFind ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprFind]);
  }
  __forceinline void invoke_preVisitExprFind ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFind> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFind> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFind ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprFind]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFind ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFind> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFind> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprKeyExists ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprKeyExists]);
  }
  __forceinline void invoke_preVisitExprKeyExists ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprKeyExists> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprKeyExists> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprKeyExists ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprKeyExists]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprKeyExists ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprKeyExists> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprKeyExists> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAscend ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAscend]);
  }
  __forceinline void invoke_preVisitExprAscend ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAscend> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAscend> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAscend ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAscend]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAscend ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAscend> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAscend> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCast ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCast]);
  }
  __forceinline void invoke_preVisitExprCast ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCast> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCast> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCast ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprCast]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCast ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCast> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCast> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprDelete ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprDelete]);
  }
  __forceinline void invoke_preVisitExprDelete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDelete> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprDelete> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprDelete ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprDelete]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprDelete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDelete> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprDelete> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprVar ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprVar]);
  }
  __forceinline void invoke_preVisitExprVar ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprVar> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprVar> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprVar ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprVar]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprVar ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprVar> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprVar> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprTag ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTag]);
  }
  __forceinline void invoke_preVisitExprTag ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTag> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprTagValue ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprTagValue]);
  }
  __forceinline void invoke_preVisitExprTagValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr, smart_ptr_raw<Expression> const  value ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,value);
  }
  __forceinline Func get_visitExprTag ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprTag]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTag ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTag> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprField]);
  }
  __forceinline void invoke_preVisitExprField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprField> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprField> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprField]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprField> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprField> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSafeField]);
  }
  __forceinline void invoke_preVisitExprSafeField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeField> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeField> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeField ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSafeField]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeField> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeField> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSwizzle ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSwizzle]);
  }
  __forceinline void invoke_preVisitExprSwizzle ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSwizzle> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSwizzle> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSwizzle ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSwizzle]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSwizzle ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSwizzle> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSwizzle> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprIsVariant]);
  }
  __forceinline void invoke_preVisitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprIsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprAsVariant]);
  }
  __forceinline void invoke_preVisitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprSafeAsVariant]);
  }
  __forceinline void invoke_preVisitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSafeAsVariant]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp1 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprOp1]);
  }
  __forceinline void invoke_preVisitExprOp1 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp1> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp1> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp1 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprOp1]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp1 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp1> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp1> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprReturn ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprReturn]);
  }
  __forceinline void invoke_preVisitExprReturn ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReturn> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprReturn> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprReturn ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprReturn]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprReturn ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReturn> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprReturn> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprYield ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprYield]);
  }
  __forceinline void invoke_preVisitExprYield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprYield> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprYield> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprYield ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprYield]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprYield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprYield> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprYield> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprBreak ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprBreak]);
  }
  __forceinline void invoke_preVisitExprBreak ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBreak> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBreak> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprBreak ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprBreak]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBreak ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBreak> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBreak> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprContinue ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprContinue]);
  }
  __forceinline void invoke_preVisitExprContinue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprContinue> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprContinue> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprContinue ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprContinue]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprContinue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprContinue> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprContinue> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_canVisitMakeBlockBody ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_canVisitMakeBlockBody]);
  }
  __forceinline bool invoke_canVisitMakeBlockBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprMakeBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeBlock]);
  }
  __forceinline void invoke_preVisitExprMakeBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeBlock ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeBlock]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeGenerator ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMakeGenerator]);
  }
  __forceinline void invoke_preVisitExprMakeGenerator ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeGenerator> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeGenerator> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeGenerator ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMakeGenerator]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeGenerator ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeGenerator> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeGenerator> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMemZero ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprMemZero]);
  }
  __forceinline void invoke_preVisitExprMemZero ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMemZero> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMemZero> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMemZero ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprMemZero]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMemZero ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMemZero> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMemZero> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConst ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConst]);
  }
  __forceinline void invoke_preVisitExprConst ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConst> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConst> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConst ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConst]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConst ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConst> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConst> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstPtr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstPtr]);
  }
  __forceinline void invoke_preVisitExprConstPtr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstPtr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstPtr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstPtr ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstPtr]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstPtr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstPtr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstPtr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstEnumeration]);
  }
  __forceinline void invoke_preVisitExprConstEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstEnumeration> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstEnumeration> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstEnumeration ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstEnumeration]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstEnumeration> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstEnumeration> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstBitfield ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstBitfield]);
  }
  __forceinline void invoke_preVisitExprConstBitfield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBitfield> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstBitfield> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstBitfield ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstBitfield]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstBitfield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBitfield> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstBitfield> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt8 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt8]);
  }
  __forceinline void invoke_preVisitExprConstInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt8> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt8 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt8]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt8> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt16 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt16]);
  }
  __forceinline void invoke_preVisitExprConstInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt16> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt16 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt16]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt16> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt64]);
  }
  __forceinline void invoke_preVisitExprConstInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt64]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt]);
  }
  __forceinline void invoke_preVisitExprConstInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt2]);
  }
  __forceinline void invoke_preVisitExprConstInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt2]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt3]);
  }
  __forceinline void invoke_preVisitExprConstInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt3]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstInt4]);
  }
  __forceinline void invoke_preVisitExprConstInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstInt4]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt8 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt8]);
  }
  __forceinline void invoke_preVisitExprConstUInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt8> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt8 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt8]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt8> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt16 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt16]);
  }
  __forceinline void invoke_preVisitExprConstUInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt16> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt16 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt16]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt16> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt64]);
  }
  __forceinline void invoke_preVisitExprConstUInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt64]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt]);
  }
  __forceinline void invoke_preVisitExprConstUInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt2]);
  }
  __forceinline void invoke_preVisitExprConstUInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt2]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt3]);
  }
  __forceinline void invoke_preVisitExprConstUInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt3]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstUInt4]);
  }
  __forceinline void invoke_preVisitExprConstUInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstUInt4]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstRange ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstRange]);
  }
  __forceinline void invoke_preVisitExprConstRange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstRange> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstRange ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstRange]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstRange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstRange> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstURange ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstURange]);
  }
  __forceinline void invoke_preVisitExprConstURange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstURange> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstURange ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstURange]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstURange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstURange> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstRange64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstRange64]);
  }
  __forceinline void invoke_preVisitExprConstRange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstRange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstRange64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstRange64]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstRange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstRange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstURange64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstURange64]);
  }
  __forceinline void invoke_preVisitExprConstURange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstURange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstURange64 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstURange64]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstURange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstURange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstBool ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstBool]);
  }
  __forceinline void invoke_preVisitExprConstBool ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBool> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstBool> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstBool ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstBool]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstBool ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBool> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstBool> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstFloat]);
  }
  __forceinline void invoke_preVisitExprConstFloat ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstFloat]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstFloat2]);
  }
  __forceinline void invoke_preVisitExprConstFloat2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat2 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstFloat2]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstFloat3]);
  }
  __forceinline void invoke_preVisitExprConstFloat3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat3 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstFloat3]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstFloat4]);
  }
  __forceinline void invoke_preVisitExprConstFloat4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat4 ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstFloat4]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstString ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstString]);
  }
  __forceinline void invoke_preVisitExprConstString ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstString> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstString> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstString ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstString]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstString ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstString> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstString> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstDouble ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprConstDouble]);
  }
  __forceinline void invoke_preVisitExprConstDouble ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstDouble> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstDouble> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstDouble ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprConstDouble]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstDouble ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstDouble> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstDouble> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFakeContext ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprFakeContext]);
  }
  __forceinline void invoke_preVisitExprFakeContext ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeContext> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFakeContext> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFakeContext ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprFakeContext]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFakeContext ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeContext> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFakeContext> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFakeLineInfo ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprFakeLineInfo]);
  }
  __forceinline void invoke_preVisitExprFakeLineInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeLineInfo> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFakeLineInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFakeLineInfo ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprFakeLineInfo]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFakeLineInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeLineInfo> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFakeLineInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprReader ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprReader]);
  }
  __forceinline void invoke_preVisitExprReader ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReader> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprReader ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprReader]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprReader ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReader> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprUnsafe ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprUnsafe]);
  }
  __forceinline void invoke_preVisitExprUnsafe ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprUnsafe> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprUnsafe> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprUnsafe ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprUnsafe]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprUnsafe ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprUnsafe> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprUnsafe> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCallMacro ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisitExprCallMacro]);
  }
  __forceinline void invoke_preVisitExprCallMacro ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCallMacro ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprCallMacro]);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCallMacro ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
};


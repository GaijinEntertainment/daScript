class AstFunctionAnnotation_Adapter {
protected:
  enum {
    __fn_transform = 0,
    __fn_verifyCall = 1,
    __fn_apply = 2,
    __fn_finish = 3,
    __fn_patch = 4,
    __fn_fixup = 5,
    __fn_lint = 6,
    __fn_complete = 7,
    __fn_isCompatible = 8,
    __fn_isSpecialized = 9,
  };
protected:
  int _das_class_method_offset[10];
public:
  AstFunctionAnnotation_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_transform] = adapt_field_offset("transform",info);
      _das_class_method_offset[__fn_verifyCall] = adapt_field_offset("verifyCall",info);
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
      _das_class_method_offset[__fn_patch] = adapt_field_offset("patch",info);
      _das_class_method_offset[__fn_fixup] = adapt_field_offset("fixup",info);
      _das_class_method_offset[__fn_lint] = adapt_field_offset("lint",info);
      _das_class_method_offset[__fn_complete] = adapt_field_offset("complete",info);
      _das_class_method_offset[__fn_isCompatible] = adapt_field_offset("isCompatible",info);
      _das_class_method_offset[__fn_isSpecialized] = adapt_field_offset("isSpecialized",info);
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
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,ctx);
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
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
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
  };
protected:
  int _das_class_method_offset[4];
public:
  AstStructureAnnotation_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
      _das_class_method_offset[__fn_patch] = adapt_field_offset("patch",info);
      _das_class_method_offset[__fn_complete] = adapt_field_offset("complete",info);
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
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,ctx);
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
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
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
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
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
      _das_class_method_offset[__fn_visitExprIsVariant] = adapt_field_offset("visitExprIsVariant",info);
      _das_class_method_offset[__fn_visitExprAsVariant] = adapt_field_offset("visitExprAsVariant",info);
      _das_class_method_offset[__fn_visitExprSafeAsVariant] = adapt_field_offset("visitExprSafeAsVariant",info);
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
      _das_class_method_offset[__fn_accept] = adapt_field_offset("accept",info);
      _das_class_method_offset[__fn_visit] = adapt_field_offset("visit",info);
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
  };
protected:
  int _das_class_method_offset[3];
public:
  AstCommentReader_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_open] = adapt_field_offset("open",info);
      _das_class_method_offset[__fn_accept] = adapt_field_offset("accept",info);
      _das_class_method_offset[__fn_close] = adapt_field_offset("close",info);
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
};

class AstCallMacro_Adapter {
protected:
  enum {
    __fn_preVisit = 0,
    __fn_visit = 1,
  };
protected:
  int _das_class_method_offset[2];
public:
  AstCallMacro_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_preVisit] = adapt_field_offset("preVisit",info);
      _das_class_method_offset[__fn_visit] = adapt_field_offset("visit",info);
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
      _das_class_method_offset[__fn_getAstChange] = adapt_field_offset("getAstChange",info);
      _das_class_method_offset[__fn_getAstType] = adapt_field_offset("getAstType",info);
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


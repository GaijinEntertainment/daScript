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
    __fn_isCompatible = 7,
    __fn_isSpecialized = 8,
  };
protected:
  static int _das_class_method_offset[9];
public:
  AstFunctionAnnotation_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_transform] = adapt_field_offset("transform",info);
      _das_class_method_offset[__fn_verifyCall] = adapt_field_offset("verifyCall",info);
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
      _das_class_method_offset[__fn_patch] = adapt_field_offset("patch",info);
      _das_class_method_offset[__fn_fixup] = adapt_field_offset("fixup",info);
      _das_class_method_offset[__fn_lint] = adapt_field_offset("lint",info);
      _das_class_method_offset[__fn_isCompatible] = adapt_field_offset("isCompatible",info);
      _das_class_method_offset[__fn_isSpecialized] = adapt_field_offset("isSpecialized",info);
    }
  }
  static __forceinline Func get_transform ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_transform]);
  }
  static __forceinline smart_ptr<Expression> invoke_transform ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, das::string & errors ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,errors).marshal();
  }
  static __forceinline Func get_verifyCall ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_verifyCall]);
  }
  static __forceinline bool invoke_verifyCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,args,progArgs,errors);
  }
  static __forceinline Func get_apply ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  static __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,errors);
  }
  static __forceinline Func get_finish ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  static __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  static __forceinline Func get_patch ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_patch]);
  }
  static __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors, bool & astChanged ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors,astChanged);
  }
  static __forceinline Func get_fixup ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_fixup]);
  }
  static __forceinline bool invoke_fixup ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  static __forceinline Func get_lint ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_lint]);
  }
  static __forceinline bool invoke_lint ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  static __forceinline Func get_isCompatible ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_isCompatible]);
  }
  static __forceinline bool invoke_isCompatible ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, das::vector<smart_ptr<TypeDecl>> & types, AnnotationDeclaration const  & decl, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>> &,AnnotationDeclaration const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,types,decl,errors);
  }
  static __forceinline Func get_isSpecialized ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_isSpecialized]);
  }
  static __forceinline bool invoke_isSpecialized ( Context * __context__, Func __funcCall__, void * self ) {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
};
int AstFunctionAnnotation_Adapter::_das_class_method_offset[9];

class AstBlockAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
    __fn_finish = 1,
  };
protected:
  static int _das_class_method_offset[2];
public:
  AstBlockAnnotation_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
    }
  }
  static __forceinline Func get_apply ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  static __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,errors);
  }
  static __forceinline Func get_finish ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  static __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,progArgs,errors);
  }
};
int AstBlockAnnotation_Adapter::_das_class_method_offset[2];

class AstStructureAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
    __fn_finish = 1,
    __fn_patch = 2,
  };
protected:
  static int _das_class_method_offset[3];
public:
  AstStructureAnnotation_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
      _das_class_method_offset[__fn_finish] = adapt_field_offset("finish",info);
      _das_class_method_offset[__fn_patch] = adapt_field_offset("patch",info);
    }
  }
  static __forceinline Func get_apply ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  static __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  static __forceinline Func get_finish ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_finish]);
  }
  static __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  static __forceinline Func get_patch ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_patch]);
  }
  static __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors, bool & astChanged ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors,astChanged);
  }
};
int AstStructureAnnotation_Adapter::_das_class_method_offset[3];

class AstEnumerationAnnotation_Adapter {
protected:
  enum {
    __fn_apply = 0,
  };
protected:
  static int _das_class_method_offset[1];
public:
  AstEnumerationAnnotation_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
    }
  }
  static __forceinline Func get_apply ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  static __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Enumeration>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
};
int AstEnumerationAnnotation_Adapter::_das_class_method_offset[1];

class AstPassMacro_Adapter {
protected:
  enum {
    __fn_apply = 0,
  };
protected:
  static int _das_class_method_offset[1];
public:
  AstPassMacro_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_apply] = adapt_field_offset("apply",info);
    }
  }
  static __forceinline Func get_apply ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_apply]);
  }
  static __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod);
  }
};
int AstPassMacro_Adapter::_das_class_method_offset[1];

class AstVarianMacro_Adapter {
protected:
  enum {
    __fn_visitExprIsVariant = 0,
    __fn_visitExprAsVariant = 1,
    __fn_visitExprSafeAsVariant = 2,
  };
protected:
  static int _das_class_method_offset[3];
public:
  AstVarianMacro_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_visitExprIsVariant] = adapt_field_offset("visitExprIsVariant",info);
      _das_class_method_offset[__fn_visitExprAsVariant] = adapt_field_offset("visitExprAsVariant",info);
      _das_class_method_offset[__fn_visitExprSafeAsVariant] = adapt_field_offset("visitExprSafeAsVariant",info);
    }
  }
  static __forceinline Func get_visitExprIsVariant ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprIsVariant]);
  }
  static __forceinline smart_ptr<Expression> invoke_visitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprIsVariant> const  expr ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  static __forceinline Func get_visitExprAsVariant ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprAsVariant]);
  }
  static __forceinline smart_ptr<Expression> invoke_visitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprAsVariant> const  expr ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  static __forceinline Func get_visitExprSafeAsVariant ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visitExprSafeAsVariant]);
  }
  static __forceinline smart_ptr<Expression> invoke_visitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprSafeAsVariant> const  expr ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};
int AstVarianMacro_Adapter::_das_class_method_offset[3];

class AstReaderMacro_Adapter {
protected:
  enum {
    __fn_accept = 0,
    __fn_visit = 1,
  };
protected:
  static int _das_class_method_offset[2];
public:
  AstReaderMacro_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_accept] = adapt_field_offset("accept",info);
      _das_class_method_offset[__fn_visit] = adapt_field_offset("visit",info);
    }
  }
  static __forceinline Func get_accept ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_accept]);
  }
  static __forceinline bool invoke_accept ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, ExprReader * const  expr, int32_t ch, LineInfo const  & info ) {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr,ch,info);
  }
  static __forceinline Func get_visit ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visit]);
  }
  static __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprReader> const  expr ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};
int AstReaderMacro_Adapter::_das_class_method_offset[2];

class AstCallMacro_Adapter {
protected:
  enum {
    __fn_preVisit = 0,
    __fn_visit = 1,
  };
protected:
  static int _das_class_method_offset[2];
public:
  AstCallMacro_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_preVisit] = adapt_field_offset("preVisit",info);
      _das_class_method_offset[__fn_visit] = adapt_field_offset("visit",info);
    }
  }
  static __forceinline Func get_preVisit ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_preVisit]);
  }
  static __forceinline void invoke_preVisit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr);
  }
  static __forceinline Func get_visit ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_visit]);
  }
  static __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};
int AstCallMacro_Adapter::_das_class_method_offset[2];

class AstTypeInfoMacro_Adapter {
protected:
  enum {
    __fn_getAstChange = 0,
    __fn_getAstType = 1,
  };
protected:
  static int _das_class_method_offset[2];
public:
  AstTypeInfoMacro_Adapter ( const StructInfo * info ) {
    if ( _das_class_method_offset[0]==0 ) {
      _das_class_method_offset[__fn_getAstChange] = adapt_field_offset("getAstChange",info);
      _das_class_method_offset[__fn_getAstType] = adapt_field_offset("getAstType",info);
    }
  }
  static __forceinline Func get_getAstChange ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_getAstChange]);
  }
  static __forceinline smart_ptr<Expression> invoke_getAstChange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,expr,errors).marshal();
  }
  static __forceinline Func get_getAstType ( void * self ) {
    return getDasClassMethod(self,_das_class_method_offset[__fn_getAstType]);
  }
  static __forceinline smart_ptr<TypeDecl> invoke_getAstType ( Context * __context__, Func __funcCall__, void * self, ModuleLibrary & lib, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,ModuleLibrary &,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,lib,expr,errors).marshal();
  }
};
int AstTypeInfoMacro_Adapter::_das_class_method_offset[2];


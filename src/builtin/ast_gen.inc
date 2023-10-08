class AstFunctionAnnotation_Adapter {
public:
  AstFunctionAnnotation_Adapter ( const StructInfo * ) {}
  __forceinline Func get_transform ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline smart_ptr<Expression> invoke_transform ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,errors).marshal();
  }
  __forceinline Func get_verifyCall ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_verifyCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallFunc> call, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,call,args,progArgs,errors);
  }
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,errors);
  }
  __forceinline Func get_generic_apply ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline bool invoke_generic_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_patch ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors, bool & astChanged ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors,astChanged);
  }
  __forceinline Func get_fixup ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline bool invoke_fixup ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_lint ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline bool invoke_lint ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,group,args,progArgs,errors);
  }
  __forceinline Func get_complete ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function>,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,func,ctx);
  }
  __forceinline Func get_isCompatible ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline bool invoke_isCompatible ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> func, das::vector<smart_ptr<TypeDecl>> & types, AnnotationDeclaration const  & decl, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>> &,AnnotationDeclaration const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,types,decl,errors);
  }
  __forceinline Func get_isSpecialized ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline bool invoke_isSpecialized ( Context * __context__, Func __funcCall__, void * self ) const {
    return das_invoke_function<bool>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_appendToMangledName ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline void invoke_appendToMangledName ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  func, AnnotationDeclaration const  & decl, das::string & mangledName ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,AnnotationDeclaration const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,func,decl,mangledName);
  }
};

class AstBlockAnnotation_Adapter {
public:
  AstBlockAnnotation_Adapter ( const StructInfo * ) {}
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> blk, ModuleGroup & group, AnnotationArgumentList const  & args, AnnotationArgumentList const  & progArgs, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprBlock>,ModuleGroup &,AnnotationArgumentList const  &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,blk,group,args,progArgs,errors);
  }
};

class AstStructureAnnotation_Adapter {
public:
  AstStructureAnnotation_Adapter ( const StructInfo * ) {}
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  __forceinline Func get_finish ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_finish ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
  __forceinline Func get_patch ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline bool invoke_patch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors, bool & astChanged ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Structure>,ModuleGroup &,AnnotationArgumentList const  &,das::string &,bool &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors,astChanged);
  }
  __forceinline Func get_complete ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline void invoke_complete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, smart_ptr_raw<Context> ctx ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,smart_ptr_raw<Context>>
        (__context__,nullptr,__funcCall__,
          self,st,ctx);
  }
  __forceinline Func get_aotPrefix ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline void invoke_aotPrefix ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
  __forceinline Func get_aotBody ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline void invoke_aotBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
  __forceinline Func get_aotSuffix ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline void invoke_aotSuffix ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> st, AnnotationArgumentList const  & args, StringBuilderWriter & writer ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure>,AnnotationArgumentList const  &,StringBuilderWriter &>
        (__context__,nullptr,__funcCall__,
          self,st,args,writer);
  }
};

class AstEnumerationAnnotation_Adapter {
public:
  AstEnumerationAnnotation_Adapter ( const StructInfo * ) {}
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> st, ModuleGroup & group, AnnotationArgumentList const  & args, das::string & errors ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Enumeration>,ModuleGroup &,AnnotationArgumentList const  &,das::string &>
        (__context__,nullptr,__funcCall__,
          self,st,group,args,errors);
  }
};

class AstPassMacro_Adapter {
public:
  AstPassMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_apply ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_apply ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod);
  }
};

class AstVarianMacro_Adapter {
public:
  AstVarianMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_visitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_visitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_visitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstForLoopMacro_Adapter {
public:
  AstForLoopMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_visitExprFor ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprFor> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstCaptureMacro_Adapter {
public:
  AstCaptureMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_captureExpression ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline smart_ptr<Expression> invoke_captureExpression ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Module * const  mod, smart_ptr_raw<Expression> const  expr, smart_ptr_raw<TypeDecl> const  etype ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,Program * const ,Module * const ,smart_ptr_raw<Expression> const ,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr,etype).marshal();
  }
  __forceinline Func get_captureFunction ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline void invoke_captureFunction ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Module * const  mod, Structure * lcs, smart_ptr_raw<Function> fun ) const {
    das_invoke_function<void>::invoke
      <void *,Program * const ,Module * const ,Structure *,smart_ptr_raw<Function>>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,lcs,fun);
  }
};

class AstSimulateMacro_Adapter {
public:
  AstSimulateMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_preSimulate ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_preSimulate ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Context * const  ctx ) const {
    return das_invoke_function<bool>::invoke
      <void *,Program * const ,Context * const >
        (__context__,nullptr,__funcCall__,
          self,prog,ctx);
  }
  __forceinline Func get_simulate ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline bool invoke_simulate ( Context * __context__, Func __funcCall__, void * self, Program * const  prog, Context * const  ctx ) const {
    return das_invoke_function<bool>::invoke
      <void *,Program * const ,Context * const >
        (__context__,nullptr,__funcCall__,
          self,prog,ctx);
  }
};

class AstReaderMacro_Adapter {
public:
  AstReaderMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_accept ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline bool invoke_accept ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, ExprReader * const  expr, int32_t ch, LineInfo const  & info ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr,ch,info);
  }
  __forceinline Func get_visit ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprReader> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
};

class AstCommentReader_Adapter {
public:
  AstCommentReader_Adapter ( const StructInfo * ) {}
  __forceinline Func get_open ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline void invoke_open ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, bool cpp, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,bool,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,cpp,info);
  }
  __forceinline Func get_accept ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline void invoke_accept ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, int32_t ch, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,int32_t,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,ch,info);
  }
  __forceinline Func get_close ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline void invoke_close ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeStructure ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline void invoke_beforeStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterStructure ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline void invoke_afterStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  st, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,st,prog,mod,info);
  }
  __forceinline Func get_beforeStructureFields ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline void invoke_beforeStructureFields ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterStructureField ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline void invoke_afterStructureField ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_afterStructureFields ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline void invoke_afterStructureFields ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeFunction ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline void invoke_beforeFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterFunction ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline void invoke_afterFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fn, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,fn,prog,mod,info);
  }
  __forceinline Func get_beforeGlobalVariables ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline void invoke_beforeGlobalVariables ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterGlobalVariable ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline void invoke_afterGlobalVariable ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_afterGlobalVariables ( void * self ) const {
    return getDasClassMethod(self,112);
  }
  __forceinline void invoke_afterGlobalVariables ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_beforeVariant ( void * self ) const {
    return getDasClassMethod(self,120);
  }
  __forceinline void invoke_beforeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterVariant ( void * self ) const {
    return getDasClassMethod(self,128);
  }
  __forceinline void invoke_afterVariant ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_beforeEnumeration ( void * self ) const {
    return getDasClassMethod(self,136);
  }
  __forceinline void invoke_beforeEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterEnumeration ( void * self ) const {
    return getDasClassMethod(self,144);
  }
  __forceinline void invoke_afterEnumeration ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
  __forceinline Func get_beforeAlias ( void * self ) const {
    return getDasClassMethod(self,152);
  }
  __forceinline void invoke_beforeAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,prog,mod,info);
  }
  __forceinline Func get_afterAlias ( void * self ) const {
    return getDasClassMethod(self,160);
  }
  __forceinline void invoke_afterAlias ( Context * __context__, Func __funcCall__, void * self, char * const  name, smart_ptr_raw<Program> const  prog, Module * const  mod, LineInfo const  & info ) const {
    das_invoke_function<void>::invoke
      <void *,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const  &>
        (__context__,nullptr,__funcCall__,
          self,name,prog,mod,info);
  }
};

class AstCallMacro_Adapter {
public:
  AstCallMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_preVisit ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline void invoke_preVisit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr);
  }
  __forceinline Func get_visit ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline smart_ptr<Expression> invoke_visit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod,expr).marshal();
  }
  __forceinline Func get_canVisitArgument ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline bool invoke_canVisitArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr, int32_t argIndex ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const ,int32_t>
        (__context__,nullptr,__funcCall__,
          self,expr,argIndex);
  }
  __forceinline Func get_canFoldReturnResult ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline bool invoke_canFoldReturnResult ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
};

class AstTypeInfoMacro_Adapter {
public:
  AstTypeInfoMacro_Adapter ( const StructInfo * ) {}
  __forceinline Func get_getAstChange ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline smart_ptr<Expression> invoke_getAstChange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,expr,errors).marshal();
  }
  __forceinline Func get_getAstType ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline smart_ptr<TypeDecl> invoke_getAstType ( Context * __context__, Func __funcCall__, void * self, ModuleLibrary & lib, smart_ptr_raw<ExprTypeInfo> const  expr, das::string & errors ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,ModuleLibrary &,smart_ptr_raw<ExprTypeInfo> const ,das::string &>
        (__context__,nullptr,__funcCall__,
          self,lib,expr,errors).marshal();
  }
};

class AstVisitor_Adapter {
public:
  AstVisitor_Adapter ( const StructInfo * ) {}
  __forceinline Func get_preVisitProgram ( void * self ) const {
    return getDasClassMethod(self,16);
  }
  __forceinline void invoke_preVisitProgram ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_visitProgram ( void * self ) const {
    return getDasClassMethod(self,24);
  }
  __forceinline void invoke_visitProgram ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  porg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,porg);
  }
  __forceinline Func get_preVisitProgramBody ( void * self ) const {
    return getDasClassMethod(self,32);
  }
  __forceinline void invoke_preVisitProgramBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const ,Module * const >
        (__context__,nullptr,__funcCall__,
          self,prog,mod);
  }
  __forceinline Func get_preVisitModule ( void * self ) const {
    return getDasClassMethod(self,40);
  }
  __forceinline void invoke_preVisitModule ( Context * __context__, Func __funcCall__, void * self, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,Module * const >
        (__context__,nullptr,__funcCall__,
          self,mod);
  }
  __forceinline Func get_visitModule ( void * self ) const {
    return getDasClassMethod(self,48);
  }
  __forceinline void invoke_visitModule ( Context * __context__, Func __funcCall__, void * self, Module * const  mod ) const {
    das_invoke_function<void>::invoke
      <void *,Module * const >
        (__context__,nullptr,__funcCall__,
          self,mod);
  }
  __forceinline Func get_preVisitExprTypeDecl ( void * self ) const {
    return getDasClassMethod(self,56);
  }
  __forceinline void invoke_preVisitExprTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeDecl> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTypeDecl ( void * self ) const {
    return getDasClassMethod(self,64);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeDecl> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitTypeDecl ( void * self ) const {
    return getDasClassMethod(self,72);
  }
  __forceinline void invoke_preVisitTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,typ);
  }
  __forceinline Func get_visitTypeDecl ( void * self ) const {
    return getDasClassMethod(self,80);
  }
  __forceinline smart_ptr<TypeDecl> invoke_visitTypeDecl ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,typ).marshal();
  }
  __forceinline Func get_preVisitAlias ( void * self ) const {
    return getDasClassMethod(self,88);
  }
  __forceinline void invoke_preVisitAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ, das::string const  & name ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<TypeDecl> const ,das::string const  &>
        (__context__,nullptr,__funcCall__,
          self,typ,name);
  }
  __forceinline Func get_visitAlias ( void * self ) const {
    return getDasClassMethod(self,96);
  }
  __forceinline smart_ptr<TypeDecl> invoke_visitAlias ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<TypeDecl> const  typ, das::string const  & name ) const {
    return das_invoke_function<smart_ptr_raw<TypeDecl>>::invoke
      <void *,smart_ptr_raw<TypeDecl> const ,das::string const  &>
        (__context__,nullptr,__funcCall__,
          self,typ,name).marshal();
  }
  __forceinline Func get_canVisitEnumeration ( void * self ) const {
    return getDasClassMethod(self,104);
  }
  __forceinline bool invoke_canVisitEnumeration ( Context * __context__, Func __funcCall__, void * self, Enumeration * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Enumeration * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitEnumeration ( void * self ) const {
    return getDasClassMethod(self,112);
  }
  __forceinline void invoke_preVisitEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Enumeration> const >
        (__context__,nullptr,__funcCall__,
          self,enu);
  }
  __forceinline Func get_preVisitEnumerationValue ( void * self ) const {
    return getDasClassMethod(self,120);
  }
  __forceinline void invoke_preVisitEnumerationValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu, das::string const  & name, smart_ptr_raw<Expression> const  value, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Enumeration> const ,das::string const  &,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,enu,name,value,last);
  }
  __forceinline Func get_visitEnumerationValue ( void * self ) const {
    return getDasClassMethod(self,128);
  }
  __forceinline smart_ptr<Expression> invoke_visitEnumerationValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu, das::string const  & name, smart_ptr_raw<Expression> const  value, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Enumeration> const ,das::string const  &,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,enu,name,value,last).marshal();
  }
  __forceinline Func get_visitEnumeration ( void * self ) const {
    return getDasClassMethod(self,136);
  }
  __forceinline smart_ptr<Enumeration> invoke_visitEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Enumeration> const  enu ) const {
    return das_invoke_function<smart_ptr_raw<Enumeration>>::invoke
      <void *,smart_ptr_raw<Enumeration> const >
        (__context__,nullptr,__funcCall__,
          self,enu).marshal();
  }
  __forceinline Func get_canVisitStructure ( void * self ) const {
    return getDasClassMethod(self,144);
  }
  __forceinline bool invoke_canVisitStructure ( Context * __context__, Func __funcCall__, void * self, Structure * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Structure * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitStructure ( void * self ) const {
    return getDasClassMethod(self,152);
  }
  __forceinline void invoke_preVisitStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const >
        (__context__,nullptr,__funcCall__,
          self,str);
  }
  __forceinline Func get_preVisitStructureField ( void * self ) const {
    return getDasClassMethod(self,160);
  }
  __forceinline void invoke_preVisitStructureField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str, Structure::FieldDeclaration const  & decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,str,decl,last);
  }
  __forceinline Func get_visitStructureField ( void * self ) const {
    return getDasClassMethod(self,168);
  }
  __forceinline void invoke_visitStructureField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str, Structure::FieldDeclaration const  & decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Structure> const ,Structure::FieldDeclaration const  &,bool>
        (__context__,nullptr,__funcCall__,
          self,str,decl,last);
  }
  __forceinline Func get_visitStructure ( void * self ) const {
    return getDasClassMethod(self,176);
  }
  __forceinline smart_ptr<Structure> invoke_visitStructure ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Structure> const  str ) const {
    return das_invoke_function<smart_ptr_raw<Structure>>::invoke
      <void *,smart_ptr_raw<Structure> const >
        (__context__,nullptr,__funcCall__,
          self,str).marshal();
  }
  __forceinline Func get_canVisitFunction ( void * self ) const {
    return getDasClassMethod(self,184);
  }
  __forceinline bool invoke_canVisitFunction ( Context * __context__, Func __funcCall__, void * self, Function * const  fun ) const {
    return das_invoke_function<bool>::invoke
      <void *,Function * const >
        (__context__,nullptr,__funcCall__,
          self,fun);
  }
  __forceinline Func get_canVisitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,192);
  }
  __forceinline bool invoke_canVisitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, Function * const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    return das_invoke_function<bool>::invoke
      <void *,Function * const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value);
  }
  __forceinline Func get_preVisitFunction ( void * self ) const {
    return getDasClassMethod(self,200);
  }
  __forceinline void invoke_preVisitFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const >
        (__context__,nullptr,__funcCall__,
          self,fun);
  }
  __forceinline Func get_visitFunction ( void * self ) const {
    return getDasClassMethod(self,208);
  }
  __forceinline smart_ptr<Function> invoke_visitFunction ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun ) const {
    return das_invoke_function<smart_ptr_raw<Function>>::invoke
      <void *,smart_ptr_raw<Function> const >
        (__context__,nullptr,__funcCall__,
          self,fun).marshal();
  }
  __forceinline Func get_preVisitFunctionArgument ( void * self ) const {
    return getDasClassMethod(self,216);
  }
  __forceinline void invoke_preVisitFunctionArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,fun,arg,lastArg);
  }
  __forceinline Func get_visitFunctionArgument ( void * self ) const {
    return getDasClassMethod(self,224);
  }
  __forceinline smart_ptr<Variable> invoke_visitFunctionArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,fun,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,232);
  }
  __forceinline void invoke_preVisitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value);
  }
  __forceinline Func get_visitFunctionArgumentInit ( void * self ) const {
    return getDasClassMethod(self,240);
  }
  __forceinline smart_ptr<Expression> invoke_visitFunctionArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  value ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,arg,value).marshal();
  }
  __forceinline Func get_preVisitFunctionBody ( void * self ) const {
    return getDasClassMethod(self,248);
  }
  __forceinline void invoke_preVisitFunctionBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,expr);
  }
  __forceinline Func get_visitFunctionBody ( void * self ) const {
    return getDasClassMethod(self,256);
  }
  __forceinline smart_ptr<Expression> invoke_visitFunctionBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Function> const  fun, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Function> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,fun,expr).marshal();
  }
  __forceinline Func get_preVisitExpression ( void * self ) const {
    return getDasClassMethod(self,264);
  }
  __forceinline void invoke_preVisitExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExpression ( void * self ) const {
    return getDasClassMethod(self,272);
  }
  __forceinline smart_ptr<Expression> invoke_visitExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlock ( void * self ) const {
    return getDasClassMethod(self,280);
  }
  __forceinline void invoke_preVisitExprBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_visitExprBlock ( void * self ) const {
    return getDasClassMethod(self,288);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk).marshal();
  }
  __forceinline Func get_preVisitExprBlockArgument ( void * self ) const {
    return getDasClassMethod(self,296);
  }
  __forceinline void invoke_preVisitExprBlockArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,blk,arg,lastArg);
  }
  __forceinline Func get_visitExprBlockArgument ( void * self ) const {
    return getDasClassMethod(self,304);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprBlockArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,blk,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitExprBlockArgumentInit ( void * self ) const {
    return getDasClassMethod(self,312);
  }
  __forceinline void invoke_preVisitExprBlockArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr);
  }
  __forceinline Func get_visitExprBlockArgumentInit ( void * self ) const {
    return getDasClassMethod(self,320);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockArgumentInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlockExpression ( void * self ) const {
    return getDasClassMethod(self,328);
  }
  __forceinline void invoke_preVisitExprBlockExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr);
  }
  __forceinline Func get_visitExprBlockExpression ( void * self ) const {
    return getDasClassMethod(self,336);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr).marshal();
  }
  __forceinline Func get_preVisitExprBlockFinal ( void * self ) const {
    return getDasClassMethod(self,344);
  }
  __forceinline void invoke_preVisitExprBlockFinal ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_visitExprBlockFinal ( void * self ) const {
    return getDasClassMethod(self,352);
  }
  __forceinline void invoke_visitExprBlockFinal ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const >
        (__context__,nullptr,__funcCall__,
          self,blk);
  }
  __forceinline Func get_preVisitExprBlockFinalExpression ( void * self ) const {
    return getDasClassMethod(self,360);
  }
  __forceinline void invoke_preVisitExprBlockFinalExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr);
  }
  __forceinline Func get_visitExprBlockFinalExpression ( void * self ) const {
    return getDasClassMethod(self,368);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBlockFinalExpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBlock> const  blk, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBlock> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,expr).marshal();
  }
  __forceinline Func get_preVisitExprLet ( void * self ) const {
    return getDasClassMethod(self,376);
  }
  __forceinline void invoke_preVisitExprLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLet ( void * self ) const {
    return getDasClassMethod(self,384);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLet> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLetVariable ( void * self ) const {
    return getDasClassMethod(self,392);
  }
  __forceinline void invoke_preVisitExprLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,lastArg);
  }
  __forceinline Func get_visitExprLetVariable ( void * self ) const {
    return getDasClassMethod(self,400);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  expr, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitExprLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,408);
  }
  __forceinline void invoke_preVisitExprLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr);
  }
  __forceinline Func get_visitExprLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,416);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLet> const  blk, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLet> const ,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,blk,arg,expr).marshal();
  }
  __forceinline Func get_canVisitGlobalVariable ( void * self ) const {
    return getDasClassMethod(self,424);
  }
  __forceinline bool invoke_canVisitGlobalVariable ( Context * __context__, Func __funcCall__, void * self, Variable * const  arg ) const {
    return das_invoke_function<bool>::invoke
      <void *,Variable * const >
        (__context__,nullptr,__funcCall__,
          self,arg);
  }
  __forceinline Func get_preVisitGlobalLet ( void * self ) const {
    return getDasClassMethod(self,432);
  }
  __forceinline void invoke_preVisitGlobalLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_visitGlobalLet ( void * self ) const {
    return getDasClassMethod(self,440);
  }
  __forceinline void invoke_visitGlobalLet ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Program> const  prog ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Program> const >
        (__context__,nullptr,__funcCall__,
          self,prog);
  }
  __forceinline Func get_preVisitGlobalLetVariable ( void * self ) const {
    return getDasClassMethod(self,448);
  }
  __forceinline void invoke_preVisitGlobalLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,arg,lastArg);
  }
  __forceinline Func get_visitGlobalLetVariable ( void * self ) const {
    return getDasClassMethod(self,456);
  }
  __forceinline smart_ptr<Variable> invoke_visitGlobalLetVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, bool lastArg ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,arg,lastArg).marshal();
  }
  __forceinline Func get_preVisitGlobalLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,464);
  }
  __forceinline void invoke_preVisitGlobalLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,arg,expr);
  }
  __forceinline Func get_visitGlobalLetVariableInit ( void * self ) const {
    return getDasClassMethod(self,472);
  }
  __forceinline smart_ptr<Expression> invoke_visitGlobalLetVariableInit ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<Variable> const  arg, smart_ptr_raw<Expression> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<Variable> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,arg,expr).marshal();
  }
  __forceinline Func get_preVisitExprStringBuilder ( void * self ) const {
    return getDasClassMethod(self,480);
  }
  __forceinline void invoke_preVisitExprStringBuilder ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprStringBuilder ( void * self ) const {
    return getDasClassMethod(self,488);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStringBuilder ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprStringBuilderElement ( void * self ) const {
    return getDasClassMethod(self,496);
  }
  __forceinline void invoke_preVisitExprStringBuilderElement ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr, smart_ptr_raw<Expression> const  elem, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,elem,last);
  }
  __forceinline Func get_visitExprStringBuilderElement ( void * self ) const {
    return getDasClassMethod(self,504);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStringBuilderElement ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStringBuilder> const  expr, smart_ptr_raw<Expression> const  elem, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStringBuilder> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,elem,last).marshal();
  }
  __forceinline Func get_preVisitExprNew ( void * self ) const {
    return getDasClassMethod(self,512);
  }
  __forceinline void invoke_preVisitExprNew ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNew> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNew ( void * self ) const {
    return getDasClassMethod(self,520);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNew ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNew> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNewArgument ( void * self ) const {
    return getDasClassMethod(self,528);
  }
  __forceinline void invoke_preVisitExprNewArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprNewArgument ( void * self ) const {
    return getDasClassMethod(self,536);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNewArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNew> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNew> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprNamedCall ( void * self ) const {
    return getDasClassMethod(self,544);
  }
  __forceinline void invoke_preVisitExprNamedCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNamedCall ( void * self ) const {
    return getDasClassMethod(self,552);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNamedCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNamedCallArgument ( void * self ) const {
    return getDasClassMethod(self,560);
  }
  __forceinline void invoke_preVisitExprNamedCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr, smart_ptr_raw<MakeFieldDecl> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprNamedCallArgument ( void * self ) const {
    return getDasClassMethod(self,568);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprNamedCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNamedCall> const  expr, smart_ptr_raw<MakeFieldDecl> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprNamedCall> const ,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprLooksLikeCall ( void * self ) const {
    return getDasClassMethod(self,576);
  }
  __forceinline void invoke_preVisitExprLooksLikeCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLooksLikeCall ( void * self ) const {
    return getDasClassMethod(self,584);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLooksLikeCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLooksLikeCallArgument ( void * self ) const {
    return getDasClassMethod(self,592);
  }
  __forceinline void invoke_preVisitExprLooksLikeCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprLooksLikeCallArgument ( void * self ) const {
    return getDasClassMethod(self,600);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLooksLikeCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLooksLikeCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLooksLikeCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_canVisitCall ( void * self ) const {
    return getDasClassMethod(self,608);
  }
  __forceinline bool invoke_canVisitCall ( Context * __context__, Func __funcCall__, void * self, ExprCall * const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,ExprCall * const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprCall ( void * self ) const {
    return getDasClassMethod(self,616);
  }
  __forceinline void invoke_preVisitExprCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCall ( void * self ) const {
    return getDasClassMethod(self,624);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCall ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCall> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCallArgument ( void * self ) const {
    return getDasClassMethod(self,632);
  }
  __forceinline void invoke_preVisitExprCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last);
  }
  __forceinline Func get_visitExprCallArgument ( void * self ) const {
    return getDasClassMethod(self,640);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCallArgument ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCall> const  expr, smart_ptr_raw<Expression> const  arg, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCall> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,arg,last).marshal();
  }
  __forceinline Func get_preVisitExprNullCoalescing ( void * self ) const {
    return getDasClassMethod(self,648);
  }
  __forceinline void invoke_preVisitExprNullCoalescing ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprNullCoalescing ( void * self ) const {
    return getDasClassMethod(self,656);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprNullCoalescing ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprNullCoalescingDefault ( void * self ) const {
    return getDasClassMethod(self,664);
  }
  __forceinline void invoke_preVisitExprNullCoalescingDefault ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprNullCoalescing> const  expr, smart_ptr_raw<Expression> const  defval ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprNullCoalescing> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,defval);
  }
  __forceinline Func get_preVisitExprAt ( void * self ) const {
    return getDasClassMethod(self,672);
  }
  __forceinline void invoke_preVisitExprAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAt ( void * self ) const {
    return getDasClassMethod(self,680);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAtIndex ( void * self ) const {
    return getDasClassMethod(self,688);
  }
  __forceinline void invoke_preVisitExprAtIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr, smart_ptr_raw<Expression> const  index ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,index);
  }
  __forceinline Func get_preVisitExprSafeAt ( void * self ) const {
    return getDasClassMethod(self,696);
  }
  __forceinline void invoke_preVisitExprSafeAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeAt ( void * self ) const {
    return getDasClassMethod(self,704);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeAt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeAtIndex ( void * self ) const {
    return getDasClassMethod(self,712);
  }
  __forceinline void invoke_preVisitExprSafeAtIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAt> const  expr, smart_ptr_raw<Expression> const  index ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAt> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,index);
  }
  __forceinline Func get_preVisitExprIs ( void * self ) const {
    return getDasClassMethod(self,720);
  }
  __forceinline void invoke_preVisitExprIs ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIs> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIs ( void * self ) const {
    return getDasClassMethod(self,728);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIs ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIs> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIsType ( void * self ) const {
    return getDasClassMethod(self,736);
  }
  __forceinline void invoke_preVisitExprIsType ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIs> const  expr, smart_ptr_raw<TypeDecl> const  typeDecl ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIs> const ,smart_ptr_raw<TypeDecl> const >
        (__context__,nullptr,__funcCall__,
          self,expr,typeDecl);
  }
  __forceinline Func get_preVisitExprOp2 ( void * self ) const {
    return getDasClassMethod(self,744);
  }
  __forceinline void invoke_preVisitExprOp2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp2 ( void * self ) const {
    return getDasClassMethod(self,752);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp2Right ( void * self ) const {
    return getDasClassMethod(self,760);
  }
  __forceinline void invoke_preVisitExprOp2Right ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp2> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp2> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprOp3 ( void * self ) const {
    return getDasClassMethod(self,768);
  }
  __forceinline void invoke_preVisitExprOp3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp3 ( void * self ) const {
    return getDasClassMethod(self,776);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp3Left ( void * self ) const {
    return getDasClassMethod(self,784);
  }
  __forceinline void invoke_preVisitExprOp3Left ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr, smart_ptr_raw<Expression> const  left ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,left);
  }
  __forceinline Func get_preVisitExprOp3Right ( void * self ) const {
    return getDasClassMethod(self,792);
  }
  __forceinline void invoke_preVisitExprOp3Right ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp3> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp3> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprCopy ( void * self ) const {
    return getDasClassMethod(self,800);
  }
  __forceinline void invoke_preVisitExprCopy ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCopy> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCopy ( void * self ) const {
    return getDasClassMethod(self,808);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCopy ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCopy> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCopyRight ( void * self ) const {
    return getDasClassMethod(self,816);
  }
  __forceinline void invoke_preVisitExprCopyRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCopy> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCopy> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprMove ( void * self ) const {
    return getDasClassMethod(self,824);
  }
  __forceinline void invoke_preVisitExprMove ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMove> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMove ( void * self ) const {
    return getDasClassMethod(self,832);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMove ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMove> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMoveRight ( void * self ) const {
    return getDasClassMethod(self,840);
  }
  __forceinline void invoke_preVisitExprMoveRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMove> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMove> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprClone ( void * self ) const {
    return getDasClassMethod(self,848);
  }
  __forceinline void invoke_preVisitExprClone ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprClone> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprClone ( void * self ) const {
    return getDasClassMethod(self,856);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprClone ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprClone> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCloneRight ( void * self ) const {
    return getDasClassMethod(self,864);
  }
  __forceinline void invoke_preVisitExprCloneRight ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprClone> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprClone> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_canVisitWithAliasSubexpression ( void * self ) const {
    return getDasClassMethod(self,872);
  }
  __forceinline bool invoke_canVisitWithAliasSubexpression ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprAssume ( void * self ) const {
    return getDasClassMethod(self,880);
  }
  __forceinline void invoke_preVisitExprAssume ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAssume ( void * self ) const {
    return getDasClassMethod(self,888);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAssume ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssume> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAssume> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWith ( void * self ) const {
    return getDasClassMethod(self,896);
  }
  __forceinline void invoke_preVisitExprWith ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWith> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprWith ( void * self ) const {
    return getDasClassMethod(self,904);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprWith ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprWith> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWithBody ( void * self ) const {
    return getDasClassMethod(self,912);
  }
  __forceinline void invoke_preVisitExprWithBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWith> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWith> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprWhile ( void * self ) const {
    return getDasClassMethod(self,920);
  }
  __forceinline void invoke_preVisitExprWhile ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWhile> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprWhile ( void * self ) const {
    return getDasClassMethod(self,928);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprWhile ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprWhile> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprWhileBody ( void * self ) const {
    return getDasClassMethod(self,936);
  }
  __forceinline void invoke_preVisitExprWhileBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprWhile> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprWhile> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprTryCatch ( void * self ) const {
    return getDasClassMethod(self,944);
  }
  __forceinline void invoke_preVisitExprTryCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTryCatch ( void * self ) const {
    return getDasClassMethod(self,952);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTryCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprTryCatchCatch ( void * self ) const {
    return getDasClassMethod(self,960);
  }
  __forceinline void invoke_preVisitExprTryCatchCatch ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTryCatch> const  expr, smart_ptr_raw<Expression> const  right ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTryCatch> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,right);
  }
  __forceinline Func get_preVisitExprIfThenElse ( void * self ) const {
    return getDasClassMethod(self,968);
  }
  __forceinline void invoke_preVisitExprIfThenElse ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIfThenElse ( void * self ) const {
    return getDasClassMethod(self,976);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIfThenElse ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIfThenElseIfBlock ( void * self ) const {
    return getDasClassMethod(self,984);
  }
  __forceinline void invoke_preVisitExprIfThenElseIfBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr, smart_ptr_raw<Expression> const  ifBlock ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,ifBlock);
  }
  __forceinline Func get_preVisitExprIfThenElseElseBlock ( void * self ) const {
    return getDasClassMethod(self,992);
  }
  __forceinline void invoke_preVisitExprIfThenElseElseBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIfThenElse> const  expr, smart_ptr_raw<Expression> const  elseBlock ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIfThenElse> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,elseBlock);
  }
  __forceinline Func get_preVisitExprFor ( void * self ) const {
    return getDasClassMethod(self,1000);
  }
  __forceinline void invoke_preVisitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFor ( void * self ) const {
    return getDasClassMethod(self,1008);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFor ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprForVariable ( void * self ) const {
    return getDasClassMethod(self,1016);
  }
  __forceinline void invoke_preVisitExprForVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Variable> const  svar, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,svar,last);
  }
  __forceinline Func get_visitExprForVariable ( void * self ) const {
    return getDasClassMethod(self,1024);
  }
  __forceinline smart_ptr<Variable> invoke_visitExprForVariable ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Variable> const  svar, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Variable>>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Variable> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,svar,last).marshal();
  }
  __forceinline Func get_preVisitExprForSource ( void * self ) const {
    return getDasClassMethod(self,1032);
  }
  __forceinline void invoke_preVisitExprForSource ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Expression> const  source, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,source,last);
  }
  __forceinline Func get_visitExprForSource ( void * self ) const {
    return getDasClassMethod(self,1040);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprForSource ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr, smart_ptr_raw<Expression> const  source, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFor> const ,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,source,last).marshal();
  }
  __forceinline Func get_preVisitExprForStack ( void * self ) const {
    return getDasClassMethod(self,1048);
  }
  __forceinline void invoke_preVisitExprForStack ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprForBody ( void * self ) const {
    return getDasClassMethod(self,1056);
  }
  __forceinline void invoke_preVisitExprForBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFor> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFor> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprMakeVariant ( void * self ) const {
    return getDasClassMethod(self,1064);
  }
  __forceinline void invoke_preVisitExprMakeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeVariant ( void * self ) const {
    return getDasClassMethod(self,1072);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeVariantField ( void * self ) const {
    return getDasClassMethod(self,1080);
  }
  __forceinline void invoke_preVisitExprMakeVariantField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last);
  }
  __forceinline Func get_visitExprMakeVariantField ( void * self ) const {
    return getDasClassMethod(self,1088);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprMakeVariantField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeVariant> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprMakeVariant> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last).marshal();
  }
  __forceinline Func get_canVisitMakeStructBody ( void * self ) const {
    return getDasClassMethod(self,1096);
  }
  __forceinline bool invoke_canVisitMakeStructBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_canVisitMakeStructBlock ( void * self ) const {
    return getDasClassMethod(self,1104);
  }
  __forceinline bool invoke_canVisitMakeStructBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, smart_ptr_raw<Expression> const  blk ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,blk);
  }
  __forceinline Func get_preVisitExprMakeStruct ( void * self ) const {
    return getDasClassMethod(self,1112);
  }
  __forceinline void invoke_preVisitExprMakeStruct ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeStruct ( void * self ) const {
    return getDasClassMethod(self,1120);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeStruct ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeStructIndex ( void * self ) const {
    return getDasClassMethod(self,1128);
  }
  __forceinline void invoke_preVisitExprMakeStructIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,last);
  }
  __forceinline Func get_visitExprMakeStructIndex ( void * self ) const {
    return getDasClassMethod(self,1136);
  }
  __forceinline void invoke_visitExprMakeStructIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,last);
  }
  __forceinline Func get_preVisitExprMakeStructField ( void * self ) const {
    return getDasClassMethod(self,1144);
  }
  __forceinline void invoke_preVisitExprMakeStructField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last);
  }
  __forceinline Func get_visitExprMakeStructField ( void * self ) const {
    return getDasClassMethod(self,1152);
  }
  __forceinline smart_ptr<MakeFieldDecl> invoke_visitExprMakeStructField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeStruct> const  expr, int32_t index, smart_ptr_raw<MakeFieldDecl> const  decl, bool last ) const {
    return das_invoke_function<smart_ptr_raw<MakeFieldDecl>>::invoke
      <void *,smart_ptr_raw<ExprMakeStruct> const ,int32_t,smart_ptr_raw<MakeFieldDecl> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,decl,last).marshal();
  }
  __forceinline Func get_preVisitExprMakeArray ( void * self ) const {
    return getDasClassMethod(self,1160);
  }
  __forceinline void invoke_preVisitExprMakeArray ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeArray ( void * self ) const {
    return getDasClassMethod(self,1168);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeArray ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeArrayIndex ( void * self ) const {
    return getDasClassMethod(self,1176);
  }
  __forceinline void invoke_preVisitExprMakeArrayIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last);
  }
  __forceinline Func get_visitExprMakeArrayIndex ( void * self ) const {
    return getDasClassMethod(self,1184);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeArrayIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeArray> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeArray> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last).marshal();
  }
  __forceinline Func get_preVisitExprMakeTuple ( void * self ) const {
    return getDasClassMethod(self,1192);
  }
  __forceinline void invoke_preVisitExprMakeTuple ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeTuple ( void * self ) const {
    return getDasClassMethod(self,1200);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeTuple ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeTupleIndex ( void * self ) const {
    return getDasClassMethod(self,1208);
  }
  __forceinline void invoke_preVisitExprMakeTupleIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last);
  }
  __forceinline Func get_visitExprMakeTupleIndex ( void * self ) const {
    return getDasClassMethod(self,1216);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeTupleIndex ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeTuple> const  expr, int32_t index, smart_ptr_raw<Expression> const  init, bool last ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeTuple> const ,int32_t,smart_ptr_raw<Expression> const ,bool>
        (__context__,nullptr,__funcCall__,
          self,expr,index,init,last).marshal();
  }
  __forceinline Func get_preVisitExprArrayComprehension ( void * self ) const {
    return getDasClassMethod(self,1224);
  }
  __forceinline void invoke_preVisitExprArrayComprehension ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprArrayComprehension ( void * self ) const {
    return getDasClassMethod(self,1232);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprArrayComprehension ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprArrayComprehensionSubexpr ( void * self ) const {
    return getDasClassMethod(self,1240);
  }
  __forceinline void invoke_preVisitExprArrayComprehensionSubexpr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr, smart_ptr_raw<Expression> const  subexrp ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,subexrp);
  }
  __forceinline Func get_preVisitExprArrayComprehensionWhere ( void * self ) const {
    return getDasClassMethod(self,1248);
  }
  __forceinline void invoke_preVisitExprArrayComprehensionWhere ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprArrayComprehension> const  expr, smart_ptr_raw<Expression> const  filter ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprArrayComprehension> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,filter);
  }
  __forceinline Func get_preVisitExprTypeInfo ( void * self ) const {
    return getDasClassMethod(self,1256);
  }
  __forceinline void invoke_preVisitExprTypeInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprTypeInfo ( void * self ) const {
    return getDasClassMethod(self,1264);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTypeInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTypeInfo> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTypeInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprPtr2Ref ( void * self ) const {
    return getDasClassMethod(self,1272);
  }
  __forceinline void invoke_preVisitExprPtr2Ref ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprPtr2Ref> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprPtr2Ref> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprPtr2Ref ( void * self ) const {
    return getDasClassMethod(self,1280);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprPtr2Ref ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprPtr2Ref> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprPtr2Ref> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprLabel ( void * self ) const {
    return getDasClassMethod(self,1288);
  }
  __forceinline void invoke_preVisitExprLabel ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLabel> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprLabel> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprLabel ( void * self ) const {
    return getDasClassMethod(self,1296);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprLabel ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprLabel> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprLabel> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprGoto ( void * self ) const {
    return getDasClassMethod(self,1304);
  }
  __forceinline void invoke_preVisitExprGoto ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprGoto> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprGoto> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprGoto ( void * self ) const {
    return getDasClassMethod(self,1312);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprGoto ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprGoto> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprGoto> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprRef2Value ( void * self ) const {
    return getDasClassMethod(self,1320);
  }
  __forceinline void invoke_preVisitExprRef2Value ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Value> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprRef2Value> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprRef2Value ( void * self ) const {
    return getDasClassMethod(self,1328);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprRef2Value ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Value> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprRef2Value> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprRef2Ptr ( void * self ) const {
    return getDasClassMethod(self,1336);
  }
  __forceinline void invoke_preVisitExprRef2Ptr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Ptr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprRef2Ptr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprRef2Ptr ( void * self ) const {
    return getDasClassMethod(self,1344);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprRef2Ptr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprRef2Ptr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprRef2Ptr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAddr ( void * self ) const {
    return getDasClassMethod(self,1352);
  }
  __forceinline void invoke_preVisitExprAddr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAddr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAddr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAddr ( void * self ) const {
    return getDasClassMethod(self,1360);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAddr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAddr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAddr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAssert ( void * self ) const {
    return getDasClassMethod(self,1368);
  }
  __forceinline void invoke_preVisitExprAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAssert ( void * self ) const {
    return getDasClassMethod(self,1376);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAssert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprStaticAssert ( void * self ) const {
    return getDasClassMethod(self,1384);
  }
  __forceinline void invoke_preVisitExprStaticAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStaticAssert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprStaticAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprStaticAssert ( void * self ) const {
    return getDasClassMethod(self,1392);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprStaticAssert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprStaticAssert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprStaticAssert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprQuote ( void * self ) const {
    return getDasClassMethod(self,1400);
  }
  __forceinline void invoke_preVisitExprQuote ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprQuote> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprQuote> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprQuote ( void * self ) const {
    return getDasClassMethod(self,1408);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprQuote ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprQuote> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprQuote> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprDebug ( void * self ) const {
    return getDasClassMethod(self,1416);
  }
  __forceinline void invoke_preVisitExprDebug ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDebug> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprDebug> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprDebug ( void * self ) const {
    return getDasClassMethod(self,1424);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprDebug ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDebug> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprDebug> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprInvoke ( void * self ) const {
    return getDasClassMethod(self,1432);
  }
  __forceinline void invoke_preVisitExprInvoke ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprInvoke> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprInvoke> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprInvoke ( void * self ) const {
    return getDasClassMethod(self,1440);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprInvoke ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprInvoke> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprInvoke> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprErase ( void * self ) const {
    return getDasClassMethod(self,1448);
  }
  __forceinline void invoke_preVisitExprErase ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprErase> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprErase> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprErase ( void * self ) const {
    return getDasClassMethod(self,1456);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprErase ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprErase> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprErase> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSetInsert ( void * self ) const {
    return getDasClassMethod(self,1464);
  }
  __forceinline void invoke_preVisitExprSetInsert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSetInsert> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSetInsert> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSetInsert ( void * self ) const {
    return getDasClassMethod(self,1472);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSetInsert ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSetInsert> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSetInsert> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFind ( void * self ) const {
    return getDasClassMethod(self,1480);
  }
  __forceinline void invoke_preVisitExprFind ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFind> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFind> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFind ( void * self ) const {
    return getDasClassMethod(self,1488);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFind ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFind> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFind> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprKeyExists ( void * self ) const {
    return getDasClassMethod(self,1496);
  }
  __forceinline void invoke_preVisitExprKeyExists ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprKeyExists> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprKeyExists> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprKeyExists ( void * self ) const {
    return getDasClassMethod(self,1504);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprKeyExists ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprKeyExists> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprKeyExists> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAscend ( void * self ) const {
    return getDasClassMethod(self,1512);
  }
  __forceinline void invoke_preVisitExprAscend ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAscend> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAscend> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAscend ( void * self ) const {
    return getDasClassMethod(self,1520);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAscend ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAscend> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAscend> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCast ( void * self ) const {
    return getDasClassMethod(self,1528);
  }
  __forceinline void invoke_preVisitExprCast ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCast> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCast> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCast ( void * self ) const {
    return getDasClassMethod(self,1536);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCast ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCast> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCast> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprDelete ( void * self ) const {
    return getDasClassMethod(self,1544);
  }
  __forceinline void invoke_preVisitExprDelete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDelete> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprDelete> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprDelete ( void * self ) const {
    return getDasClassMethod(self,1552);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprDelete ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprDelete> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprDelete> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprVar ( void * self ) const {
    return getDasClassMethod(self,1560);
  }
  __forceinline void invoke_preVisitExprVar ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprVar> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprVar> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprVar ( void * self ) const {
    return getDasClassMethod(self,1568);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprVar ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprVar> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprVar> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprTag ( void * self ) const {
    return getDasClassMethod(self,1576);
  }
  __forceinline void invoke_preVisitExprTag ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTag> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprTagValue ( void * self ) const {
    return getDasClassMethod(self,1584);
  }
  __forceinline void invoke_preVisitExprTagValue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr, smart_ptr_raw<Expression> const  value ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprTag> const ,smart_ptr_raw<Expression> const >
        (__context__,nullptr,__funcCall__,
          self,expr,value);
  }
  __forceinline Func get_visitExprTag ( void * self ) const {
    return getDasClassMethod(self,1592);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprTag ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprTag> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprTag> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprField ( void * self ) const {
    return getDasClassMethod(self,1600);
  }
  __forceinline void invoke_preVisitExprField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprField> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprField> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprField ( void * self ) const {
    return getDasClassMethod(self,1608);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprField> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprField> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeField ( void * self ) const {
    return getDasClassMethod(self,1616);
  }
  __forceinline void invoke_preVisitExprSafeField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeField> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeField> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeField ( void * self ) const {
    return getDasClassMethod(self,1624);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeField ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeField> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeField> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSwizzle ( void * self ) const {
    return getDasClassMethod(self,1632);
  }
  __forceinline void invoke_preVisitExprSwizzle ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSwizzle> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSwizzle> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSwizzle ( void * self ) const {
    return getDasClassMethod(self,1640);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSwizzle ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSwizzle> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSwizzle> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,1648);
  }
  __forceinline void invoke_preVisitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprIsVariant ( void * self ) const {
    return getDasClassMethod(self,1656);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprIsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprIsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprIsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,1664);
  }
  __forceinline void invoke_preVisitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprAsVariant ( void * self ) const {
    return getDasClassMethod(self,1672);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,1680);
  }
  __forceinline void invoke_preVisitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprSafeAsVariant ( void * self ) const {
    return getDasClassMethod(self,1688);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprSafeAsVariant ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprSafeAsVariant> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprSafeAsVariant> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprOp1 ( void * self ) const {
    return getDasClassMethod(self,1696);
  }
  __forceinline void invoke_preVisitExprOp1 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp1> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprOp1> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprOp1 ( void * self ) const {
    return getDasClassMethod(self,1704);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprOp1 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprOp1> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprOp1> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprReturn ( void * self ) const {
    return getDasClassMethod(self,1712);
  }
  __forceinline void invoke_preVisitExprReturn ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReturn> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprReturn> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprReturn ( void * self ) const {
    return getDasClassMethod(self,1720);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprReturn ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReturn> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprReturn> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprYield ( void * self ) const {
    return getDasClassMethod(self,1728);
  }
  __forceinline void invoke_preVisitExprYield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprYield> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprYield> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprYield ( void * self ) const {
    return getDasClassMethod(self,1736);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprYield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprYield> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprYield> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprBreak ( void * self ) const {
    return getDasClassMethod(self,1744);
  }
  __forceinline void invoke_preVisitExprBreak ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBreak> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprBreak> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprBreak ( void * self ) const {
    return getDasClassMethod(self,1752);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprBreak ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprBreak> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprBreak> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprContinue ( void * self ) const {
    return getDasClassMethod(self,1760);
  }
  __forceinline void invoke_preVisitExprContinue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprContinue> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprContinue> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprContinue ( void * self ) const {
    return getDasClassMethod(self,1768);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprContinue ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprContinue> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprContinue> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_canVisitMakeBlockBody ( void * self ) const {
    return getDasClassMethod(self,1776);
  }
  __forceinline bool invoke_canVisitMakeBlockBody ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    return das_invoke_function<bool>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_preVisitExprMakeBlock ( void * self ) const {
    return getDasClassMethod(self,1784);
  }
  __forceinline void invoke_preVisitExprMakeBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeBlock ( void * self ) const {
    return getDasClassMethod(self,1792);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeBlock ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeBlock> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeBlock> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMakeGenerator ( void * self ) const {
    return getDasClassMethod(self,1800);
  }
  __forceinline void invoke_preVisitExprMakeGenerator ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeGenerator> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMakeGenerator> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMakeGenerator ( void * self ) const {
    return getDasClassMethod(self,1808);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMakeGenerator ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMakeGenerator> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMakeGenerator> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprMemZero ( void * self ) const {
    return getDasClassMethod(self,1816);
  }
  __forceinline void invoke_preVisitExprMemZero ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMemZero> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprMemZero> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprMemZero ( void * self ) const {
    return getDasClassMethod(self,1824);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprMemZero ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprMemZero> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprMemZero> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConst ( void * self ) const {
    return getDasClassMethod(self,1832);
  }
  __forceinline void invoke_preVisitExprConst ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConst> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConst> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConst ( void * self ) const {
    return getDasClassMethod(self,1840);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConst ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConst> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConst> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstPtr ( void * self ) const {
    return getDasClassMethod(self,1848);
  }
  __forceinline void invoke_preVisitExprConstPtr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstPtr> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstPtr> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstPtr ( void * self ) const {
    return getDasClassMethod(self,1856);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstPtr ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstPtr> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstPtr> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstEnumeration ( void * self ) const {
    return getDasClassMethod(self,1864);
  }
  __forceinline void invoke_preVisitExprConstEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstEnumeration> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstEnumeration> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstEnumeration ( void * self ) const {
    return getDasClassMethod(self,1872);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstEnumeration ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstEnumeration> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstEnumeration> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstBitfield ( void * self ) const {
    return getDasClassMethod(self,1880);
  }
  __forceinline void invoke_preVisitExprConstBitfield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBitfield> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstBitfield> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstBitfield ( void * self ) const {
    return getDasClassMethod(self,1888);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstBitfield ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBitfield> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstBitfield> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt8 ( void * self ) const {
    return getDasClassMethod(self,1896);
  }
  __forceinline void invoke_preVisitExprConstInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt8> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt8 ( void * self ) const {
    return getDasClassMethod(self,1904);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt8> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt16 ( void * self ) const {
    return getDasClassMethod(self,1912);
  }
  __forceinline void invoke_preVisitExprConstInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt16> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt16 ( void * self ) const {
    return getDasClassMethod(self,1920);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt16> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt64 ( void * self ) const {
    return getDasClassMethod(self,1928);
  }
  __forceinline void invoke_preVisitExprConstInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt64 ( void * self ) const {
    return getDasClassMethod(self,1936);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt ( void * self ) const {
    return getDasClassMethod(self,1944);
  }
  __forceinline void invoke_preVisitExprConstInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt ( void * self ) const {
    return getDasClassMethod(self,1952);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt2 ( void * self ) const {
    return getDasClassMethod(self,1960);
  }
  __forceinline void invoke_preVisitExprConstInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt2 ( void * self ) const {
    return getDasClassMethod(self,1968);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt3 ( void * self ) const {
    return getDasClassMethod(self,1976);
  }
  __forceinline void invoke_preVisitExprConstInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt3 ( void * self ) const {
    return getDasClassMethod(self,1984);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstInt4 ( void * self ) const {
    return getDasClassMethod(self,1992);
  }
  __forceinline void invoke_preVisitExprConstInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstInt4 ( void * self ) const {
    return getDasClassMethod(self,2000);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstInt4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt8 ( void * self ) const {
    return getDasClassMethod(self,2008);
  }
  __forceinline void invoke_preVisitExprConstUInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt8> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt8 ( void * self ) const {
    return getDasClassMethod(self,2016);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt8 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt8> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt8> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt16 ( void * self ) const {
    return getDasClassMethod(self,2024);
  }
  __forceinline void invoke_preVisitExprConstUInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt16> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt16 ( void * self ) const {
    return getDasClassMethod(self,2032);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt16 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt16> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt16> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt64 ( void * self ) const {
    return getDasClassMethod(self,2040);
  }
  __forceinline void invoke_preVisitExprConstUInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt64 ( void * self ) const {
    return getDasClassMethod(self,2048);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt ( void * self ) const {
    return getDasClassMethod(self,2056);
  }
  __forceinline void invoke_preVisitExprConstUInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt ( void * self ) const {
    return getDasClassMethod(self,2064);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt2 ( void * self ) const {
    return getDasClassMethod(self,2072);
  }
  __forceinline void invoke_preVisitExprConstUInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt2 ( void * self ) const {
    return getDasClassMethod(self,2080);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt3 ( void * self ) const {
    return getDasClassMethod(self,2088);
  }
  __forceinline void invoke_preVisitExprConstUInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt3 ( void * self ) const {
    return getDasClassMethod(self,2096);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstUInt4 ( void * self ) const {
    return getDasClassMethod(self,2104);
  }
  __forceinline void invoke_preVisitExprConstUInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstUInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstUInt4 ( void * self ) const {
    return getDasClassMethod(self,2112);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstUInt4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstUInt4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstUInt4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstRange ( void * self ) const {
    return getDasClassMethod(self,2120);
  }
  __forceinline void invoke_preVisitExprConstRange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstRange> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstRange ( void * self ) const {
    return getDasClassMethod(self,2128);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstRange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstRange> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstURange ( void * self ) const {
    return getDasClassMethod(self,2136);
  }
  __forceinline void invoke_preVisitExprConstURange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstURange> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstURange ( void * self ) const {
    return getDasClassMethod(self,2144);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstURange ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstURange> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstRange64 ( void * self ) const {
    return getDasClassMethod(self,2152);
  }
  __forceinline void invoke_preVisitExprConstRange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstRange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstRange64 ( void * self ) const {
    return getDasClassMethod(self,2160);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstRange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstRange64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstRange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstURange64 ( void * self ) const {
    return getDasClassMethod(self,2168);
  }
  __forceinline void invoke_preVisitExprConstURange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange64> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstURange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstURange64 ( void * self ) const {
    return getDasClassMethod(self,2176);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstURange64 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstURange64> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstURange64> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstBool ( void * self ) const {
    return getDasClassMethod(self,2184);
  }
  __forceinline void invoke_preVisitExprConstBool ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBool> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstBool> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstBool ( void * self ) const {
    return getDasClassMethod(self,2192);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstBool ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstBool> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstBool> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat ( void * self ) const {
    return getDasClassMethod(self,2200);
  }
  __forceinline void invoke_preVisitExprConstFloat ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat ( void * self ) const {
    return getDasClassMethod(self,2208);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat2 ( void * self ) const {
    return getDasClassMethod(self,2216);
  }
  __forceinline void invoke_preVisitExprConstFloat2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat2> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat2> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat2 ( void * self ) const {
    return getDasClassMethod(self,2224);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat2 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat2> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat2> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat3 ( void * self ) const {
    return getDasClassMethod(self,2232);
  }
  __forceinline void invoke_preVisitExprConstFloat3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat3> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat3> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat3 ( void * self ) const {
    return getDasClassMethod(self,2240);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat3 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat3> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat3> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstFloat4 ( void * self ) const {
    return getDasClassMethod(self,2248);
  }
  __forceinline void invoke_preVisitExprConstFloat4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat4> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstFloat4> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstFloat4 ( void * self ) const {
    return getDasClassMethod(self,2256);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstFloat4 ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstFloat4> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstFloat4> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstString ( void * self ) const {
    return getDasClassMethod(self,2264);
  }
  __forceinline void invoke_preVisitExprConstString ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstString> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstString> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstString ( void * self ) const {
    return getDasClassMethod(self,2272);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstString ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstString> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstString> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprConstDouble ( void * self ) const {
    return getDasClassMethod(self,2280);
  }
  __forceinline void invoke_preVisitExprConstDouble ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstDouble> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprConstDouble> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprConstDouble ( void * self ) const {
    return getDasClassMethod(self,2288);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprConstDouble ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprConstDouble> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprConstDouble> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFakeContext ( void * self ) const {
    return getDasClassMethod(self,2296);
  }
  __forceinline void invoke_preVisitExprFakeContext ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeContext> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFakeContext> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFakeContext ( void * self ) const {
    return getDasClassMethod(self,2304);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFakeContext ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeContext> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFakeContext> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprFakeLineInfo ( void * self ) const {
    return getDasClassMethod(self,2312);
  }
  __forceinline void invoke_preVisitExprFakeLineInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeLineInfo> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprFakeLineInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprFakeLineInfo ( void * self ) const {
    return getDasClassMethod(self,2320);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprFakeLineInfo ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprFakeLineInfo> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprFakeLineInfo> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprReader ( void * self ) const {
    return getDasClassMethod(self,2328);
  }
  __forceinline void invoke_preVisitExprReader ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReader> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprReader ( void * self ) const {
    return getDasClassMethod(self,2336);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprReader ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprReader> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprReader> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprUnsafe ( void * self ) const {
    return getDasClassMethod(self,2344);
  }
  __forceinline void invoke_preVisitExprUnsafe ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprUnsafe> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprUnsafe> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprUnsafe ( void * self ) const {
    return getDasClassMethod(self,2352);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprUnsafe ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprUnsafe> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprUnsafe> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
  __forceinline Func get_preVisitExprCallMacro ( void * self ) const {
    return getDasClassMethod(self,2360);
  }
  __forceinline void invoke_preVisitExprCallMacro ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    das_invoke_function<void>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr);
  }
  __forceinline Func get_visitExprCallMacro ( void * self ) const {
    return getDasClassMethod(self,2368);
  }
  __forceinline smart_ptr<Expression> invoke_visitExprCallMacro ( Context * __context__, Func __funcCall__, void * self, smart_ptr_raw<ExprCallMacro> const  expr ) const {
    return das_invoke_function<smart_ptr_raw<Expression>>::invoke
      <void *,smart_ptr_raw<ExprCallMacro> const >
        (__context__,nullptr,__funcCall__,
          self,expr).marshal();
  }
};


#include "daScript/misc/platform.h"

#include "unitTest.h"
#include "module_unitTest.h"

#include "daScript/simulate/simulate_visit_op.h"

//sample of your-engine-float3-type to be aliased as float3 in daScript.
template<> struct das::cast <Point3>  : cast_fVec<Point3> {};

namespace das {
	template <>
	struct typeFactory<Point3> {
	    static TypeDeclPtr make(const ModuleLibrary &) {
	        auto t = make_shared<TypeDecl>(Type::tFloat3);
	        t->alias = "Point3";
	        t->aotAlias = true;
	        return t;
	    }
	};
}

DAS_BASE_BIND_ENUM_98(SomeEnum_16, SomeEnum_16, SomeEnum_16_zero, SomeEnum_16_one, SomeEnum_16_two)

//sample of your engine annotated struct
MAKE_TYPE_FACTORY(TestObjectFoo,TestObjectFoo)
MAKE_TYPE_FACTORY(TestObjectBar, TestObjectBar)
MAKE_TYPE_FACTORY(TestObjectNotLocal, TestObjectNotLocal)

MAKE_TYPE_FACTORY(SomeDummyType, SomeDummyType)

/*
namespace das {
    template <>
    struct cast<TestObjectFoo> : cast_fVec_half<TestObjectFoo> {};
}
*/

struct TestObjectNotLocalAnnotation : ManagedStructureAnnotation <TestObjectNotLocal> {
    TestObjectNotLocalAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TestObjectNotLocal", ml) {
        addField<DAS_BIND_MANAGED_FIELD(fooData)>("fooData");
    }
    virtual bool isLocal() const { return false; }
};

struct TestObjectFooAnnotation : ManagedStructureAnnotation <TestObjectFoo> {
    TestObjectFooAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TestObjectFoo", ml) {
        addField<DAS_BIND_MANAGED_FIELD(hit)>("hit");
        addField<DAS_BIND_MANAGED_FIELD(fooData)>("fooData");
        addField<DAS_BIND_MANAGED_FIELD(e16)>("e16");
        addProperty<DAS_BIND_MANAGED_PROP(propAdd13)>("propAdd13");
        addProperty<DAS_BIND_MANAGED_PROP(hitPos)>("hitPos");
    }
    virtual bool isLocal() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool canCopy() const override { return true; }
};

struct TestObjectBarAnnotation : ManagedStructureAnnotation <TestObjectBar> {
    TestObjectBarAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TestObjectBar", ml) {
        addField<DAS_BIND_MANAGED_FIELD(fooPtr)>("fooPtr");
        addField<DAS_BIND_MANAGED_FIELD(barData)>("barData");
        addProperty<DAS_BIND_MANAGED_PROP(getFoo)>("getFoo");
        addProperty<DAS_BIND_MANAGED_PROP(getFooPtr)>("getFooPtr");
    }
    virtual bool isLocal() const { return true; }
};

void testFoo ( TestObjectFoo & foo ) {
    foo.fooData = 1234;
}

void testAdd ( int & a, int b ) {
    a += b;
}

struct IntFields {
    das_hash_map<string,int32_t> fields;
};

struct CheckRange : StructureAnnotation {
    CheckRange() : StructureAnnotation("checkRange") {}
    virtual bool touch(const StructurePtr & st, ModuleGroup &,
        const AnnotationArgumentList & args, string & ) override {
        // this is here for the 'example' purposes 
        // lets add a sample 'dummy' field
        if (args.getBoolOption("dummy",false) && !st->findField("dummy")) {
            st->fields.emplace_back("dummy", make_shared<TypeDecl>(Type::tInt), 
                nullptr /*init*/, AnnotationArgumentList(), false /*move_to_init*/, LineInfo());
        }
        return true;
    }
    virtual bool look ( const StructurePtr & st, ModuleGroup &,
                        const AnnotationArgumentList & args, string & err ) override {
        bool ok = true;
        if (!args.getBoolOption("disable", false)) {
            for (auto & fd : st->fields) {
                if (fd.type->isSimpleType(Type::tInt) && fd.annotation.size()) {
                    int32_t val = 0;
                    int32_t minVal = INT32_MIN;
                    int32_t maxVal = INT32_MAX;
                    if (fd.init && fd.init->rtti_isConstant()) {
                        val = static_pointer_cast<ExprConstInt>(fd.init)->getValue();
                    }
                    if (auto minA = fd.annotation.find("min", Type::tInt)) {
                        minVal = minA->iValue;
                    }
                    if (auto maxA = fd.annotation.find("max", Type::tInt)) {
                        maxVal = maxA->iValue;
                    }
                    if (val<minVal || val>maxVal) {
                        err += fd.name + " out of annotated range [" + to_string(minVal) + ".." + to_string(maxVal) + "]\n";
                        ok = false;
                    }
                }
            }
        }
        return ok;
    }
};

struct IntFieldsAnnotation : StructureTypeAnnotation {

    // NOTE - SafeGetFieldPtr is not necessary, since its Int always

    // FIELD .
    struct SimNode_IntFieldDeref : SimNode {
        DAS_PTR_NODE;
        SimNode_IntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode(at), value(rv), name(n) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(IntFieldDeref);
            V_SUB(value);
            V_ARG(name);
            V_END();
        }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override {
            SimNode_IntFieldDeref * that = (SimNode_IntFieldDeref *) SimNode::copyNode(context, code);
            that->name = code->allocateName(name);
            return that;
        }
        char * compute ( Context & context ) {
            DAS_PROFILE_NODE
            vec4f rv = value->eval(context);
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return (char *) (&it->second);
                } else {
                    context.throw_error_at(debugInfo, "field %s not found", name);
                    return nullptr;
                }
            } else {
                context.throw_error_at(debugInfo,"dereferencing null pointer");
                return nullptr;
            }
        }
        SimNode *   value;
        char *      name;
    };

    struct SimNode_IntFieldDerefR2V : SimNode_IntFieldDeref {
        DAS_INT_NODE;
        SimNode_IntFieldDerefR2V ( const LineInfo & at, SimNode * rv, char * n )
            : SimNode_IntFieldDeref(at,rv,n) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(IntFieldDerefR2V);
            V_SUB(value);
            V_ARG(name);
            V_END();
        }
        __forceinline int32_t compute ( Context & context ) {
            DAS_PROFILE_NODE
            vec4f rv = value->eval(context);
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return it->second;
                } else {
                    context.throw_error_at(debugInfo,"field %s not found",name);
                    return 0;
                }
            } else {
                context.throw_error_at(debugInfo,"dereferencing null pointer");
                return 0;
            }
        }
    };

    // FIELD ?.
    struct SimNode_SafeIntFieldDeref : SimNode_IntFieldDeref {
        DAS_PTR_NODE;
        SimNode_SafeIntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode_IntFieldDeref(at,rv,n) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(SafeIntFieldDeref);
            V_SUB(value);
            V_ARG(name);
            V_END();
        }
        __forceinline char * compute ( Context & context ) {
            DAS_PROFILE_NODE
            vec4f rv = value->eval(context);
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return (char *) &it->second;
                } else {
                    return nullptr;
                }
            } else {
                return nullptr;
            }
        }
    };

    IntFieldsAnnotation() : StructureTypeAnnotation("IntFields") {}
    virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
        shared_ptr<IntFieldsAnnotation> cp =  p ? static_pointer_cast<IntFieldsAnnotation>(p) : make_shared<IntFieldsAnnotation>();
        return StructureTypeAnnotation::clone(cp);
    }
    virtual bool create ( const shared_ptr<Structure> & st, const AnnotationArgumentList & args, string & err ) override {
        if( !StructureTypeAnnotation::create(st,args,err) )
            return false;
        bool fail = false;
        for ( auto & f : st->fields ) {
            if ( !f.type->isSimpleType(Type::tInt) ) {
                err += "field " + f.name + " must be int\n";
                fail = true;
            }
        }
        return !fail;
    }
    virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
        if ( auto pF = makeSafeFieldType(na) ) {
            pF->ref = true;
            return pF;
        } else {
            return nullptr;
        }
    }
    virtual TypeDeclPtr makeSafeFieldType ( const string & na ) const override {
        auto pF = structureType->findField(na);
        return pF ? make_shared<TypeDecl>(*pF->type) : nullptr;
    }
    virtual SimNode * simulateGetField ( const string & na, Context & context,
                                        const LineInfo & at, const ExpressionPtr & rv ) const  override {
        return context.code->makeNode<SimNode_IntFieldDeref>(at,rv->simulate(context),context.code->allocateName(na));
    }
    virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context,
                                           const LineInfo & at, const ExpressionPtr & rv ) const  override {
        return context.code->makeNode<SimNode_IntFieldDerefR2V>(at,rv->simulate(context),context.code->allocateName(na));
    }    virtual SimNode * simulateSafeGetField ( const string & na, Context & context,
                                            const LineInfo & at, const ExpressionPtr & rv ) const  override {
        return context.code->makeNode<SimNode_SafeIntFieldDeref>(at,rv->simulate(context),context.code->allocateName(na));
    }
    virtual size_t getSizeOf() const override { return sizeof(IntFields); }
    virtual size_t getAlignOf() const override { return alignof(IntFields); }
};

void testFields ( Context * ctx ) {
    int32_t t = 0;
    IntFields x;
    auto fx = ctx->findFunction("testFields");
    if (!fx) {
        ctx->throw_error("function testFields not found");
        return;
    }
    vec4f args[1] = { cast<IntFields *>::from(&x) };
    x.fields["a"] = 1;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(!ctx->getException());
    assert(t==1);
    x.fields["b"] = 2;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(!ctx->getException());
    assert(t==3);
    x.fields["c"] = 3;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(!ctx->getException());
    assert(t==6);
    x.fields["d"] = 4;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(!ctx->getException());
    assert(t==10);
    x.fields.erase("b");
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(!ctx->getException());
    assert(t==8);
}

void test_das_string(const Block & block, Context * context) {
    string str = "test_das_string";
    string str2;
    vec4f args[2];
    args[0] = cast<void *>::from(&str);
    args[1] = cast<void *>::from(&str2);
    context->invoke(block, args, nullptr);
    if (str != "out_of_it") context->throw_error("test string mismatch");
    if (str2 != "test_das_string") context->throw_error("test string clone mismatch");
}

vec4f new_and_init ( Context & context, SimNode_CallBase * call, vec4f * ) {
    TypeInfo * typeInfo = call->types[0];
    if ( typeInfo->dim || typeInfo->type!=Type::tStructure ) {
        context.throw_error("invalid type");
        return v_zero();
    }
    auto size = getTypeSize(typeInfo);
    auto data = context.heap.allocate(size);
    if ( typeInfo->structType && typeInfo->structType->initializer!=-1 ) {
        auto fn = context.getFunction(typeInfo->structType->initializer);
        context.callWithCopyOnReturn(fn, nullptr, data, 0);
    } else {
        memset(data, 0, size);
    }
    return cast<char *>::from(data);
}
int g_st = 0;
int *getPtr() {return &g_st;}

uint2 get_screen_dimensions() {return uint2{1280, 720};}

uint32_t CheckEid ( char * const name, Context * context ) {
    if (!name) context->throw_error("invalid id");
    return hash_function(*context, name);
}

uint32_t CheckEidHint ( char * const name, uint32_t hashHint, Context * context ) {
    if (!name) context->throw_error("invalid id");
    uint32_t hv = hash_function(*context, name);
    if ( hv != hashHint ) context->throw_error("invalid hash value");
    return hashHint;
}

struct CheckEidFunctionAnnotation : TransformFunctionAnnotation {
    CheckEidFunctionAnnotation() : TransformFunctionAnnotation("check_eid") { }
    virtual ExpressionPtr transformCall ( ExprCallFunc * call, string & err ) override {
        auto arg = call->arguments[0];
        if ( arg->type && arg->type->isString() && arg->type->isConst() && arg->rtti_isConstant() ) {
            auto starg = static_pointer_cast<ExprConstString>(arg);
            if (!starg->getValue().empty()) {
                uint32_t hv = hash_blockz32((uint8_t *)starg->text.c_str());
                auto hconst = make_shared<ExprConstUInt>(arg->at, hv);
                hconst->type = make_shared<TypeDecl>(Type::tUInt);
                hconst->type->constant = true;
                auto newCall = static_pointer_cast<ExprCallFunc>(call->clone());
                newCall->arguments.insert(newCall->arguments.begin() + 1, hconst);
                return newCall;
            } else {
                err = "EID can't be an empty string";
            }
        }
        return nullptr;
    }
};

class CheckEid2Macro : public VisitorMacro {
public:
    CheckEid2Macro ( Module * tm ) : VisitorMacro("CheckEid2") , thisModule(tm) {}
protected:
    virtual ExpressionPtr visit ( ExprCall * call ) override {
        if ( call->name=="CheckEid2" && call->func->module==thisModule && call->arguments.size()==2 ) {
            const auto & nameArg = call->arguments[0];
            if ( nameArg->rtti_isStringConstant() ) {
                // add 2nd argument, which is hash of the string
                auto name = static_pointer_cast<ExprConstString>(nameArg)->getValue();
                if (!name.empty()) {
                    auto hv = hash_blockz32((uint8_t *)name.c_str());
                    auto hvc = make_shared<ExprConstUInt>(nameArg->at, hv);
                    call->arguments.insert(call->arguments.begin() + 1, hvc);
                    reportFolding();
                    return call->shared_from_this();
                } else {
                    program->error("EID can't be an empty string", 
                        call->at, CompilationError::invalid_argument_type);
                }
            }
        }
        return VisitorMacro::visit(call);
    }
protected:
    Module * thisModule;
};

class LintEidMacro : public LintMacro {
public:
    LintEidMacro ( Module * tm ) : LintMacro("LintEidMacro"), thisModule(tm) {}
protected:
    virtual ExpressionPtr visit ( ExprCall * call ) override {
        if (    call->name=="CheckEid3"
            &&  call->func->module==thisModule 
            &&  call->arguments.size()==2 ) {
            if (call->arguments[0]->rtti_isStringConstant()) {
                auto cst = static_pointer_cast<ExprConstString>(call->arguments[0]);
                if (cst->getValue().empty()) {
                    program->error("EID can't be an empty string", 
                        call->at, CompilationError::invalid_argument_type);
                }
            }
        }
        return LintMacro::visit(call);
    }
protected:
    Module * thisModule;
};

struct EventRegistrator : StructureAnnotation {
    EventRegistrator() : StructureAnnotation("event") {}
    bool touch ( const StructurePtr & st, ModuleGroup & /*libGroup*/,
        const AnnotationArgumentList & /*args*/, string & /*err*/ ) override {
        st->fields.emplace(st->fields.begin(), "eventFlags", make_shared<TypeDecl>(Type::tUInt16),
            ExpressionPtr(), AnnotationArgumentList(), false, st->at);
        st->fields.emplace(st->fields.begin(), "eventSize", make_shared<TypeDecl>(Type::tUInt16),
            ExpressionPtr(), AnnotationArgumentList(), false, st->at);
        st->fields.emplace(st->fields.begin(), "eventType", make_shared<TypeDecl>(Type::tUInt),
            ExpressionPtr(), AnnotationArgumentList(), false, st->at);
        return true;
    }
    bool look (const StructurePtr & /*st*/, ModuleGroup & /*libGroup*/,
        const AnnotationArgumentList & /*args*/, string & /* err */ ) override {
        return true;
    }
};

#if defined(__APPLE__)

void builtin_printw(char * utf8string) {
    fprintf(stdout, "%s", utf8string);
    fflush(stdout);
}

#else

#include <iostream>
#include <codecvt>
#include <locale>

#if defined(_MSC_VER)
#include <fcntl.h>
#include <io.h>
#endif

void builtin_printw(char * utf8string) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    auto outs = utf8_conv.from_bytes(utf8string);
#if defined(_MSC_VER)
    _setmode(_fileno(stdout), _O_U8TEXT);
#else
    std::wcout.sync_with_stdio(false);
    std::wcout.imbue(std::locale("en_US.utf8"));
#endif
    std::wcout << outs;
#if defined(_MSC_VER)
    _setmode(_fileno(stdout), _O_TEXT);
#endif
}

#endif

bool tempArrayExample( const TArray<char *> & arr, 
    const TBlock<void, TTemporary<const TArray<char *>>> & blk, Context * context ) {
    vec4f args[1];
    args[0] = cast<void *>::from(&arr);
    context->invoke(blk, args, nullptr);
    return (arr.size == 1) && (strcmp(arr[0], "one") == 0);
}

Module_UnitTest::Module_UnitTest() : Module("UnitTest") {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();
    addEnumTest(lib);
    // structure annotations
    addAnnotation(make_shared<CheckRange>());
    addAnnotation(make_shared<IntFieldsAnnotation>());
    // dummy type example
    addAnnotation(make_shared<DummyTypeAnnotation>("SomeDummyType", "SomeDummyType", sizeof(SomeDummyType), alignof(SomeDummyType)));
    // register types
    addAnnotation(make_shared<TestObjectNotLocalAnnotation>(lib));
    addAnnotation(make_shared<TestObjectFooAnnotation>(lib));
    addAnnotation(make_shared<TestObjectBarAnnotation>(lib));
    // events
    addAnnotation(make_shared<EventRegistrator>());
    // utf8 print
    addExtern<DAS_BIND_FUN(builtin_printw)>(*this, lib, "printw", SideEffects::modifyExternal, "builtin_printw");
    // register function
    addEquNeq<TestObjectFoo>(*this, lib);
    addExtern<DAS_BIND_FUN(complex_bind)>(*this, lib, "complex_bind", SideEffects::modifyExternal, "complex_bind");
    addInterop<new_and_init,void *,vec4f>(*this, lib, "new_and_init", SideEffects::none, "new_and_init");
    addExtern<DAS_BIND_FUN(get_screen_dimensions)>(*this, lib, "get_screen_dimensions", SideEffects::none, "get_screen_dimensions");
    addExtern<DAS_BIND_FUN(test_das_string)>(*this, lib, "test_das_string", SideEffects::modifyExternal, "test_das_string");
    addExtern<DAS_BIND_FUN(testFoo)>(*this, lib, "testFoo", SideEffects::modifyArgument, "testFoo");
    addExtern<DAS_BIND_FUN(testAdd)>(*this, lib, "testAdd", SideEffects::modifyArgument, "testAdd");
    addExtern<DAS_BIND_FUN(testFields)>(*this, lib, "testFields", SideEffects::modifyExternal, "testFields");
    addExtern<DAS_BIND_FUN(getSamplePoint3)>(*this, lib, "getSamplePoint3", SideEffects::modifyExternal, "getSamplePoint3");
    addExtern<DAS_BIND_FUN(doubleSamplePoint3)>(*this, lib, "doubleSamplePoint3", SideEffects::none, "doubleSamplePoint3");
    addExtern<DAS_BIND_FUN(project_to_nearest_navmesh_point)>(*this, lib, "project_to_nearest_navmesh_point", SideEffects::modifyArgument, "project_to_nearest_navmesh_point");
    addExtern<DAS_BIND_FUN(getPtr)>(*this, lib, "getPtr", SideEffects::modifyExternal, "getPtr");
    /*
     addExtern<DAS_BIND_FUN(makeDummy)>(*this, lib, "makeDummy", SideEffects::none, "makeDummy");
     */
    addExtern<DAS_BIND_FUN(makeDummy), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "makeDummy", SideEffects::none, "makeDummy");
    addExtern<DAS_BIND_FUN(takeDummy)>(*this, lib, "takeDummy", SideEffects::none, "takeDummy");
    // register Cpp alignment functions
    addExtern<DAS_BIND_FUN(CppS1Size)>(*this, lib, "CppS1Size", SideEffects::modifyExternal, "CppS1Size");
    addExtern<DAS_BIND_FUN(CppS2Size)>(*this, lib, "CppS2Size", SideEffects::modifyExternal, "CppS2Size");
    addExtern<DAS_BIND_FUN(CppS2DOffset)>(*this, lib, "CppS2DOffset", SideEffects::modifyExternal, "CppS2DOffset");
    // register CheckEid functions
    addExtern<DAS_BIND_FUN(CheckEidHint)>(*this, lib, "CheckEid", SideEffects::modifyExternal, "CheckEidHint");
    auto ceid = addExtern<DAS_BIND_FUN(CheckEid)>(*this, lib, "CheckEid", SideEffects::modifyExternal, "CheckEid");
    auto ceid_decl = make_shared<AnnotationDeclaration>();
    ceid_decl->annotation = make_shared<CheckEidFunctionAnnotation>();
    ceid->annotations.push_back(ceid_decl);
    // register CheckEid2 functoins and macro
    addExtern<DAS_BIND_FUN(CheckEidHint)>(*this, lib, "CheckEid2", SideEffects::modifyExternal, "CheckEidHint");
    addExtern<DAS_BIND_FUN(CheckEid)>(*this, lib, "CheckEid2", SideEffects::modifyExternal, "CheckEid");
    macros.push_back(make_shared<CheckEid2Macro>(this));
    addExtern<DAS_BIND_FUN(CheckEid)>(*this, lib, "CheckEid3", SideEffects::modifyExternal, "CheckEid");
    lintMacros.push_back(make_shared<LintEidMacro>(this));
    // extra tests
    addExtern<DAS_BIND_FUN(start_effect)>(*this, lib, "start_effect", SideEffects::modifyExternal, "start_effect");
    addExtern<DAS_BIND_FUN(tempArrayExample)>(*this, lib, "temp_array_example", SideEffects::modifyExternal, "tempArrayExample");
    // compiled functions
    appendCompiledFunctions();
    // and verify
    verifyAotReady();
}

Type Module_UnitTest::getOptionType ( const string & optName ) const {
    if ( optName=="unit_test" ) return Type::tFloat;   
    return Type::none;
}

ModuleAotType Module_UnitTest::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"unitTest.h\"\n";
    return ModuleAotType::cpp;
}

#include "unit_test.das.inc"
bool Module_UnitTest::appendCompiledFunctions() {
    return compileBuiltinModule("unit_test.das",unit_test_das, sizeof(unit_test_das));
}

REGISTER_MODULE(Module_UnitTest);

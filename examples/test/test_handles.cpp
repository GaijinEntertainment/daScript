#include "daScript/misc/platform.h"

#include "unitTest.h"
#include "module_unitTest.h"

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

//sample of your engine annotated struct
MAKE_TYPE_FACTORY(TestObjectFoo,TestObjectFoo)
MAKE_TYPE_FACTORY(TestObjectBar, TestObjectBar)
MAKE_TYPE_FACTORY(TestObjectNotLocal, TestObjectNotLocal)

struct TestObjectNotLocalAnnotation : ManagedStructureAnnotation <TestObjectNotLocal> {
    TestObjectNotLocalAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TestObjectNotLocal", ml) {
        addField<DAS_BIND_MANAGED_FIELD(fooData)>("fooData");
    }
    virtual bool isLocal() const { return false; }
};

struct TestObjectFooAnnotation : ManagedStructureAnnotation <TestObjectFoo> {
    TestObjectFooAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TestObjectFoo", ml) {
        addField<DAS_BIND_MANAGED_FIELD(fooData)>("fooData");
        addProperty<DAS_BIND_MANAGED_PROP(propAdd13)>("propAdd13");
    }
    virtual bool isLocal() const { return true; }
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
    map<string,int32_t> fields;
};

struct CheckRange : StructureAnnotation {
    CheckRange() : StructureAnnotation("checkRange") {}
    virtual bool touch(const StructurePtr & st, ModuleGroup &,
        const AnnotationArgumentList & args, string & ) override {
        // this is here for the 'example' purposes 
        // lets add a sample 'dummy' field
        if (args.getOption("dummy",false) && !st->findField("dummy")) {
            st->fields.emplace_back("dummy", make_shared<TypeDecl>(Type::tInt), 
                nullptr /*init*/, AnnotationArgumentList(), false /*move_to_init*/, LineInfo());
        }
        return true;
    }
    virtual bool look ( const StructurePtr & st, ModuleGroup &,
                        const AnnotationArgumentList & args, string & err ) override {
        bool ok = true;
        if (!args.getOption("disable", false)) {
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
        char * compute ( Context & context ) {
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
        __forceinline int32_t compute ( Context & context ) {
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
        SimNode *   value;
        char *      name;
    };

    // FIELD ?.
    struct SimNode_SafeIntFieldDeref : SimNode_IntFieldDeref {
        DAS_PTR_NODE;
        SimNode_SafeIntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode_IntFieldDeref(at,rv,n) {}
        __forceinline char * compute ( Context & context ) {
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

das::uint2 get_screen_dimensions() {return das::uint2{1280, 720};}

Module_UnitTest::Module_UnitTest() : Module("UnitTest") {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();
    addEnumTest(lib);
    // structure annotations
    addAnnotation(make_shared<CheckRange>());
    addAnnotation(make_shared<IntFieldsAnnotation>());
    // register types
    addAnnotation(make_shared<TestObjectNotLocalAnnotation>(lib));
    addAnnotation(make_shared<TestObjectFooAnnotation>(lib));
    addAnnotation(make_shared<TestObjectBarAnnotation>(lib));
    // register function
    addEquNeq<TestObjectFoo>(*this, lib);
    addInterop<new_and_init,void *,vec4f>(*this, lib, "new_and_init", SideEffects::none);
    addExtern<DAS_BIND_FUN(get_screen_dimensions)>(*this, lib, "get_screen_dimensions", SideEffects::none);
    addExtern<DAS_BIND_FUN(test_das_string)>(*this, lib, "test_das_string", SideEffects::none);
    addExtern<DAS_BIND_FUN(testFoo)>(*this, lib, "testFoo", SideEffects::modifyArgument);
    addExtern<DAS_BIND_FUN(testAdd)>(*this, lib, "testAdd", SideEffects::modifyArgument);
    addExtern<DAS_BIND_FUN(testFields)>(*this, lib, "testFields", SideEffects::modifyExternal);
    addExtern<DAS_BIND_FUN(getSamplePoint3)>(*this, lib, "getSamplePoint3", SideEffects::none);
    addExtern<DAS_BIND_FUN(doubleSamplePoint3)>(*this, lib, "doubleSamplePoint3", SideEffects::modifyArgument);
    addExtern<DAS_BIND_FUN(getPtr)>(*this, lib, "getPtr", SideEffects::modifyExternal);
}

bool Module_UnitTest::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"unitTest.h\"\n";
    return true;
}

REGISTER_MODULE(Module_UnitTest);

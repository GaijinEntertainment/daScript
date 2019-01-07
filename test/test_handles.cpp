#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"
#include "ast_handle.h"
#include "interop.h"

using namespace std;
using namespace das;

struct TestObjectFoo {
    int fooData;
};

template <>
struct typeFactory<TestObjectFoo> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectFoo");
    }
};

struct TestObjectBar {
    TestObjectFoo * fooPtr;
    float           barData;
};

template <>
struct typeFactory<TestObjectBar> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectBar");
    }
};

struct TestObjectFooAnnotation : ManagedStructureAnnotation <TestObjectFoo> {
    TestObjectFooAnnotation() : ManagedStructureAnnotation ("TestObjectFoo") {
        addField("fooData", offsetof(TestObjectFoo,fooData),make_shared<TypeDecl>(Type::tInt));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        TestObjectFoo * of = (TestObjectFoo *) data;
        ss << "{fooData=" << of->fooData << "}";
    }
};

struct TestObjectBarAnnotation : ManagedStructureAnnotation <TestObjectBar> {
    TestObjectBarAnnotation(ModuleLibrary & lib) : ManagedStructureAnnotation ("TestObjectBar") {
        auto fooPtr = make_shared<TypeDecl>(Type::tPointer);
        fooPtr->firstType = lib.makeHandleType("TestObjectFoo");
        addField("fooPtr", offsetof(TestObjectBar,fooPtr),fooPtr);
        addField("barData", offsetof(TestObjectBar,barData),make_shared<TypeDecl>(Type::tFloat));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        TestObjectBar * ob = (TestObjectBar *) data;
        ss << "{";
        if ( ob->fooPtr ) {
            ss << "fooPtr.fooData=" << ob->fooPtr->fooData;
        } else {
            ss << "fooPtr=null";
        }
        ss << ",barData=" << ob->barData << "}";
    }
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

struct IntFieldsAnnotation : StructureTypeAnnotation {
    
    // NOTE - SafeGetFieldPtr is not necessary, since its Int always
    
    // FIELD .
    struct SimNode_IntFieldDeref : SimNode {
        DAS_PTR_NODE;
        SimNode_IntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode(at), value(rv), name(n) {}
        char * compute ( Context & context ) {
            __m128 rv = value->eval(context);
            DAS_PTR_EXCEPTION_POINT;
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return (char *) (&it->second);
                } else {
                    context.throw_error("field not found");
                    return nullptr;
                }
            } else {
                context.throw_error("dereferencing null pointer");
                return nullptr;
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
            __m128 rv = value->eval(context);
            DAS_PTR_EXCEPTION_POINT;
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
    virtual TypeDeclPtr makeFieldType ( const string & name ) const override {
        if ( auto pF = makeSafeFieldType(name) ) {
            pF->ref = true;
            return pF;
        } else {
            return nullptr;
        }
    }
    virtual TypeDeclPtr makeSafeFieldType ( const string & name ) const override {
        auto pF = structureType->findField(name);
        return pF ? make_shared<TypeDecl>(*pF->type) : nullptr;
    }
    virtual SimNode * simulateGetField ( const string & name, Context & context, const LineInfo & at, SimNode * rv ) const  override {
        return context.makeNode<SimNode_IntFieldDeref>(at,rv,context.allocateName(name));
    }
    virtual SimNode * simulateSafeGetField ( const string & name, Context & context, const LineInfo & at, SimNode * rv ) const  override {
        return context.makeNode<SimNode_SafeIntFieldDeref>(at,rv,context.allocateName(name));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        IntFields * prv = (IntFields *) data;
        if ( !prv ) {
            ss << "null";
            return;
        }
        ss << "{";
        for ( auto & f : prv->fields )
            ss << " " << f.first << ":" << f.second;
        ss << " }";
    }
};

void testFields ( Context * ctx ) {
    int32_t t = 0;
    IntFields x;
    auto fx = ctx->findFunction("testFields");
    __m128 args[1] = { cast<IntFields *>::from(&x) };
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

class Module_UnitTest : public Module {
public:
    Module_UnitTest() : Module("UnitTest") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // structure annotations
        addAnnotation(make_shared<IntFieldsAnnotation>());
        // register types
        addAnnotation(make_shared<TestObjectFooAnnotation>());
        addAnnotation(make_shared<TestObjectBarAnnotation>(lib));
        // register function
        addExtern<decltype(testFoo), testFoo>(*this, lib, "testFoo");
        addExtern<decltype(testAdd), testAdd>(*this, lib, "testAdd");
        addExtern<decltype(testFields), testFields>(*this, lib, "testFields");
    }
};

REGISTER_MODULE(Module_UnitTest);

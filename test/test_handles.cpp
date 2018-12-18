#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"
#include "ast_handle.h"
#include "interop.h"

#include <dirent.h>

using namespace std;
using namespace yzg;

struct TestObjectFoo {
    int fooData;
};

template <>
struct typeFactory<TestObjectFoo *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectFoo");
    }
};

struct TestObjectBar {
    TestObjectFoo * fooPtr;
    float           barData;
};

template <>
struct typeFactory<TestObjectBar *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectBar");
    }
};

struct TestObjectFooAnnotation : StructureTypeAnnotation<TestObjectFoo> {
    TestObjectFooAnnotation() : StructureTypeAnnotation("TestObjectFoo") {
        addField("fooData", offsetof(TestObjectFoo,fooData),make_shared<TypeDecl>(Type::tInt));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        TestObjectFoo * of = (TestObjectFoo *) data;
        ss << "{fooData=" << of->fooData << "}";
    }
};

struct TestObjectBarAnnotation : StructureTypeAnnotation<TestObjectBar> {
    TestObjectBarAnnotation(ModuleLibrary & lib) : StructureTypeAnnotation("TestObjectBar") {
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

void testFoo ( TestObjectFoo * foo ) {
    foo->fooData = 1234;
}

class Module_UnitTest : public Module {
public:
    Module_UnitTest() : Module("UnitTest") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // register types
        addHandle(make_unique<TestObjectFooAnnotation>());
        addHandle(make_unique<TestObjectBarAnnotation>(lib));
        // register function
        addExtern<decltype(testFoo), testFoo>(*this, lib, "testFoo");
    }
};

REGISTER_MODULE(Module_UnitTest);

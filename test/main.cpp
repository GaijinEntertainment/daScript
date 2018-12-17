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

struct TestObjectBar {
    TestObjectFoo * fooPtr;
    float           barData;
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

class Module_UnitTest : public Module {
public:
    Module_UnitTest() : Module("unitTest") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // register types
        addHandle(make_unique<TestObjectFooAnnotation>());
        addHandle(make_unique<TestObjectBarAnnotation>(lib));
    }
};

bool g_reportCompilationFailErrors = false;

bool compilation_fail_test ( const string & fn ) {
    cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str()) ) {
        if ( program->failed() ) {
            bool failed = false;
            auto errors = program->expectErrors;
            for ( auto err : program->errors ) {
                int count = -- errors[err.cerr];
                if ( g_reportCompilationFailErrors || count<0 ) {
                    cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
                }
                if ( count <0 ) {
                    failed = true;
                }
            }
            bool any_errors = false;
            for ( auto err : errors ) {
                if ( err.second > 0 ) {
                    any_errors = true;
                    break;
                }
            }
            if ( any_errors || failed ) {
                cout << "failed";
                if ( any_errors ) {
                    cout << ", expecting errors";
                    for ( auto cerr : errors  ) {
                        if ( cerr.second > 0 ) {
                            cout << " " << int(cerr.first) << ":" << cerr.second;
                        }
                    }
                }
                cout << "\n";
                return false;
            }
            cout << "ok\n";
            return true;
        } else {
            cout << "failed, compiled without errors\n";
            return false;
        }
    } else {
        cout << "failed, not expected to compile\n";
        return false;
    }
}

bool unit_test ( const string & fn ) {
    cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str()) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            // cout << *program << "\n";
            Context ctx(&str);
            program->simulate(ctx);
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
                if ( auto ex = ctx.getException() ) {
                    cout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    cout << "failed\n";
                    return false;
                }
                cout << "ok\n";
                return true;
            } else {
                cout << "function 'test' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}

bool run_tests( const string & path, bool (*test_fn)(const string &) ) {
    bool ok = true;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if ( strstr(ent->d_name,".das") ) {
                ok = test_fn(path + "/" + ent->d_name) && ok;
            }
        }
        closedir (dir);
    }
    return ok;
}

bool run_unit_tests( const string & path ) {
    return run_tests(path, unit_test);
}

bool run_compilation_fail_tests( const string & path ) {
    return run_tests(path, compilation_fail_test);
}


int main(int argc, const char * argv[]) {
    
    auto module_unitTest = make_unique<Module_UnitTest>();
    
#if 0 // Debug this one test
    compilation_fail_test("../../test/hello_world.das");
    return 0;
#endif
#if 0 // Debug this one test
    unit_test("../../test/unit_tests/handle.das");
    return 0;
#endif
    bool ok = true;
    ok = run_compilation_fail_tests("../../test/compilation_fail_tests") && ok;
    ok = run_unit_tests("../../test/unit_tests") && ok;
    cout << "TESTS " << (ok ? "PASSED" : "FAILED!!!") << "\n";
    return ok ? 0 : -1;
}

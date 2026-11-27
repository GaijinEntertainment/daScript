#define DAS_SHOULD_USE_STUBS 1

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptBind.h"

#include <cstring>

using namespace das;

int stub_host_only ( int a );
inline int stub_inline ( int a ) { return stub_host_only(a); }

struct StubHost {
    int hostOnly () const;
    int twice () const { return hostOnly() * 2; }
    int add ( int a ) { return hostOnly() + a; }
};

MAKE_TYPE_FACTORY(StubHost, StubHost)

struct StubHostAnnotation : ManagedStructureAnnotation<StubHost, false> {
    StubHostAnnotation ( ModuleLibrary & ml ) : ManagedStructureAnnotation("StubHost", ml) {
        addPropertyForManagedType<DAS_PROP_DECL(ManagedType::twice), true>("twice", "twice");
    }
};

class Module_HostStubTest : public Module {
public:
    Module_HostStubTest() : Module("host_stub_test") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addAnnotation(new StubHostAnnotation(lib));
        addExternHost<decltype(&stub_inline), &stub_inline>(*this, lib, "stub_inline",
            SideEffects::none, "stub_inline");
        addExternHost<decltype(&StubHost::add), &StubHost::add>(*this, lib, "stub_add",
            SideEffects::none, "das_call_member<decltype(&StubHost::add),&StubHost::add>::invoke");
    }
};
REGISTER_MODULE(Module_HostStubTest);

TEST_CASE("an AOT compiler binds host code it does not link") {
    const char * src =
        "options gen2\n"
        "require host_stub_test\n"
        "def never_called(var h : StubHost) : int {\n"
        "    return stub_inline(1) + stub_add(h, 2) + h.twice\n"
        "}\n";
    TextPrinter tout;
    ModuleGroup group;
    auto access = make_smart<FsFileAccess>();
    access->setFileInfo("host_stub.das", make_unique<TextFileInfo>(src, uint32_t(strlen(src)), false));
    auto program = compileDaScript("host_stub.das", access, tout, group);
    REQUIRE_FALSE(program->failed());
}

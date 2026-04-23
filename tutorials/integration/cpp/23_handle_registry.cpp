// Tutorial 23 — Binding C++ std::shared_ptr via Handle<T>
//
// Companion to Tutorial 12 (smart pointers). Where tutorial 12 requires
// the C++ type to inherit das::ptr_ref_count (intrusive refcount),
// this tutorial shows the alternative pattern used when the C++ type
// already uses std::shared_ptr and cannot — or should not — be retrofitted.
//
//   - std::shared_ptr<T> ownership stays on the C++ side.
//   - HandleRegistry<T>::instance() holds the live shared_ptr.
//   - Scripts see Handle<T> — a 64-bit value encoding
//       (generation << 32) | (index + 1).
//   - addHandleAnnotation<T>() registers the type plus ==, !=, is_alive,
//     and an optional destroy function.
//   - handleRegistry_registerDump (called from inside addHandleAnnotation)
//     wires up leak-at-shutdown reporting automatically.
//
// The same pattern powers dasHV (WebSocketClient / WebSocketServer /
// HttpServer) in modules/dasHV. This tutorial reproduces the mechanics
// with a self-contained toy type (Actor) — no external dependencies.

#include "daScript/daScript.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"           // ManagedHandleAnnotation, addHandleAnnotation, cast<Handle<T>>
#include "daScript/misc/handle_registry.h"     // Handle<T>, HandleRegistry<T>

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

using namespace das;

// -----------------------------------------------------------------------
// The C++ type — plain std::shared_ptr ownership, NO ptr_ref_count.
// This is the whole point of the tutorial: you do NOT need to touch
// the class to expose it.
// -----------------------------------------------------------------------

struct Actor {
    std::string name;
    float       x      = 0.f;
    float       y      = 0.f;
    int32_t     health = 100;

    explicit Actor(const char * n) : name(n ? n : "unnamed") {
        printf("  [C++] Actor('%s') constructed\n", name.c_str());
    }
    ~Actor() {
        printf("  [C++] Actor('%s') destroyed\n", name.c_str());
    }

    void    move(float dx, float dy)   { x += dx; y += dy; }
    void    take_damage(int32_t dmg)   { health -= dmg; if (health < 0) health = 0; }
    bool    is_dead() const            { return health <= 0; }
};

// Provide typeName<Actor>::name() — dumpHandleLeaks<T> uses it to label
// leak output, and typeFactory<Handle<Actor>>::make() uses it to build
// the daslang type. MAKE_TYPE_FACTORY also defines typeFactory<Actor>,
// but that specialization is unused here because Handle<T> has its own
// typeFactory in ast_handle.h.
MAKE_TYPE_FACTORY(Actor, Actor)

// -----------------------------------------------------------------------
// Factory — make_shared on the engine side, then hand out a handle.
// -----------------------------------------------------------------------

static Handle<Actor> make_actor(const char * name, float x, float y) {
    auto sp = std::make_shared<Actor>(name);
    sp->x = x;
    sp->y = y;
    return HandleRegistry<Actor>::instance().acquire(sp);
}

// -----------------------------------------------------------------------
// Bound "methods" — take Handle<Actor> by value, lookup() to resolve.
// lookup() returns an empty shared_ptr for null, stale, or reused handles;
// always null-check the result.
// -----------------------------------------------------------------------

static void actor_move(Handle<Actor> h, float dx, float dy) {
    if ( auto p = HandleRegistry<Actor>::instance().lookup(h) ) {
        p->move(dx, dy);
    }
}

static void actor_take_damage(Handle<Actor> h, int32_t dmg) {
    if ( auto p = HandleRegistry<Actor>::instance().lookup(h) ) {
        p->take_damage(dmg);
    }
}

static int32_t actor_health(Handle<Actor> h) {
    if ( auto p = HandleRegistry<Actor>::instance().lookup(h) ) return p->health;
    return 0;
}

static bool actor_is_dead(Handle<Actor> h) {
    auto p = HandleRegistry<Actor>::instance().lookup(h);
    return !p || p->is_dead();
}

static const char * actor_name(Handle<Actor> h) {
    auto p = HandleRegistry<Actor>::instance().lookup(h);
    return p ? p->name.c_str() : "";
}

// Engine-owned actor — the engine keeps its own shared_ptr so the object
// outlives the script-side handle. Demonstrates that release(h) only drops
// the registry's copy; the Actor is destroyed when the LAST shared_ptr
// (engine's or registry's) goes away.
static std::vector<std::shared_ptr<Actor>> g_engine_actors;

static Handle<Actor> engine_register(const char * name) {
    auto sp = std::make_shared<Actor>(name);
    g_engine_actors.push_back(sp);
    return HandleRegistry<Actor>::instance().acquire(sp);
}

// -----------------------------------------------------------------------
// Module
// -----------------------------------------------------------------------

class Module_Tutorial23 : public Module {
public:
    Module_Tutorial23() : Module("tutorial_23_cpp") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();

        // Register the handle type. addHandleAnnotation also registers
        //   ==, !=, is_alive, and (because destroyFnName is non-empty)
        //   destroy_actor. A per-T leak-dump hook is installed so the
        //   runtime prints any live handles at Module::Shutdown().
        //
        //   name          = "Actor"              — daslang type name
        //   destroyFnName = "destroy_actor"      — daslang release()
        //   cppTypeName   = "das::Handle<Actor>" — what AOT writes into
        //                                          generated C++ stubs
        addHandleAnnotation<Actor>(this, lib, "Actor",
            "destroy_actor", "das::Handle<Actor>");

        addExtern<DAS_BIND_FUN(make_actor)>(*this, lib, "make_actor",
            SideEffects::modifyExternal, "make_actor")
                ->args({"name","x","y"});

        // Handle<Actor> is a value type — the handle itself isn't mutated,
        // the Actor behind it (in the registry) is. That's modifyExternal,
        // NOT modifyArgument (which requires a non-const reference arg).
        addExtern<DAS_BIND_FUN(actor_move)>(*this, lib, "move_actor",
            SideEffects::modifyExternal, "actor_move")
                ->args({"actor","dx","dy"});

        addExtern<DAS_BIND_FUN(actor_take_damage)>(*this, lib, "take_damage",
            SideEffects::modifyExternal, "actor_take_damage")
                ->args({"actor","amount"});

        addExtern<DAS_BIND_FUN(actor_health)>(*this, lib, "health",
            SideEffects::accessExternal, "actor_health")
                ->args({"actor"});

        addExtern<DAS_BIND_FUN(actor_is_dead)>(*this, lib, "is_dead",
            SideEffects::accessExternal, "actor_is_dead")
                ->args({"actor"});

        addExtern<DAS_BIND_FUN(actor_name)>(*this, lib, "name_of",
            SideEffects::accessExternal, "actor_name")
                ->args({"actor"});

        addExtern<DAS_BIND_FUN(engine_register)>(*this, lib, "engine_register",
            SideEffects::modifyExternal, "engine_register")
                ->args({"name"});
    }
};

REGISTER_MODULE(Module_Tutorial23);

// -----------------------------------------------------------------------
// Host program
// -----------------------------------------------------------------------

#define SCRIPT_NAME "/tutorials/integration/cpp/23_handle_registry.das"

void tutorial() {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();

    auto program = compileDaScript(getDasRoot() + SCRIPT_NAME,
                                   fAccess, tout, dummyLibGroup);
    if ( program->failed() ) {
        tout << "Compilation failed:\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra,
                                err.fixme, err.cerr);
        }
        return;
    }

    Context ctx(program->getContextStackSize());
    if ( !program->simulate(ctx, tout) ) {
        tout << "Simulation failed:\n";
        for ( auto & err : program->errors ) {
            tout << reportError(err.at, err.what, err.extra,
                                err.fixme, err.cerr);
        }
        return;
    }

    auto fnTest = ctx.findFunction("test");
    if ( !fnTest ) {
        tout << "Function 'test' not found\n";
        return;
    }

    ctx.evalWithCatch(fnTest, nullptr);
    if ( auto ex = ctx.getException() ) {
        tout << "Exception: " << ex << "\n";
    }
}

int main(int, char * []) {
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_Tutorial23);
    Module::Initialize();
    tutorial();
    // Module::Shutdown(bool dumpHandleLeaks = true) calls
    // handleRegistry_dumpAll() directly; pass false to suppress the dump
    // (useful in CI runs that exit with known-live handles). Any live
    // Handle<Actor> would be printed there — this tutorial is designed
    // to end with zero leak output.
    Module::Shutdown();
    // Engine drops its own shared_ptrs last, so the Shopkeeper destructor
    // runs after the registry dump — proving the two lifetimes are
    // independent.
    g_engine_actors.clear();
    return 0;
}

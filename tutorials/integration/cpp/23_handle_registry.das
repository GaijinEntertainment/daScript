options gen2

// Companion script for Tutorial 23 — Binding C++ std::shared_ptr via Handle<T>.
//
// Handle<Actor> is a value type (64-bit). No `var inscope` needed —
// copies are cheap, equality is structural, and `is_alive` reports
// whether the underlying shared_ptr is still registered.

require tutorial_23_cpp

[export]
def test() {
    // --- Factory ---
    print("=== Create actors ===\n")
    var hero   = make_actor("Hero",   0.0, 0.0)
    var goblin = make_actor("Goblin", 10.0, 5.0)
    print("hero:   {name_of(hero)} hp={health(hero)} alive={!is_dead(hero)}\n")
    print("goblin: {name_of(goblin)} hp={health(goblin)} alive={!is_dead(goblin)}\n")

    // --- Handles are values — cheap copies, structural equality ---
    print("\n=== Value semantics ===\n")
    var hero_copy = hero
    print("hero == hero_copy : {hero == hero_copy}\n")
    print("hero == goblin    : {hero == goblin}\n")

    // --- Methods: the C++ side calls lookup() + null-check ---
    print("\n=== Combat ===\n")
    move_actor(hero, 3.0, 4.0)
    take_damage(goblin, 30)
    print("hero hp = {health(hero)}\n")
    print("goblin hp after 30 damage = {health(goblin)}\n")

    take_damage(goblin, 200)
    print("goblin hp after lethal    = {health(goblin)}  is_dead={is_dead(goblin)}\n")

    // --- Explicit destroy — release() on the registry ---
    print("\n=== Explicit destroy ===\n")
    print("is_alive(goblin) before destroy = {is_alive(goblin)}\n")
    destroy_actor(goblin)
    print("is_alive(goblin) after  destroy = {is_alive(goblin)}\n")

    // Calls through a dead handle are no-ops / return defaults.
    print("dead goblin.health = {health(goblin)}\n")
    print("dead goblin.name   = '{name_of(goblin)}'\n")

    // --- Engine-owned actor: registry releases, engine keeps the object ---
    print("\n=== Engine-owned lifetime ===\n")
    var npc = engine_register("Shopkeeper")
    destroy_actor(npc)
    print("after destroy: is_alive(npc) = {is_alive(npc)}\n")
    // Shopkeeper's destructor runs later, when the engine drops its vector.

    // --- Explicit lifetime: Handle<T> does NOT auto-release ---
    // Unlike smart_ptr<T> (Tutorial 12), Handle<T> is a value type with
    // no scope-based release. Any handle the script acquired via a
    // factory must be explicitly destroyed, or it leaks — the runtime
    // will print `Handle<Actor> idx=... (rc=1)` at shutdown.
    destroy_actor(hero)              // also covers hero_copy (same value)

    print("\n=== End of test — expect zero leak-dump output below ===\n")
}

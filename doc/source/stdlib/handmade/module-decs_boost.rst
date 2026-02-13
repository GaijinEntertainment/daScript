The DECS_BOOST module provides convenience macros and syntactic sugar for
the DECS entity component system, including simplified component registration,
entity creation, and system definition patterns.

See also :doc:`decs` for the core ECS runtime and :doc:`decs_state` for entity state machines.

All functions and symbols are in "decs_boost" module, use require to get access to it. ::

    require daslib/decs_boost

Example: ::

    options persistent_heap = true
        require daslib/decs_boost

        [export]
        def main() {
            restart()
            create_entity <| @(eid, cmp) {
                cmp |> set("pos", float3(1, 2, 3))
                cmp |> set("name", "hero")
            }
            commit()
            query <| $(pos : float3; name : string) {
                print("{name} at {pos}\n")
            }
        }
        // output:
        // hero at 1,2,3

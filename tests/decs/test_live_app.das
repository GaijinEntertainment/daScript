options persistent_heap = true
options gc

require daslib/decs_boost
require daslib/live

[export]
def initialize(full_restart:bool)
    if full_restart
        restart()
        create_entity <| @ ( eid, cmp )
            cmp |> set("name", "one entity")
        commit()

[export]
def act ( dt : float )
    query <| $ ( name:string )
        assert(name=="one entity")

[export]
def shutdown
    pass

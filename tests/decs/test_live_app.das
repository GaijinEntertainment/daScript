options persistent_heap = true
options gc

require daslib/decs_boost

[export]
def initialize(full_restart:bool)
    print("initialize - {full_restart}\n")
    if full_restart
        restart()
        create_entity <| @ ( eid, cmp )
            cmp |> set("name", "one entity {full_restart}")
        commit()

[export]
def act ( dt : float )
    print("dt = {dt}\n")
    query <| $ ( name:string )
        print("one entity is {name}\n")

[export]
def shutdown
    print("shutdown\n")

options gen2
options no_aliasing = true
options force_inscope_pod = true
require dastest/testing_boost public

struct Member {
    id : int
    tags : array<string>
}

struct Room {
    members : array<Member>
}

class Session {
    room : Room = Room()
}

var g_session : Session? = null

def add_host(skip : bool) : int {
    if (skip) {
        return 0
    }
    var host = Member(id = length(g_session.room.members) + 1)
    g_session.room.members |> emplace(host)
    return length(g_session.room.members)
}

[test]
def test_early_out_local_init_reads_through_pointer(t : T?) {
    unsafe {
        g_session = new Session()
    }
    t |> equal(0, add_host(true))
    t |> equal(1, add_host(false))
    t |> equal(2, add_host(false))
    t |> equal(2, g_session.room.members[1].id)
    unsafe {
        delete g_session
    }
}

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_globals( t: T? )
    print("hello")

[decs(stage = update_ecs)]
def update_ecs(pos : float3)
    pass // return

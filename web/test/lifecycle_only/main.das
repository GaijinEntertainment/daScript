options gen2
var count = 0
[export]
def init(){count = 41}
[export]
def update() : bool {verify(count == 41); count++; return false}
[export]
def shutdown(){verify(count == 42); print("LIFECYCLE_ONLY_OK\n")}

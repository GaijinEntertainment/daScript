options log_infer_passes = true

enum DeviceAnimState
    DEVICE_STATE_DISABLED
    DEVICE_STATE_DISABLED2
    DEVICE_STATE_DISABLED3
    DEVICE_STATE_DISABLED4
    DEVICE_STATE_DISABLED5

[export,unsafe]
def test
	for enum_indx, anim_state in each_enum(DeviceAnimState DEVICE_STATE_DISABLED), [[string[5] "device_disabled"; "device_prepare"; "device_use"; "device_fire"; "device_post"]]
		debug(" {int(enum_indx)} {anim_state}")
	return true

/*
struct Foo
	a : array<int>

[export,unsafe]
def test()
	var f : Foo
	resize(f.a, 1)
	var pf : Foo? = addr(f)
	var dummy : int
	pf?.a?[0] ?? dummy = 1
	print("f = {f}\n")
	return true
*/

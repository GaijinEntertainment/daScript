struct Foo
	a,b:int

var
	ds : das_string?

[sideeffects]
def get_ecs_string(obj,name) : das_string?
	return ds

def get_string(obj; name, default : string)
	let str = get_ecs_string(obj, name)
	return str != null ? string(deref(str)) : default

[export]
def test
	let abilityObj : Foo
	var name = get_string(abilityObj, "name", "")
	name = get_string(abilityObj, "name", "")
    return true


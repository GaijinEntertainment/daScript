require padd_function

def take_structure ( foo : $padd_structure(type<auto>,null) )
	debug(foo)

[export]
def main
	var foo : $padd_structure(type<int>,4)
	take_structure(foo)
	var bar : $padd_structure(type<float>,8)
	take_structure(bar)

options log


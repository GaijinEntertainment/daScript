struct Foo
	a : array<int>

struct Bar
	b : array<Foo>

def fail_field
	var a <- [[Foo a <- [1,2,3,4] ]]
	for b in a.a
		return <- a	// EXCEPTION: object type<Foo>.a contains locked elements and can't be modified (resized, pushed to, erased from, cleared, deleted, moved, or returned via move)
	return <- a

def fail_array_index
	var b : Bar
	b.b |> emplace ( [[Foo a <- [1,2,3,4] ]] )
	for a in b.b[0].a
		return <- b	// EXCEPTION: object type<Bar>.b[0].a contains locked elements and can't be modified (resized, pushed to, erased from, cleared, deleted, moved, or returned via move)
	return <- b

def fail_tuple_index
	var b : tuple<Foo>
	b._0 <- [[Foo a <- [1,2,3,4] ]]
	for a in b._0.a
		return <- b	// EXCEPTION: object type<tuple<Foo>>._0.a contains locked elements and can't be modified (resized, pushed to, erased from, cleared, deleted, moved, or returned via move)
	return <- b

def fail_variant
	var b : variant<b:Foo>
	b as b <- [[Foo a <- [1,2,3,4] ]]
	for a in (b as b).a
		return <- b // EXCEPTION: object type<variant<b:Foo>> as b.a contains locked elements and can't be modified (resized, pushed to, erased from, cleared, deleted, moved, or returned via move)
	return <- b

// suffer indeed. belly ache from laughing out loud

[export]
def main
	// fail_field()
	// fail_array_index()
	// fail_tuple_index()
	fail_variant()


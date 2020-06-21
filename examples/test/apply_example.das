require daslib/apply
require daslib/json
require daslib/json_boost

struct Bar
	x, y : float

struct Foo
	i : int
	s : string
	q : double[3]
	b : Bar

def JV ( i : int )
	return JV ( double(i) )

def JV ( i : float )
	return JV ( double(i) )

def saveToJson_any ( value ) : JsonValue?
	return JV(value)

def saveToJson_dim ( val : auto[] ) : JsonValue?
	var arr : array<JsonValue?>
	for x in val
		push(arr, saveToJson(x))
	return JV(arr)

def saveToJson_struct ( value ) : JsonValue?
	var tab : table<string; JsonValue?>
	apply(value) <| $ ( name:string; field )
		tab[name] = saveToJson(field)
	return JV(tab)

def saveToJson ( value ) : JsonValue?
	static_if typeinfo(is_dim value)
		return saveToJson_dim(value)
	static_elif typeinfo(is_struct value)
		return saveToJson_struct(value)
	// todo: support table, array, etc
	else
		return saveToJson_any(value)

[export]
def test
	let foo = [[Foo i=1, s="hello", q =[[double 1.0lf; 2.0lf; 3.0lf]] ]]
	let js_foo = saveToJson(foo)
	print(write_json(js_foo))
	return true

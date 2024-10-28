//	NOTE - this is WIP example of every single feature new syntax has to offer
//	new syntax no longer supports significant whitespace, oxoford commas, etc

/////////////////////
// module declaration

// note - this works, but commented out for now, becasue of WARNING: program is setup as both module, and endpoint
// module hello_world;	// module name

/////////////////////
// struct declaration

struct Foo {
    a : int = 5;
	b : int;
}

struct Bar : Foo {	// structure with inheritance
	c : float;
}

[safe_when_uninitialized]	// structure annotation
struct Baz : Bar {			// structure with methods
	def Baz {
		c = 3.14;
	}
	def add ( value:int ) {
		a += value;
	}
}

class Cls {			// class with methods
	@big			// with metadata
	@min=13
	@max=42
	private foobar : int;
	static foobars : float;
	def public foo {
		debug("foo");
	}
	def private private_method ( m : int ) {	// todo: make it `private def static_method`
		debug(m);
	}
	def static static_method {
		debug("static_method");
	}
	def abstract abstract_method ( a : int ) : float;
	[unsafe_deref]								// note function annotation
	def base_method {
	}

}

class private DerivedClass : Cls {	// class with inheritance
	def override base_method {
		debug("bar");
	}
	def override abstract_method ( a : int ) : float {
		return float(a);
	}
}

///////////////////
// enum declaration

enum EmptyEnum {
}

enum MyEnum {
	ONE,
	TWO,
	THREE
}

enum EnumWithTrailingComma {
	ONE,
	TWO,
}

enum Enum8u : uint8 {
	ONE,
	TWO,
	THREE = 16u8	// TODO: 16i8 causes 2x smart_ptr_leak, note - it causes 1x smart_ptr_leak with old parser
}

enum private Enum64 : uint64 {	// 64 bit enum, private
	ONE,
	TWO,
	THREE = 16ul
}

// TODO: add example of enum with annotation. for now we don't have a builtin enum one

[export]
def enum_use_example {
	let bf = MyEnum.ONE;
	debug(bf);
}

///////////
// bitfield


bitfield EmptyBitfield {
}

bitfield MyBitfield {
	ONE,
	TWO,
	THREE,
}

bitfield BitfieldWithTrailingComma {
	ONE,
	TWO,
}

[export]
def bitfield_use_example {
	let bf = MyBitfield.ONE;
	debug(bf);
}

////////////////////
// making structures

struct Mks {
	a : int = 1;
	b : float = 2.0;
	def Mks ( t : int ) {
		a = t;
		b = float(t);
	}
	def Mks ( one,two,three:int ) {
		a = one + two;
		b = float(two + three);
	}
}

[export]
def test_make_struct {
	let a = Mks();	// with default values
	debug(a);
	let A = default<Mks>;	// with default values, same as Mks()
	debug(A);
	let b = Mks(a=2,b=3.0);	// with values overwritten
	debug(b);
	let c = Mks(a=4);		// with some values overwritten
	debug(c);
	let C = Mks(5);			// with constructor
	debug(C);
	let CC = Mks(1,2,3);	// with complex constructor
	debug(CC);
	let d = struct<Mks>(a=2,b=3.0);	// with values overwritten
	debug(d);
	let e = struct<Mks>(a=1,b=1.0;a=2,b=2.0;a=3,b=3.0);	// this is array of structures // TODO: do we need better syntax_error
	debug(e);
	unsafe {
		let h = default<Mks> uninitialized;	// uninitialized, a=0, b=0.0
		debug(h);
		let i = struct<Mks>(uninitialized a=1);	// uninitialized, a=1, b=0.0
		debug(i);
	}
}

[export]
def test_new_struct {
	let a = new Mks();	    		// Mks*, on heap - default values
	debug(a);
	let A = new default<Mks>;		// Mks*, on heap - default values
	debug(A);
	let b = new Mks(5);	    		// Mks*, on heap - with constructor
	debug(b);
	let c = new<Mks>(a=1,b=2.0);	// Mks*, on heap - with values overwritten
	debug(c);
	let d = new<Mks[10]>();			// dim of 10 elements
	debug(d);
	unsafe {
		let j = new<Mks>(uninitialized b=2.0);	// Mks* - on heap, uninitialized, a=0, b=2.0
		debug(j);
		let h = new default<Mks> uninitialized;	// Mks* - on heap, uninitialized, a=0, b=0.0\
		debug(h);
	};
}

/////////////////
// comprehensions

[export]
def test_comprehensions {
	// array<int>
	let arr_comp <- [for x in range(1,100); x*x; where x%2==0];	// array comprehension
	debug(arr_comp);
	// iterator<int>
	let iter_comp <- [iterator for x in range(1,100); x*x; where x%2==0];	// iterator comprehension
	debug(iter_comp);
	// table<int>
	let set_comp <- {for x in range(1,100); x*x; where x%2==0};	// set comprehension
	debug(set_comp);
	// table<int;string>
	let table_comp <- {for x in range(1,10); x=>"{x*x}"; where x%2==0};	// table comprehension
	debug(table_comp);
}

///////
// main

[export]
def main {
	// var f <- {1=>"a", 2=>"b", 3=>"c", 4=>"d"};
	// debug(f);
}

options log;


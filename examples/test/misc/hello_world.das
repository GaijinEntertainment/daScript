//	NOTE - this is WIP example of every single feature new syntax has to offer
//	new syntax no longer supports significant whitespace, oxoford commas, etc

//  NOTE - in every case where type is specified for let or var, it is optional, and can be inferred
//	its there for the documentation purposes


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
	let a : Mks = Mks();	// with default values
	debug(a);
	let A : Mks = default<Mks>;	// with default values, same as Mks()
	debug(A);
	let b : Mks = Mks(a=2,b=3.0);	// with values overwritten
	debug(b);
	let c : Mks = Mks(a=4);		// with some values overwritten
	debug(c);
	let C : Mks = Mks(5);			// with constructor
	debug(C);
	let CC : Mks = Mks(1,2,3);	// with complex constructor
	debug(CC);
	let d : Mks = struct<Mks>(a=2,b=3.0);	// with values overwritten
	debug(d);
	let e : Mks[3] = struct<Mks>(a=1,b=1.0;a=2,b=2.0;a=3,b=3.0);	// this is array of structures // TODO: do we need better syntax_error
	debug(e);
	unsafe {
		let h : Mks = default<Mks> uninitialized;	// uninitialized, a=0, b=0.0
		debug(h);
		let i : Mks = struct<Mks>(uninitialized a=1);	// uninitialized, a=1, b=0.0
		debug(i);
	}
}

[export]
def test_new_struct {
	let a : Mks* = new Mks();	    		// Mks*, on heap - default values
	debug(a);
	let A : Mks* = new default<Mks>;		// Mks*, on heap - default values
	debug(A);
	let b : Mks* = new Mks(5);	    		// Mks*, on heap - with constructor
	debug(b);
	let c : Mks* = new<Mks>(a=1,b=2.0);	// Mks*, on heap - with values overwritten
	debug(c);
	let d : Mks*[10] = new<Mks[10]>();			// dim of 10 elements
	debug(d);
	unsafe {
		let j : Mks* = new<Mks>(uninitialized b=2.0);	// Mks* - on heap, uninitialized, a=0, b=2.0
		debug(j);
		let h : Mks* = new default<Mks> uninitialized;	// Mks* - on heap, uninitialized, a=0, b=0.0
		debug(h);
	};
}

/////////
// tuples

tuple Tup {
	a : int;
	b : float;
	c : string;
}

[export]
def test_tuples {
	let a : tuple<int;float;string> = (1,2.,"3");
	debug(a);
	let b : tuple<int;float;string> = tuple(1,2.,"3");
	debug(b);
	let c : tuple<int;float;string> = tuple<Tup>(a=1,b=2.,c="3");
	debug(c);
	let e : tuple<int;float;string> = tuple<Tup>(uninitialized a=1);	// a=1,b=0.0,c=""
	debug(e);
	let f : tuple<int;float;string>* = new tuple(1,2.,"3");	// on heap
}

///////////
// variants

variant Varr {
	a : int;
	b : float;
	c : string;
}

[export]
def test_variants {
	// NOTE - variant can't be uninitialized (it just makes no sense)
	let a : variant<a:int;b:float;c:string> = Varr(a=1);
	debug(a);
	let b : variant<a:int;b:float;c:string> = variant<Varr>(a=1);
	debug(b);
}

/////////
// arrays

[export]
def test_arrays {
	let a : array<int> <- [1,2,3,4,5];	// array of integers
	debug(a);
	let c : array<int> <- array(1,2,3,4,5);	// array of integers
	debug(c);
	let d : array<int> <- array<int>(1,2,3,4,5);	// array of integers
	debug(d);
	let b : int[5] = dim(1,2,3,4,5);	// fixed size array of integers
	debug(b);
	let e : int[5] = dim<int>(1,2,3,4,5);	// fixed size array of integers
	debug(e);
	let g : array<Mks> <- [ Mks(1), Mks(2,3,4) ];	// array of structures
	debug(g);
	let f : Mks[2] <- struct<Mks>(a=1,b=2.0; a=3,b=4.0);
	debug(f);
	let G : array<Mks> <- array struct<Mks>(a=1,b=2.0; a=3,b=4.0);	// array of structures
	debug(G);
	let h : Tup[2] <- tuple<Tup>(a=1,b=2.,c="3"; a=4,b=5.,c="6");
	debug(h);
	let H : array<Tup> <- array tuple<Tup>(a=1,b=2.,c="3"; a=4,b=5.,c="6");	// array of tuples
	debug(H);
	let v : Varr[6] <- variant<Varr>(a=1; b=2.; c="3"; a=4; b=5.; c="6");
	debug(v);
	let V : array<Varr> <- array variant<Varr>(a=1; b=2.; c="3"; a=4; b=5.; c="6");	// array of variants
	debug(V);
}

/////////////////
// comprehensions

[export]
def test_comprehensions {
	let arr_comp : array<int> <- [for x in range(1,100); x*x; where x%2==0];	// array comprehension
	debug(arr_comp);
	let iter_comp : iterator<int> <- [iterator for x in range(1,100); x*x; where x%2==0];	// iterator comprehension
	debug(iter_comp);
	let set_comp : table<int> <- {for x in range(1,100); x*x; where x%2==0};	// set comprehension
	debug(set_comp);
	let table_comp : table<int;string> <- {for x in range(1,10); x=>"{x*x}"; where x%2==0};	// table comprehension
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


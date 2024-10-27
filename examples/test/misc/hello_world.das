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

///////
// main

[export]
def main {
	var f <- {1=>"a", 2=>"b", 3=>"c", 4=>"d"};
	debug(f);
}

options log;


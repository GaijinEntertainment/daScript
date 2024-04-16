require daslib/rst_comment

bitfield bitfieldBoo
	//! This is a comment for bitfieldBoo
	bitfieldBar //! This is a comment for bitfield Bar
	bitfieldFar //! This is a comment for bitfield Far

enum enumFoo
	//! This is a comment for enumFoo
	enumBar //! This is a comment for enum Bar
	enumFar //! This is a comment for enum Far

variant variantFoo
	//! This is a comment for variatnFoo
	variantBar : int //! This is a comment for variant Bar
	variantFar : float //! This is a comment for variant Far

[export]
def main
	print("hello,world!\n")



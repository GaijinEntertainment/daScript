.. _arrays:


=================
Arrays
=================

.. index::
    single: Arrays

An array is a sequence of values indexed by a integer number from 0 to the size of the
array minus 1. Arrays elements can be obtained through their index.
::

  var a = [[int[4] 1; 2; 3; 4]] // fixed size of array is 4, and content is [1, 2, 3, 4]
  assert(a[0] == 1)

  var b: array<int>
  b.push(1)
  assert(b[0] == 1)

There are static arrays (of fixed size, allocated on stack), and dynamic array (size is dynamic, allocated on heap) ::

  var a = [[int[4] 1; 2; 3; 4]] // fixed size of array is 4, and content is [1, 2, 3, 4]
  var b: array<string>          // empty dynamic array
  push(b, "some")               // now it is 1 element of "some"
  b |> push("some")             // same as above line

Resizing, insertion, deletion of dynamic arrays and arrays elements is done through a set of
standard functions (see :ref:`built-in functions <builtin_functions>`).

List of relevant builtin functions: push, emplace, resize, erase, length, clear, capacity.

Arrays (as well as tables, structs, and handled types) will be passed to functions by reference only.

Arrays can not be assigned, only cloned or moved. ::

  def clone_array(var a, b: array<string>)
    a := b      // a is not a deep copy of b
    clone(a, b) // same as above

  def move_array(var a, b: array<string>)
    a <- b  // a is no points to same data as b, and b is empty.

Arrays can be constructed inline ::

	let arr = [[auto 1.; 2.; 3.; 4.5]]
	
Which infers to ::

	let arr : float[4] = [[float[4] 1.; 2.; 3.; 4.5]]
	
Dynamic arrays can also be constructed inline::

	let arr = [{auto "one"; "two"; "three"}]
	
Which is syntatic equivalent to::

	let arr : array<string> = to_array_move([[string[4] "one"; "two"; "three"]])
	
If only one elmeent is specified, local data construction is that element::

	let i1 = [[int 1]]	// same is the line bellow
	let i2 = 1
	
To create an array of unspecified type, use [] syntax::

	let ai1 = [[int[] 1]]	// actually [[int[1] 1]]
	let ai2 = [[autop[] 1]	// same as above

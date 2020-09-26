.. _lambdas:

======
Lambda
======

Lambda is a nameless function which captures local context by clone, copy, or reference.
Lambda is slower than block, but allows more flexibility in lifetime and capture modes  (see :ref:`Blocks <blocks>`).

Lambda type can be declared with a function-like syntax::

    lambda_type ::= lambda { optional_lambda_type }
    optional_lambda_type ::= < { optional_lambda_arguments } { : return_type } >
    optional_lambda_arguments := ( lambda_argument_list )
    lambda_argument_list := argument_name : type | lambda_argument_list ; argument_name : type

    lambda < (arg1:int;arg2:float&):bool >

Lambda can be local or global variable, it can be passed as an argument by reference.
Lambda can be moved, but can't be copied or cloned::

    def foo ( x : lambda < (arg1:int;arg2:float&):bool > )
        ...
        var y <- x
        ...

Lambda can be invoked via ``invoke``::

    def inv13 ( x : lambda < (arg1:int):int > )
        return invoke(x,13)

Lambda is typically declared via pipe syntax::

    var CNT = 0
    let counter <- @ <| (extra:int) : int
        return CNT++ + extra
    let t = invoke(counter,13)

There are a lot of similarities between lambda and block declarations.
Main difference is that blocks are specified with ``$`` symbol, where lambda is specified with ``@`` symbol.
Lambda can also be declared via inline syntax.
There is similar simplified syntax for the lambdas containing return expression only.
If lambda is sufficiently specified in the generic or function,
its types will be automatically inferred (see :ref:`Blocks <blocks_declarations>`).

-------
Capture
-------

Unlike blocks, lambda can specify it capture types explicitly. There are several available types of capture

    * by copy
    * by move
    * by clone
    * by reference

Capturing by reference requires unsafe.

By default capture by copy will be generated. If copy is not available, unsafe would be required for the default capture by move::

	var a1 <- [{int 1;2}]
	var a2 <- [{int 1;2}]
	var a3 <- [{int 1;2}]
	unsafe  // required do to capture of a1 by reference
		var lam <- @ <| [[&a1,<-a2,:=a3]]
			push(a1,1)
			push(a2,1)
			push(a3,1)
		invoke(lam)

.. _lambdas_finalizer:

Lambda can be deleted, which will cause finalizers on all captured data  (see :ref:`Finalizers <finalizers>`)::

    delete lam

Lambda can specify custom finalizer which would be invoked before the default finalizer::

    var CNT = 0
    var counter <- @ <| (extra:int) : int
        return CNT++ + extra
    finally
        print("CNT = {CNT}\n")
    var x = invoke(counter,13)
    delete counter                  // this is when the finalizer is called

.. _lambdas_iterator:

---------
Iterators
---------

Lambda is the main building block for implementing custom iterators (see :ref:`Iterators <iterators>`).

Lambda can be converted to iterator via ``each`` or ``each_ref`` function::

    var count = 0
    let lam <- @ <| (var a:int &) : bool
        if count < 10
            a = count++
            return true
        else
            return false
    for x,tx in each(lam),range(0,10)
        assert(x==tx)

To serve as an iterator lambda must

    * have single argument, which would be result of the iteration for each step
    * have boolean return type, where ``true`` means continue iteration, and ``false`` means stop

More straightforward way to make iterator is generator (see :ref:`Generators <generators>`).

----------------------
Implementation details
----------------------

Lambda is implemented by creating a nameless structure for the capture, as well as function for the body of the lambda.

Lets review the example with a singled captured variable::

    var CNT = 0
    let counter <- @ <| (extra:int) : int
        return CNT++ + extra

daScript will generated the following code

Capture structure::

    struct _lambda_thismodule_7_8_1
        __lambda : function<(__this:_lambda_thismodule_7_8_1;extra:int const):int> = @@_lambda_thismodule_7_8_1`function
        __finalize : function<(__this:_lambda_thismodule_7_8_1? -const):void> = @@_lambda_thismodule_7_8_1`finazlier
        CNT : int

Body function::

    def _lambda_thismodule_7_8_1`function ( var __this:_lambda_thismodule_7_8_1; extra:int const ) : int
        with __this
            return CNT++ + extra

Finalizer function::

    def _lambda_thismodule_7_8_1`finazlier ( var __this:_lambda_thismodule_7_8_1? explicit )
        delete *this
        delete __this

Lambda creation is replaced with the ascend of the capture structure::

    let counter:lambda<(extra:int const):int> const <- new<lambda<(extra:int const):int>> [[CNT = CNT]]

C++ Lambda class contains single void pointer for the capture data::

    struct Lambda {
        ...
        char *      capture;
        ...
    };

The rational behind passing lambda by reference is that when delete is called

    1. finalizer is invoked for the capture data
    2. capture is replaced via null

Lack of copy or move insures there are no multiple pointers to the single instance of capture data floating around.

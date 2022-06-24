.. _modules:

=======
Modules
=======

Modules provide infrastructure for the code reuse,
as well as mechanism to expose C++ functionality to daScript.
A module is a collection of types, constants, and functions.
Modules can be native to daScript, as well as built-in.

To request a module, use ``require`` keyword::

    require math
    require ast public
    require daslib/ast_boost

``public`` modifier indicates that included model is visible to everything including current module.

Module name may contain ``/`` and ``.`` symbols.
Project is responsible for resolving module names into file names (see :ref:`Project <modules_project>`).

--------------
Native modules
--------------

Native module is separate daScript file, with an optional ``module`` name::

    module custom       // specifies module name
    ...
    def foo             // defines function in module
    ...

If not specified, module name is defaulted to that of a file name.

Modules can be `private` or `public` ::

    module Foo private

    module Foo public

Default publicity of the functions, structures, or enumerations are that of the module.
I.e. if module is public and function publicity is not specified, function is public.

---------------
Builtin modules
---------------

Builtin modules are the way to expose C++ functionality to daScript (see :ref:`Builtin modules <embedding_modules>`).

--------------
Shared modules
--------------

Shared modules are modules that are shared between compilation of multiple contexts.
Typically module is compiled anew for each context, but when 'shared' keyword is specified, module gets promoted to builtin module::

    module Foo shared

That way only one instance of the module is created per compilation environment.
Macros in shared modules can't expect the module to be unique, since sharing of the modules can be disabled via code of policies.

--------------------------
Module function visibility
--------------------------

When calling function, name of the module can be specified explicitly or implicitly::

    let s1 = sin(0.0)           // implicit, assumed math::sin
    let s2 = math::sin(0.0)     // explicit, always math::sin

If function does not exist in the module, compilation error will occur.
If function is private or not directly visible, compilation error will occur.
If multiple functions match implicit function, compilation error will occur.

Module names ``_`` and ``__`` are reserved to specify `current module` and `current module only` accordingly.
Its particularly important for generic functions, which are always instanced as private functions in the current module::

    module b

    [generic]
    def from_b_get_fun_4()
        return  _::fun_4()      //  call `fun_4', as if it was implicitly called from b

    [generic]
    def from_b_get_fun_5()
        return  __::fun_5()     // always b::fun_5

Specifying empty prefix is the same as specifying no prefix.

Without ``_`` or ``__`` module prefixes overwritten functions will not be visible from the generics.
That is why ``:=`` and ``delete`` operators are always replaced with ``_::clone`` or ``_::finalize`` calls.


.. _tutorial_macro_template_struct_instance:

.. index::
   single: Tutorial; Macros; template_struct_instance
   single: Tutorial; Macros; reification
   single: Tutorial; Macros; class template

====================================================
 Macro Tutorial 20: Template struct/class instances
====================================================

A family of similar classes — same shape, different element type, a couple of
small behavior switches — usually ends up as copy-paste. daslang has a
``class template`` / ``struct template`` grammar for the shared body, and
``[template_struct_instance]`` from ``daslib/typemacro_boost`` turns that body
into concrete classes. Each stamped copy gets its own types, its own constant
values, and its own method set, with nothing shared at runtime — this stamping
is called **reification**.

An instance is just a class that inherits from the template:

- a ``typedef`` inside the instance binds a type parameter;
- an ``override`` of a ``@template_constant`` field binds a constant;
- an instance method with ``def override`` replaces the template's method;
- an ``override`` of a ``@template_call`` field redirects a free-function call;
- a ``@template_gate`` field exists only in stamps where its named bool
  constant is true.


The template
============

Templates live in a module that the instances require, so the file opens
with a ``module`` declaration.  ``require daslib/typemacro_boost public``
re-exports the annotation, so instance files get it from this one require.

Full source: :download:`template_struct_instance_mod.das <../../../../../tutorials/macros/template_struct_instance_mod.das>`

.. das-doc: file template_struct_instance_mod.das
.. code-block:: das

    options gen2

    module template_struct_instance_mod shared public

    require daslib/typemacro_boost public

    [ |> template_struct_instance]
    class template public TopKeeperT {
        best : KT
        n : int = 0
        @template_constant KEEP_LATEST : bool = false

        def feed(v : KT) {
            if (n == 0) {
                best = v
            } else {
                static_if (KEEP_LATEST) {
                    best = v
                } else {
                    if (v > best) {
                        best = v
                    }
                }
            }
            n++
        }
        def report : string {
            return "kept {best} out of {n}"
        }
    }

Three things to notice:

- ``KT`` is not declared anywhere. An unresolved name in type position is a
  **type parameter** — every instance binds it with a ``typedef``. A forgotten
  ``typedef`` shows up as an undefined-type error in the stamped code.
- ``@template_constant`` marks ``KEEP_LATEST`` as a **constant parameter**.
  Instances pick its value with ``override``; the field itself is erased from
  every stamped class, and each use site gets the value as a literal.
- ``[|> template_struct_instance]`` — the ``|>`` prefix marks the annotation
  as **inherited**: the parser copies it onto every class that derives from
  the template, ahead of that class's own annotations. Instances write no
  macro boilerplate at all, and any annotation an instance does carry (a
  dispatch or codegen macro, for example) runs after the class is already
  stamped and concrete.


The five instance patterns
==========================

Full source: :download:`20_template_struct_instance.das <../../../../../tutorials/macros/20_template_struct_instance.das>`

.. code-block:: das

    require template_struct_instance_mod

    class FloatTop : TopKeeperT {       // 1. type parameter
        typedef KT = float
    }

    class IntLatest : TopKeeperT {      // 2. constant parameter
        typedef KT = int
        override KEEP_LATEST = true
    }

    class IntShouty : TopKeeperT {      // 3. method override
        typedef KT = int

        def override report : string {
            return "BEST {best} OF {n}!"
        }
    }

``FloatTop`` takes everything from the template: ``KEEP_LATEST`` stays
``false`` and ``feed`` keeps the maximum. ``IntLatest`` flips the constant —
the ``static_if`` folds during compilation, so its stamped ``feed`` carries
the "keep the latest value" body and no branch. ``IntShouty`` replaces
``report``; the template's other methods call the replacement, resolved at
stamp time with no virtual dispatch left over.

Constants also fold inside field initializers and inside methods the instance
writes itself — a ``@template_constant`` behaves like a per-instance
compile-time value everywhere in the class body.

The fourth pattern parameterizes a **free-function call**. The template calls
a function by name; ``@template_call`` marks that name as rebindable — the
field name is what the body spells, the init is where the call goes:

.. das-doc: file template_struct_instance_mod.das
.. code-block:: das

    def public dot_i(a, b : int) : int {
        return a * b
    }

    def public dot_i_scaled(a, b : int) : int {
        return a * b * 100
    }

    [ |> template_struct_instance]
    class template public MixT {
        acc : int = 0
        @template_call dot_i = @@dot_i

        def feed(a, b : int) {
            acc += dot_i(a, b)
        }
    }

One instance keeps the default target, the other redirects it:

.. code-block:: das

    class MixPlain : MixT {         // dot_i calls stay on the real dot_i
    }

    class MixLoud : MixT {
        override dot_i = @@dot_i_scaled
    }

``MixLoud`` redirects every ``dot_i(...)`` call — and every ``@@dot_i``
address — to ``dot_i_scaled``. The value may also be a string
(``override dot_i = "dot_i_scaled"``). Like a constant, the field is erased:
the stamped class makes a plain direct call, so there is no function pointer
in the object and nothing blocks inlining. Compare this with the
``static_if`` route: a constant switch picks between branches the template
already spells out, while a call parameter is open — a new instance can
route to a function the template has never heard of.

Only the bare spelling rebinds. ``_::dot_i(...)`` and ``__::dot_i(...)``
keep their normal ``_`` / ``__`` resolution rules, so a template body that
must reach the real ``dot_i`` no matter what an instance rebinds spells the
call qualified.

The fifth pattern gates a **field on an axis**. A kernel and its batched twin
often differ by one extra field — a row table, a count buffer — and copying
the whole class for that one field is what templates exist to avoid.
``@template_gate`` names a bool ``@template_constant``; the field exists only
in stamps where that constant is true:

.. das-doc: file template_struct_instance_mod.das
.. code-block:: das

    [ |> template_struct_instance]
    class template public SumT {
        total : int = 0
        n : int = 0
        @template_gate = WEIGHTED w : int
        @template_constant WEIGHTED : bool = false

        def add(v : int) {
            static_if (WEIGHTED) {
                total += v * w
            } else {
                total += v
            }
            n++
        }
    }

The two instances differ by that one axis:

.. code-block:: das

    class PlainSum : SumT {
    }

    class WeightedSum : SumT {
        override WEIGHTED = true
    }

``WeightedSum`` has ``w`` like any ordinary field. ``PlainSum``'s stamp
erases it: the class is smaller by one int, and code that names ``w`` on a
``PlainSum`` is a compile error. The template body may still name the erased
field inside the dead arm of a ``static_if`` — the arm folds away before the
name would need to resolve. Only ``static_if`` arms get this: a
``COND ? a : b`` ternary is inferred on *both* arms, so a dead ternary arm
naming the erased field is a compile error — restructure it as a
``static_if`` on the same axis. The string form inverts the gate:
``@template_gate = "!WEIGHTED"`` keeps a field only where the axis is off.


What the reifier does
=====================

The macro runs at parse time, when the instance class is declared. It:

1. binds every template alias to the instance's ``typedef`` declarations.
   Other names are left for the compiler: module types resolve on their
   own, a later macro may still supply a ``typedef``, and a genuinely
   missing one surfaces as an undefined-type error in the stamped code;
2. harvests every ``@template_constant`` value, replaces its reads with the
   literal, and erases the field;
3. erases every ``@template_gate`` field whose axis is off for this
   instance;
4. harvests every ``@template_call`` target and renames the matching calls
   and ``@@`` addresses, erasing the field;
5. clones each template method onto the instance, retyping signatures and
   bodies, skipping methods the instance defines itself;
6. cuts the template out of the instance's ancestry. If the template itself
   derives from a concrete base class, the instance keeps that base — normal
   inheritance, upcasts, and virtual dispatch through the base still work.

After step 6 the instance is an ordinary class. Anything that inspects it
later — other structure annotations, RTTI, AOT — sees plain concrete code.


Output
======

.. code-block:: text

    FloatTop:  kept 3.25 out of 3
    IntLatest: kept 7 out of 3
    IntShouty: BEST 10 OF 3!
    MixPlain:  acc=6
    MixLoud:   acc=600
    PlainSum:    total=5
    WeightedSum: total=50


.. seealso::

   Full source:
   :download:`20_template_struct_instance.das <../../../../../tutorials/macros/20_template_struct_instance.das>`,
   :download:`template_struct_instance_mod.das <../../../../../tutorials/macros/template_struct_instance_mod.das>`

   Previous tutorial: :ref:`tutorial_macro_add_module_option`

   Related: :ref:`Macro Tutorial 16 <tutorial_macro_template_type_macro>` —
   ``[template_structure]`` generates parameterized structs from type-position
   macros; this tutorial's annotation serves the inheritance-spelled,
   annotation-driven case.

   Language reference: :ref:`Macros <macros>` — full macro system documentation

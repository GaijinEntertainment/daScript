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
- an instance method with ``def override`` replaces the template's method.


The template
============

Full source: :download:`template_struct_instance_mod.das <../../../../../tutorials/macros/template_struct_instance_mod.das>`

.. code-block:: das

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
  **type parameter** — every instance must bind it with a ``typedef``, and the
  macro reports the missing name if one forgets.
- ``@template_constant`` marks ``KEEP_LATEST`` as a **constant parameter**.
  Instances pick its value with ``override``; the field itself is erased from
  every stamped class, and each use site gets the value as a literal.
- ``[|> template_struct_instance]`` — the ``|>`` prefix marks the annotation
  as **inherited**: the parser copies it onto every class that derives from
  the template, ahead of that class's own annotations. Instances write no
  macro boilerplate at all, and any annotation an instance does carry (a
  dispatch or codegen macro, for example) runs after the class is already
  stamped and concrete.


The three instance patterns
===========================

Full source: :download:`20_template_struct_instance.das <../../../../../tutorials/macros/20_template_struct_instance.das>`

.. code-block:: das

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


What the reifier does
=====================

The macro runs at parse time, when the instance class is declared. It:

1. binds every template alias to the instance's ``typedef`` declarations
   (module-level aliases stay untouched — only a genuinely unbound name is
   an error);
2. harvests every ``@template_constant`` value, replaces its reads with the
   literal, and erases the field;
3. clones each template method onto the instance, retyping signatures and
   bodies, skipping methods the instance defines itself;
4. cuts the template out of the instance's ancestry. If the template itself
   derives from a concrete base class, the instance keeps that base — normal
   inheritance, upcasts, and virtual dispatch through the base still work.

After step 4 the instance is an ordinary class. Anything that inspects it
later — other structure annotations, RTTI, AOT — sees plain concrete code.


Output
======

.. code-block:: text

    FloatTop:  kept 3.25 out of 3
    IntLatest: kept 7 out of 3
    IntShouty: BEST 10 OF 3!


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

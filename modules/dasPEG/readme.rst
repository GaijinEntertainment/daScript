=======================================
The *das-PEG*  parser generator library
=======================================

The **das-PEG** (Parsing Expression Grammar) parser generator library is a
versatile tool that helps you design deterministic parsers with unlimited
lookahead capability. PEG is a type of parsing algorithm that describes a
formal language in terms of an *ordered* set of rules.

How does it work?
-----------------

The *das-PEG* parser generator interprets your grammar and constructs a
recursive descent parser from it. The grammar is declared in a specific
das-like syntax inside a ``parse`` macro. The PEG algorithm employs a unique
approach, using **prioritized choice** to handle ambiguity. This feature
ensures that the first matching rule is always chosen, which is why PEG is
sometimes referred to as "scannerless" – it integrates lexing and parsing into
a single activity.

Example
~~~~~~~

**The grammar** is a series of rules; each rule is introduced with
``var <rule-name>: <rule-type>``.

**The types are significant**, as they are used for binding variables.

Following each rule is a series of ``rule(...)`` calls – the alternatives that
the rule will try to match. For example, in this simplified arithmetic parser —

.. code-block:: das

   parse calc
       var add: int

       rule(add as a, "+", mul as m) <|
           return a + m       // <- a & m are int variables in this context
                              //     the action should also return an int
       rule(add as a, "-", mul as m) <|
           return a - m

       rule(mul as m) <|
           return m

       var mul: int
       ...

— the rules for plus and minus will be tried in order. If neither of these
matches then the last rule is checked against.

**Caching.** The ``a`` in ``add as a`` is not parsed several times. The parser keeps
the caches for every rule and reuses its results. This technique is known as
*packrat parsing.*

Built-in rules
~~~~~~~~~~~~~~

*das-PEG* provides a number of built-in rules designed to increase the speed
and convenience of using the parser.

- ``string_`` - stream of characters enclosed inside ``"..."``
- ``double_`` - floating point number
- ``number`` - decimal integer
- ``WS`` - *whitespace*
- ``EOF`` - *end of file*

The basic building blocks to the more complicated rules are

- **Literal match**. String literals can be enclosed in ``""`` and matched against
- **Range match**. Ranges are specified via ``range('1', '9')`` syntax
- **Text extraction**. Text extraction is specified with the interpolation
  syntax and is used to match the rule and extract its corresponding ``string``.

  .. code-block:: das

      rule("{name}", "=" , Value as v) <| $ { variables[name] = v; }

Binding to the rules
~~~~~~~~~~~~~~~~~~~~~

If you have the rule ``A: type`` you can bind its result with the ``A as <name>``
syntax. This allows you to access the name inside the action block with its
type being the type of rule ``A``.

The modifiers can alter the meaning of binding constructs.

Rule Modifiers
~~~~~~~~~~~~~~~

There are several modifiers that can be applied to the rules.

- **Repetition** (``*``)

  This modifier matches the rule *zero or more times*. Returns its result
  inside the array. Take, for example, the array from json parser -

  .. code-block:: das
     var ElementList: array<JsonValue?>

     rule(*CommaSeparatedElement as els, Element as last) <|
     print("Parsed element list\n")
     els |> push <| last
     return <- els

     var CommaSeparatedElement: JsonValue?
     ...

  - its contents are represented as `ElementList`, a list of comma separated
  elements ending with just the last `Element` (without comma).

  Here the rule `*CommaSeparatedElement as els` binds several elements and
  produces `array<JsonValue?>` - because `CommaSeparatedElement` is a
  `JsonValue?`.

- **Certain repetition** (``+``)
- **Optional** (``MB``)
- **Positive lookahead** (``PEEK``)
- **Negative lookahead** (``!``)

Available generator options
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The options are specified anywhere in the body of the parse macro with the ``options(...)`` syntax.

.. code-block:: das

   parser multilingual
       options(utf-8)
       ...

- ``utf-8`` – enables utf-8 decoding support
- ``trace`` – enable line info tracking and failure reporting

Performance
-----------

PEG parsers, including the *das-PEG* parser generator, have certain performance
characteristics that can impact the efficiency of your parsing tasks.

- **Importance of Ordering**

  Unlike some other parser types that may require
  conflict resolution, PEG parsers operate in a deterministic way, always trying
  the first alternative in order. Therefore it is advisable to place alternatives
  in their expected order of frequency. This way, the more common cases will be
  handled faster.

- **Linear Time Complexity**

  In general, PEG parsers exhibit super-linear time
  complexity with respect to the size of the input. Using caching techniques
  alleviates some of the performance penalties. However, this can be affected by
  the specifics of the grammar. For example, excessive use of repetitions ``*``
  (zero or more) or ``+`` (one or more) operators in the grammar can lead to
  super-linear performance because these operators may require the parser to
  repeatedly attempt the same parsing operation.

- **Memory Use**: Due to the use of caching this type of parsers can consume some
  additional memory.

Benchmarks
~~~~~~~~~~

The benchmark(s) are situated in the ``dasPEG/bench`` directory. For now the generated parsers are not
extremely blazingly fast – only achieveing 3x slowdown when compared with handwritten ones
base on the json canada sample.

+----------------+-----------+---------------+
| Parser         | File Size | Parsing Time  |
+================+===========+===============+
| PEG Parser     | 2 MB json | 0.61 seconds  |
+----------------+-----------+---------------+
| Stdlib Parser  | 2 MB json | 0.18 seconds  |
+----------------+-----------+---------------+

Warnings
~~~~~~~~

- **Incomplete left-recursion support**

  *das-PEG* currently provides limited support for indirectly left-recursive grammars.

- **Stack overflow**

  By default in interpreted mode the stack for a das program is quite small
  (16KB) and can easily overflow. Specify the bigger value in the beginning of
  the module to overcome this issue. ``options stack = 1000000`` – 1MB
  most probably enough.

- **Stateful actions are prohibited**

  Actions should avoid any side effects or dependence on external state. This
  is because the sequence of action execution is not fixed and could vary due
  to the **backtracking nature** of PEG parsers.

Inspiration
~~~~~~~~~~~

The creation of the *das-PEG* parser generator was substantially influenced by
Guido van Rossum's work on Python's *pegen* library. His series of detailed
articles on the internal mechanics of PEG parser generators provided a nice
blueprint for generating parsers. Another thanks goes to the excellent reification
capabilities of Daslang – the integration between the generator and the
generating program has never been so tight!

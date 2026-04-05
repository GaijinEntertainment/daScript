.. _tutorial_dasPEG_email_validator:

=======================================
PEG-04 --- Email Validator
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; Email
    single: Tutorial; Validation
    single: Tutorial; Character Sets
    single: Tutorial; Struct Return

This tutorial builds an email address parser that extracts structured
data into a daslang struct.  You will learn:

- ``not_set()`` for inverted character classes
- Composing small pattern rules into larger grammars
- Returning structs from parsers
- Multiple character-set arguments to ``set()``

Struct Returns
==============

Parser rules can return any daslang type --- including structs.
Define the struct outside the grammar, then construct it in the rule
body:

.. code-block:: das

   struct Email {
       user : string
       domain : string
   }

The Grammar
===========

A simplified email format: ``user@domain.tld``.

.. code-block:: das

   def parse_email(input : string;
                   blk : block<(val : Email; err : array<ParsingError>) : void>) {
       parse(input) {
           var email : Email
           rule("{+user_char}" as user, "@", "{+domain_char}" as domain, EOF) {
               return Email(user = user, domain = domain)
           }

           var user_char : void?
           rule(set('a'..'z', 'A'..'Z', '0'..'9', '.', '-', '_', '+')) {
               return null
           }

           var domain_char : void?
           rule(set('a'..'z', 'A'..'Z', '0'..'9', '-', '.')) {
               return null
           }
       }
   }

The ``user_char`` and ``domain_char`` rules are ``void?`` pattern rules
--- they match characters but produce no value.  Wrapping them in
``"{+...}" as name`` extracts the matched text.

Composing Pattern Rules
=======================

Small ``void?`` rules act as building blocks.  Each matches a single
character class; combining them with ``+`` (one-or-more) and text
extraction (``"{...}" as name``) builds complex patterns from simple
pieces.

This is the PEG equivalent of regex character classes like
``[a-zA-Z0-9._+-]+`` --- but type-checked at compile time.

not_set()
=========

``not_set()`` matches any character **not** in the given set.  It is the
complement of ``set()``:

.. code-block:: das

   not_set('\n', '\r')     // any character except newlines
   not_set('"')            // any character except double-quote

Usage
=====

.. code-block:: das

   parse_email("alice@example.com") $(val; err) {
       print("user={val.user}, domain={val.domain}\n")
       // user=alice, domain=example.com
   }

.. seealso::

   Full source: :download:`tutorials/dasPEG/04_email_validator.das <../../../../tutorials/dasPEG/04_email_validator.das>`

   Next tutorial: :ref:`tutorial_dasPEG_json_parser`

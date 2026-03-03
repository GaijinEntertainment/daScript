.. _tutorial_dasPUGIXML_serialization:

===========================================
PUGIXML-04 — Struct Serialization
===========================================

.. index::
    single: Tutorial; XML Serialization
    single: Tutorial; dasPUGIXML
    single: Tutorial; to_XML
    single: Tutorial; from_XML

This tutorial covers automatic struct ↔ XML serialization using
``to_XML``, ``from_XML``, and the ``XML()`` EDSL in
``pugixml/PUGIXML_boost``.

Basic roundtrip
===============

``to_XML`` serializes any struct to an XML string, and ``from_XML``
deserializes it back:

.. code-block:: das

   struct Settings
       resolution_x : int = 1920
       resolution_y : int = 1080
       fullscreen : bool = true
       volume : float = 0.8

   var s = Settings()
   let xml = to_XML(s)              // struct → XML string
   var restored <- from_XML(xml, type<Settings>)  // XML string → struct

Custom root element
===================

By default the root element uses the struct name. Override it with the
optional second parameter:

.. code-block:: das

   let xml = to_XML(s, "config")
   // <config> ... </config> instead of <Settings> ... </Settings>

Nested structs
==============

Nested structs serialize as child elements:

.. code-block:: das

   struct Vec2
       x : float = 0.0
       y : float = 0.0

   struct Transform
       position : Vec2
       rotation : float = 0.0

   var t = Transform(position = Vec2(x = 10.0, y = 20.0), rotation = 45.0)
   let xml = to_XML(t)

Enums and bitfields
===================

Enums serialize as their name by default, or as integers with
``@enum_as_int``:

.. code-block:: das

   enum Color
       Red
       Green
       Blue

   struct Pixel
       @enum_as_int color : Color = Color.Red    // serializes as "0"
       x : int = 0

Bitfields serialize as comma-separated flag names:

.. code-block:: das

   bitfield Flags
       visible
       enabled
       selected

   struct Widget
       flags : Flags = bitfield(1u << 0u | 1u << 1u)

Arrays and tables
=================

``array<T>`` serializes with repeated child elements, ``table<K;V>``
with key-value pairs:

.. code-block:: das

   struct Inventory
       items : array<string>
       counts : table<string; int>

   var inv = Inventory()
   inv.items |> push("sword")
   inv.items |> push("shield")
   inv.counts["sword"] = 1
   let xml = to_XML(inv)

Tuples and variants
===================

Tuples serialize with ``_0``, ``_1``, … field names. Variants serialize
with the active variant name and value:

.. code-block:: das

   struct Data
       pair : tuple<int; string> = (42 => "hello")

Dim (fixed-size arrays)
========================

Fixed-size arrays serialize with indexed child elements:

.. code-block:: das

   struct Grid
       cells : int[3]

The ``@rename`` annotation
==========================

Rename fields in XML without changing the daslang field name:

.. code-block:: das

   struct ApiResponse
       @rename("status-code") status_code : int = 200
       @rename("content-type") content_type : string = "text/xml"

The low-level ``XML()`` builder
===============================

``XML()`` gives direct control over the XML tree before serializing:

.. code-block:: das

   with_doc() <| $(doc) {
       XML(doc, s)                          // serialize struct fields to node
       var restored <- from_XML(doc.document_element, type<Settings>)
   }

This is useful when you need to add extra elements, attributes, or
processing instructions alongside serialized data.

Complete example
================

A full game-state roundtrip — serialize, deserialize, and verify:

.. code-block:: das

   struct GameState
       level : int = 1
       score : int = 0
       player_name : string = "Hero"
       is_paused : bool = false
       health : float = 100.0

   var state = GameState(level = 5, score = 12000, player_name = "Alice",
                         health = 85.5)
   let xml = to_XML(state)
   var loaded <- from_XML(xml, type<GameState>)
   assert(loaded.level == 5)
   assert(loaded.player_name == "Alice")
   assert(loaded.health == 85.5)

.. seealso::

   Full source: :download:`tutorials/dasPUGIXML/04_serialization.das <../../../../tutorials/dasPUGIXML/04_serialization.das>`

   Previous tutorial: :ref:`tutorial_dasPUGIXML_xpath`

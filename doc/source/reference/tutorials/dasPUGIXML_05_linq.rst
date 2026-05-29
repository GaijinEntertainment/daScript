.. _tutorial_dasPUGIXML_linq:

===========================================
PUGIXML-05 — LINQ over XML
===========================================

.. index::
    single: Tutorial; LINQ over XML
    single: Tutorial; dasPUGIXML
    single: Tutorial; from_xml_node

This tutorial covers ``from_xml_node`` — a typed, lazy iterator that walks
the child elements of an XML node and materializes each into a struct, so
ordinary comprehensions and ``daslib/linq_boost`` queries run straight over an
XML document. It lives in ``pugixml/PUGIXML_boost``.

Typed rows from attributes
==========================

``from_xml_node(root, type<Row>)`` walks every child element of ``root`` and
fills a ``Row`` by reading **same-named attributes**. Each yielded value is
fully materialized — no XML node escapes into the loop body. A field whose
attribute is absent keeps the struct's declared default:

.. code-block:: das

   require pugixml/PUGIXML_boost

   struct Car {
       id : int
       make : string
       price : float
       year : int = 2000        // used when the attribute is absent
       in_stock : bool
   }

   let CATALOG = ("<cars>" +
       "<car id=\"1\" make=\"Audi\"   price=\"45000.0\" year=\"2021\" in_stock=\"true\"/>" +
       "<car id=\"2\" make=\"Toyota\" price=\"28000.0\" year=\"2020\" in_stock=\"true\"/>" +
       "<car id=\"3\" make=\"Ford\"   price=\"31000.0\" in_stock=\"false\"/>" +
       "</cars>")

   parse_xml(CATALOG) $(doc, ok) {
       let root = doc.document_element
       for (car in from_xml_node(root, type<Car>)) {
           print("#{car.id} {car.make} ({car.year})\n")
       }
   }
   // #1 Audi (2021)
   // #2 Toyota (2020)
   // #3 Ford (2000)   <- year attribute absent, default kept

Supported field types are the XML scalar attribute types: ``int``, ``uint``,
``float``, ``double``, ``bool``, ``string``. Fields of other types keep their
default (child-element / text mapping is a planned growth path).

LINQ comprehensions
===================

The iterator is just an iterable, so comprehensions filter and project directly:

.. code-block:: das

   parse_xml(CATALOG) $(doc, ok) {
       let root = doc.document_element
       let affordable <- [for (car in from_xml_node(root, type<Car>));
           car.make;
           where car.in_stock && car.price < 40000.0]
       print("{affordable}\n")
       // [ Toyota]
   }

Tag-filtered walk
=================

``from_xml_node(root, "tag", type<Row>)`` walks only children with that tag —
useful when a parent interleaves several element kinds:

.. code-block:: das

   let mixed = ("<fleet>" +
       "<car id=\"10\" make=\"Mazda\" price=\"26000.0\"/>" +
       "<note>service due</note>" +
       "<car id=\"11\" make=\"Honda\" price=\"24000.0\"/>" +
       "</fleet>")
   parse_xml(mixed) $(doc, ok) {
       let root = doc.document_element
       let ids <- [for (car in from_xml_node(root, "car", type<Car>)); car.id]
       print("{ids}\n")
       // [ 10, 11]   <- <note> skipped
   }

Results outlive the document
============================

Rows are owned values — string fields are cloned out of the document — so
collecting them with ``to_array`` and using them after the RAII block closes is
safe. Because ``from_xml_node`` is an ``[unsafe_outside_of_for]`` iterator,
consuming it outside a ``for`` loop needs an ``unsafe`` block:

.. code-block:: das

   var inventory : array<Car>
   parse_xml(CATALOG) $(doc, ok) {
       let root = doc.document_element
       unsafe {
           inventory <- from_xml_node(root, type<Car>) |> to_array()
       }
   }
   // doc is freed here — inventory and its strings remain valid
   print("{length(inventory)} cars; first = {inventory[0].make}\n")

.. seealso::

   Full source: :download:`tutorials/dasPUGIXML/05_linq_over_xml.das <../../../../tutorials/dasPUGIXML/05_linq_over_xml.das>`

   Previous tutorial: :ref:`tutorial_dasPUGIXML_serialization`

   Related: :ref:`tutorial_linq`, :ref:`comprehensions`

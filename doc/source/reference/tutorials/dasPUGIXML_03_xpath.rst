.. _tutorial_dasPUGIXML_xpath:

===========================================
PUGIXML-03 — XPath Queries
===========================================

.. index::
    single: Tutorial; XPath
    single: Tutorial; dasPUGIXML
    single: Tutorial; XML Queries

This tutorial demonstrates querying XML documents with XPath, using both
convenience wrappers and compiled queries in ``pugixml/PUGIXML_boost``.

Quick text selection
====================

``select_text`` returns the first matching node's text content:

.. code-block:: das

   var doc = open_xml("books.xml")
   let title = select_text(doc, "//book[1]/title")
   print("First book: {title}\n")

``select_value`` returns string/int/float/bool from XPath expressions:

.. code-block:: das

   let total = select_value(doc, "count(//book)", 0.0)
   let cheapest = select_value(doc, "//book[price < 40]/title", "none")

Iterating matches
=================

``for_each_select`` invokes a block for each matching node:

.. code-block:: das

   for_each_select(doc, "//book") <| $(book) {
       let title = book.child("title") |> node_text("")
       let price = book.child("price") |> node_text(0.0)
       print("  {title}: ${price}\n")
   }

This is the most common pattern for processing multiple results.

Compiled XPath queries
======================

For repeated queries, ``with_xpath`` pre-compiles the expression:

.. code-block:: das

   with_xpath("//book[@lang='en']") <| $(query) {
       for_each_select(doc, query) <| $(book) {
           print("English: {book.child(\"title\") |> node_text(\"\")}\n")
       }
   }

This avoids reparsing the XPath string on each call.

Low-level XPath API
===================

Direct access to pugixml XPath types:

.. code-block:: das

   let node = select_node(doc, "//book[1]")
   // Access the matched node via node.node

   var nodes <- select_nodes(doc, "//book[@lang='en']")
   for (i in range(length(nodes))) {
       let n = nodes[i]
       print("{n.node.child(\"title\") |> node_text(\"\")}\n")
   }

Practical example
=================

Querying the sample ``books.xml`` for specific data:

.. code-block:: das

   // Find books under $40
   for_each_select(doc, "//book[price < 40]") <| $(book) {
       print("{book.child(\"title\") |> node_text(\"\")}\n")
   }

   // Get average price
   let avg = select_value(doc, "sum(//book/price) div count(//book)", 0.0)
   print("Average price: ${avg}\n")

.. seealso::

   Full source: :download:`tutorials/dasPUGIXML/03_xpath.das <../../../../tutorials/dasPUGIXML/03_xpath.das>`

   Previous tutorial: :ref:`tutorial_dasPUGIXML_building`

   Next tutorial: :ref:`tutorial_dasPUGIXML_serialization`


.. _stdlib_linked_list:

=====================
Intrusive linked list
=====================

The LINKED_LIST module implements intrusive linked list data structures.
Elements contain embedded next/prev pointers, avoiding separate node
allocations. Useful for implementing queues, work lists, and other
dynamic collections with O(1) insertion and removal.

All functions and symbols are in "linked_list" module, use require to get access to it. ::

    require daslib/linked_list

+++++++++
Constants
+++++++++

.. _global-linked_list-TLinkedList:

.. das:attribute:: TLinkedList = %spoof_template

Spoof template that generates a doubly-linked list class for a given element type.
Usage: ``apply_template(TLinkedList, "MyList", "MyElement")``


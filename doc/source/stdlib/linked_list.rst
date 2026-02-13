
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

.. das:attribute:: TLinkedList = "(ListClass,Foo) //! Spoof template that generates a doubly-linked list class for a given element type.\nstruct LLNode_%ListClass \{\n    //! Node of the doubly-linked list. Contains data pointer and prev/next links.\n    data : %Foo?                        //! pointer to the stored element\n    prev, next : LLNode_%ListClass?     //! links to previous and next nodes\n\}\n\nclass %ListClass \{\n    //! Doubly-linked list class. Maintains head and tail pointers.\n    head, tail : LLNode_%ListClass?     //! head and tail of the list\n    def %ListClass \{\n        head = null\n        tail = null\n    \}\n\n    def add ( var data : %Foo? ) \{\n        //! Appends an element to the end of the list.\n        var node = new LLNode_%ListClass(data=data)\n        if (head == null) \{\n            head = node\n            tail = node\n        \} else \{\n            tail.next = node\n            node.prev = tail\n            tail = node\n        \}\n    \}\n\n    def remove ( data : %Foo? ) \{\n        //! Removes the first node whose data matches the given pointer.\n        var node = head\n        while (node != null) \{\n            if (node.data == data) \{\n                if (node.prev != null) \{\n                    node.prev.next = node.next\n                \} else \{\n                    head = node.next\n                \}\n                if (node.next != null) \{\n                    node.next.prev = node.prev\n                \} else \{\n                    tail = node.prev\n                \}\n                return\n            \}\n            node = node.next\n        \}\n    \}\n    def each : iterator<%Foo?> \{\n        //! Returns an iterator over all elements in the list, from head to tail.\n        return <- generator <%Foo?> () <| $() \{\n            var node = head\n            while (node != null) \{\n                yield node.data\n                node = node.next\n            \}\n            return false\n        \}\n    \}\n\}\n"

|detail/Variable-linked_list-TLinkedList|



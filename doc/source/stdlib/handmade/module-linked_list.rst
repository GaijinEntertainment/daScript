The LINKED_LIST module implements intrusive linked list data structures.
Elements contain embedded next/prev pointers, avoiding separate node
allocations. Useful for implementing queues, work lists, and other
dynamic collections with O(1) insertion and removal.

All functions and symbols are in "linked_list" module, use require to get access to it. ::

    require daslib/linked_list

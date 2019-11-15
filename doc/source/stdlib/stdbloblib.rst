.. _stdlib_stdbloblib:

==================
The Blob library
==================
The blob library implements binary data manipulations routines. The library is
based on `blob objects` that represent a buffer of arbitrary
binary data.

---------------
daScript API
---------------

+++++++++++++++
Global symbols
+++++++++++++++

.. das:function:: binary_save(f, block<blob: string>)

    saves f to returned blob (provided to a block)

.. das:function:: binary_load(var obj; blob: string)

    loads obj from blob

++++++++++++++++++
The blob
++++++++++++++++++

The blob object is a buffer of arbitrary binary data, which still is typed as string.

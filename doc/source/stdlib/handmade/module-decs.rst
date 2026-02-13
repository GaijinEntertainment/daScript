The DECS module implements a Data-oriented Entity Component System.
Entities are identified by integer IDs and store components as typed data.
Systems query and process entities by their component signatures,
enabling cache-friendly batch processing of game objects.

All functions and symbols are in "decs" module, use require to get access to it. ::

    require daslib/decs

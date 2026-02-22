Generates a Structure-of-Arrays layout from a regular struct.
For a struct ``Foo`` with fields ``x : float`` and ``y : float``, generates:

  * ``Foo`SOA`` — struct where every field is ``array<FieldType>``
  * ``operator []`` returning ``SOA_INDEX`` proxy
  * ``length``, ``capacity``, ``push``, ``push_clone``, ``emplace``, ``erase``, ``pop``, ``clear``, ``resize``, ``reserve``, ``swap``, ``from_array``, ``to_array`` functions

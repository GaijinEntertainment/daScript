Generates a Structure-of-Arrays layout from a regular struct.
For a struct ``Foo`` with fields ``x : float`` and ``y : float``, generates:

  * ``Foo`SOA`` — struct where every field is ``array<FieldType>``
  * ``operator []`` returning ``SOA_INDEX`` proxy
  * ``length``, ``push``, ``emplace``, ``push_clone``, ``erase`` functions

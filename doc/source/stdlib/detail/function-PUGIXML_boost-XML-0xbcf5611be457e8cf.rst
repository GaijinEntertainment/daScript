Serializes a native value into XML child elements of *node*.
Structs/tuples: each field becomes a child element (via ``apply``).
Variants: stores ``_variant`` attribute with the active index.
Tables (key-value): each entry becomes an ``entry`` child with ``_key`` / ``_val`` sub-elements.
Tables (key-only): each key becomes an ``item`` child.
Arrays/dim: each element becomes an ``item`` child.
Enumerations: stored as string name. Bitfields: stored as integer value.
Primitives: sets the text content of *node*.

//! void as a struct field type does not compile
// void type in struct field
// verifies compiler rejects void as a struct field type
options gen2
expect 30237    // invalid_structure_field_type

struct Foo {
    a : void
}


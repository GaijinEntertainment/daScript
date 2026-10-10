[
  (constant_string)
  (string_builder)
] @string

(identifier) @variable

((identifier) @constant
  (#match? @constant "^[A-Z][A-Z0-9_]*$"))

((identifier) @variable.builtin
  (#eq? @variable.builtin "self"))

(_
  arguments: (variable_declaration
    name: (identifier) @variable.parameter))

(_
  argument_names: (identifier) @variable.member)

(field
  name: (identifier) @variable.member)

(safe_field
  name: (identifier) @variable.member)

(field_declaration
  name: (identifier) @variable.member)

(make_field_declaration
  name: (identifier) @variable.member)

(is_variant
  name: (identifier) @variable.member)

(as_variant
  name: (identifier) @variable.member)

(safe_as_variant
  name: (identifier) @variable.member)

(operator_name
  field: (identifier) @variable.member)

(annotation_argument
  name: _ @property)

(enumeration_entry
  name: (identifier) @constant)

(bitfield_entry
  name: (identifier) @constant)

(bitfield_type
  argument_names: (identifier) @constant)

(module_declaration
  name: _ @module)

(module_path
  (identifier) @module)

(require_declaration
  alias: (identifier) @module)

(require_declaration
  group: (identifier) @module)

(qualified_name
  module: (identifier) @module)

(basic_type) @type.builtin

(bitfield_type
  [
    "uint8"
    "uint16"
    "uint"
    "uint64"
  ] @type.builtin)

(structure_type
  (identifier) @type)

(structure_type
  (qualified_name
    name: (identifier) @type))

(type_macro
  name: (identifier) @type)

(type_macro
  name: (qualified_name
    name: (identifier) @type))

(auto_type
  alias: (identifier) @type)

(local_type_alias
  alias: (identifier) @type)

[
  (struct_declaration
    name: (identifier) @type)
  (class_declaration
    name: (identifier) @type)
  (enumeration_declaration
    name: (identifier) @type)
  (typedef_declaration
    name: (identifier) @type)
  (tuple_alias_declaration
    name: (identifier) @type)
  (variant_alias_declaration
    name: (identifier) @type)
  (bitfield_alias_declaration
    name: (identifier) @type)
]

(_
  parent: (identifier) @type)

(_
  parent: (qualified_name
    name: (identifier) @type))

(typedef_declaration
  kind: (identifier) @keyword.modifier)

(capture_entry
  mode: (identifier) @keyword.modifier)

(function_declaration
  name: [
    (identifier)
    (basic_type)
  ] @function)

[
  (struct_declaration
    methods: (function_declaration
      name: (identifier) @function.method))
  (class_declaration
    methods: (function_declaration
      name: (identifier) @function.method))
]

(address
  target: (identifier) @function)

(address
  target: (qualified_name
    name: (identifier) @function))

(call
  name: (identifier) @function.call)

(call
  name: (qualified_name
    name: (identifier) @function.call))

(named_call
  name: (identifier) @function.call)

(named_call
  name: (qualified_name
    name: (identifier) @function.call))

(pipe
  function_call: (variable
    name: (identifier) @function.call))

(pipe
  function_call: (variable
    name: (qualified_name
      name: (identifier) @function.call)))

(invoke
  name: (identifier) @function.method.call)

(named_call
  value: _
  name: (identifier) @function.method.call)

(type_info
  trait: (identifier) @function.builtin)

(annotation_declaration
  name: (identifier) @attribute)

(annotation_declaration
  name: (qualified_name
    name: (identifier) @attribute))

(reader
  macro: (identifier) @function.macro)

(reader
  macro: (qualified_name
    name: (identifier) @function.macro))

(inline_reader) @function.macro

[
  "$e"
  "$i"
  "$v"
  "$b"
  "$a"
  "$t"
  "$c"
  "$f"
  "..."
] @function.macro

[
  (constant_integer)
  (constant_unsigned_integer)
  (constant_integer64)
  (constant_unsigned_integer64)
  (constant_unsigned_integer8)
] @number

((constant_integer) @character
  (#match? @character "^'"))

[
  (constant_float)
  (constant_double)
  (constant_float16)
] @number.float

(label_expression
  label_name: (constant_integer) @label)

(goto_expression
  label_name: (constant_integer) @label)

(constant_boolean) @boolean

(constant_pointer) @constant.builtin

(escape_sequence) @string.escape

(reader_text) @string.special

(format_specifier) @string.special

[
  (comment)
  (block_comment)
] @comment

(line_directive) @keyword.directive

(include_directive) @keyword.import

[
  "let"
  "var"
  "aka"
  "assume"
  "capture"
  "default"
  "goto"
  "label"
  "pass"
  "unsafe"
  "with"
] @keyword

[
  "abstract"
  "const"
  "explicit"
  "implicit"
  "inscope"
  "override"
  "private"
  "public"
  "sealed"
  "shared"
  "static"
  "template"
  "uninitialized"
] @keyword.modifier

[
  "def"
  "operator"
] @keyword.function

[
  "class"
  "enum"
  "struct"
  "typedef"
] @keyword.type

[
  "return"
  "yield"
] @keyword.return

"generator" @keyword.coroutine

[
  "if"
  "static_if"
  "elif"
  "static_elif"
  "else"
] @keyword.conditional

[
  "for"
  "while"
  "where"
  (break_expression)
  (continue_expression)
] @keyword.repeat

[
  "try"
  "recover"
  "finally"
] @keyword.exception

[
  "module"
  "require"
] @keyword.import

[
  "options"
  "expect"
] @keyword.directive

[
  "addr"
  "as"
  "cast"
  "delete"
  "deref"
  "in"
  "is"
  "new"
  "reinterpret"
  "type"
  "typedecl"
  "typeinfo"
  "upcast"
  "!as"
  "!is"
  "!?as"
] @keyword.operator

[
  "array"
  "auto"
  "bitfield"
  "block"
  "fixed_array"
  "function"
  "iterator"
  "lambda"
  "smart_ptr"
  "table"
  "tuple"
  "variant"
] @type.builtin

[
  "="
  "<-"
  ":="
  "!=="
  "!<-"
  "!:="
  "+="
  "-="
  "*="
  "/="
  "%="
  "&="
  "|="
  "^="
  "&&="
  "||="
  "^^="
  "<<="
  ">>="
  "<<<="
  ">>>="
  "+"
  "-"
  "*"
  "/"
  "%"
  "++"
  "--"
  "=="
  "!="
  "<"
  ">"
  "<="
  ">="
  "<<"
  ">>"
  "<<<"
  ">>>"
  "&"
  "|"
  "^"
  "~"
  "!"
  "&&"
  "||"
  "^^"
  "?"
  "??"
  "!??"
  "#"
  ".."
  "=>"
  "<|"
  "|>"
  "@@"
] @operator

[
  "("
  ")"
  "["
  "]"
  "{"
  "}"
  "?["
  "!["
  "!?["
] @punctuation.bracket

(_
  "<" @punctuation.bracket
  ">")

(_
  "<"
  ">" @punctuation.bracket)

[
  ","
  ";"
  ":"
  "."
  "::"
  "?."
  "!."
  "!?."
  "->"
] @punctuation.delimiter

(module_path
  [
    "%"
    ".."
    "/"
  ] @punctuation.delimiter)

(ternary_operation
  [
    "?"
    ":"
  ] @keyword.conditional.ternary)

(safe_as_variant
  "?" @keyword.operator)

(make_block
  [
    "$"
    "@"
    "@@"
  ] @keyword.function)

(type_macro
  "$" @function.macro)

(tag
  "@@" @function.macro)

[
  (field_declaration
    "@" @attribute)
  (variable_declaration
    "@" @attribute)
  (for_expression
    "@" @attribute)
  (while_expression
    "@" @attribute)
]

(reader
  [
    "%"
    "~"
  ] @function.macro)

[
  (tuple_alias_declaration
    "tuple" @keyword.type)
  (variant_alias_declaration
    "variant" @keyword.type)
  (bitfield_alias_declaration
    "bitfield" @keyword.type)
]

(require_declaration
  "as" @keyword.import)

(interpolation
  [
    "{"
    "}"
  ] @punctuation.special)

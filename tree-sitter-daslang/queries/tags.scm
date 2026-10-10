(
  [
    (struct_declaration
      (comment)* @doc
      .
      methods: (function_declaration
        name: (_) @name) @definition.method)
    (class_declaration
      (comment)* @doc
      .
      methods: (function_declaration
        name: (_) @name) @definition.method)
  ]
  (#strip! @doc "^//\\s?")
  (#select-adjacent! @doc @definition.method)
)

(
  (comment)* @doc
  .
  (function_declaration
    name: (_) @name) @definition.function
  (#strip! @doc "^//\\s?")
  (#select-adjacent! @doc @definition.function)
)

(
  (comment)* @doc
  .
  [
    (struct_declaration
      name: (identifier) @name)
    (class_declaration
      name: (identifier) @name)
    (enumeration_declaration
      name: (identifier) @name)
  ] @definition.class
  (#strip! @doc "^//\\s?")
  (#select-adjacent! @doc @definition.class)
)

(
  (comment)* @doc
  .
  [
    (typedef_declaration
      name: (identifier) @name)
    (tuple_alias_declaration
      name: (identifier) @name)
    (variant_alias_declaration
      name: (identifier) @name)
    (bitfield_alias_declaration
      name: (identifier) @name)
  ] @definition.type
  (#strip! @doc "^//\\s?")
  (#select-adjacent! @doc @definition.type)
)

(module_declaration
  name: (identifier) @name) @definition.module

(global_let
  "let"
  variables: (variable_declaration
    name: (identifier) @name)) @definition.constant

[
  (struct_declaration
    parent: [
      (identifier) @name
      (qualified_name
        name: (identifier) @name)
    ])
  (class_declaration
    parent: [
      (identifier) @name
      (qualified_name
        name: (identifier) @name)
    ])
] @reference.class

[
  (call
    name: [
      (identifier) @name
      (qualified_name
        name: (identifier) @name)
    ])
  (named_call
    name: [
      (identifier) @name
      (qualified_name
        name: (identifier) @name)
    ])
  (invoke
    name: (identifier) @name)
  (pipe
    function_call: (variable
      name: [
        (identifier) @name
        (qualified_name
          name: (identifier) @name)
      ]))
] @reference.call

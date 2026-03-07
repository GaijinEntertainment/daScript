; tree-sitter-daslang highlights.scm
; Highlight queries for the daslang programming language

; ============================================================================
; Comments
; ============================================================================

(line_comment) @comment.line
(block_comment) @comment.block

; ============================================================================
; Literals
; ============================================================================

(integer_literal) @number
(float_literal) @number.float
(float_trailing_dot) @number.float
(boolean_literal) @boolean
(null_literal) @constant.builtin
(character_literal) @character
(string_literal) @string
(string_interpolation) @string
(escape_sequence) @string.escape
(format_specifier) @string.special

; ============================================================================
; Types
; ============================================================================

(basic_type) @type.builtin

(named_type (identifier) @type)
(named_type (scoped_identifier) @type)

(auto_type "auto" @type.builtin)

(pointer_type) @type
(smart_pointer_type "smart_ptr" @type.builtin)
(array_type "array" @type.builtin)
(table_type "table" @type.builtin)
(iterator_type "iterator" @type.builtin)
(block_type "block" @type.builtin)
(function_type "function" @type.builtin)
(lambda_type "lambda" @type.builtin)
(tuple_type "tuple" @type.builtin)
(variant_type "variant" @type.builtin)
(bitfield_type "bitfield" @type.builtin)
(typedecl_type "typedecl" @type.builtin)

(template_type) @type
(type_macro name: (identifier) @type)

; Dim array suffix
(dim_type) @type

; ============================================================================
; Functions
; ============================================================================

; Function declarations
(function_declaration
  name: (function_name (identifier) @function))

; Operator overloads
(function_declaration
  name: (function_name (operator_name) @function.builtin))

; Structure methods
(structure_method
  name: (function_name (identifier) @function.method))
(structure_method
  name: (function_name (operator_name) @function.builtin))

; Function calls
(call_expression
  function: (identifier) @function.call)
(call_expression
  function: (scoped_identifier) @function.call)

; Method calls (obj.method())
(method_call_expression
  method: (identifier) @function.method.call)

; Arrow calls (obj->method())
(arrow_call_expression
  method: (identifier) @function.method.call)

; Call with block
(call_with_block_expression
  (call_expression
    function: (identifier) @function.call))

; Function address
(func_addr_expression
  function: (identifier) @function)
(func_addr_expression
  function: (scoped_identifier) @function)

; ============================================================================
; Declarations and definitions
; ============================================================================

; Module name
(module_declaration (identifier) @module)

; Require (import)
(require_declaration
  (require_module_name) @module)

; Struct/class names
(structure_declaration
  name: (identifier) @type.definition)

; Enum names
(enum_declaration
  name: (identifier) @type.definition)

; Enum entries
(enum_entry
  name: (identifier) @constant)

; Bitfield entries
(bitfield_entry (identifier) @constant)

; Typedef names
(typedef_declaration
  name: (identifier) @type.definition)
(typedef_statement
  name: (identifier) @type.definition)
(structure_typedef
  name: (identifier) @type.definition)

; Tuple alias
(tuple_alias_declaration
  name: (identifier) @type.definition)

; Variant alias
(variant_alias_declaration
  name: (identifier) @type.definition)

; Bitfield alias
(bitfield_alias_declaration
  name: (identifier) @type.definition)

; Variable declarations
(variable_binding
  name: (identifier) @variable)

; Global variable declarations
(global_variable_binding
  name: (identifier) @variable)

; Function arguments
(function_argument
  name: (identifier) @variable.parameter)

; For loop variables
(for_variable (identifier) @variable)

; Struct fields
(structure_field
  name: (identifier) @property)

; Struct constructor fields
(make_struct_field
  name: (identifier) @property)

; Variant entries
(variant_entry (identifier) @property)

; Tuple entries
(tuple_entry (identifier) @property)

; Capture entries
(capture_entry (identifier) @variable)

; ============================================================================
; Field access
; ============================================================================

(field_expression
  field: (identifier) @property)

(safe_field_expression
  field: (identifier) @property)

; ============================================================================
; Annotations
; ============================================================================

(annotation_declaration
  name: (annotation_name (identifier) @attribute))
(annotation_declaration
  name: (annotation_name (scoped_identifier) @attribute))

(annotation_argument
  name: (identifier) @attribute)

(metadata_argument_list) @attribute

; ============================================================================
; Keywords
; ============================================================================

; Control flow
"if" @keyword.conditional
"elif" @keyword.conditional
"else" @keyword.conditional
"static_if" @keyword.conditional
"static_elif" @keyword.conditional

; Loops
"for" @keyword.repeat
"while" @keyword.repeat
"break" @keyword.repeat
"continue" @keyword.repeat

; Functions
"def" @keyword.function
"return" @keyword.return
"yield" @keyword.return

; Exception handling
"try" @keyword.exception
"recover" @keyword.exception

; Declarations
"struct" @keyword.type
"class" @keyword.type
"enum" @keyword.type
"typedef" @keyword.type
"variant" @keyword.type
"tuple" @keyword.type
"bitfield" @keyword.type

; Modifiers
"let" @keyword
"var" @keyword
"const" @keyword
"override" @keyword
"sealed" @keyword
"abstract" @keyword
"static" @keyword
"public" @keyword
"private" @keyword
"shared" @keyword
"inscope" @keyword
"implicit" @keyword
"explicit" @keyword

; Other keywords
"module" @keyword.import
"require" @keyword.import
"include" @keyword.import
"options" @keyword
"expect" @keyword
"assume" @keyword
"with" @keyword
"where" @keyword
"label" @keyword
"goto" @keyword
"pass" @keyword
"delete" @keyword
"new" @keyword
"deref" @keyword
"addr" @keyword
"cast" @keyword
"upcast" @keyword
"reinterpret" @keyword
"typeinfo" @keyword
"type" @keyword
"unsafe" @keyword
"finally" @keyword
"in" @keyword
"is" @keyword
"as" @keyword
"aka" @keyword
"operator" @keyword
"template" @keyword
"default" @keyword
"uninitialized" @keyword
"generator" @keyword
"lambda" @keyword
"capture" @keyword

; Boolean keywords
"true" @boolean
"false" @boolean

; ============================================================================
; Operators
; ============================================================================

; Arithmetic
"+" @operator
"-" @operator
"*" @operator
"/" @operator
"%" @operator

; Comparison
"==" @operator
"!=" @operator
"<" @operator
">" @operator
"<=" @operator
">=" @operator

; Logical
"&&" @operator
"||" @operator
"^^" @operator
"!" @operator

; Bitwise
"&" @operator
"|" @operator
"^" @operator
"~" @operator
"<<" @operator
">>" @operator
"<<<" @operator
">>>" @operator

; Assignment
"=" @operator
":=" @operator
"<-" @operator
"+=" @operator
"-=" @operator
"*=" @operator
"/=" @operator
"%=" @operator
"&=" @operator
"|=" @operator
"^=" @operator
"&&=" @operator
"||=" @operator
"^^=" @operator
"<<=" @operator
">>=" @operator
"<<<=" @operator
">>>=" @operator

; Other
"??" @operator
".." @operator
"=>" @operator
"|>" @operator
"<|" @operator
"++" @operator
"--" @operator
"?as" @operator
"->" @operator
"@@" @operator

; ============================================================================
; Punctuation
; ============================================================================

"(" @punctuation.bracket
")" @punctuation.bracket
"[" @punctuation.bracket
"]" @punctuation.bracket
"{" @punctuation.bracket
"}" @punctuation.bracket

";" @punctuation.delimiter
"," @punctuation.delimiter
":" @punctuation.delimiter
"." @punctuation.delimiter
"?." @punctuation.delimiter
"?[" @punctuation.bracket

; ============================================================================
; Quote expressions (macro interpolation)
; ============================================================================

(quote_expression) @function.macro
(quote_type) @function.macro

; ============================================================================
; Reader macros
; ============================================================================

(reader_macro
  name: (identifier) @function.macro)
(reader_macro_content) @string

; ============================================================================
; Special expressions
; ============================================================================

(spread_expression) @punctuation.special
(uninitialized_expression) @keyword

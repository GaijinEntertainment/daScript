/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

// Operator precedence levels (higher = tighter binding)
const PREC = {
  COMMA: -1,
  RANGE: 1,       // ..
  TUPLE_ARROW: 1, // =>
  ASSIGN: 2,      // = <- := += -= etc
  TERNARY: 3,     // ? :
  OR: 4,          // ||
  XOR: 5,         // ^^
  AND: 6,         // &&
  BIT_OR: 7,      // |
  BIT_XOR: 8,     // ^
  BIT_AND: 9,     // &
  EQUAL: 10,      // == !=
  COMPARE: 11,    // < > <= >=
  SHIFT: 12,      // << >> <<< >>>
  ADD: 13,        // + -
  MUL: 14,        // * / %
  COALESCE: 15,   // ??
  UNARY: 16,      // ! ~ + - ++ --
  IS_AS: 17,      // is as
  PIPE: 18,       // |> <|
  POSTFIX: 19,    // ++ -- ->
  DEREF: 19,      // * (deref)
  DOT: 20,        // . ?. ?.
  INDEX: 21,      // [] ?[]
  CALL: 22,       // ()
};

const basic_types = [
  'bool', 'void', 'string',
  'int', 'int2', 'int3', 'int4',
  'int8', 'int16', 'int64',
  'uint', 'uint2', 'uint3', 'uint4',
  'uint8', 'uint16', 'uint64',
  'float', 'float2', 'float3', 'float4',
  'double',
  'range', 'urange', 'range64', 'urange64',
  'bitfield',
];

const keywords = [
  'module', 'require', 'options', 'expect',
  'def', 'struct', 'class', 'enum',
  'let', 'var',
  'if', 'elif', 'else', 'static_if', 'static_elif',
  'for', 'while', 'in',
  'return', 'yield', 'break', 'continue', 'pass',
  'try', 'recover', 'finally',
  'new', 'delete',
  'typeinfo', 'type', 'typedecl', 'typedef',
  'cast', 'upcast', 'reinterpret',
  'assume', 'with', 'unsafe',
  'is', 'as',
  'where',
  'null', 'true', 'false',
  'const', 'override', 'sealed', 'abstract',
  'public', 'private', 'shared',
  'operator', 'implicit', 'explicit',
  'addr', 'deref',
  'block', 'function', 'lambda', 'generator',
  'array', 'table', 'iterator', 'smart_ptr', 'fixed_array',
  'tuple', 'variant',
  'template',
  'aka', 'inscope', 'static',
  'capture', 'default', 'uninitialized',
  'label', 'goto',
];

module.exports = grammar({
  name: 'daslang',

  extras: $ => [
    /\s/,
    $.line_comment,
    $.block_comment,
  ],

  word: $ => $.identifier,

  externals: $ => [
    $._string_content,
    $._automatic_semicolon,
    $.float_trailing_dot,
  ],

  supertypes: $ => [
    $._expression,
    $._statement,
    $._declaration,
    $._type,
  ],

  inline: $ => [
    $._expression_or_braced,
  ],

  conflicts: $ => [
    [$.call_expression, $.struct_constructor, $.named_call_expression],
    [$.global_variable_binding, $.dim_type],
    [$.global_variable_binding, $.remove_modifier],
    [$.variable_binding, $.dim_type],
    [$.variable_binding, $.remove_modifier],
    [$.typedef_declaration, $.dim_type],
    [$.typedef_declaration, $.remove_modifier],
    [$.typedef_statement, $.dim_type],
    [$.typedef_statement, $.remove_modifier],
    [$.structure_typedef, $.dim_type],
    [$.structure_typedef, $.remove_modifier],
    [$.structure_field, $.dim_type],
    [$.structure_field, $.remove_modifier],
    [$.function_return_type, $.dim_type],
    [$.function_return_type, $.remove_modifier],
    [$.expression_statement, $.table_literal],
    [$.func_addr_expression, $.lambda_expression],
    [$.annotation_name, $._expression],
  ],

  rules: {
    // ========================================================================
    // Top-level program structure
    // ========================================================================

    source_file: $ => repeat($._top_level_item),

    _top_level_item: $ => choice(
      $._declaration,
      // Statement types allowed at top level for ast-grep pattern matching.
      // Excludes variable_declaration_statement and typedef_statement
      // to avoid ambiguity with top-level declarations.
      $.expression_statement,
      $.if_statement,
      $.for_statement,
      $.while_statement,
      $.with_statement,
      $.unsafe_block,
      $.try_recover_statement,
      $.return_statement,
      $.yield_statement,
      $.break_statement,
      $.continue_statement,
      $.delete_statement,
      $.assume_statement,
      $.label_statement,
      $.goto_statement,
      $.pass_statement,
    ),

    _declaration: $ => choice(
      $.module_declaration,
      $.require_declaration,
      $.options_declaration,
      $.expect_declaration,
      $.function_declaration,
      $.structure_declaration,
      $.enum_declaration,
      $.global_variable_declaration,
      $.typedef_declaration,
      $.tuple_alias_declaration,
      $.variant_alias_declaration,
      $.bitfield_alias_declaration,
    ),

    // ========================================================================
    // Module, require, options, expect
    // ========================================================================

    module_declaration: $ => seq(
      'module',
      field('name', choice($.identifier, '$')),
      optional('shared'),
      optional(choice('public', 'private')),
      optional(seq('!', 'inscope')),
      $._semicolon,
    ),

    require_declaration: $ => seq(
      'require',
      field('module', $.require_module_name),
      optional(seq('as', field('alias', $.identifier))),
      optional('public'),
      $._semicolon,
    ),

    require_module_name: $ => seq(
      optional('%'),
      $.identifier,
      repeat(seq(choice('.', '/'), $.identifier)),
    ),

    options_declaration: $ => seq(
      'options',
      $.annotation_argument_list,
      $._semicolon,
    ),

    expect_declaration: $ => seq(
      'expect',
      sep1($.expect_error, ','),
      $._semicolon,
    ),

    expect_error: $ => seq(
      $.integer_literal,
      optional(seq(':', $.integer_literal)),
    ),

    // ========================================================================
    // Annotations
    // ========================================================================

    annotation_list: $ => seq(
      '[',
      sep1($.annotation_declaration, choice(',', '&&', '||', '^^')),
      ']',
    ),

    annotation_declaration: $ => seq(
      optional(choice(
        seq('|', '>'),  // inherited annotation
        '!',
      )),
      field('name', $.annotation_name),
      optional(seq('(', $.annotation_argument_list, ')')),
    ),

    annotation_name: $ => choice(
      $._name_in_namespace,
      'require',
      'private',
      'template',
    ),

    annotation_argument_list: $ => sep1($.annotation_argument, ','),

    annotation_argument: $ => seq(
      field('name', $._annotation_argument_name),
      optional(seq('=', field('value', $._annotation_argument_value))),
    ),

    _annotation_argument_name: $ => choice(
      $.identifier,
      'type',
      'in',
    ),

    _annotation_argument_value: $ => choice(
      $.string_literal,
      $.identifier,
      $.integer_literal,
      $.float_literal,
      'true',
      'false',
      seq('(', sep1($._annotation_argument_value, ','), ')'),
    ),

    // Metadata annotations on fields: @name = value
    metadata_argument_list: $ => repeat1(seq(
      '@',
      $.annotation_argument,
    )),

    // ========================================================================
    // Function declarations
    // ========================================================================

    function_declaration: $ => seq(
      optional($.annotation_list),
      'def',
      optional('template'),
      optional(choice('public', 'private')),
      field('name', $.function_name),
      optional($.function_argument_list),
      optional($.function_return_type),
      field('body', $.block),
    ),

    function_name: $ => choice(
      $.identifier,
      $.operator_name,
      $.basic_type,
    ),

    operator_name: $ => prec.left(seq(
      'operator',
      choice(
        '!', '~',
        '+=', '-=', '*=', '/=', '%=',
        '&=', '|=', '^=',
        '&&=', '||=', '^^=',
        '&&', '||', '^^',
        '+', '-', '*', '/', '%',
        '<', '>',
        '..', '==', '!=', '<=', '>=',
        '&', '|', '^',
        seq('++', optional(token.immediate('operator'))),
        seq('--', optional(token.immediate('operator'))),
        '++', '--',  // prefix (shown as +++ --- in AST)
        '<<', '>>', '<<=', '>>=',
        '<<<', '>>>', '<<<=', '>>>=',
        seq('[', ']'),
        seq('[', ']', choice('=', '<-', ':=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '&&=', '||=', '^^=')),
        seq('?', '[', ']'),
        '.', '?.',
        seq('.', $.identifier, ':='),
        seq('.', $.identifier),
        ':=',
        'delete',
        '??',
        // Longer matches first for greedy matching
        seq('is', choice($.identifier, $.basic_type)),
        seq('as', choice($.identifier, $.basic_type)),
        seq('?', 'as', choice($.identifier, $.basic_type)),
        seq('?', 'as'),
        'is', 'as',
      ),
    )),

    function_argument_list: $ => seq(
      '(',
      optional(sep1($.function_argument, choice(';', ','))),
      ')',
    ),

    function_argument: $ => seq(
      optional($.metadata_argument_list),
      optional(choice('let', 'var')),
      field('name', $._variable_name_list),
      optional(seq(':', field('type', $._type))),
      optional(seq(choice('=', '<-'), field('default', $._expression))),
    ),

    function_return_type: $ => seq(
      choice(':', '->'),
      field('type', $._type),
    ),

    // ========================================================================
    // Struct / class declarations
    // ========================================================================

    structure_declaration: $ => seq(
      optional($.annotation_list),
      field('kind', choice(
        'struct',
        'class',
        seq('struct', 'template'),
        seq('class', 'template'),
      )),
      optional(choice('public', 'private')),
      optional('sealed'),
      field('name', $.identifier),
      optional(seq(':', field('parent', $._name_in_namespace))),
      '{',
      repeat($.structure_member),
      '}',
    ),

    structure_member: $ => choice(
      $.structure_field,
      $.structure_method,
      $.structure_typedef,
    ),

    structure_field: $ => seq(
      optional($.metadata_argument_list),
      optional('static'),
      optional(choice('override', 'sealed')),
      optional(choice('public', 'private')),
      field('name', $._variable_name_list),
      optional(seq(':', field('type', $._type))),
      optional(seq(choice('=', '<-', ':='), field('value', $._expression))),
      $._semicolon,
    ),

    structure_method: $ => seq(
      optional($.annotation_list),
      'def',
      optional(choice('public', 'private')),
      optional('static'),
      optional(choice('override', 'sealed')),
      // const and abstract can appear in any order
      optional('abstract'),
      optional('const'),
      optional('abstract'),
      field('name', $.function_name),
      optional($.function_argument_list),
      optional($.function_return_type),
      optional(choice(
        ';',
        field('body', $.block),
      )),
    ),

    structure_typedef: $ => seq(
      'typedef',
      field('name', $.identifier),
      '=',
      field('type', $._type),
      $._semicolon,
    ),

    // ========================================================================
    // Enum declarations
    // ========================================================================

    enum_declaration: $ => seq(
      optional($.annotation_list),
      'enum',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional(seq(':', field('base_type', $.enum_base_type))),
      '{',
      repeat(seq($.enum_entry, optional(choice(',', ';')))),
      '}',
    ),

    enum_base_type: $ => choice(
      'int', 'int8', 'int16', 'int64',
      'uint', 'uint8', 'uint16', 'uint64',
    ),

    enum_entry: $ => seq(
      field('name', $.identifier),
      optional(seq('=', field('value', $._expression))),
    ),

    // ========================================================================
    // Global variable declarations
    // ========================================================================

    global_variable_declaration: $ => seq(
      choice('let', 'var'),
      optional('shared'),
      optional(choice('public', 'private')),
      choice(
        // Single declaration
        seq(
          optional($.metadata_argument_list),
          $.global_variable_binding,
        ),
        // Block of declarations
        seq('{', repeat(seq(
          optional($.metadata_argument_list),
          $.global_variable_binding,
        )), '}'),
      ),
    ),

    global_variable_binding: $ => seq(
      field('name', $._global_variable_name_list),
      choice(
        seq(':', field('type', $._type), optional(seq(choice('=', '<-', ':='), field('value', $._expression)))),
        seq(optional('&'), choice('=', '<-', ':='), field('value', $._expression)),
      ),
      $._semicolon,
    ),

    _global_variable_name_list: $ => sep1($.identifier, ','),

    // ========================================================================
    // Type alias declarations
    // ========================================================================

    typedef_declaration: $ => choice(
      // typedef Name = Type
      seq(
        optional($.annotation_list),
        'typedef',
        optional(choice('public', 'private')),
        field('name', $.identifier),
        '=',
        field('type', $._type),
        $._semicolon,
      ),
    ),

    tuple_alias_declaration: $ => seq(
      'tuple',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      '{',
      repeat(seq($.tuple_entry, optional(choice(';', ',')))),
      '}',
    ),

    tuple_entry: $ => seq(
      optional(seq(field('name', $.identifier), ':')),
      field('type', $._type),
    ),

    variant_alias_declaration: $ => seq(
      'variant',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      '{',
      repeat(seq($.variant_entry, optional(choice(';', ',')))),
      '}',
    ),

    variant_entry: $ => seq(
      field('name', $.identifier),
      ':',
      field('type', $._type),
    ),

    bitfield_alias_declaration: $ => seq(
      'bitfield',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional(seq(':', field('base_type', choice('uint8', 'uint16', 'uint', 'uint64')))),
      '{',
      repeat(seq($.bitfield_entry, optional(choice(',', ';')))),
      '}',
    ),

    bitfield_entry: $ => seq(
      field('name', $.identifier),
      optional(seq('=', field('value', $._expression))),
    ),

    // ========================================================================
    // Statements
    // ========================================================================

    block: $ => prec(1, seq(
      '{',
      repeat(choice($._statement, ';')),
      '}',
      optional($.finally_block),
    )),

    finally_block: $ => seq(
      'finally',
      '{',
      repeat(choice($._statement, ';')),
      '}',
    ),

    _statement: $ => choice(
      $.expression_statement,
      $.variable_declaration_statement,
      $.if_statement,
      $.for_statement,
      $.while_statement,
      $.with_statement,
      $.unsafe_block,
      $.try_recover_statement,
      $.return_statement,
      $.yield_statement,
      $.break_statement,
      $.continue_statement,
      $.delete_statement,
      $.assume_statement,
      $.typedef_statement,
      $.label_statement,
      $.goto_statement,
      $.pass_statement,
      $.block,
    ),

    _semicolon: $ => choice(';', $._automatic_semicolon),

    expression_statement: $ => seq($._expression_or_assignment, $._semicolon),

    _expression_or_assignment: $ => choice(
      $._expression,
      $.assignment_expression,
    ),

    assignment_expression: $ => prec.right(PREC.ASSIGN, seq(
      field('left', $._expression),
      field('operator', choice(
        '=', '<-', ':=',
        '+=', '-=', '*=', '/=', '%=',
        '&=', '|=', '^=',
        '&&=', '||=', '^^=',
        '<<=', '>>=', '<<<=', '>>>=',
      )),
      field('right', $._expression),
    )),

    // ---- Variable declarations ----

    variable_declaration_statement: $ => seq(
      choice('let', 'var'),
      optional('inscope'),
      choice(
        seq($.variable_binding, $._semicolon),
        seq('{', repeat($.variable_binding), '}'),
        seq($.tuple_expansion_binding, $._semicolon),
      ),
    ),

    variable_binding: $ => seq(
      field('name', $._variable_name_list),
      choice(
        seq(':', field('type', $._type), optional(seq(choice('=', '<-', ':='), field('value', $._expression)))),
        seq(optional('&'), choice('=', '<-', ':='), field('value', $._expression)),
      ),
    ),

    tuple_expansion_binding: $ => seq(
      '(',
      sep1($.identifier, ','),
      ')',
      optional(seq(':', field('type', $._type))),
      choice('=', '<-', ':='),
      field('value', $._expression),
    ),

    _variable_name_list: $ => prec.left(sep1($._variable_name, ',')),

    _variable_name: $ => seq(
      $.identifier,
      optional(seq('aka', field('alias', $.identifier))),
    ),

    // ---- Control flow ----

    if_statement: $ => prec.right(seq(
      choice('if', 'static_if'),
      '(',
      field('condition', $._expression),
      ')',
      field('consequence', $._if_body),
      repeat($.elif_clause),
      optional($.else_clause),
    )),

    elif_clause: $ => seq(
      choice('elif', 'static_elif'),
      '(',
      field('condition', $._expression),
      ')',
      field('body', $._if_body),
    ),

    else_clause: $ => seq(
      'else',
      field('body', $._if_body),
    ),

    _if_body: $ => choice(
      $.block,
      $._one_liner_body,
    ),

    _one_liner_body: $ => choice($._expression, $.return_statement, $.yield_statement, $.break_statement, $.continue_statement),

    // Oneliner if: `expr if (cond) else expr`
    // This is an expression, handled in expressions section

    for_statement: $ => seq(
      'for',
      '(',
      field('variables', sep1($.for_variable, ',')),
      'in',
      field('iterators', sep1($._expression, ',')),
      ')',
      field('body', $.block),
    ),

    for_variable: $ => choice(
      seq($.identifier, optional(seq('aka', $.identifier))),
      seq('(', sep1($.identifier, ','), ')'),  // tuple expansion
    ),

    while_statement: $ => seq(
      'while',
      '(',
      field('condition', $._expression),
      ')',
      field('body', $.block),
    ),

    with_statement: $ => seq(
      'with',
      '(',
      field('object', $._expression),
      ')',
      field('body', $.block),
    ),

    unsafe_block: $ => seq(
      'unsafe',
      $.block,
    ),

    try_recover_statement: $ => seq(
      'try',
      field('body', $.block),
      'recover',
      field('handler', $.block),
    ),

    return_statement: $ => prec.right(seq(
      'return',
      optional(seq(optional('<-'), $._expression)),
      $._semicolon,
    )),

    yield_statement: $ => prec.right(seq(
      'yield',
      optional('<-'),
      $._expression,
      $._semicolon,
    )),

    break_statement: $ => seq('break', $._semicolon),
    continue_statement: $ => seq('continue', $._semicolon),
    pass_statement: $ => seq('pass', $._semicolon),

    delete_statement: $ => seq(
      'delete',
      optional('explicit'),
      $._expression,
      $._semicolon,
    ),

    assume_statement: $ => seq(
      'assume',
      field('name', $.identifier),
      '=',
      field('value', $._expression),
      $._semicolon,
    ),

    typedef_statement: $ => seq(
      'typedef',
      field('name', $.identifier),
      '=',
      field('type', $._type),
      $._semicolon,
    ),

    label_statement: $ => seq(
      'label',
      field('name', $.integer_literal),
      ':',
    ),

    goto_statement: $ => seq(
      'goto',
      choice(
        seq('label', field('name', $.integer_literal)),
        field('target', $._expression),
      ),
      $._semicolon,
    ),

    // ========================================================================
    // Expressions
    // ========================================================================

    _expression: $ => choice(
      $._name_in_namespace,
      $._literal,
      $.parenthesized_expression,
      $.unary_expression,
      $.binary_expression,
      $.ternary_expression,
      $.null_coalescing_expression,
      $.range_expression,
      $.tuple_expression,
      $.is_expression,
      $.as_expression,
      $.call_expression,
      $.struct_constructor,
      $.named_call_expression,
      $.method_call_expression,
      $.arrow_call_expression,
      $.pipe_expression,
      $.field_expression,
      $.safe_field_expression,
      $.index_expression,
      $.safe_index_expression,
      $.deref_expression,
      $.addr_expression,
      $.cast_expression,
      $.typeinfo_expression,
      $.type_expression,
      $.new_expression,
      $.func_addr_expression,
      $.block_expression,
      $.lambda_expression,
      $.generator_expression,
      $.array_literal,
      $.table_literal,
      $.array_comprehension,
      $.table_comprehension,
      $.fixed_array_expression,
      $.array_constructor,
      $.table_constructor,
      $.tuple_constructor,
      $.struct_make_expression,
      $.default_expression,
      $.unsafe_expression,
      $.postfix_expression,
      $.call_with_block_expression,
      $.oneliner_if_expression,
      $.reader_macro,
    ),

    // Wrap expressions that start with '{' (table literal, table comprehension)
    // vs bare blocks at statement level — tree-sitter handles by context
    _expression_or_braced: $ => choice(
      $._expression,
    ),

    parenthesized_expression: $ => seq(
      '(',
      sep1($._expression_or_assignment, ','),
      optional(','),
      ')',
    ),

    // ---- Unary operators ----

    unary_expression: $ => choice(
      prec.right(PREC.UNARY, seq('!', $._expression)),
      prec.right(PREC.UNARY, seq('~', $._expression)),
      prec.right(PREC.UNARY, seq('+', $._expression)),
      prec.right(PREC.UNARY, seq('-', $._expression)),
      prec.right(PREC.UNARY, seq('++', $._expression)),
      prec.right(PREC.UNARY, seq('--', $._expression)),
    ),

    postfix_expression: $ => choice(
      prec.left(PREC.POSTFIX, seq($._expression, '++')),
      prec.left(PREC.POSTFIX, seq($._expression, '--')),
    ),

    // ---- Binary operators ----

    binary_expression: $ => choice(
      // Arithmetic
      prec.left(PREC.ADD, seq($._expression, '+', $._expression)),
      prec.left(PREC.ADD, seq($._expression, '-', $._expression)),
      prec.left(PREC.MUL, seq($._expression, '*', $._expression)),
      prec.left(PREC.MUL, seq($._expression, '/', $._expression)),
      prec.left(PREC.MUL, seq($._expression, '%', $._expression)),
      // Shift / Rotate
      prec.left(PREC.SHIFT, seq($._expression, '<<', $._expression)),
      prec.left(PREC.SHIFT, seq($._expression, '>>', $._expression)),
      prec.left(PREC.SHIFT, seq($._expression, '<<<', $._expression)),
      prec.left(PREC.SHIFT, seq($._expression, '>>>', $._expression)),
      // Comparison
      prec.left(PREC.COMPARE, seq($._expression, '<', $._expression)),
      prec.left(PREC.COMPARE, seq($._expression, '>', $._expression)),
      prec.left(PREC.COMPARE, seq($._expression, '<=', $._expression)),
      prec.left(PREC.COMPARE, seq($._expression, '>=', $._expression)),
      // Equality
      prec.left(PREC.EQUAL, seq($._expression, '==', $._expression)),
      prec.left(PREC.EQUAL, seq($._expression, '!=', $._expression)),
      // Bitwise
      prec.left(PREC.BIT_AND, seq($._expression, '&', $._expression)),
      prec.left(PREC.BIT_OR, seq($._expression, '|', $._expression)),
      prec.left(PREC.BIT_XOR, seq($._expression, '^', $._expression)),
      // Logical
      prec.left(PREC.AND, seq($._expression, '&&', $._expression)),
      prec.left(PREC.OR, seq($._expression, '||', $._expression)),
      prec.left(PREC.XOR, seq($._expression, '^^', $._expression)),
    ),

    ternary_expression: $ => prec.right(PREC.TERNARY, seq(
      field('condition', $._expression),
      '?',
      field('consequence', $._expression),
      ':',
      field('alternative', $._expression),
    )),

    null_coalescing_expression: $ => prec.right(PREC.COALESCE, seq(
      field('value', $._expression),
      '??',
      field('default', $._expression),
    )),

    range_expression: $ => prec.left(PREC.RANGE, seq(
      field('start', $._expression),
      '..',
      field('end', $._expression),
    )),

    tuple_expression: $ => prec.left(PREC.TUPLE_ARROW, seq(
      field('first', $._expression),
      '=>',
      field('second', $._expression),
    )),

    // ---- is / as ----

    is_expression: $ => prec.left(PREC.IS_AS, seq(
      field('value', $._expression),
      'is',
      field('type', choice(
        seq('type', '<', $._type, '>'),
        $.basic_type,
        $.identifier,
      )),
    )),

    as_expression: $ => prec.left(PREC.IS_AS, seq(
      field('value', $._expression),
      field('operator', choice('as', '?as')),
      field('type', choice(
        seq('type', '<', $._type, '>'),
        $.basic_type,
        $.identifier,
      )),
    )),

    // ---- Pipe operators ----

    pipe_expression: $ => choice(
      prec.left(PREC.PIPE, seq($._expression, '|>', $._expression)),
      prec.left(PREC.PIPE, seq($._expression, '<|', $._expression)),
    ),

    // ---- Access expressions ----

    field_expression: $ => prec.left(PREC.DOT, seq(
      field('object', $._expression),
      '.',
      field('field', $.identifier),
    )),

    safe_field_expression: $ => prec.left(PREC.DOT, seq(
      field('object', $._expression),
      '?.',
      field('field', $.identifier),
    )),

    index_expression: $ => prec.left(PREC.INDEX, seq(
      field('object', $._expression),
      '[',
      field('index', $._expression),
      ']',
    )),

    safe_index_expression: $ => prec.left(PREC.INDEX, seq(
      field('object', $._expression),
      '?[',
      field('index', $._expression),
      ']',
    )),

    deref_expression: $ => choice(
      prec.right(PREC.DEREF, seq('*', $._expression)),
      seq('deref', '(', $._expression, ')'),
    ),

    addr_expression: $ => seq(
      'addr', '(', $._expression, ')',
    ),

    // ---- Calls ----

    call_expression: $ => prec(PREC.CALL, seq(
      field('function', choice($._name_in_namespace, $.basic_type)),
      '(',
      optional($.argument_list),
      ')',
    )),

    struct_constructor: $ => prec(PREC.CALL, seq(
      field('type', $._name_in_namespace),
      '(',
      optional('uninitialized'),
      $.make_struct_fields,
      ')',
    )),

    named_call_expression: $ => prec(PREC.CALL, seq(
      field('function', $._name_in_namespace),
      '(',
      optional($.argument_list),
      '[',
      $.make_struct_fields,
      ']',
      ')',
    )),

    // UFCS: obj.method(args) — also handles obj.field (already in field_expression)
    method_call_expression: $ => prec.left(PREC.CALL, seq(
      field('object', $._expression),
      '.',
      field('method', choice($.identifier, $.basic_type)),
      '(',
      optional($.argument_list),
      ')',
    )),

    // obj->method(args)
    arrow_call_expression: $ => prec.left(PREC.POSTFIX, seq(
      field('object', $._expression),
      '->',
      field('method', $.identifier),
      '(',
      optional($.argument_list),
      ')',
    )),

    // func(args) block — call with trailing block
    call_with_block_expression: $ => prec(PREC.CALL, seq(
      choice(
        $.call_expression,
        $.method_call_expression,
        $.field_expression,
      ),
      field('block', choice($.block_expression, $.lambda_expression, $.block)),
    )),

    argument_list: $ => prec.left(seq(
      $._argument,
      repeat(seq(',', $._argument)),
      optional(','),
    )),

    _argument: $ => seq(
      optional('<-'),
      $._expression,
    ),

    // ---- Cast / typeinfo / type ----

    cast_expression: $ => prec(PREC.UNARY, seq(
      field('kind', choice('cast', 'upcast', 'reinterpret')),
      '<',
      field('type', $._type),
      '>',
      field('value', $._expression),
    )),

    typeinfo_expression: $ => seq(
      'typeinfo',
      field('trait', $._name_in_namespace),
      optional(seq('<', field('subtrait', $.identifier), optional(seq(choice(',', ';'), field('extra', $.identifier))), '>')),
      '(',
      field('value', $._expression),
      ')',
    ),

    type_expression: $ => seq(
      'type',
      '<',
      field('type', $._type),
      '>',
    ),

    // ---- New ----

    new_expression: $ => prec.right(PREC.UNARY, seq(
      'new',
      field('type', choice(
        seq('<', $._type, '>'),
        $._name_in_namespace,
      )),
      optional(seq(
        '(',
        optional(choice(
          seq(optional('uninitialized'), $.make_struct_fields),
          'uninitialized',
          $.argument_list,
        )),
        ')',
      )),
    )),

    // ---- Function address ----

    func_addr_expression: $ => prec.right(choice(
      // @@function_name or @@<type>function_name or @@<(args):ret>function_name
      seq(
        '@@',
        optional(seq('<', choice(
          $._type,
          seq(optional($.function_argument_list), optional($.function_return_type)),
        ), '>')),
        field('function', $._name_in_namespace),
      ),
      // @@(params) : ret { body } — anonymous function (no capture)
      seq(
        '@@',
        optional($.annotation_list),
        optional($.capture_list),
        optional($.function_argument_list),
        optional($.function_return_type),
        choice(
          field('body', $.block),
          seq('=>', field('body', $._expression)),
        ),
      ),
    )),

    // ---- Block / lambda / generator ----

    block_expression: $ => seq(
      '$',
      optional($.annotation_list),
      optional($.capture_list),
      optional($.function_argument_list),
      optional($.function_return_type),
      choice(
        field('body', $.block),
        seq('=>', field('body', $._expression)),
      ),
    ),

    lambda_expression: $ => seq(
      choice('@', '@@'),
      optional($.annotation_list),
      optional($.capture_list),
      optional($.function_argument_list),
      optional($.function_return_type),
      choice(
        field('body', $.block),
        seq('=>', field('body', $._expression)),
      ),
    ),

    generator_expression: $ => seq(
      'generator',
      '<',
      field('type', $._type),
      '>',
      optional($.capture_list),
      choice(
        seq('(', optional($._expression), ')'),
        field('body', $.block),
      ),
    ),

    capture_list: $ => seq(
      'capture',
      '(',
      sep1($.capture_entry, ','),
      ')',
    ),

    capture_entry: $ => choice(
      seq(choice('&', '=', '<-', ':='), $.identifier),
      seq('move', '(', $.identifier, ')'),
      seq('clone', '(', $.identifier, ')'),
    ),

    // ---- Reader macro: %name~ content %% ----

    reader_macro: $ => seq(
      '%',
      field('name', $.identifier),
      token.immediate('~'),
      field('content', optional($.reader_macro_content)),
      '%%',
    ),

    reader_macro_content: $ => /[^%]+(%[^%][^%]*)*/,

    // ---- Oneliner if expression: `expr if (cond) else expr` ----

    oneliner_if_expression: $ => prec.right(PREC.TERNARY, seq(
      field('consequence', $._expression),
      'if',
      '(',
      field('condition', $._expression),
      ')',
      optional(seq('else', field('alternative', $._expression))),
    )),

    // ---- Make / literal expressions ----

    // [1, 2, 3] — array literal
    array_literal: $ => seq(
      '[',
      optional(seq(
        sep1($._expression, ','),
        optional(','),
      )),
      ']',
    ),

    // { "k" => v, "k2" => v2 } or { 1, 2, 3, 4 } — table literal
    table_literal: $ => prec(1, seq(
      '{',
      sep1($._expression, ','),
      optional(','),
      '}',
    )),

    // [for (x in range(10)); x*x] — array/table comprehension with []
    array_comprehension: $ => seq(
      '[',
      optional('iterator'),
      'for',
      '(',
      field('variables', sep1($.for_variable, ',')),
      'in',
      field('iterators', sep1($._expression, ',')),
      ')',
      ';',
      field('value', $._expression),
      optional(seq(';', 'where', field('filter', $._expression))),
      ']',
    ),

    // {for (x in range(10)); x => x*x} — table comprehension with {}
    table_comprehension: $ => seq(
      '{',
      'for',
      '(',
      field('variables', sep1($.for_variable, ',')),
      'in',
      field('iterators', sep1($._expression, ',')),
      ')',
      ';',
      field('value', $._expression),
      optional(seq(';', 'where', field('filter', $._expression))),
      '}',
    ),

    // fixed_array(1, 2, 3)
    fixed_array_expression: $ => seq(
      'fixed_array',
      optional(seq('<', $._type, '>')),
      '(',
      $.argument_list,
      ')',
    ),

    // array(1, 2, 3) or array<Type>(...)
    array_constructor: $ => seq(
      'array',
      optional(seq('<', $._type, '>')),
      '(',
      optional($.argument_list),
      ')',
    ),

    // table("k" => v) or table<K;V>(...)
    table_constructor: $ => seq(
      'table',
      optional(seq('<', sep1($._type, choice(',', ';')), '>')),
      '(',
      optional($.argument_list),
      ')',
    ),

    // tuple(a, b, c) or tuple<...>(...)
    tuple_constructor: $ => seq(
      'tuple',
      optional(seq('<', sep1($._type, choice(',', ';')), '>')),
      '(',
      optional(choice(
        $.argument_list,
        seq(optional('uninitialized'), optional($.make_struct_fields)),
      )),
      ')',
    ),

    // struct<Type>(field=val) or class<Type>(...)
    struct_make_expression: $ => seq(
      choice('struct', 'class', 'variant'),
      optional(seq('type')),
      '<',
      $._type,
      '>',
      '(',
      optional(choice(
        $.argument_list,
        seq(optional('uninitialized'), optional($.make_struct_fields)),
      )),
      ')',
    ),

    // default<Type>
    default_expression: $ => seq(
      'default',
      '<',
      $._type,
      '>',
      optional('uninitialized'),
    ),

    // unsafe(expr) — expression form
    unsafe_expression: $ => seq(
      'unsafe',
      '(',
      $._expression,
      ')',
    ),

    // Named struct fields: field = expr, field <- expr
    make_struct_fields: $ => prec.left(seq(
      sep1($.make_struct_field, ','),
      optional(','),
    )),

    make_struct_field: $ => seq(
      field('name', $.identifier),
      field('operator', choice('=', '<-', ':=')),
      field('value', $._expression),
    ),

    // ========================================================================
    // Type declarations
    // ========================================================================

    _type: $ => choice(
      $.basic_type,
      $.named_type,
      $.auto_type,
      $.pointer_type,
      $.smart_pointer_type,
      $.array_type,
      $.table_type,
      $.iterator_type,
      $.block_type,
      $.function_type,
      $.lambda_type,
      $.tuple_type,
      $.variant_type,
      $.bitfield_type,
      $.typedecl_type,
      $.option_type,
      $._type_modifier,
    ),

    basic_type: $ => choice(...basic_types),

    named_type: $ => $._name_in_namespace,

    auto_type: $ => prec.left(seq(
      'auto',
      optional(seq('(', $.identifier, ')')),
    )),

    pointer_type: $ => prec.left(seq(
      $._type,
      choice('?', '??'),
    )),

    smart_pointer_type: $ => seq(
      'smart_ptr',
      '<',
      $._type,
      '>',
    ),

    array_type: $ => seq(
      'array',
      '<',
      $._type,
      '>',
    ),

    table_type: $ => seq(
      'table',
      '<',
      $._type,
      optional(seq(choice(',', ';'), $._type)),
      '>',
    ),

    iterator_type: $ => seq(
      'iterator',
      '<',
      $._type,
      '>',
    ),

    block_type: $ => choice(
      'block',
      seq('block', '<', choice(
        $._type,
        seq(optional($.function_argument_list), optional($.function_return_type)),
      ), '>'),
    ),

    function_type: $ => choice(
      'function',
      seq('function', '<', choice(
        $._type,
        seq(optional($.function_argument_list), optional($.function_return_type)),
      ), '>'),
    ),

    lambda_type: $ => choice(
      'lambda',
      seq('lambda', '<', choice(
        $._type,
        seq(optional($.function_argument_list), optional($.function_return_type)),
      ), '>'),
    ),

    tuple_type: $ => seq(
      'tuple',
      '<',
      sep1(choice(
        seq($.identifier, ':', $._type),  // named: tuple<x : int; y : float>
        $._type,                           // unnamed: tuple<int; float>
      ), choice(',', ';')),
      '>',
    ),

    variant_type: $ => seq(
      'variant',
      '<',
      sep1(seq($.identifier, ':', $._type), choice(',', ';')),
      '>',
    ),

    bitfield_type: $ => seq(
      'bitfield',
      optional(seq(':', choice('uint8', 'uint16', 'uint', 'uint64'))),
      '<',
      optional(sep1($.identifier, choice(',', ';'))),
      '>',
    ),

    typedecl_type: $ => seq(
      'typedecl',
      '(',
      $._expression,
      ')',
    ),

    option_type: $ => prec.left(seq(
      $._type,
      '|',
      choice($._type, '#'),
    )),

    // Type modifiers (postfix)
    _type_modifier: $ => choice(
      $.const_type,
      $.ref_type,
      $.temp_type,
      $.implicit_type,
      $.explicit_type,
      $.dim_type,
      $.remove_modifier,
      $.explicit_const_type,
      $.explicit_ref_type,
    ),

    const_type: $ => prec.left(seq($._type, 'const')),
    ref_type: $ => prec.left(seq($._type, '&')),
    temp_type: $ => prec.left(seq($._type, '#')),
    implicit_type: $ => prec.left(seq($._type, 'implicit')),
    explicit_type: $ => prec.left(seq($._type, 'explicit')),
    dim_type: $ => prec.left(seq($._type, '[', optional($._expression), ']')),
    explicit_const_type: $ => prec.left(seq($._type, '==', 'const')),
    explicit_ref_type: $ => prec.left(seq($._type, '==', '&')),

    remove_modifier: $ => prec.left(choice(
      seq($._type, '-', 'const'),
      seq($._type, '-', '&'),
      seq($._type, '-', '#'),
      seq($._type, '-', '[', ']'),
    )),

    // ========================================================================
    // Literals
    // ========================================================================

    _literal: $ => choice(
      $.integer_literal,
      $.float_literal,
      $.float_trailing_dot,
      $.string_literal,
      $.boolean_literal,
      $.null_literal,
      $.character_literal,
    ),

    // Integer literals: 42, 42u, 42l, 42ul, 42u8, 0xFF, 0xFFu, etc.
    integer_literal: $ => token(choice(
      // Hex with suffix
      seq(/0[xX][0-9a-fA-F][0-9a-fA-F_]*/, optional(choice(
        /[uU][lL]/, /[lL]/, /[uU]8/, /[uU]/,
      ))),
      // Decimal with suffix
      seq(/[0-9][0-9_]*/, optional(choice(
        /[uU][lL]/, /[lL]/, /[uU]8/, /[uU]/,
      ))),
    )),

    // Float literals: 3.14, 3.14f, 3.14d, 3.14lf, 1e10, 1.5e-3
    // Note: `0.` without digits after dot requires a suffix (d/f/lf) to avoid ambiguity with `..`
    float_literal: $ => token(choice(
      seq(/[0-9][0-9_]*\.[0-9][0-9_]*/, optional(/[eE][+-]?[0-9]+/), optional(choice(/[dD]/, /[fF]/, /[lL][fF]/))),
      seq(/[0-9][0-9_]*[eE][+-]?[0-9]+/, optional(choice(/[dD]/, /[fF]/, /[lL][fF]/))),
      seq(/[0-9][0-9_]*\./, choice(/[dD]/, /[fF]/, /[lL][fF]/)),
      seq(/[0-9][0-9_]*/, choice(/[dD]/, /[lL][fF]/)),
    )),

    // Character literals: 'a', '\n', 'a'u, 'a'u8
    character_literal: $ => token(seq(
      "'",
      choice(
        /\\[btnfr\\']/,
        /[^'\\]/,
      ),
      "'",
      optional(choice(/[uU]8/, /[uU]/)),
    )),

    boolean_literal: $ => choice('true', 'false'),
    null_literal: $ => 'null',

    // ---- Strings ----

    // String literal — may contain interpolations: "hello {name}!"
    string_literal: $ => seq(
      '"',
      repeat(choice(
        $._string_content,
        $.escape_sequence,
        $.string_interpolation,
      )),
      '"',
    ),

    escape_sequence: $ => token.immediate(
      /\\([\\"{}'nrtbf0]|x[0-9a-fA-F]{2}|u[0-9a-fA-F]{4})/,
    ),

    string_interpolation: $ => seq(
      token.immediate('{'),
      $._expression,
      optional(seq(':', $.format_specifier)),
      '}',
    ),

    format_specifier: $ => /[^}]+/,

    // ========================================================================
    // Identifiers and names
    // ========================================================================

    // Identifiers can contain backticks
    identifier: $ => /[#a-zA-Z_][a-zA-Z0-9_`]*/,

    _name_in_namespace: $ => choice(
      $.identifier,
      $.scoped_identifier,
    ),

    scoped_identifier: $ => prec(1, choice(
      seq(field('scope', $.identifier), '::', field('name', $.identifier)),
      seq('::', field('name', $.identifier)),
    )),

    // ========================================================================
    // Comments
    // ========================================================================

    line_comment: $ => token(seq('//', /[^\n]*/)),

    block_comment: $ => token(seq(
      '/*',
      /[^*]*\*+([^/*][^*]*\*+)*/,
      '/',
    )),
  },
});

// Helper: separated by delimiter, 1 or more
function sep1(rule, delimiter) {
  return seq(rule, repeat(seq(delimiter, rule)));
}

/**
 * @file Daslang grammar for tree-sitter
 * @author Gaijin Entertainment
 * @author Anton Zinovyev <xog3@yandex.ru>
 * @license BSD-3-Clause
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

// constraint: follows the operator precedence of src/parser/ds2_parser.ypp, lowest first
const PREC = {
  ARROW_BODY: 1,
  TERNARY: 2,
  OR: 3,
  XOR: 4,
  AND: 5,
  BIT_OR: 6,
  BIT_XOR: 7,
  BIT_AND: 8,
  EQUALITY: 9,
  RELATIONAL: 10,
  SHIFT: 11,
  ADDITIVE: 12,
  MULTIPLICATIVE: 13,
  NULL_COALESCING: 14,
  UNARY: 15,
  IS_AS: 16,
  PIPE: 17,
  POSTFIX: 18,
  FIELD: 19,
  INDEX: 20,
};

// constraint: follows basic_type_declaration of the compiler parser
const BASIC_TYPES = [
  'bool', 'string', 'int', 'int8', 'int16', 'int64', 'int2', 'int3', 'int4', 'uint', 'uint8', 'uint16', 'uint64',
  'uint2', 'uint3', 'uint4', 'float', 'float2', 'float3', 'float4', 'float16', 'half2', 'half3', 'half4', 'half8',
  'short2', 'short3', 'short4', 'short8', 'ushort2', 'ushort3', 'ushort4', 'ushort8', 'byte2', 'byte3', 'byte4',
  'byte8', 'byte16', 'ubyte2', 'ubyte3', 'ubyte4', 'ubyte8', 'ubyte16', 'void', 'range', 'urange', 'range64',
  'urange64', 'double', 'bitfield',
];

// constraint: das_type_name of the compiler parser, the basic types that name a function or a method
const FUNCTION_TYPE_NAMES = BASIC_TYPES.filter(name => name !== 'void' && name !== 'bitfield');

// constraint: the compiler lexer (src/parser/ds2_lexer.lpp) reads each of these words as a keyword and never as a name
const KEYWORDS = [
  ...BASIC_TYPES,
  'module', 'public', 'private', 'shared', 'inscope', 'options', 'require', 'as', 'is', 'expect', 'type', 'in',
  'default', 'true', 'false', 'null', 'let', 'var', 'def', 'template', 'operator', 'auto', 'typedecl', 'smart_ptr',
  'array', 'fixed_array', 'table', 'iterator', 'block', 'function', 'lambda', 'tuple', 'variant', 'const', 'implicit',
  'explicit', 'capture', 'generator', 'struct', 'class', 'enum', 'static', 'override', 'sealed', 'abstract', 'new',
  'delete', 'cast', 'upcast', 'reinterpret', 'addr', 'deref', 'typeinfo', 'uninitialized', 'unsafe', 'for', 'while',
  'if', 'static_if', 'elif', 'static_elif', 'else', 'with', 'aka', 'assume', 'typedef', 'try', 'recover', 'label',
  'goto', 'return', 'yield', 'break', 'continue', 'pass', 'where',
];

const ASSIGNMENT_OPERATORS = ['&=', '|=', '^=', '&&=', '||=', '^^=', '+=', '-=', '*=', '/=', '%=', '<<=', '<<<='];

/**
 * @param {RuleOrLiteral} rule
 * @param {RuleOrLiteral} separator
 */
function sepBy1(rule, separator) {
  return seq(rule, repeat(seq(separator, rule)));
}

/**
 * @param {GrammarSymbols<string>} $
 * @param {RuleOrLiteral} rule
 */
function parens($, rule) {
  return seq(alias($._open_paren, '('), rule, alias($._close_paren, ')'));
}

/**
 * @param {GrammarSymbols<string>} $
 * @param {RuleOrLiteral} rule
 */
function brackets($, rule) {
  return seq(alias($._open_bracket, '['), rule, alias($._close_bracket, ']'));
}

/**
 * @param {GrammarSymbols<string>} $
 * @param {RuleOrLiteral} rule
 */
function angles($, rule) {
  return seq('<', rule, alias($._greater, '>'));
}

/**
 * The comma-separated expressions and move arguments of a call or a constructor, under the given field.
 *
 * @param {GrammarSymbols<string>} $
 * @param {string} name
 */
function expressionList($, name) {
  return field(name, $._expression_list);
}

/**
 * @param {GrammarSymbols<string>} $
 * @param {string} keyword
 */
function structure($, keyword) {
  return seq(
    optional($._annotation_list_line),
    keyword,
    optional('template'),
    optional(choice('private', 'public')),
    optional('sealed'),
    field('name', $.identifier),
    optional(seq(':', field('parent', $._name_in_namespace))),
    optional($._newline_semicolons),
    choice(
      ';',
      seq(alias($._brace_open, '{'), repeat($._structure_member), alias($._close_brace, '}')),
    ),
  );
}

/**
 * Entries that one or more separators divide, with separators before, between, and after them.
 *
 * @param {RuleOrLiteral} entry
 * @param {RuleOrLiteral} separators
 */
function separatedEntries(entry, separators) {
  return seq(optional(separators), optional(seq(entry, repeat(seq(separators, entry)), optional(separators))));
}

export default grammar({
  name: 'daslang',

  externals: $ => [
    $._newline_semicolon,
    $._newline_comma,
    $._open_paren,
    $._close_paren,
    $._open_bracket,
    $._close_bracket,
    $._safe_open_bracket,
    $._not_open_bracket,
    $._not_safe_open_bracket,
    $._block_open,
    $._list_open,
    $._table_open,
    $._brace_open,
    $._close_brace,
    $._interpolation_open,
    $._interpolation_close,
    $._string_content,
    $.escape_sequence,
    $._format_string,
    $._reader_body,
    $._keyword_start,
    $._keyword_end,
    $._at_field,
    $._tag_e,
    $._tag_i,
    $._tag_v,
    $._tag_b,
    $._tag_a,
    $._tag_t,
    $._tag_c,
    $._tag_f,
    $._not_is,
    $._not_as,
    $._not_safe_as,
    $._map_to,
    $._left_arrow,
    $._greater,
    $._greater_equal,
    $._shift_right,
    $._shift_right_assign,
    $._rotate_right,
    $._rotate_right_assign,
    $._finally,
    $._integer,
    $._unsigned_integer,
    $._long_integer,
    $._long_integer_min,
    $._unsigned_long_integer,
    $._unsigned_int8,
    $._float,
    $._double,
    $._float16,
    $.block_comment,
    $.include_directive,
    $._line_end,
    $._lexer_error,
    $._error_sentinel,
  ],

  extras: $ => [
    /[ \t\r\n]/,
    /\\[ \t\r]*\n/,
    $.comment,
    $.block_comment,
    $.line_directive,
    $.include_directive,
    $._line_end,
  ],

  word: $ => $.identifier,

  reserved: {
    global: _ => KEYWORDS,
  },

  supertypes: $ => [$._expression, $._type],

  inline: $ => [$._expression_list, $._make_struct_fields, $._named_fields, $._type_macro_arguments],

  rules: {
    source_file: $ => repeat($._top_level_item),

    _top_level_item: $ => choice(
      $.module_declaration,
      $.expect_declaration,
      seq($.require_declaration, $._semicolon),
      seq($.options_declaration, $._semicolon),
      $.function_declaration,
      seq(alias($.single_global_let, $.global_let), $._semicolon),
      alias($.list_global_let, $.global_let),
      $.struct_declaration,
      $.class_declaration,
      $.enumeration_declaration,
      seq($.typedef_declaration, $._semicolon),
      $.tuple_alias_declaration,
      $.variant_alias_declaration,
      $.bitfield_alias_declaration,
      seq($.reader, $._semicolon),
      $._semicolon,
    ),

    _semicolon: $ => choice(';', $._newline_semicolon),

    _newline_semicolons: $ => repeat1($._newline_semicolon),

    _comma_or_semicolon: $ => choice(',', $._newline_comma, $._semicolon),

    // Directives

    module_declaration: $ => seq(
      'module',
      field('name', choice($.identifier, '$')),
      optional(field('shared', 'shared')),
      optional(field('visibility', choice('public', 'private'))),
      optional(field('visible_everywhere', seq('!', 'inscope'))),
    ),

    options_declaration: $ => seq('options', sepBy1($.annotation_argument, ',')),

    require_declaration: $ => seq(
      'require',
      optional(field('guard', seq('?', $.module_path))),
      choice(
        seq(field('name', $.module_path), optional(seq('as', field('alias', $.identifier)))),
        brackets($, field('group', $.identifier)),
      ),
      optional(field('public', 'public')),
    ),

    // constraint: the compiler continues a path at each `.` or `/` after a name, also the path of a require guard
    module_path: $ => prec.right(seq(
      repeat(choice('%', seq(choice('.', '..', '%'), '/'))),
      sepBy1($.identifier, choice('.', '/')),
    )),

    expect_declaration: $ => seq('expect', sepBy1($.expected_error, ',')),

    expected_error: $ => seq(
      field('code', alias($._integer, $.constant_integer)),
      optional(seq(':', field('count', alias($._integer, $.constant_integer)))),
    ),

    annotation_argument: $ => seq(
      field('name', choice($.identifier, 'type', 'in', 'default')),
      optional(seq(
        '=',
        field('value', choice(
          $._annotation_argument_value,
          seq('@@', $.identifier),
          parens($, sepBy1($._annotation_argument_value, ',')),
        )),
      )),
    ),

    _annotation_argument_value: $ => choice(
      $.constant_string,
      $.identifier,
      alias($._integer, $.constant_integer),
      alias($._long_integer, $.constant_integer64),
      alias($._unsigned_long_integer, $.constant_unsigned_integer64),
      alias($._float, $.constant_float),
      $.constant_boolean,
      seq('-', choice(
        alias($._integer, $.constant_integer),
        alias($._long_integer, $.constant_integer64),
        alias($._long_integer_min, $.constant_integer64),
        alias($._float, $.constant_float),
      )),
    ),

    _metadata: $ => seq(
      alias($._at_field, '@'),
      field('annotation', $.annotation_argument),
      optional($._newline_semicolons),
    ),

    // Global variables

    single_global_let: $ => seq(
      $._global_let_keywords,
      field('variables', alias($.global_variable_declaration, $.variable_declaration)),
    ),

    list_global_let: $ => seq(
      $._global_let_keywords,
      alias($._brace_open, '{'),
      repeat(choice(
        $._semicolon,
        seq(field('variables', alias($.annotated_variable_declaration, $.variable_declaration)), $._semicolon),
      )),
      alias($._close_brace, '}'),
    ),

    _global_let_keywords: $ => seq(choice('let', 'var'), optional('shared'), optional(choice('public', 'private'))),

    global_variable_declaration: $ => seq(
      repeat($._metadata),
      sepBy1(field('name', $.identifier), ','),
      $._variable_tail,
    ),

    let_variable_declaration: $ => seq(sepBy1($._variable_name, ','), $._variable_tail),

    annotated_variable_declaration: $ => seq(repeat($._metadata), sepBy1($._variable_name, ','), $._variable_tail),

    _variable_tail: $ => choice(
      seq(':', field('type', $._type_no_options), optional(seq($._copy_move_or_clone, field('init', $._expression)))),
      seq(optional('&'), $._copy_move_or_clone, field('init', $._expression)),
    ),

    _variable_name: $ => choice(
      field('name', $.identifier),
      seq(field('name', $.identifier), 'aka', field('aka', $.identifier)),
      field('name', $._name_tag),
    ),

    _copy_or_move: $ => choice('=', alias($._left_arrow, '<-')),

    _copy_move_or_clone: $ => choice('=', alias($._left_arrow, '<-'), ':='),

    // Types and aliases

    struct_declaration: $ => structure($, 'struct'),

    class_declaration: $ => structure($, 'class'),

    _structure_member: $ => choice(
      $._newline_semicolon,
      seq(field('aliases', alias($.structure_typedef, $.typedef_declaration)), $._semicolon),
      seq(field('fields', $.field_declaration), $._semicolon),
      field('methods', alias($.abstract_method, $.function_declaration)),
      field('methods', alias($.method, $.function_declaration)),
    ),

    structure_typedef: $ => seq('typedef', field('name', $.identifier), '=', field('type', $._type)),

    field_declaration: $ => seq(
      repeat($._metadata),
      optional('static'),
      optional(choice('override', 'sealed')),
      optional(choice('public', 'private')),
      sepBy1($._variable_name, ','),
      choice(
        seq(':', field('type', $._type), optional(seq($._copy_or_move, field('init', $._expression)))),
        optional(choice('&', seq($._copy_or_move, field('init', $._expression)))),
      ),
    ),

    abstract_method: $ => seq(
      optional($._annotation_list_line),
      'def',
      optional(choice('public', 'private')),
      'abstract',
      optional('const'),
      $._function_header,
      $._semicolon,
    ),

    method: $ => seq(
      optional($._annotation_list_line),
      'def',
      optional(choice('public', 'private')),
      optional('static'),
      optional(choice('override', 'sealed')),
      optional('const'),
      $._function_header,
      optional($._newline_semicolons),
      $._function_body,
    ),

    enumeration_declaration: $ => seq(
      optional($._annotation_list_line),
      'enum',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional(seq(':', field('base_type', alias(
        choice('int', 'int8', 'int16', 'uint', 'uint8', 'uint16', 'int64', 'uint64'),
        $.basic_type,
      )))),
      optional($._newline_semicolons),
      alias($._list_open, '{'),
      separatedEntries(field('list', $.enumeration_entry), $._commas),
      alias($._close_brace, '}'),
    ),

    _commas: $ => repeat1(choice(',', $._newline_comma)),

    _semicolons: $ => repeat1($._semicolon),

    enumeration_entry: $ => seq(field('name', $.identifier), optional(seq('=', field('value', $._expression)))),

    typedef_declaration: $ => seq(
      'typedef',
      optional(choice('public', 'private')),
      optional(field('kind', $.identifier)),
      field('name', $.identifier),
      '=',
      field('type', $._type),
    ),

    tuple_alias_declaration: $ => seq(
      'tuple',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional($._newline_semicolons),
      alias($._brace_open, '{'),
      separatedEntries($._tuple_alias_entry, $._semicolons),
      alias($._close_brace, '}'),
    ),

    _tuple_alias_entry: $ => choice(
      field('argument_types', $._type),
      seq(field('argument_names', $.identifier), ':', field('argument_types', $._type)),
    ),

    variant_alias_declaration: $ => seq(
      'variant',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional($._newline_semicolons),
      alias($._brace_open, '{'),
      separatedEntries($._variant_alias_entry, $._semicolons),
      alias($._close_brace, '}'),
    ),

    _variant_alias_entry: $ => seq(field('argument_names', $.identifier), ':', field('argument_types', $._type)),

    bitfield_alias_declaration: $ => seq(
      'bitfield',
      optional(choice('public', 'private')),
      field('name', $.identifier),
      optional(seq(':', alias(choice('uint8', 'uint16', 'uint', 'uint64'), $.basic_type))),
      optional($._newline_semicolons),
      alias($._list_open, '{'),
      separatedEntries(field('argument_names', $.bitfield_entry), $._commas),
      alias($._close_brace, '}'),
    ),

    bitfield_entry: $ => seq(field('name', $.identifier), optional(seq('=', field('value', $._expression)))),

    // Functions

    function_declaration: $ => seq(
      optional($._annotation_list_line),
      'def',
      optional('template'),
      optional(choice('private', 'public')),
      $._function_header,
      optional($._newline_semicolons),
      $._function_body,
    ),

    _annotation_list_line: $ => seq(field('annotations', $.annotation_list), optional($._newline_semicolons)),

    _function_header: $ => seq(
      field('name', choice($.identifier, $.operator_name, alias(choice(...FUNCTION_TYPE_NAMES), $.basic_type))),
      optional($._argument_list),
      optional($._return_type),
    ),

    _function_body: $ => choice(
      field('body', $.block_expression),
      prec(PREC.ARROW_BODY, seq(alias($._map_to, '=>'), optional(alias($._left_arrow, '<-')), field('body', $._operand))),
    ),

    operator_name: $ => seq(
      choice(
        seq(choice('++', '--'), 'operator'),
        seq('operator', choice(
          '!', '=', alias($._left_arrow, '<-'), '~', ...ASSIGNMENT_OPERATORS, alias($._shift_right_assign, '>>='),
          alias($._rotate_right_assign, '>>>='), '&&', '||', '^^', '+', '-', '*', '/', '%', '<',
          alias($._greater, '>'), '..', '==', '!=', '<=', alias($._greater_equal, '>='), '&', '|', '^', '++', '--',
          '<<', alias($._shift_right, '>>'), '<<<', alias($._rotate_right, '>>>'), ':=', 'delete', '??', '.', '?.',
          seq(alias($._open_bracket, '['), alias($._close_bracket, ']'), optional(choice(
            '=', alias($._left_arrow, '<-'), ':=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '&&=', '||=',
            '^^=',
          ))),
          seq(alias($._safe_open_bracket, '?['), alias($._close_bracket, ']')),
          seq('.', field('field', $.identifier), optional(choice(':=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=',
            '&&=', '||=', '^^='))),
          seq('?.', field('field', $.identifier)),
          seq(choice('is', 'as', seq('?', 'as')), optional(field('field', choice(
            $.identifier,
            alias(choice(...FUNCTION_TYPE_NAMES), $.basic_type),
          )))),
        )),
      ),
    ),

    _argument_list: $ => parens($, optional($._arguments)),

    _arguments: $ => choice(
      field('arguments', alias($.untyped_argument, $.variable_declaration)),
      field('arguments', alias($.typed_argument, $.variable_declaration)),
      seq(field('arguments', alias($.untyped_argument, $.variable_declaration)), ';', $._arguments),
      seq(field('arguments', alias($.typed_argument, $.variable_declaration)), choice(';', ','), $._arguments),
    ),

    untyped_argument: $ => seq(
      repeat($._metadata),
      optional(choice('let', 'var')),
      sepBy1($._variable_name, ','),
      optional(choice('&', seq($._copy_or_move, field('init', $._expression)))),
    ),

    typed_argument: $ => choice(
      seq(
        repeat($._metadata),
        optional(choice('let', 'var')),
        sepBy1($._variable_name, ','),
        ':',
        field('type', $._type),
        optional(seq($._copy_or_move, field('init', $._expression))),
      ),
      field('name', alias($.argument_tag, $.tag)),
    ),

    _return_type: $ => seq(choice(':', '->'), field('result', $._type)),

    annotation_list: $ => brackets($, sepBy1($._annotation, ',')),

    _annotation: $ => choice(
      $.annotation_declaration,
      alias($.annotation_operation, $.annotation_declaration),
      parens($, $._annotation),
      prec(PREC.PIPE, seq('|>', $._annotation)),
    ),

    annotation_declaration: $ => seq(
      field('name', choice($._name_in_namespace, alias(choice('require', 'private', 'template'), $.identifier))),
      optional(parens($, sepBy1(field('arguments', $.annotation_argument), ','))),
    ),

    annotation_operation: $ => choice(
      prec(PREC.UNARY, seq(field('operator', '!'), field('arguments', $._annotation))),
      prec.left(PREC.AND, seq(field('arguments', $._annotation), field('operator', '&&'), field('arguments', $._annotation))),
      prec.left(PREC.XOR, seq(field('arguments', $._annotation), field('operator', '^^'), field('arguments', $._annotation))),
      prec.left(PREC.OR, seq(field('arguments', $._annotation), field('operator', '||'), field('arguments', $._annotation))),
    ),

    // Statements

    block_expression: $ => seq(
      alias($._block_open, '{'),
      repeat($._statement),
      alias($._close_brace, '}'),
      optional($.finally_block),
    ),

    finally_block: $ => seq(
      alias($._finally, 'finally'),
      alias($._block_open, '{'),
      repeat($._statement),
      alias($._close_brace, '}'),
    ),

    _statement: $ => choice(
      $._semicolon,
      seq($._expression_statement, $._semicolon),
      seq($.delete_expression, $._semicolon),
      $._let_statement,
      seq($._keyword_start, $.while_expression, $._keyword_end),
      $.unsafe_expression,
      seq($._keyword_start, $.with_expression, $._keyword_end),
      seq(choice($.assume_expression, $.local_type_alias), $._semicolon),
      seq($._keyword_start, $.for_expression, $._keyword_end),
      seq(choice($.break_expression, $.continue_expression, $.return_expression, $.yield_expression), $._semicolon),
      seq($._keyword_start, alias($.braced_if_then_else, $.if_then_else), $._keyword_end),
      seq($._keyword_start, alias($.short_if_then_else, $.if_then_else)),
      alias($.postfix_if_then_else, $.if_then_else),
      $.try_catch,
      seq(choice($.label_expression, $.goto_expression), $._semicolon),
      seq('pass', $._semicolon),
      $.block_expression,
    ),

    _expression_statement: $ => choice(
      $._expression_no_bracket,
      $.copy,
      $.move,
      $.clone,
      alias($.assignment_operation, $.binary_operation),
    ),

    copy: $ => seq(
      field('left', $._expression_no_bracket),
      choice('=', '!=='),
      field('right', $._expression_no_bracket),
    ),

    move: $ => seq(
      field('left', $._expression_no_bracket),
      choice(alias($._left_arrow, '<-'), '!<-'),
      field('right', choice($._expression_no_bracket, $.make_table, $.array_comprehension)),
    ),

    clone: $ => seq(
      field('left', $._expression_no_bracket),
      choice(':=', '!:='),
      field('right', $._expression_no_bracket),
    ),

    assignment_operation: $ => seq(
      field('left', $._expression_no_bracket),
      field('operator', choice(
        ...ASSIGNMENT_OPERATORS,
        alias($._shift_right_assign, '>>='),
        alias($._rotate_right_assign, '>>>='),
      )),
      field('right', $._expression_no_bracket),
    ),

    delete_expression: $ => seq('delete', optional('explicit'), field('subexpression', $._expression)),

    _let_statement: $ => choice(
      seq(alias($.single_let, $.let_expression), $._semicolon),
      alias($.list_let, $.let_expression),
    ),

    single_let: $ => seq(
      choice('let', 'var'),
      optional('inscope'),
      choice(
        field('variables', alias($.annotated_variable_declaration, $.variable_declaration)),
        field('variables', alias($.tuple_variable_declaration, $.variable_declaration)),
      ),
    ),

    list_let: $ => seq(
      choice('let', 'var'),
      optional('inscope'),
      alias($._brace_open, '{'),
      repeat(choice(
        $._semicolon,
        seq(field('variables', alias($.let_variable_declaration, $.variable_declaration)), $._semicolon),
      )),
      alias($._close_brace, '}'),
    ),

    tuple_variable_declaration: $ => seq(
      field('name', $.tuple_expansion),
      choice(
        seq(':', field('type', $._type_no_options), $._copy_move_or_clone, field('init', $._expression)),
        seq(optional('&'), $._copy_move_or_clone, field('init', $._expression)),
      ),
    ),

    tuple_expansion: $ => parens($, sepBy1($.identifier, ',')),

    while_expression: $ => seq(
      'while',
      optional($._loop_annotations),
      parens($, field('condition', $._expression)),
      optional($._newline_semicolons),
      field('body', $.block_expression),
    ),

    _loop_annotations: $ => choice(
      brackets($, sepBy1(field('annotations', $.annotation_argument), ',')),
      repeat1(seq(alias($._at_field, '@'), field('annotations', $.annotation_argument), optional($._newline_semicolons))),
    ),

    for_expression: $ => seq(
      'for',
      optional($._loop_annotations),
      parens($, seq($._iterators, 'in', $._sources)),
      optional($._newline_semicolons),
      field('body', $.block_expression),
    ),

    _iterators: $ => sepBy1(choice(
      field('iterators', $.identifier),
      seq(field('iterators', $.identifier), 'aka', field('iterators_aka', $.identifier)),
      field('iterators', $._name_tag),
      field('iterators', $.tuple_expansion),
    ), ','),

    _sources: $ => sepBy1(field('sources', $._list_element), ','),

    with_expression: $ => seq(
      'with',
      parens($, choice(
        field('with', $._expression),
        seq('module', field('module_name', $.module_path)),
      )),
      optional($._newline_semicolons),
      field('body', $.block_expression),
    ),

    unsafe_expression: $ => seq('unsafe', optional($._newline_semicolons), field('body', $.block_expression)),

    try_catch: $ => seq('try', field('try_block', $.block_expression), 'recover', field('catch_block', $.block_expression)),

    assume_expression: $ => seq('assume', field('alias', $.identifier), '=', field('subexpression', $._expression)),

    local_type_alias: $ => seq('typedef', field('alias', $.identifier), '=', field('assume_type', $._type)),

    break_expression: _ => 'break',

    continue_expression: _ => 'continue',

    return_expression: $ => seq(
      'return',
      optional(seq(optional(alias($._left_arrow, '<-')), field('subexpression', $._expression))),
    ),

    yield_expression: $ => seq(
      'yield',
      optional(alias($._left_arrow, '<-')),
      field('subexpression', $._expression),
    ),

    label_expression: $ => seq('label', field('label_name', alias($._integer, $.constant_integer)), ':'),

    goto_expression: $ => seq(
      'goto',
      choice(
        seq('label', field('label_name', alias($._integer, $.constant_integer))),
        field('subexpression', $._expression),
      ),
    ),

    // An `if` statement whose bodies all have braces: the keyword flag of the compiler stays set until it ends.
    braced_if_then_else: $ => seq(
      choice('if', 'static_if'),
      $._if_condition,
      field('if_true', $.block_expression),
      optional($._braced_else),
    ),

    braced_elif: $ => seq(
      choice('elif', 'static_elif'),
      $._if_condition,
      field('if_true', $.block_expression),
      optional($._braced_else),
    ),

    _braced_else: $ => choice(
      seq('else', optional($._newline_semicolons), field('if_false', $.block_expression)),
      field('if_false', alias($.braced_elif, $.if_then_else)),
    ),

    // An `if` statement with a one-line body: the keyword flag of the compiler ends where the first one starts.
    short_if_then_else: $ => seq(
      choice('if', 'static_if'),
      $._if_condition,
      $._short_if_branches,
    ),

    short_elif: $ => seq(
      choice('elif', 'static_elif'),
      $._if_condition,
      $._short_if_branches,
    ),

    _short_if_branches: $ => choice(
      seq($._keyword_end, field('if_true', $._one_liner), $._semicolon, optional($._plain_else)),
      seq(field('if_true', $.block_expression), choice(
        seq('else', optional($._newline_semicolons), $._keyword_end, field('if_false', $._one_liner), $._semicolon),
        field('if_false', alias($.short_elif, $.if_then_else)),
      )),
    ),

    _plain_else: $ => choice(
      seq('else', optional($._newline_semicolons), choice(
        field('if_false', $.block_expression),
        seq(field('if_false', $._one_liner), $._semicolon),
      )),
      field('if_false', alias($.plain_elif, $.if_then_else)),
    ),

    plain_elif: $ => seq(
      choice('elif', 'static_elif'),
      $._if_condition,
      choice(
        field('if_true', $.block_expression),
        seq(field('if_true', $._one_liner), $._semicolon),
      ),
      optional($._plain_else),
    ),

    _if_condition: $ => seq(parens($, field('condition', $._expression)), optional($._newline_semicolons)),

    postfix_if_then_else: $ => seq(
      field('if_true', $._one_liner),
      'if',
      parens($, field('condition', $._expression)),
      optional(seq('else', field('if_false', $._one_liner))),
      $._semicolon,
    ),

    _one_liner: $ => choice(
      $._expression_no_bracket,
      $.return_expression,
      $.yield_expression,
      $.break_expression,
      $.continue_expression,
    ),

    // Expressions

    _expression: $ => choice(
      $._expression_no_bracket,
      $.make_table,
      $.array_comprehension,
    ),

    _expression_no_bracket: $ => choice(
      $._operand,
      $.interval,
      alias($.map_tuple, $.make_tuple),
    ),

    interval: $ => seq(field('arguments', $._operand), '..', field('arguments', $._operand)),

    map_tuple: $ => seq(
      field('values', $._operand),
      alias($._map_to, '=>'),
      field('values', choice($._operand, $.make_table, $.array_comprehension)),
    ),

    _operand: $ => choice(
      $.constant_pointer,
      $.constant_boolean,
      $._number,
      $.constant_string,
      $.string_builder,
      $.reader,
      $.inline_reader,
      $.variable,
      $.parenthesized_expression,
      $.make_tuple,
      alias($.tuple_call, $.make_tuple),
      $.make_struct,
      $.make_variant,
      $.make_array,
      alias($.bracket_comprehension, $.array_comprehension),
      alias($.table_call, $.make_table),
      $.call,
      $.named_call,
      alias($.method_named_call, $.named_call),
      $.invoke,
      $.field,
      $.safe_field,
      $.at,
      $.safe_at,
      $.unary_operation,
      $.binary_operation,
      $.ternary_operation,
      $.null_coalescing,
      $.is_expression,
      $.is_variant,
      $.as_variant,
      $.safe_as_variant,
      $.pipe,
      $.pointer_to_reference,
      $.reference_to_pointer,
      $.address,
      $.cast_expression,
      $.type_info,
      $.type_declaration,
      $.new_expression,
      $.ascend,
      $.make_block,
      $.make_generator,
      $.unsafe_call,
      $.tag,
    ),

    constant_pointer: _ => 'null',

    constant_boolean: _ => choice('true', 'false'),

    _number: $ => choice(
      alias($._integer, $.constant_integer),
      alias($._unsigned_integer, $.constant_unsigned_integer),
      alias($._long_integer, $.constant_integer64),
      alias($.negative_integer64_minimum, $.constant_integer64),
      alias($._unsigned_long_integer, $.constant_unsigned_integer64),
      alias($._unsigned_int8, $.constant_unsigned_integer8),
      alias($._float, $.constant_float),
      alias($._float16, $.constant_float16),
      alias($._double, $.constant_double),
    ),

    negative_integer64_minimum: $ => seq('-', $._long_integer_min),

    _string_text: $ => repeat1(choice($._string_content, $.escape_sequence)),

    constant_string: $ => seq('"', optional($._string_text), '"'),

    string_builder: $ => seq(
      '"',
      optional($._string_text),
      $.interpolation,
      repeat(choice($._string_content, $.escape_sequence, $.interpolation)),
      '"',
    ),

    interpolation: $ => seq(
      alias($._interpolation_open, '{'),
      $._expression,
      optional(seq(':', optional(alias($._format_string, $.format_specifier)))),
      alias($._interpolation_close, '}'),
    ),

    reader: $ => seq('%', field('macro', $._name_in_namespace), '~', field('sequence', alias($._reader_body, $.reader_text))),

    // constraint: the compiler lexer reads `%name!`, the text, and the first `%%` as one token, and parses the text that
    // the macro returns in its place
    inline_reader: _ => token(seq('%', /[_a-zA-Z][_a-zA-Z0-9`]*/, '!', /([^%]|%[^%])*/, '%%')),

    variable: $ => field('name', $._name_in_namespace),

    _name_in_namespace: $ => choice($.identifier, $.qualified_name),

    qualified_name: $ => seq(optional(field('module', $.identifier)), '::', field('name', $.identifier)),

    parenthesized_expression: $ => parens($, $._list_element),

    make_tuple: $ => choice(
      parens($, seq(
        field('values', $._list_element),
        choice(',', seq(repeat1(seq(',', field('values', $._list_element))), optional(','))),
      )),
      parens($, seq(sepBy1(field('values', alias($.make_field, $.make_field_declaration)), ','), optional(','))),
    ),

    tuple_call: $ => choice(
      seq('tuple', parens($, seq(sepBy1(field('values', $._list_element), ','), optional(',')))),
      seq('tuple', angles($, $._tuple_type_list), $._make_struct_arguments),
    ),

    _list_element: $ => choice($._expression, $.move_argument),

    _expression_list: $ => sepBy1($._list_element, ','),

    move_argument: $ => seq(alias($._left_arrow, '<-'), field('arguments', $._expression)),


    make_field: $ => seq(
      field('name', choice($.identifier, $._field_tag)),
      choice($._copy_or_move, ':='),
      field('value', $._expression),
    ),

    _make_struct_fields: $ => sepBy1(field('structs', alias($.make_field, $.make_field_declaration)), ','),

    _make_struct_arguments: $ => parens($, seq(
      optional('uninitialized'),
      optional(choice(
        $._make_struct_fields,
        seq(sepBy1(parens($, $._make_struct_fields), ','), optional(',')),
      )),
    )),

    make_struct: $ => choice($._typed_make_struct, $._named_make_struct),

    typed_make_struct: $ => $._typed_make_struct,

    named_make_struct: $ => $._named_make_struct,

    _named_make_struct: $ => seq(
      field('make_type', alias($._name_in_namespace, $.structure_type)),
      parens($, choice('uninitialized', seq(optional('uninitialized'), $._make_struct_fields, optional(',')))),
    ),

    _typed_make_struct: $ => choice(
      seq(choice('struct', 'class'), angles($, field('make_type', $._type_no_options)), $._make_struct_arguments),
      seq('default', angles($, field('make_type', $._type_no_options)), optional('uninitialized')),
    ),

    make_variant: $ => seq(
      'variant',
      choice(angles($, $._variant_type_list), seq('type', angles($, field('make_type', $._type_no_options)))),
      parens($, seq(optional('uninitialized'), optional($._make_struct_fields))),
    ),

    make_array: $ => choice(
      brackets($, optional(seq(expressionList($, 'values'), optional(',')))),
      seq('array', 'struct', angles($, field('make_type', $._type_no_options)), $._make_struct_arguments),
      seq('array', 'tuple', angles($, $._tuple_type_list), $._make_struct_arguments),
      seq('array', 'variant', angles($, $._variant_type_list), parens($, optional($._make_struct_fields))),
      seq('array', parens($, seq(expressionList($, 'values'), optional(',')))),
      seq('array', angles($, field('make_type', $._type_no_options)), parens($, optional(seq(expressionList($, 'values'), optional(','))))),
      seq('fixed_array', parens($, seq(expressionList($, 'values'), optional(',')))),
      seq('fixed_array', angles($, field('make_type', $._type_no_options)), parens($, seq(expressionList($, 'values'), optional(',')))),
    ),

    make_table: $ => seq(
      alias($._table_open, '{'),
      optional($._newline_semicolons),
      optional(seq(sepBy1(field('values', $._expression), ','), optional(','))),
      alias($._close_brace, '}'),
    ),

    table_call: $ => choice(
      seq('table', parens($, seq(sepBy1(field('values', $._expression), ','), optional(',')))),
      seq(
        'table',
        '<',
        field('make_type', $._type_no_options),
        optional(seq($._comma_or_semicolon, field('make_type', $._type_no_options))),
        alias($._greater, '>'),
        parens($, optional(seq(sepBy1(field('values', $._expression), ','), optional(',')))),
      ),
    ),

    _comprehension: $ => seq(
      'for',
      parens($, seq($._iterators, 'in', $._sources)),
      ';',
      field('subexpression', $._expression),
      optional(seq(';', 'where', field('expression_where', $._expression))),
    ),

    bracket_comprehension: $ => brackets($, seq(optional('iterator'), $._comprehension)),

    array_comprehension: $ => seq(
      alias($._table_open, '{'),
      optional($._newline_semicolons),
      $._comprehension,
      alias($._close_brace, '}'),
    ),

    call: $ => choice(
      seq(field('name', $._name_in_namespace), $._call_arguments),
      seq(field('name', $.basic_type), $._call_arguments),
    ),

    _call_arguments: $ => parens($, optional(expressionList($, 'arguments'))),

    named_call: $ => seq(field('name', $._name_in_namespace), $._named_arguments),

    _named_arguments: $ => parens($, choice(
      brackets($, $._named_fields),
      seq(expressionList($, 'non_named_arguments'), ',', brackets($, $._named_fields)),
      seq(expressionList($, 'non_named_arguments'), ',', $._named_fields),
    )),

    _named_fields: $ => sepBy1(field('arguments', alias($.make_field, $.make_field_declaration)), ','),

    invoke: $ => prec.left(PREC.FIELD, choice(
      seq(field('value', $._operand), choice('.', '!.', '->'), field('name', $.identifier), $._call_arguments),
      seq(field('value', $._operand), '.', field('name', $.basic_type), $._call_arguments),
    )),

    method_named_call: $ => prec.left(PREC.FIELD, seq(
      field('value', $._operand),
      choice('.', '->'),
      field('name', $.identifier),
      choice(
        parens($, brackets($, $._named_fields)),
        parens($, seq(expressionList($, 'non_named_arguments'), ',', $._named_fields)),
        parens($, $._named_fields),
      ),
    )),

    field: $ => prec.left(PREC.FIELD, choice(
      seq(field('value', $._operand), choice('.', '!.', seq('.', '.')), field('name', choice($.identifier, $._field_tag))),
      seq(field('value', $._operand), $._dot_without_name),
    )),

    // constraint: the compiler parser recovers from a `.` without a name through an error rule that reports nothing,
    // and takes the name after a `.` wherever one follows
    _dot_without_name: _ => prec(-1, '.'),

    safe_field: $ => prec.left(PREC.FIELD, seq(
      field('value', $._operand),
      choice('?.', seq('.', '?.'), '!?.'),
      field('name', choice($.identifier, $._field_tag)),
    )),

    at: $ => prec.left(PREC.INDEX, seq(
      field('subexpression', $._operand),
      choice(seq(optional('.'), alias($._open_bracket, '[')), alias($._not_open_bracket, '![')),
      field('index', $._expression),
      alias($._close_bracket, ']'),
    )),

    safe_at: $ => prec.left(PREC.INDEX, seq(
      field('subexpression', $._operand),
      choice(seq(optional('.'), alias($._safe_open_bracket, '?[')), alias($._not_safe_open_bracket, '!?[')),
      field('index', $._expression),
      alias($._close_bracket, ']'),
    )),

    unary_operation: $ => choice(
      prec.right(PREC.UNARY, seq(field('operator', choice('!', '~', '+', '-', '++', '--')), field('subexpression', $._operand))),
      // constraint: the compiler parser shifts a postfix `++` or `--` at the precedence of the unary operators
      prec.right(PREC.UNARY, seq(field('subexpression', $._operand), field('operator', choice('++', '--')))),
    ),

    binary_operation: $ => {
      /** @type {[RuleOrLiteral, number][]} */
      const table = [
        ['||', PREC.OR],
        ['^^', PREC.XOR],
        ['&&', PREC.AND],
        ['|', PREC.BIT_OR],
        ['^', PREC.BIT_XOR],
        ['&', PREC.BIT_AND],
        [choice('==', '!='), PREC.EQUALITY],
        [choice('<', '<=', alias($._greater, '>'), alias($._greater_equal, '>=')), PREC.RELATIONAL],
        [choice('<<', '<<<', alias($._shift_right, '>>'), alias($._rotate_right, '>>>')), PREC.SHIFT],
        [choice('+', '-'), PREC.ADDITIVE],
        [choice('*', '/', '%'), PREC.MULTIPLICATIVE],
      ];
      return choice(...table.map(([operator, precedence]) => prec.left(precedence, seq(
        field('left', $._operand),
        field('operator', operator),
        field('right', $._operand),
      ))));
    },

    ternary_operation: $ => prec.right(PREC.TERNARY, seq(
      field('subexpression', $._operand),
      '?',
      field('left', $._expression_no_bracket),
      ':',
      field('right', $._operand),
    )),

    null_coalescing: $ => prec.right(PREC.NULL_COALESCING, seq(
      field('subexpression', $._operand),
      choice('??', '!??'),
      field('default_value', $._operand),
    )),

    is_expression: $ => prec.left(PREC.IS_AS, seq(
      field('subexpression', $._operand),
      choice('is', alias($._not_is, '!is')),
      field('type_expression', choice($.basic_type, seq('type', angles($, $._type_no_options)))),
    )),

    is_variant: $ => prec.left(PREC.IS_AS, seq(
      field('value', $._operand),
      choice('is', alias($._not_is, '!is')),
      field('name', choice($.identifier, $._field_tag)),
    )),

    as_variant: $ => prec.left(PREC.IS_AS, seq(
      field('value', $._operand),
      choice('as', alias($._not_as, '!as')),
      field('name', choice($.identifier, $.basic_type, seq('type', angles($, $._type)), $._field_tag)),
    )),

    safe_as_variant: $ => {
      const name = field('name', choice($.identifier, $.basic_type, seq('type', angles($, $._type)), $._field_tag));
      return choice(
        // constraint: the compiler parser shifts a `?` at the precedence of `? :`, also the `?` of `?as`
        seq(prec.right(PREC.TERNARY, seq(field('value', $._operand), '?')), 'as', prec.left(PREC.IS_AS, name)),
        prec.left(PREC.IS_AS, seq(field('value', $._operand), alias($._not_safe_as, '!?as'), name)),
      );
    },

    pipe: $ => choice(
      prec.left(PREC.PIPE, seq(field('function_call', $._operand), '<|', field('argument', $._operand))),
      prec.left(PREC.PIPE, seq(field('argument', $._operand), '|>', field('function_call', choice($._operand, $.basic_type)))),
      prec.left(PREC.PIPE, seq(
        field('function_call', choice(
          $.call,
          alias($.named_make_struct, $.make_struct),
          $.named_call,
          alias($.method_named_call, $.named_call),
          $.invoke,
          $.field,
        )),
        field('argument', alias($.piped_block, $.make_block)),
      )),
    ),

    piped_block: $ => choice(
      $._block_literal,
      field('body', alias($.plain_block, $.block_expression)),
    ),

    plain_block: $ => seq(alias($._brace_open, '{'), repeat($._statement), alias($._close_brace, '}')),

    pointer_to_reference: $ => choice(
      prec(PREC.POSTFIX, seq('*', field('subexpression', $._operand))),
      seq('deref', parens($, field('subexpression', $._expression))),
    ),

    reference_to_pointer: $ => seq('addr', parens($, field('subexpression', $._expression))),

    address: $ => seq(
      '@@',
      optional(angles($, choice(
        field('function_type', $._type_no_options),
        seq(optional($._argument_list), optional($._return_type)),
      ))),
      field('target', choice($._name_in_namespace, $._name_tag)),
    ),

    cast_expression: $ => choice(
      seq(
        choice('cast', 'upcast', 'reinterpret', 'addr'),
        angles($, field('cast_type', $._type_no_options)),
        parens($, field('subexpression', $._expression)),
      ),
    ),

    type_info: $ => seq(
      'typeinfo',
      field('trait', $._name_in_namespace),
      optional(angles($, seq(
        field('subtrait', $.identifier),
        optional(seq($._comma_or_semicolon, field('extratrait', $.identifier))),
      ))),
      parens($, field('subexpression', $._expression)),
    ),

    type_declaration: $ => seq('type', angles($, field('type_expression', $._type))),

    new_expression: $ => seq(
      'new',
      field('type_expression', choice(angles($, $._type), $.structure_type)),
      optional(parens($, optional(choice('uninitialized', expressionList($, 'arguments'))))),
    ),

    ascend: $ => seq('new', field('subexpression', choice(
      alias($.ascend_struct, $.make_struct),
      alias($.typed_make_struct, $.make_struct),
      $.make_variant,
      $.make_array,
      alias($.bracket_comprehension, $.array_comprehension),
      alias($.table_call, $.make_table),
      alias($.tuple_call, $.make_tuple),
      $.make_table,
      $.array_comprehension,
    ))),

    ascend_struct: $ => seq(
      field('make_type', choice(angles($, $._type), $.structure_type)),
      parens($, seq(optional('uninitialized'), $._make_struct_fields, optional(','))),
    ),

    make_block: $ => $._block_literal,

    _block_literal: $ => seq(
      choice('$', '@', '@@'),
      optional(field('annotations', $.annotation_list)),
      optional($._capture_list),
      optional($._argument_list),
      optional($._return_type),
      optional($._newline_semicolons),
      $._function_body,
    ),

    _capture_list: $ => seq('capture', parens($, sepBy1(field('capture', $.capture_entry), ','))),

    capture_entry: $ => choice(
      seq(choice('&', '=', alias($._left_arrow, '<-'), ':='), field('name', $.identifier)),
      seq(field('mode', $.identifier), parens($, field('name', $.identifier))),
    ),

    make_generator: $ => seq(
      'generator',
      '<',
      field('iterator_type', $._type_no_options),
      alias($._greater, '>'),
      optional($._capture_list),
      choice(
        parens($, optional(field('subexpression', $._expression))),
        seq(optional($._newline_semicolons), field('subexpression', $.block_expression)),
      ),
    ),

    unsafe_call: $ => seq('unsafe', parens($, field('subexpression', $._expression))),

    tag: $ => choice(
      seq(choice(alias($._tag_e, '$e'), alias($._tag_i, '$i'), alias($._tag_v, '$v'), alias($._tag_b, '$b'),
        alias($._tag_a, '$a')), parens($, field('subexpression', $._expression))),
      '...',
      seq(alias($._tag_c, '$c'), parens($, field('subexpression', $._expression)), $._call_arguments),
      seq('@@', alias($._tag_c, '$c'), parens($, field('subexpression', $._expression))),
    ),

    _name_tag: $ => alias($.name_tag, $.tag),

    name_tag: $ => seq(alias($._tag_i, '$i'), parens($, field('subexpression', $._expression))),

    _field_tag: $ => alias($.field_tag, $.tag),

    field_tag: $ => seq(alias($._tag_f, '$f'), parens($, field('subexpression', $._expression))),

    argument_tag: $ => seq(alias($._tag_a, '$a'), parens($, field('subexpression', $._expression))),

    // Types

    _type: $ => choice($._type_no_options, $.option_type),

    option_type: $ => seq(
      field('argument_types', $._type_no_options),
      repeat1(seq('|', field('argument_types', choice($._type_no_options, '#')))),
    ),

    _type_no_options: $ => choice($._type_no_dimension, $.fixed_array_type),

    fixed_array_type: $ => prec.left(seq(
      field('first_type', $._type_no_dimension),
      repeat1(brackets($, optional(field('fixed_dimension_expression', $._expression)))),
    )),

    _type_no_dimension: $ => choice(
      $.basic_type,
      $.auto_type,
      alias($.type_tag, $.tag),
      $.bitfield_type,
      $.structure_type,
      $.type_type,
      $.typedecl_type,
      $.type_macro,
      $.modified_type,
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
    ),

    basic_type: _ => choice(...BASIC_TYPES),

    auto_type: $ => seq('auto', optional(parens($, field('alias', $.identifier)))),

    type_tag: $ => seq(alias($._tag_t, '$t'), parens($, field('subexpression', $._expression))),

    bitfield_type: $ => seq(
      'bitfield',
      optional(seq(':', choice('uint8', 'uint16', 'uint', 'uint64'))),
      '<',
      optional(sepBy1(field('argument_names', $.identifier), choice(';', ','))),
      alias($._greater, '>'),
    ),

    structure_type: $ => $._name_in_namespace,

    type_type: $ => seq('type', angles($, field('first_type', $._type))),

    typedecl_type: $ => seq('typedecl', parens($, field('type_macro_expression', $._expression))),

    type_macro: $ => choice(
      seq(field('name', $._name_in_namespace), parens($, optional(seq($._type_macro_arguments, optional(','))))),
      seq('$', field('name', $._name_in_namespace), optional(parens($, seq($._type_macro_arguments, optional(','))))),
      seq(
        optional('$'),
        field('name', $._name_in_namespace),
        '<',
        sepBy1(field('type_macro_expression', $._type), $._comma_or_semicolon),
        alias($._greater, '>'),
        optional(parens($, seq($._type_macro_arguments, optional(',')))),
      ),
    ),

    _type_macro_arguments: $ => sepBy1(field('type_macro_expression', $._list_element), ','),

    modified_type: $ => prec.left(seq(
      $._type_no_options,
      choice(
        seq('-', alias($._open_bracket, '['), alias($._close_bracket, ']')),
        'explicit',
        'const',
        seq('-', 'const'),
        '&',
        seq('-', '&'),
        '#',
        'implicit',
        seq('-', '#'),
        seq('==', 'const'),
        seq('==', '&'),
      ),
    )),

    pointer_type: $ => prec.left(seq(field('first_type', $._type_no_options), choice('?', '??'))),

    smart_pointer_type: $ => seq('smart_ptr', angles($, field('first_type', $._type))),

    array_type: $ => seq('array', angles($, field('first_type', $._type))),

    table_type: $ => seq(
      'table',
      angles($, seq(
        field('first_type', $._type),
        optional(seq($._comma_or_semicolon, field('second_type', $._type))),
      )),
    ),

    iterator_type: $ => seq('iterator', angles($, field('first_type', $._type))),

    block_type: $ => seq('block', optional($._function_type_arguments)),

    function_type: $ => seq('function', optional($._function_type_arguments)),

    lambda_type: $ => seq('lambda', optional($._function_type_arguments)),

    _function_type_arguments: $ => angles($, choice(
      field('first_type', $._type),
      seq(optional($._argument_list), optional($._return_type)),
    )),

    tuple_type: $ => seq('tuple', angles($, $._tuple_type_list)),

    _tuple_type_list: $ => sepBy1(choice(
      field('argument_types', $._type),
      seq(field('argument_names', $.identifier), ':', field('argument_types', $._type)),
    ), $._comma_or_semicolon),

    variant_type: $ => seq('variant', angles($, $._variant_type_list)),

    _variant_type_list: $ => sepBy1(
      seq(field('argument_names', $.identifier), ':', field('argument_types', $._type)),
      $._comma_or_semicolon,
    ),

    // Lexical

    identifier: _ => /[_a-zA-Z][_a-zA-Z0-9`]*/,

    comment: _ => token(seq('//', /.*/)),

    line_directive: _ => token(seq('#', /[0-9]+/, ',', /[0-9]+/, ',', /"[^"]+"/, '#')),
  },
});

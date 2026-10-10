#include "tree_sitter/alloc.h"
#include "tree_sitter/parser.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// constraint: the order matches the externals array in grammar.js
enum TokenType {
  NEWLINE_SEMICOLON,
  NEWLINE_COMMA,
  OPEN_PAREN,
  CLOSE_PAREN,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  SAFE_OPEN_BRACKET,
  NOT_OPEN_BRACKET,
  NOT_SAFE_OPEN_BRACKET,
  BLOCK_OPEN,
  LIST_OPEN,
  TABLE_OPEN,
  BRACE_OPEN,
  CLOSE_BRACE,
  INTERPOLATION_OPEN,
  INTERPOLATION_CLOSE,
  STRING_CONTENT,
  ESCAPE_SEQUENCE,
  FORMAT_STRING,
  READER_BODY,
  KEYWORD_START,
  KEYWORD_END,
  AT_FIELD,
  TAG_E,
  TAG_I,
  TAG_V,
  TAG_B,
  TAG_A,
  TAG_T,
  TAG_C,
  TAG_F,
  NOT_IS,
  NOT_AS,
  NOT_SAFE_AS,
  MAP_TO,
  LEFT_ARROW,
  GREATER,
  GREATER_EQUAL,
  SHIFT_RIGHT,
  SHIFT_RIGHT_ASSIGN,
  ROTATE_RIGHT,
  ROTATE_RIGHT_ASSIGN,
  FINALLY,
  INTEGER,
  UNSIGNED_INTEGER,
  LONG_INTEGER,
  LONG_INTEGER_MIN,
  UNSIGNED_LONG_INTEGER,
  UNSIGNED_INT8,
  FLOAT,
  DOUBLE,
  FLOAT16,
  BLOCK_COMMENT,
  INCLUDE_DIRECTIVE,
  LINE_END,
  LEXER_ERROR,
  ERROR_SENTINEL,
};

// The compiler lexer keeps one level of this state for each open statement block or enumeration body: the depth of
// `(` and `[`, the token that a line end returns, and the flag that a statement keyword sets.
typedef struct {
  uint8_t parens;
  uint8_t squares;
  bool comma_mode;
  bool keyword;
} Level;

typedef enum { BRACE_BLOCK, BRACE_LIST, BRACE_TABLE, BRACE_PLAIN, BRACE_INTERPOLATION } BraceKind;

// shortcut: the scanner records 240 open braces and stops tracking the levels of deeper braces - raise the limit if a
// real file nests deeper
enum { BRACES_MAX = 240 };

typedef struct {
  BraceKind kind;
  Level saved;
} Brace;

// constraint: the runtime restores this state from the last external token, so each scan that changes it returns one
typedef struct {
  Level level;
  bool brace_after_semicolon;
  bool eof_done;
  bool after_finally;
  bool line_end_pending;
  uint16_t interpolations;
  uint16_t braces;
  Brace stack[BRACES_MAX];
} Scanner;

static void advance(TSLexer *lexer) { lexer->advance(lexer, false); }

static void skip(TSLexer *lexer) { lexer->advance(lexer, true); }

static bool accept(TSLexer *lexer, enum TokenType token) {
  lexer->result_symbol = token;
  return true;
}

static bool accept_char(TSLexer *lexer, enum TokenType token) {
  advance(lexer);
  lexer->mark_end(lexer);
  return accept(lexer, token);
}

static bool is_digit(int32_t c) { return c >= '0' && c <= '9'; }

static bool is_hex_digit(int32_t c) { return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

static bool is_alpha(int32_t c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

static bool is_word_char(int32_t c) { return is_alpha(c) || is_digit(c); }

static bool is_name_char(int32_t c) { return is_word_char(c) || c == '`'; }

static bool can_break_line(const Level *level) {
  return !level->keyword && level->parens == 0 && level->squares == 0;
}

static bool inside_interpolation(const Scanner *scanner) { return scanner->interpolations > 0; }

static void push_brace(Scanner *scanner, BraceKind kind) {
  if (scanner->braces < BRACES_MAX) {
    scanner->stack[scanner->braces].kind = kind;
    scanner->stack[scanner->braces].saved = scanner->level;
  }
  if (scanner->braces < UINT16_MAX) {
    scanner->braces++;
  }
  switch (kind) {
    case BRACE_BLOCK:
    case BRACE_LIST:
      scanner->level.parens = 0;
      scanner->level.squares = 0;
      scanner->level.comma_mode = kind == BRACE_LIST;
      scanner->level.keyword = false;
      break;
    case BRACE_TABLE:
      if (scanner->level.parens < UINT8_MAX) {
        scanner->level.parens++;
      }
      break;
    case BRACE_INTERPOLATION:
      scanner->interpolations++;
      break;
    case BRACE_PLAIN:
      break;
  }
}

static BraceKind pop_brace(Scanner *scanner) {
  if (scanner->braces == 0) {
    return BRACE_PLAIN;
  }
  scanner->braces--;
  if (scanner->braces >= BRACES_MAX) {
    return BRACE_PLAIN;
  }
  const Brace *brace = &scanner->stack[scanner->braces];
  switch (brace->kind) {
    case BRACE_BLOCK:
    case BRACE_LIST:
      scanner->level = brace->saved;
      break;
    case BRACE_TABLE:
      if (scanner->level.parens > 0) {
        scanner->level.parens--;
      }
      break;
    case BRACE_INTERPOLATION:
      if (scanner->interpolations > 0) {
        scanner->interpolations--;
      }
      break;
    case BRACE_PLAIN:
      break;
  }
  return brace->kind;
}

static bool top_brace_is(const Scanner *scanner, BraceKind kind) {
  return scanner->braces > 0 && scanner->braces <= BRACES_MAX && scanner->stack[scanner->braces - 1].kind == kind;
}

// Reads a word from the current position and leaves the lexer after it; the caller marks the token end first.
static unsigned read_word(TSLexer *lexer, char *buffer, unsigned size) {
  unsigned length = 0;
  while (is_name_char(lexer->lookahead)) {
    if (length + 1 < size) {
      buffer[length] = (char)lexer->lookahead;
    }
    length++;
    advance(lexer);
  }
  buffer[length + 1 < size ? length : size - 1] = '\0';
  return length;
}

static bool word_is_one_of(const char *word, unsigned length, const char *const *words, unsigned count) {
  for (unsigned i = 0; i < count; i++) {
    if (strlen(words[i]) == length && strcmp(word, words[i]) == 0) {
      return true;
    }
  }
  return false;
}

static bool scan_block_comment(TSLexer *lexer) {
  // The caller consumed "/*".
  unsigned depth = 1;
  while (depth > 0) {
    if (lexer->eof(lexer)) {
      lexer->mark_end(lexer);
      return accept(lexer, LEXER_ERROR);
    }
    int32_t c = lexer->lookahead;
    advance(lexer);
    if (c == '/' && lexer->lookahead == '*') {
      advance(lexer);
      depth++;
    } else if (c == '*' && lexer->lookahead == '/') {
      advance(lexer);
      depth--;
    }
  }
  lexer->mark_end(lexer);
  return accept(lexer, BLOCK_COMMENT);
}

// constraint: follows unescapeString in src/simulate/runtime_string.cpp of the compiler, which takes these escapes
static bool scan_escape(TSLexer *lexer) {
  // The caller is at the backslash and has marked the token start.
  advance(lexer);
  int32_t c = lexer->lookahead;
  if (c == 'x') {
    advance(lexer);
    if (!is_hex_digit(lexer->lookahead)) {
      return false;
    }
    advance(lexer);
    if (is_hex_digit(lexer->lookahead)) {
      advance(lexer);
    }
    return true;
  }
  if (c == 'u' || c == 'U') {
    int digits = c == 'u' ? 4 : 8;
    advance(lexer);
    for (int i = 0; i < digits; i++) {
      if (!is_hex_digit(lexer->lookahead)) {
        return false;
      }
      advance(lexer);
    }
    return true;
  }
  if (c == '\r') {
    advance(lexer);
    if (lexer->lookahead == '\n') {
      advance(lexer);
    }
    return true;
  }
  if (c == '"' || c == '/' || c == '\\' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' || c == 'v' ||
      c == '\n' || c == '{' || c == '}') {
    advance(lexer);
    return true;
  }
  return false;
}

// Reads the text of a string constant up to the closing quote, an interpolation, or an escape sequence. The compiler
// lexer takes a backslash and the character after it as one piece, so `\"`, `\{` and `\}` never end the text.
static bool scan_string_part(Scanner *scanner, TSLexer *lexer) {
  if (lexer->lookahead == '{') {
    if (inside_interpolation(scanner)) {
      return false;
    }
    advance(lexer);
    lexer->mark_end(lexer);
    push_brace(scanner, BRACE_INTERPOLATION);
    return accept(lexer, INTERPOLATION_OPEN);
  }
  bool has_content = false;
  if (lexer->lookahead == '\\') {
    if (scan_escape(lexer)) {
      lexer->mark_end(lexer);
      return accept(lexer, ESCAPE_SEQUENCE);
    }
    has_content = true;
  }
  for (;;) {
    lexer->mark_end(lexer);
    int32_t c = lexer->lookahead;
    if (lexer->eof(lexer) || c == '"' || c == '{') {
      break;
    }
    if (c == '\\') {
      advance(lexer);
      int32_t next = lexer->lookahead;
      bool simple = next == '"' || next == '/' || next == '\\' || next == 'b' || next == 'f' || next == 'n' ||
                    next == 'r' || next == 't' || next == 'v' || next == '\n' || next == '\r' || next == '{' ||
                    next == '}';
      bool hex = next == 'x' || next == 'u' || next == 'U';
      if (simple || hex) {
        // A known escape ends the text before the backslash. An `\x`, `\u` or `\U` without enough digits is text.
        if (simple) {
          break;
        }
        int digits = next == 'x' ? 1 : next == 'u' ? 4 : 8;
        advance(lexer);
        int found = 0;
        while (found < digits && is_hex_digit(lexer->lookahead)) {
          found++;
          advance(lexer);
        }
        if (found == digits) {
          break;
        }
        has_content = true;
        continue;
      }
      if (!lexer->eof(lexer)) {
        advance(lexer);
      }
      has_content = true;
      continue;
    }
    advance(lexer);
    has_content = true;
  }
  return has_content && accept(lexer, STRING_CONTENT);
}

static bool scan_format_string(TSLexer *lexer) {
  bool has_content = false;
  while (!lexer->eof(lexer) && lexer->lookahead != '}') {
    advance(lexer);
    has_content = true;
  }
  lexer->mark_end(lexer);
  return has_content && accept(lexer, FORMAT_STRING);
}

// constraint: every reader macro of daslib and the compiler's fallback for an unknown macro end their text at `%%`
static bool scan_reader_body(TSLexer *lexer) {
  bool percent = false;
  while (!lexer->eof(lexer)) {
    int32_t c = lexer->lookahead;
    advance(lexer);
    if (c == '%' && percent) {
      lexer->mark_end(lexer);
      return accept(lexer, READER_BODY);
    }
    percent = c == '%';
  }
  lexer->mark_end(lexer);
  return accept(lexer, LEXER_ERROR);
}

typedef struct {
  uint64_t value;
  bool overflow;
} Digits;

static void add_digit(Digits *digits, unsigned base, unsigned digit) {
  if (digits->value > (UINT64_MAX - digit) / base) {
    digits->overflow = true;
  } else {
    digits->value = digits->value * base + digit;
  }
}

static unsigned hex_value(int32_t c) {
  if (is_digit(c)) {
    return (unsigned)(c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return (unsigned)(c - 'a' + 10);
  }
  return (unsigned)(c - 'A' + 10);
}

static void consume(TSLexer *lexer) {
  advance(lexer);
  lexer->mark_end(lexer);
}

// The caller has marked the end of the integer token.
static bool accept_integer(TSLexer *lexer, enum TokenType token, Digits digits) {
  uint64_t limit = UINT64_MAX;
  switch (token) {
    case INTEGER:
      limit = INT32_MAX;
      break;
    case UNSIGNED_INTEGER:
      limit = UINT32_MAX;
      break;
    case UNSIGNED_INT8:
      limit = UINT8_MAX;
      break;
    case LONG_INTEGER:
      limit = INT64_MAX;
      if (!digits.overflow && digits.value == (uint64_t)INT64_MAX + 1) {
        return accept(lexer, LONG_INTEGER_MIN);
      }
      break;
    default:
      break;
  }
  if (digits.overflow || digits.value > limit) {
    return accept(lexer, LEXER_ERROR);
  }
  return accept(lexer, token);
}

enum { DECIMAL_DIGITS = 31 };

// The significant digits and the decimal exponent of a float constant, for the check against the double range.
typedef struct {
  char digits[DECIMAL_DIGITS];
  unsigned count;
  bool nonzero;
  bool in_fraction;
  int32_t exponent;
  int32_t explicit_exponent;
} Decimal;

static void add_decimal_digit(Decimal *decimal, int32_t c) {
  if (!decimal->nonzero) {
    if (c == '0') {
      if (decimal->in_fraction) {
        decimal->exponent--;
      }
      return;
    }
    decimal->nonzero = true;
    decimal->exponent = decimal->in_fraction ? decimal->exponent - 1 : 0;
  } else if (!decimal->in_fraction) {
    decimal->exponent++;
  }
  if (decimal->count < DECIMAL_DIGITS) {
    decimal->digits[decimal->count++] = (char)c;
  }
}

static int compare_significand(const Decimal *decimal, const char *bound) {
  for (unsigned i = 0; i < DECIMAL_DIGITS; i++) {
    char digit = i < decimal->count ? decimal->digits[i] : '0';
    char limit = bound[i] != '\0' ? bound[i] : '0';
    if (digit != limit) {
      return digit < limit ? -1 : 1;
    }
  }
  return 0;
}

// constraint: the compiler reads each float constant with fast_float::from_chars into a double and reports a value
// that rounds to infinity or to zero; the bounds are the midpoints that round to those values
static bool in_double_range(const Decimal *decimal) {
  if (!decimal->nonzero) {
    return true;
  }
  int32_t exponent = decimal->exponent + decimal->explicit_exponent;
  if (exponent != 308 && exponent != -324) {
    return exponent < 308 && exponent > -324;
  }
  if (exponent == 308) {
    return compare_significand(decimal, "1797693134862315807937289714053") < 0;
  }
  return compare_significand(decimal, "2470328229206232720882843964341") > 0;
}

static bool scan_exponent(TSLexer *lexer, Decimal *decimal) {
  // The caller is at `e` or `E` and has marked the end before it.
  advance(lexer);
  bool negative = false;
  if (lexer->lookahead == '+' || lexer->lookahead == '-') {
    negative = lexer->lookahead == '-';
    advance(lexer);
  }
  if (!is_digit(lexer->lookahead)) {
    return false;
  }
  int32_t value = 0;
  while (is_digit(lexer->lookahead)) {
    if (value < 1000000) {
      value = value * 10 + (lexer->lookahead - '0');
    }
    advance(lexer);
  }
  decimal->explicit_exponent = negative ? -value : value;
  lexer->mark_end(lexer);
  return true;
}

static bool accept_float(TSLexer *lexer, enum TokenType token, const Decimal *decimal) {
  return accept(lexer, in_double_range(decimal) ? token : LEXER_ERROR);
}

static bool accept_float_suffix(TSLexer *lexer, const Decimal *decimal) {
  int32_t c = lexer->lookahead;
  if (c == 'f' || c == 'F') {
    consume(lexer);
    return accept_float(lexer, FLOAT, decimal);
  }
  if (c == 'h' || c == 'H') {
    consume(lexer);
    return accept_float(lexer, FLOAT16, decimal);
  }
  if (c == 'd') {
    consume(lexer);
    return accept_float(lexer, DOUBLE, decimal);
  }
  if (c == 'l') {
    advance(lexer);
    if (lexer->lookahead == 'f') {
      consume(lexer);
      return accept_float(lexer, DOUBLE, decimal);
    }
  }
  return accept_float(lexer, FLOAT, decimal);
}

// Reads the digits after the decimal point, the exponent, and the suffix of a float constant. The caller consumed
// the point.
static bool scan_fraction(TSLexer *lexer, Decimal *decimal) {
  decimal->in_fraction = true;
  while (is_digit(lexer->lookahead)) {
    add_decimal_digit(decimal, lexer->lookahead);
    advance(lexer);
  }
  lexer->mark_end(lexer);
  if ((lexer->lookahead == 'e' || lexer->lookahead == 'E') && !scan_exponent(lexer, decimal)) {
    return accept_float(lexer, FLOAT, decimal);
  }
  return accept_float_suffix(lexer, decimal);
}

static bool accept_unsigned_suffix(TSLexer *lexer, Digits digits) {
  // The caller is at `u` or `U`.
  consume(lexer);
  if (lexer->lookahead == 'l' || lexer->lookahead == 'L') {
    consume(lexer);
    return accept_integer(lexer, UNSIGNED_LONG_INTEGER, digits);
  }
  if (lexer->lookahead == '8') {
    consume(lexer);
    return accept_integer(lexer, UNSIGNED_INT8, digits);
  }
  return accept_integer(lexer, UNSIGNED_INTEGER, digits);
}

// constraint: follows the number rules of ds2_lexer.lpp, where the longest rule wins and a range `1..2` starts with an
// integer; the limits are those of the compiler's from_chars calls
static bool scan_number(TSLexer *lexer) {
  Digits digits = {0, false};
  Decimal decimal = {{0}, 0, false, false, 0, 0};
  if (lexer->lookahead == '0') {
    consume(lexer);
    if (lexer->lookahead == 'x' || lexer->lookahead == 'X') {
      advance(lexer);
      if (!is_hex_digit(lexer->lookahead)) {
        return accept(lexer, INTEGER);
      }
      while (is_hex_digit(lexer->lookahead) || lexer->lookahead == '_') {
        if (lexer->lookahead != '_') {
          add_digit(&digits, 16, hex_value(lexer->lookahead));
        }
        advance(lexer);
      }
      lexer->mark_end(lexer);
      int32_t c = lexer->lookahead;
      if (c == 'l' || c == 'L') {
        consume(lexer);
        return accept_integer(lexer, UNSIGNED_LONG_INTEGER, digits);
      }
      if (c == 'u' || c == 'U') {
        return accept_unsigned_suffix(lexer, digits);
      }
      return accept_integer(lexer, UNSIGNED_INTEGER, digits);
    }
  } else {
    add_digit(&digits, 10, (unsigned)(lexer->lookahead - '0'));
    add_decimal_digit(&decimal, lexer->lookahead);
    advance(lexer);
  }
  bool underscore = false;
  while (is_digit(lexer->lookahead) || lexer->lookahead == '_') {
    if (lexer->lookahead == '_') {
      underscore = true;
    } else {
      add_digit(&digits, 10, (unsigned)(lexer->lookahead - '0'));
      add_decimal_digit(&decimal, lexer->lookahead);
    }
    advance(lexer);
  }
  lexer->mark_end(lexer);
  int32_t c = lexer->lookahead;
  if (!underscore) {
    if (c == '.') {
      advance(lexer);
      if (lexer->lookahead == '.') {
        return accept_integer(lexer, INTEGER, digits);
      }
      return scan_fraction(lexer, &decimal);
    }
    if (c == 'e' || c == 'E') {
      if (scan_exponent(lexer, &decimal)) {
        return accept_float_suffix(lexer, &decimal);
      }
      return accept_integer(lexer, INTEGER, digits);
    }
    if (c == 'f' || c == 'F' || c == 'h' || c == 'H' || c == 'd') {
      return accept_float_suffix(lexer, &decimal);
    }
  }
  if (c == 'l' || c == 'L') {
    consume(lexer);
    if (!underscore && c == 'l' && lexer->lookahead == 'f') {
      consume(lexer);
      return accept_float(lexer, DOUBLE, &decimal);
    }
    return accept_integer(lexer, LONG_INTEGER, digits);
  }
  if (c == 'u' || c == 'U') {
    return accept_unsigned_suffix(lexer, digits);
  }
  return accept_integer(lexer, INTEGER, digits);
}

// constraint: the compiler lexer reads `'x'` with one byte, or one of the escapes \b \t \n \f \r \\ \', and an optional
// u8 or u suffix
static bool scan_character(TSLexer *lexer) {
  advance(lexer);
  int32_t c = lexer->lookahead;
  if (c == '\\') {
    advance(lexer);
    int32_t escape = lexer->lookahead;
    if (escape == '\'') {
      advance(lexer);
      lexer->mark_end(lexer);
      if (lexer->lookahead == '\'') {
        advance(lexer);
        lexer->mark_end(lexer);
      }
    } else if (escape == 'b' || escape == 't' || escape == 'n' || escape == 'f' || escape == 'r' ||
               escape == '\\') {
      advance(lexer);
      if (lexer->lookahead != '\'') {
        return false;
      }
      advance(lexer);
      lexer->mark_end(lexer);
    } else {
      return false;
    }
  } else {
    if (c == '\n' || c >= 0x80 || lexer->eof(lexer)) {
      return false;
    }
    advance(lexer);
    if (lexer->lookahead != '\'') {
      return false;
    }
    advance(lexer);
    lexer->mark_end(lexer);
  }
  if (lexer->lookahead == 'u' || lexer->lookahead == 'U') {
    advance(lexer);
    lexer->mark_end(lexer);
    if (lexer->lookahead == '8') {
      advance(lexer);
      lexer->mark_end(lexer);
      return accept(lexer, UNSIGNED_INT8);
    }
    return accept(lexer, UNSIGNED_INTEGER);
  }
  return accept(lexer, INTEGER);
}

// constraint: `$$` and `$e`, `$i`, `$v`, `$b`, `$a`, `$t`, `$c`, `$f` before a character that is not a letter, a digit,
// or `_` are reification tags in the compiler lexer
static bool scan_tag(TSLexer *lexer) {
  advance(lexer);
  int32_t c = lexer->lookahead;
  if (c == '$') {
    return accept_char(lexer, TAG_E);
  }
  enum TokenType token;
  switch (c) {
    case 'e':
      token = TAG_E;
      break;
    case 'i':
      token = TAG_I;
      break;
    case 'v':
      token = TAG_V;
      break;
    case 'b':
      token = TAG_B;
      break;
    case 'a':
      token = TAG_A;
      break;
    case 't':
      token = TAG_T;
      break;
    case 'c':
      token = TAG_C;
      break;
    case 'f':
      token = TAG_F;
      break;
    default:
      return false;
  }
  advance(lexer);
  lexer->mark_end(lexer);
  if (lexer->eof(lexer) || is_word_char(lexer->lookahead)) {
    return false;
  }
  return accept(lexer, token);
}

// constraint: the compiler lexer returns `@` before a letter or `_` as the field annotation token, except before the
// word `capture` and a character that ends the word
static bool scan_at_field(TSLexer *lexer) {
  advance(lexer);
  lexer->mark_end(lexer);
  if (!is_alpha(lexer->lookahead)) {
    return false;
  }
  static const char capture[] = "capture";
  unsigned matched = 0;
  while (capture[matched] != '\0' && lexer->lookahead == capture[matched]) {
    advance(lexer);
    matched++;
  }
  bool before_capture = capture[matched] == '\0' && !lexer->eof(lexer) && !is_word_char(lexer->lookahead);
  return !before_capture && accept(lexer, AT_FIELD);
}

// constraint: the compiler lexer reads `!is`, `!as`, and `!?as` as one token only before a character that is not a
// letter, a digit, or `_`
static bool scan_negated_word(TSLexer *lexer, const char *word, enum TokenType token) {
  for (const char *p = word; *p != '\0'; p++) {
    if (lexer->lookahead != *p) {
      return false;
    }
    advance(lexer);
  }
  if (lexer->eof(lexer) || is_word_char(lexer->lookahead)) {
    return false;
  }
  lexer->mark_end(lexer);
  return accept(lexer, token);
}

// constraint: the compiler lexer takes the spaces and the line end after a `=>` at the end of a line into the token
static bool scan_map_to(TSLexer *lexer) {
  advance(lexer);
  if (lexer->lookahead != '>') {
    return false;
  }
  consume(lexer);
  while (lexer->lookahead == ' ' || lexer->lookahead == '\t' || lexer->lookahead == '\r') {
    advance(lexer);
  }
  if (lexer->lookahead == '\n') {
    consume(lexer);
  }
  return accept(lexer, MAP_TO);
}

// The caller is at the `[` that ends the token.
static bool accept_open_square(Scanner *scanner, TSLexer *lexer, enum TokenType token) {
  if (scanner->level.squares < UINT8_MAX) {
    scanner->level.squares++;
  }
  return accept_char(lexer, token);
}

static bool scan_exclamation(Scanner *scanner, TSLexer *lexer) {
  advance(lexer);
  switch (lexer->lookahead) {
    case '[':
      return accept_open_square(scanner, lexer, NOT_OPEN_BRACKET);
    case 'i':
      return scan_negated_word(lexer, "is", NOT_IS);
    case 'a':
      return scan_negated_word(lexer, "as", NOT_AS);
    case '?':
      advance(lexer);
      if (lexer->lookahead == '[') {
        return accept_open_square(scanner, lexer, NOT_SAFE_OPEN_BRACKET);
      }
      return scan_negated_word(lexer, "as", NOT_SAFE_AS);
    default:
      return false;
  }
}

// constraint: the compiler lexer reads `<-` as one token wherever it appears
static bool scan_less(TSLexer *lexer) {
  advance(lexer);
  return lexer->lookahead == '-' && accept_char(lexer, LEFT_ARROW);
}

// constraint: the compiler lexer reads `>=`, `>>=`, and `>>>=` as one token wherever they appear, and splits `>>` and
// `>>>` into `>` tokens while a `<` of a type is open; a state that takes `>` and no shift operator is inside one
static bool scan_greater(TSLexer *lexer, const bool *valid_symbols) {
  bool inside_type = valid_symbols[GREATER] && !valid_symbols[SHIFT_RIGHT] && !valid_symbols[ROTATE_RIGHT];
  consume(lexer);
  if (lexer->lookahead == '=') {
    return accept_char(lexer, GREATER_EQUAL);
  }
  if (lexer->lookahead != '>') {
    return accept(lexer, GREATER);
  }
  advance(lexer);
  if (lexer->lookahead == '=') {
    return accept_char(lexer, SHIFT_RIGHT_ASSIGN);
  }
  if (lexer->lookahead != '>') {
    if (inside_type) {
      return accept(lexer, GREATER);
    }
    lexer->mark_end(lexer);
    return accept(lexer, SHIFT_RIGHT);
  }
  advance(lexer);
  if (lexer->lookahead == '=') {
    return accept_char(lexer, ROTATE_RIGHT_ASSIGN);
  }
  if (inside_type) {
    return accept(lexer, GREATER);
  }
  lexer->mark_end(lexer);
  return accept(lexer, ROTATE_RIGHT);
}

static bool accept_finally(Scanner *scanner, TSLexer *lexer) {
  lexer->mark_end(lexer);
  scanner->after_finally = true;
  return accept(lexer, FINALLY);
}

static bool is_finally(const char *word, unsigned length) { return length == 7 && strcmp(word, "finally") == 0; }

// constraint: the compiler parser opens a statement level right after `finally`, so a line end before its `{` returns a
// semicolon even inside `(` or after an `if`
static bool scan_finally(Scanner *scanner, TSLexer *lexer) {
  static const char word[] = "finally";
  for (const char *p = word; *p != '\0'; p++) {
    if (lexer->lookahead != *p) {
      return false;
    }
    advance(lexer);
  }
  return !is_name_char(lexer->lookahead) && accept_finally(scanner, lexer);
}

static bool scan_open_brace(Scanner *scanner, TSLexer *lexer, const bool *valid_symbols) {
  scanner->after_finally = false;
  advance(lexer);
  lexer->mark_end(lexer);
  if (valid_symbols[TABLE_OPEN] && !valid_symbols[BLOCK_OPEN]) {
    push_brace(scanner, BRACE_TABLE);
    return accept(lexer, TABLE_OPEN);
  }
  if (valid_symbols[LIST_OPEN]) {
    push_brace(scanner, BRACE_LIST);
    return accept(lexer, LIST_OPEN);
  }
  if (valid_symbols[BRACE_OPEN] && !valid_symbols[BLOCK_OPEN]) {
    push_brace(scanner, BRACE_PLAIN);
    return accept(lexer, BRACE_OPEN);
  }
  push_brace(scanner, BRACE_BLOCK);
  return accept(lexer, BLOCK_OPEN);
}

// constraint: the compiler lexer returns a line-end semicolon before a `}` where a line end would return one, and
// returns the `}` itself at the next scan
static bool scan_close_brace(Scanner *scanner, TSLexer *lexer) {
  if (top_brace_is(scanner, BRACE_INTERPOLATION)) {
    pop_brace(scanner);
    scanner->brace_after_semicolon = false;
    return accept_char(lexer, INTERPOLATION_CLOSE);
  }
  if (!scanner->brace_after_semicolon && !inside_interpolation(scanner) && !scanner->level.comma_mode &&
      can_break_line(&scanner->level) && scanner->braces > 0) {
    scanner->brace_after_semicolon = true;
    lexer->mark_end(lexer);
    return accept(lexer, NEWLINE_SEMICOLON);
  }
  scanner->brace_after_semicolon = false;
  pop_brace(scanner);
  return accept_char(lexer, CLOSE_BRACE);
}

static const char *const STATEMENT_KEYWORDS[] = {"if", "static_if", "for", "while", "with"};

static bool is_blank(int32_t c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

// The compiler lexer reads `include`, skips blanks and line breaks, and takes the next run of non-blank characters as
// the name of the file whose text replaces the directive; at the end of the text, the directive has no file name.
static bool accept_include(TSLexer *lexer) {
  // The caller read the word `include`.
  while (is_blank(lexer->lookahead)) {
    advance(lexer);
  }
  while (!lexer->eof(lexer) && !is_blank(lexer->lookahead)) {
    advance(lexer);
  }
  lexer->mark_end(lexer);
  return accept(lexer, INCLUDE_DIRECTIVE);
}

static const char *const CONTINUATION_KEYWORDS[] = {"else", "elif", "static_elif"};

// Reads the include directive and the keyword markers. The compiler parser sets a flag before the keyword of an `if`,
// `for`, `while`, or `with` statement and clears it when the statement ends or a one-line body starts. While the flag
// is set, a line end returns nothing.
static bool scan_word(Scanner *scanner, TSLexer *lexer, const bool *valid_symbols, bool error_recovery, bool *matched) {
  *matched = false;
  bool markers = !error_recovery;
  if (markers && valid_symbols[KEYWORD_END] && scanner->level.keyword &&
      !(lexer->lookahead == '{' && valid_symbols[BLOCK_OPEN])) {
    char word[16];
    lexer->mark_end(lexer);
    unsigned length = read_word(lexer, word, sizeof word);
    *matched = true;
    if (is_finally(word, length)) {
      return accept_finally(scanner, lexer);
    }
    if (length > 0 && word_is_one_of(word, length, CONTINUATION_KEYWORDS, 3)) {
      *matched = false;
      return false;
    }
    scanner->level.keyword = false;
    return accept(lexer, KEYWORD_END);
  }
  if (lexer->lookahead == 'i' || (markers && valid_symbols[KEYWORD_START] && is_alpha(lexer->lookahead))) {
    char word[16];
    lexer->mark_end(lexer);
    unsigned length = read_word(lexer, word, sizeof word);
    if (length == 7 && strcmp(word, "include") == 0) {
      *matched = true;
      return accept_include(lexer);
    }
    if (!markers || !valid_symbols[KEYWORD_START]) {
      return false;
    }
    if (is_finally(word, length)) {
      *matched = true;
      return accept_finally(scanner, lexer);
    }
    if (word_is_one_of(word, length, STATEMENT_KEYWORDS, 5)) {
      *matched = true;
      scanner->level.keyword = true;
      return accept(lexer, KEYWORD_START);
    }
  }
  return false;
}

// The token that a line end returns is zero-width and ends before the line end, so that no node of the tree takes the
// line break; the next scan returns the line break itself as the extra `_line_end`.
static bool accept_line_end(Scanner *scanner, TSLexer *lexer) {
  bool comma = scanner->level.comma_mode && !scanner->after_finally;
  scanner->after_finally = false;
  return accept(lexer, comma ? NEWLINE_COMMA : NEWLINE_SEMICOLON);
}

static bool scan_code(Scanner *scanner, TSLexer *lexer, const bool *valid_symbols, bool error_recovery) {
  for (;;) {
    int32_t c = lexer->lookahead;
    if (c == ' ' || c == '\t' || c == '\r') {
      skip(lexer);
    } else if (c == '\\') {
      skip(lexer);
      while (lexer->lookahead == ' ' || lexer->lookahead == '\t' || lexer->lookahead == '\r') {
        skip(lexer);
      }
      if (lexer->lookahead != '\n') {
        return false;
      }
      skip(lexer);
    } else if (c == '\n') {
      if (scanner->line_end_pending) {
        scanner->line_end_pending = false;
        advance(lexer);
        lexer->mark_end(lexer);
        return accept(lexer, LINE_END);
      }
      if (can_break_line(&scanner->level) || scanner->after_finally) {
        lexer->mark_end(lexer);
        scanner->line_end_pending = true;
        return accept_line_end(scanner, lexer);
      }
      skip(lexer);
    } else {
      break;
    }
  }
  scanner->line_end_pending = false;
  lexer->mark_end(lexer);
  int32_t c = lexer->lookahead;
  if (lexer->eof(lexer)) {
    if (!scanner->eof_done && (can_break_line(&scanner->level) || scanner->after_finally)) {
      scanner->eof_done = true;
      return accept_line_end(scanner, lexer);
    }
    return false;
  }
  if (c == '/') {
    advance(lexer);
    if (lexer->lookahead == '*') {
      advance(lexer);
      return scan_block_comment(lexer);
    }
    return false;
  }
  bool matched = false;
  bool found = scan_word(scanner, lexer, valid_symbols, error_recovery, &matched);
  if (found && lexer->result_symbol == KEYWORD_END && c == '}') {
    scanner->brace_after_semicolon = true;
  }
  if (found || matched || lexer->lookahead != c) {
    return found;
  }
  switch (c) {
    case '(':
      if (scanner->level.parens < UINT8_MAX) {
        scanner->level.parens++;
      }
      return accept_char(lexer, OPEN_PAREN);
    case ')':
      if (scanner->level.parens > 0) {
        scanner->level.parens--;
      }
      return accept_char(lexer, CLOSE_PAREN);
    case '[':
      return accept_open_square(scanner, lexer, OPEN_BRACKET);
    case ']':
      if (scanner->level.squares > 0) {
        scanner->level.squares--;
      }
      return accept_char(lexer, CLOSE_BRACKET);
    case '?':
      advance(lexer);
      return lexer->lookahead == '[' && accept_open_square(scanner, lexer, SAFE_OPEN_BRACKET);
    case '!':
      return scan_exclamation(scanner, lexer);
    case '=':
      return scan_map_to(lexer);
    case '<':
      return scan_less(lexer);
    case '>':
      return scan_greater(lexer, valid_symbols);
    case 'f':
      return scan_finally(scanner, lexer);
    case '{':
      if (error_recovery) {
        advance(lexer);
        lexer->mark_end(lexer);
        push_brace(scanner, BRACE_BLOCK);
        return accept(lexer, BLOCK_OPEN);
      }
      return scan_open_brace(scanner, lexer, valid_symbols);
    case '}':
      return scan_close_brace(scanner, lexer);
    case '*':
      advance(lexer);
      return lexer->lookahead == '/' && accept_char(lexer, LEXER_ERROR);
    case '@':
      return scan_at_field(lexer);
    case '$':
      return scan_tag(lexer);
    case '\'':
      return scan_character(lexer);
    case '.': {
      advance(lexer);
      Decimal decimal = {{0}, 0, false, false, 0, 0};
      return is_digit(lexer->lookahead) && scan_fraction(lexer, &decimal);
    }
    default:
      return is_digit(c) && scan_number(lexer);
  }
}

void *tree_sitter_daslang_external_scanner_create(void) { return ts_calloc(1, sizeof(Scanner)); }

void tree_sitter_daslang_external_scanner_destroy(void *payload) { ts_free(payload); }

enum { HEADER_SIZE = 8, BRACE_SIZE = 4 };

static uint8_t pack_flags(const Level *level) {
  return (uint8_t)((level->comma_mode ? 1 : 0) | (level->keyword ? 2 : 0));
}

static void unpack_flags(Level *level, uint8_t flags) {
  level->comma_mode = (flags & 1) != 0;
  level->keyword = (flags & 2) != 0;
}

unsigned tree_sitter_daslang_external_scanner_serialize(void *payload, char *buffer) {
  Scanner *scanner = payload;
  uint8_t *out = (uint8_t *)buffer;
  out[0] = scanner->level.parens;
  out[1] = scanner->level.squares;
  out[2] = pack_flags(&scanner->level);
  out[3] = (uint8_t)((scanner->brace_after_semicolon ? 1 : 0) | (scanner->eof_done ? 2 : 0) |
                     (scanner->after_finally ? 4 : 0) | (scanner->line_end_pending ? 8 : 0));
  memcpy(out + 4, &scanner->interpolations, sizeof scanner->interpolations);
  memcpy(out + 6, &scanner->braces, sizeof scanner->braces);
  unsigned recorded = scanner->braces < BRACES_MAX ? scanner->braces : BRACES_MAX;
  for (unsigned i = 0; i < recorded; i++) {
    const Brace *brace = &scanner->stack[i];
    uint8_t *slot = out + HEADER_SIZE + i * BRACE_SIZE;
    slot[0] = (uint8_t)brace->kind;
    slot[1] = brace->saved.parens;
    slot[2] = brace->saved.squares;
    slot[3] = pack_flags(&brace->saved);
  }
  return HEADER_SIZE + recorded * BRACE_SIZE;
}

void tree_sitter_daslang_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  Scanner *scanner = payload;
  memset(scanner, 0, sizeof *scanner);
  if (length < HEADER_SIZE) {
    return;
  }
  const uint8_t *in = (const uint8_t *)buffer;
  scanner->level.parens = in[0];
  scanner->level.squares = in[1];
  unpack_flags(&scanner->level, in[2]);
  scanner->brace_after_semicolon = (in[3] & 1) != 0;
  scanner->eof_done = (in[3] & 2) != 0;
  scanner->after_finally = (in[3] & 4) != 0;
  scanner->line_end_pending = (in[3] & 8) != 0;
  memcpy(&scanner->interpolations, in + 4, sizeof scanner->interpolations);
  memcpy(&scanner->braces, in + 6, sizeof scanner->braces);
  unsigned recorded = (length - HEADER_SIZE) / BRACE_SIZE;
  for (unsigned i = 0; i < recorded && i < BRACES_MAX; i++) {
    const uint8_t *slot = in + HEADER_SIZE + i * BRACE_SIZE;
    scanner->stack[i].kind = (BraceKind)slot[0];
    scanner->stack[i].saved.parens = slot[1];
    scanner->stack[i].saved.squares = slot[2];
    unpack_flags(&scanner->stack[i].saved, slot[3]);
  }
}

bool tree_sitter_daslang_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  Scanner *scanner = payload;
  bool error_recovery = valid_symbols[ERROR_SENTINEL];
  if (!error_recovery) {
    if (valid_symbols[READER_BODY]) {
      return scan_reader_body(lexer);
    }
    if (valid_symbols[FORMAT_STRING] && lexer->lookahead != '}') {
      return scan_format_string(lexer);
    }
    if (valid_symbols[STRING_CONTENT]) {
      if (lexer->lookahead == '"' || lexer->eof(lexer)) {
        return false;
      }
      return scan_string_part(scanner, lexer);
    }
  }
  return scan_code(scanner, lexer, valid_symbols, error_recovery);
}

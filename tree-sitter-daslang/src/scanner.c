#include "tree_sitter/parser.h"
#include <string.h>

enum TokenType {
  STRING_CONTENT,
  AUTOMATIC_SEMICOLON,
  FLOAT_TRAILING_DOT,
};

void *tree_sitter_daslang_external_scanner_create(void) {
  return NULL;
}

void tree_sitter_daslang_external_scanner_destroy(void *payload) {}

unsigned tree_sitter_daslang_external_scanner_serialize(void *payload,
                                                        char *buffer) {
  return 0;
}

void tree_sitter_daslang_external_scanner_deserialize(void *payload,
                                                       const char *buffer,
                                                       unsigned length) {}

static bool scan_automatic_semicolon(TSLexer *lexer) {
  lexer->mark_end(lexer);  // zero-width token

  bool found_newline = false;

  for (;;) {
    switch (lexer->lookahead) {
      case ' ':
      case '\t':
      case '\f':
        lexer->advance(lexer, true);
        break;
      case '\n':
      case '\r':
        found_newline = true;
        lexer->advance(lexer, true);
        break;
      case '/':
        lexer->advance(lexer, true);
        if (lexer->lookahead == '/') {
          // Line comment — skip to end of line
          while (lexer->lookahead != '\n' && lexer->lookahead != '\r' &&
                 !lexer->eof(lexer)) {
            lexer->advance(lexer, true);
          }
          // The newline will be caught in the next iteration
        } else if (lexer->lookahead == '*') {
          // Block comment — skip to */
          lexer->advance(lexer, true);
          while (!lexer->eof(lexer)) {
            if (lexer->lookahead == '\n' || lexer->lookahead == '\r') {
              found_newline = true;
            }
            if (lexer->lookahead == '*') {
              lexer->advance(lexer, true);
              if (lexer->lookahead == '/') {
                lexer->advance(lexer, true);
                break;
              }
            } else {
              lexer->advance(lexer, true);
            }
          }
        } else {
          // Just a '/' operator — not a comment, stop scanning
          goto done;
        }
        break;
      default:
        goto done;
    }
  }

done:
  if (found_newline || lexer->eof(lexer) || lexer->lookahead == '}') {
    lexer->result_symbol = AUTOMATIC_SEMICOLON;
    return true;
  }

  return false;
}

// Scan string content: match any characters that are not ", \, or {
// This allows the grammar to distinguish between:
// - string_literal (no interpolation): "hello"
// - string_builder (with interpolation): "hello {name}"
bool tree_sitter_daslang_external_scanner_scan(void *payload,
                                                TSLexer *lexer,
                                                const bool *valid_symbols) {
  // Float with trailing dot: 2. but not 2.. (which is range)
  // Try this whenever FLOAT_TRAILING_DOT is valid, even alongside ASI.
  // Tree-sitter restores lexer position when we return false.
  if (valid_symbols[FLOAT_TRAILING_DOT]) {
    // Skip horizontal whitespace only — preserve newlines for ASI
    while (lexer->lookahead == ' ' || lexer->lookahead == '\t' ||
           lexer->lookahead == '\f') {
      lexer->advance(lexer, true);
    }
    if (lexer->lookahead >= '0' && lexer->lookahead <= '9') {
      lexer->advance(lexer, false);
      while ((lexer->lookahead >= '0' && lexer->lookahead <= '9') ||
             lexer->lookahead == '_') {
        lexer->advance(lexer, false);
      }
      if (lexer->lookahead == '.') {
        lexer->advance(lexer, false);
        // Check next char is NOT '.', digit, or float suffix letter (d/f/l/e)
        if (lexer->lookahead != '.' &&
            !(lexer->lookahead >= '0' && lexer->lookahead <= '9') &&
            lexer->lookahead != 'd' && lexer->lookahead != 'D' &&
            lexer->lookahead != 'f' && lexer->lookahead != 'F' &&
            lexer->lookahead != 'l' && lexer->lookahead != 'L' &&
            lexer->lookahead != 'e' && lexer->lookahead != 'E') {
          lexer->mark_end(lexer);
          lexer->result_symbol = FLOAT_TRAILING_DOT;
          return true;
        }
      }
    }
    // No trailing-dot float found — fall through to other token types
  }

  // In error recovery, all symbols are valid — don't emit ASI
  if (valid_symbols[AUTOMATIC_SEMICOLON] && !valid_symbols[STRING_CONTENT]) {
    if (scan_automatic_semicolon(lexer)) {
      return true;
    }
  }

  if (valid_symbols[STRING_CONTENT]) {
    bool has_content = false;
    while (lexer->lookahead != 0 && lexer->lookahead != '"' &&
           lexer->lookahead != '\\' && lexer->lookahead != '{') {
      has_content = true;
      lexer->advance(lexer, false);
    }

    if (has_content) {
      lexer->result_symbol = STRING_CONTENT;
      return true;
    }
  }

  return false;
}

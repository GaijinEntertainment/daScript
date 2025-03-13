#pragma once

namespace das {

    struct DasKeyword {
        bool    needs_oxford_comma = false;
        bool    is_type_function = false;
        string  keyword;
    };

    struct Nesteds {
        int     parentheses = 0;
        int     square_braces = 0;
        int     emit_char = 0;
        bool    keyword = false;
    };

    struct DasParserState {
        int das_current_line_indent = 0;
        int das_indent_level = 0;
        int das_def_tab_size = 4;
        int das_tab_size = 4;
        int das_nested_parentheses = 0;
        int das_nested_curly_braces = 0;
        int das_nested_square_braces = 0;
        int das_nested_sb = 0;
        int das_yycolumn = 0;
        int das_c_style_depth = 0;
        int das_arrow_depth = 0;
        bool das_need_oxford_comma = true;
        bool das_force_oxford_comma = false;
        bool das_suppress_errors = false;
        bool das_in_normal = false;
        bool das_has_type_declarations = false;
        bool das_gen2_make_syntax = false;
        bool das_keyword = false;
        int  das_indent_char = ';';
        LineInfo last_token_end; // Sometimes we need to know previous matched token end location
        vector<int> das_line_no;
        vector<Nesteds> das_nesteds;
        das_hash_set<string> das_already_include;
        das_hash_map<string,string>   das_module_alias;
        das_hash_map<string,DasKeyword> das_keywords;
        Structure * g_thisStructure = nullptr;
        vector<CommentReader *> g_CommentReaders;
        ReaderMacro * g_ReaderMacro = nullptr;
        ExprReader * g_ReaderExpr = nullptr;
        vector<FileInfo *> g_FileAccessStack;
        FileAccessPtr g_Access;
        ProgramPtr g_Program;
        void push_nesteds ( int Ch) {
            // printf("push nesteds\n");
            das_indent_level ++;
            das_nesteds.push_back({das_nested_parentheses, das_nested_square_braces, das_indent_char, das_keyword});
            das_nested_parentheses = 0;
            das_nested_square_braces = 0;
            das_indent_char = Ch;
            das_keyword = false;
        }
        void pop_nesteds() {
            if ( das_nesteds.size() ) {
                // printf("pop nesteds\n");
                das_indent_level --;
                auto & n = das_nesteds.back();
                das_nested_parentheses = n.parentheses;
                das_nested_square_braces = n.square_braces;
                das_indent_char = n.emit_char;
                das_keyword = n.keyword;
                das_nesteds.pop_back();
            }
        }
    };
}

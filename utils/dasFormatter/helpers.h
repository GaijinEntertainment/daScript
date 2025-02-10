#ifndef DAS_HELPERS_H
#define DAS_HELPERS_H

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <ostream>
#include <daScript/ast/ast_typedecl.h>

#include "formatter.h"

namespace das::format {

    template <typename T>
    string convert_to_string(const vector<T> &vec, string sep = ", ") {
        if (vec.empty()) {
            return "";
        }
        return accumulate(
            next(vec.begin()), vec.end(),
            format::get_substring(vec.front()->at),
            [sep] (const string& a, const T &b) {
                return a + sep + format::get_substring(b->at);
            }
        );
    }

    optional<string> type_to_string(const TypeDecl *type_decl, LineInfo loc /* pass loc, since it's sometimes incorrect on type itself*/) {
        if (type_decl->isTemp(false) || type_decl->alias == "``MACRO``TAG``") {
            return "struct<" + format::get_substring(loc) + ">";
        } else if (type_decl->isAuto() && !type_decl->isPointer()) {
            return nullopt;
        } else {
            return get_substring(loc);
        }
    }

    string handle_msd(ExprMakeStruct* msd, string type_name, bool is_initialized = true) {
        auto structs = msd->structs;
        if (structs.empty()) {
            return "";
        }
        auto maybe_uninit = is_initialized ? "" : "uninitialized ";
        auto converter = [&type_name, &maybe_uninit] (string a, MakeStructPtr tuple) {
            a += ", " + type_name + "(" + maybe_uninit + das::format::convert_to_string(*tuple.get()) + ")";
            return a;
        };

        return accumulate(
            next(structs.begin()), structs.end(),
            converter("", structs.front()).substr(2), // rid of ", "
            converter
        );
    }

    string handle_mka(ExprMakeArray* mka) {
        const auto &values = mka->values;
        if (values.empty()) {
            return "";
        }
        auto converter = [] (string a, const ExpressionPtr &tuple) {
            a += ", ";
            if ( tuple->rtti_isMakeTuple() ) {
                a += "(" + das::format::convert_to_string(static_cast<ExprMakeTuple *>(tuple.get())->values) + ")";
            } else {
                a += das::format::get_substring(tuple->at);
            }
            return a;
        };

        return accumulate(
            next(values.begin()), values.end(),
            converter("", values.front()).substr(2), // rid of ", "
            converter
        );
    }

    size_t find_comma_place(const std::string &line) { // dirty hack to find last meaningful character.
        auto comment_start = line.find("//");
        if (comment_start == 0) {
            return 0;
        }
        const auto npos = -1;
        if (comment_start != npos) {
            const auto &maybe_comment = line.substr(comment_start);
            const auto quotes = std::count(maybe_comment.begin(), maybe_comment.end(), '"');
            if (quotes % 2 != 0) {
                comment_start = line.size();
            }
        }
        return line.find_last_not_of(" \t\r", comment_start == npos ? npos : comment_start - 1);
    }

    void handle_brace(Pos prev_loc, int value, const std::string &internal, size_t tab_size, Pos end_loc) {
        if (format::is_replace_braces() && value != 0xdeadbeef &&
            format::prepare_rule(prev_loc)) {
            format::get_writer() << " {" << internal << "\n" << std::string(value * tab_size, ' ') + "}";
            format::finish_rule(end_loc);
        }
    }

    void replace_with(Pos start, LineInfo internal, Pos end, const std::string &open, const std::string &close) {
        if (format::is_replace_braces() && format::prepare_rule(start)) {
            format::get_writer() << open << format::get_substring(internal) << close;
            format::finish_rule(end);
        }
    }

    void wrap_par_expr(LineInfo real_expr, LineInfo info_expr) {
        if (format::is_replace_braces() && real_expr == info_expr && format::prepare_rule(Pos::from(real_expr))) {
            format::get_writer() << "(" << format::get_substring(real_expr) << ")";
            format::finish_rule(Pos::from_last(real_expr));
        }

    }

    LineInfo concat(LineInfo first, LineInfo last) {
        return LineInfo(
            first.fileInfo,
            first.column,
            first.line,
            last.last_column,
            last.last_line);
    }
}

#endif //DAS_HELPERS_H

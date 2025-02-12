#ifndef DAS_HELPERS_H
#define DAS_HELPERS_H

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <daScript/ast/ast_typedecl.h>

#include "formatter.h"

namespace das::format {

    static inline constexpr int npos = -1;

    template <typename T>
    string convert_to_string(const vector<T> &vec, string sep = ",", string prev_sep = ",") {
        if (vec.empty()) {
            return "";
        }
        string result = format::get_substring(vec.front()->at);
        Pos last = Pos::from_last(vec.front()->at);
        std::for_each(vec.begin()+1, vec.end(), [&last, &prev_sep, &result, &sep](const auto& el) {
            auto concat = format::get_substring(last, Pos::from(el->at));
            auto prev_end = concat.find(prev_sep);
//            assert(prev_end != npos); // incorrect prev_sep
            if (prev_end != npos) {
                concat.replace(prev_end, prev_sep.size(), sep);
            } else {
                assert(concat.find("=>") != npos);
                cerr << "Be careful, => is not safe yet" << endl;
                concat.replace(concat.find("=>"), 2, ","); // It's not safe!
            }
            auto new_line = format::get_substring(el->at);
            result += concat;
            result += new_line;
            last = Pos::from_last(el->at);
        });
        return result;
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

    string handle_msd(LineInfo start, ExprMakeStruct* msd, LineInfo end, string type_name, bool is_initialized = true) {
        // ";" -> ","
        auto structs = msd->structs;
        if (structs.empty()) {
            return "";
        }
        const string prev_sep = ";";
        string maybe_uninit = is_initialized ? "" : "uninitialized";
        const string prefix = type_name + "(";
        const string sep = "), ";
        const string suffix = ")";

        const auto front = structs.front();
        string result;
        auto last = start;
        for (size_t i = 0; i < structs.size(); i++) {
            const auto &el = structs.at(i);
            auto concat = format::substring_between(last, el->front()->at);
            auto prev_end = concat.find(prev_sep);
            assert(i == 0 || prev_end != npos); // incorrect prev_sep
            {
                auto can_init = can_init_with(type_name, el->size());
                switch (can_init) {
                    case CanInit::Can: maybe_uninit.clear(); break;
                    case CanInit::Cannot: maybe_uninit = "uninitialized"; break;
                    case CanInit::Unknown: break;
                }
            }
            const auto cur_prefix = prefix + maybe_uninit;
            if (i == 0) {
                concat = cur_prefix + concat;
            } else if (prev_end != npos) {
                concat.replace(prev_end, prev_sep.size(), sep + cur_prefix);
            } else {
                std::abort();
            }
            auto new_line = convert_to_string(*el.get());
            result
                .append(concat)
                .append(new_line);
            last = el->back()->at;
        }
        result.append(format::substring_between(last, end))
              .append(suffix);
        return result;
    }

    string handle_mka(LineInfo start, ExprMakeArray* mka, LineInfo end) {
        const auto &values = mka->values;
        if (values.empty()) {
            return "";
        }
        const string prev_sep = ";";
        const string sep = ",";

        const auto &front = static_cast<ExprMakeTuple *>(values.front().get())->values;
        string result;
        string suffix;
        Pos last = Pos::from_last(start);
        for (size_t i = 0; i < values.size(); i++) {
            const auto &el = values.at(i);
            string middle;
            string prefix;
            Pos from;
            Pos to;
            auto real_sep = suffix + sep;
            if ( el->rtti_isMakeTuple() ) {
                const auto &values = static_cast<ExprMakeTuple *>(el.get())->values;
                prefix = "(";
                suffix = ")";
                middle = convert_to_string(values);
                from = Pos::from(values.front()->at);
                to = Pos::from_last(values.back()->at);
            } else {
                suffix.clear();
                middle = format::get_substring(el->at);
                from = Pos::from(el->at);
                to = Pos::from_last(el->at);
            }
            auto concat = format::get_substring(last, from);
            auto prev_end = concat.find(prev_sep);
            assert(i == 0 || prev_end != npos); // incorrect prev_sep
            if (i == 0) {
                concat = prefix + concat;
            } else if (prev_end != npos) {
                real_sep.append(prefix);
                concat.replace(prev_end, prev_sep.size(), real_sep);
            }
            result.append(concat)
                  .append(middle);
            last = to;
        }
        result.append(get_substring(last, Pos::from(end)))
              .append(suffix);

        return result;
    }

    size_t find_comma_place(const std::string &line) { // dirty hack to find last meaningful character.
        auto comment_start = line.find("//");
        if (comment_start == 0) {
            return 0;
        }
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
//            format::get_writer() << " {"
//                                 << substring_between(prev_loc, mid)
//                                 << internal()
//                                 << substring_between(mid, end_loc)
//                                 << "}";
//            format::finish_rule(Pos::from_last(end_loc));
        }
    }

//    void handle_brace(LineInfo prev_loc, int value, LineInfo mid, LineInfo end_loc) {
//        handle_brace(prev_loc, value, mid, [&mid](){ return get_substring(mid);}, end_loc);
//    }

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

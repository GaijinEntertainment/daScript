#ifndef DAS_HELPERS_H
#define DAS_HELPERS_H

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
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
        if (type_decl->isTemp(false)) {
            return "struct<" + format::get_substring(loc) + ">";
        } else if (type_decl->isAuto() && !type_decl->isPointer()) {
            return nullopt;
        } else {
            return format::get_substring(loc);
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

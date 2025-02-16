#ifndef DAS_FORMATTER_H
#define DAS_FORMATTER_H

#include <string>
#include <optional>
#include <utility>
#include "daScript/daScript.h"

namespace das::format {

    // formatter config
    enum class FormatOpt {
        AlwaysBraces,
    };

    class FormatOptions {
    public:
        FormatOptions() = default;
        FormatOptions(unordered_set<FormatOpt> options_) : options(move(options_)) {}

        bool contains(FormatOpt opt) {
            return options.count(opt);
        }
    private:
        unordered_set<FormatOpt> options;
    };

    struct Pos {
        uint32_t line;
        uint32_t column;

        static Pos from(LineInfo info) {
            return Pos{info.line, info.column};
        }

        static Pos from_end_prev_line(LineInfo info);

        static Pos from_last(LineInfo info) {
            return Pos{info.last_line, info.last_column};
        }

        bool operator < (const Pos &rhs) const {
            return line < rhs.line || (line == rhs.line && column < rhs.column);
        }
    };

    // Should be called before and after each formatting operation. Maintains internal compiler state
    void init(stringstream *ss, string content, FormatOptions options);
    void destroy();

    /**
     * @return output stream
     */
    ostream& get_writer();

    /**
     * This function should be called before handling each rule
     * @param pos beginning of the rule
     * @return true if we can handle it now
     *
     * Ex: A(B()). Bison will parse in the following order: B(), A(  ).
     * Due to simplicity of formatter, we can't handle rule, if some part of it was somehow modified.
     * So if B() already handled we can't handle A(   ) in current iteration
     */
    bool prepare_rule(Pos pos);

    /**
     * Input file will be printed until current position.
     * @param pos end of the rule
     */
    void finish_rule(Pos pos);

    /**
     * @return specified substring of input file
     */
    string get_substring(LineInfo info);
    string get_substring(Pos pos1, Pos pos2);

    // Maybe we should replace it with getConfig(), if there will be a lot of options
    bool is_replace_braces();
    bool is_enabled();
}

#endif //DAS_FORMATTER_H

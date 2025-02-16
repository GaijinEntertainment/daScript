#include "formatter.h"

#include <vector>
#include <iostream>
#include <sstream>

namespace das::format {

    struct State {
        FormatOptions options;
        vector<string> content_;
        Pos last;
        bool enabled = false;
        stringstream *ss;
    };

    static State state;

    optional<Pos> try_print_until(Pos info) {
        if (!state.enabled) {
            return nullopt;
        }
        if (info.line < state.last.line) {
            return state.last;
        }
        get_writer() << get_substring(state.last, info);
        state.last = info;
        return nullopt;
    }


    void init(stringstream *ss, string content, FormatOptions options) {
        string line;
        content.push_back('\n'); // easiest way to flush the last line
        for (const auto c: content) {
            if (c != '\n') {
                line.push_back(c);
            } else {
                state.content_.emplace_back(move(line));
                line.clear();
            }
        }
        state.ss = ss;
        state.enabled = true;
        state.last = {.line=1, .column=0};
        state.options = move(options);
    }

    void destroy() {
        if (!state.content_.empty()) {
            try_print_until(Pos{static_cast<uint32_t>(state.content_.size()),
                                static_cast<uint32_t>(state.content_.back().size())});
        }
        state.ss = nullptr;
        state.enabled = false;
        state.last = {};
        state.content_ = {};
    }

    void set_to(Pos info) {
        assert(state.last < info);
        state.last = info;
    }

    string get_substring(Pos pos1, Pos pos2) {
        if (!state.enabled || pos1 < state.last) {
            return "";
        }

        string result;
        for (; pos1.line < pos2.line; pos1.line++, pos1.column = 0) {
            if (pos1.line > state.content_.size()) {
                cerr << "Warning, location line out of range" << endl;
                return "";
            } else if (pos1.column > state.content_[pos1.line - 1].length()) {
                cout << "incorrect location info, extra symbols "
                          << pos1.column << " "
                          << state.content_[pos1.line - 1].length() << endl;
            } else {
                result += state.content_[pos1.line - 1].substr(pos1.column);
            }
            result += '\n';
        }
        if (pos2.column < pos1.column || pos2.line < pos1.line) {
            assert(result.empty());
            assert(pos1.line == pos2.line);
            return "";
        }

        if (pos1.line > state.content_.size()) {
            cerr << "Warning, location line out of range" << endl;
            return "";
        }
        if (pos1.column > state.content_[pos1.line - 1].size()) {
            cerr << "Warning, location column out of range" << endl;
            return "";
        }
        result += state.content_[pos1.line - 1].substr(pos1.column, pos2.column - pos1.column);
        return result;
    }

    string get_substring(LineInfo info) {
        return get_substring(Pos{info.line, info.column},
                             Pos{info.last_line, info.last_column});
    }

    ostream& get_writer() {
        return *state.ss;
    }

    bool prepare_rule(Pos pos) {
        if (state.enabled && state.last < pos) {
            try_print_until(pos);
            return true;
        }
        return false;
    }

    void finish_rule(Pos pos) {
        assert(state.enabled && state.last < pos);
        set_to(pos);
    }

    Pos Pos::from_end_prev_line(LineInfo info) {
        if (!state.enabled) {
            return {};
        }
        const auto prev_line = info.line - 1;
        return Pos{prev_line, static_cast<uint32_t>(state.content_.at(prev_line - 1).size())};
    }

    bool is_replace_braces() {
        return state.options.contains(FormatOpt::AlwaysBraces);
    }

    bool is_enabled() {
        return state.enabled;
    }

}


#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "dasIMGUI.h"
#include "aot_dasIMGUI.h"

#include <md4c.h>
extern "C" {
#include <entity.h>
}

#include <cstdlib>
#include <cstdint>
#include <string>

namespace das {

    namespace {

        enum MarkdownEvent : int32_t {
            EnterBlock = 0,
            LeaveBlock = 1,
            EnterSpan = 2,
            LeaveSpan = 3,
            Text = 4,
            Debug = 5
        };

        struct MarkdownBridge {
            const char * source = nullptr;
            int32_t source_size = 0;
            const MarkdownEventBlock * block = nullptr;
            Context * context = nullptr;
            LineInfoArg * at = nullptr;
        };

        static void appendUtf8(std::string & out, unsigned codepoint) {
            if (codepoint == 0u || codepoint > 0x10ffffu
                    || (codepoint >= 0xd800u && codepoint <= 0xdfffu)) {
                codepoint = 0xfffdu;
            }
            if (codepoint < 0x80u) {
                out.push_back(static_cast<char>(codepoint));
            } else if (codepoint < 0x800u) {
                out.push_back(static_cast<char>(0xc0u | (codepoint >> 6u)));
                out.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
            } else if (codepoint < 0x10000u) {
                out.push_back(static_cast<char>(0xe0u | (codepoint >> 12u)));
                out.push_back(static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3fu)));
                out.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
            } else {
                out.push_back(static_cast<char>(0xf0u | (codepoint >> 18u)));
                out.push_back(static_cast<char>(0x80u | ((codepoint >> 12u) & 0x3fu)));
                out.push_back(static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3fu)));
                out.push_back(static_cast<char>(0x80u | (codepoint & 0x3fu)));
            }
        }

        static std::string decodeEntity(const std::string & value) {
            unsigned codepoints[2] = {};
            bool found = false;
            if (value.size() >= 4 && value[0] == '&' && value[1] == '#') {
                const bool hex = value[2] == 'x' || value[2] == 'X';
                const size_t begin = hex ? 3u : 2u;
                char * end = nullptr;
                const std::string digits = value.substr(begin, value.size() - begin - 1u);
                const unsigned long parsed = std::strtoul(digits.c_str(), &end, hex ? 16 : 10);
                if (end && *end == '\0') {
                    codepoints[0] = static_cast<unsigned>(parsed);
                    found = true;
                }
            } else if (const ENTITY * entity = entity_lookup(value.data(), value.size())) {
                codepoints[0] = entity->codepoints[0];
                codepoints[1] = entity->codepoints[1];
                found = true;
            }
            if (!found) return value;
            std::string decoded;
            appendUtf8(decoded, codepoints[0]);
            if (codepoints[1] != 0u) appendUtf8(decoded, codepoints[1]);
            return decoded;
        }

        static std::string attributeText(const MD_ATTRIBUTE & attr) {
            if (!attr.text || !attr.size) return {};
            std::string out;
            for (size_t i = 0; attr.substr_offsets[i] < attr.size; ++i) {
                const size_t begin = attr.substr_offsets[i];
                const size_t end = attr.substr_offsets[i + 1];
                if (attr.substr_types[i] == MD_TEXT_ENTITY) {
                    out += decodeEntity(std::string(attr.text + begin, end - begin));
                } else if (attr.substr_types[i] == MD_TEXT_NULLCHAR) {
                    appendUtf8(out, 0u);
                } else {
                    out.append(attr.text + begin, end - begin);
                }
            }
            return out;
        }

        static int32_t sourceOffset(const MarkdownBridge & bridge, const char * ptr, MD_SIZE size) {
            if (!ptr || !bridge.source || bridge.source_size < 0) {
                return -1;
            }
            const auto base = reinterpret_cast<uintptr_t>(bridge.source);
            const auto end = base + static_cast<uintptr_t>(bridge.source_size);
            const auto pos = reinterpret_cast<uintptr_t>(ptr);
            const auto tail = pos + static_cast<uintptr_t>(size);
            return pos >= base && tail >= pos && tail <= end
                ? static_cast<int32_t>(pos - base) : -1;
        }

        static void emit(MarkdownBridge & bridge, int32_t event, int32_t kind,
                         int32_t flags = 0, int32_t offset = -1, int32_t size = 0,
                         int32_t aux0 = 0, int32_t aux1 = 0,
                         const std::string & text = {}, const std::string & text2 = {}) {
            // Callback strings may be retained by daScript's immutable string
            // model. Allocate them in the caller context instead of exposing
            // std::string::c_str() pointers that expire when this event returns.
            const char * dasText = text.empty()
                ? "" : bridge.context->allocateString(text, bridge.at);
            const char * dasText2 = text2.empty()
                ? "" : bridge.context->allocateString(text2, bridge.at);
            das_invoke<void>::invoke(bridge.context, bridge.at, *bridge.block,
                event, kind, flags, offset, size, aux0, aux1,
                dasText, dasText2);
        }

        static void blockDetail(MD_BLOCKTYPE type, void * detail,
                                int32_t & flags, int32_t & offset, int32_t & size,
                                int32_t & aux0, int32_t & aux1,
                                std::string & text, std::string & text2) {
            if (!detail) return;
            switch (type) {
                case MD_BLOCK_UL: {
                    const auto & d = *static_cast<MD_BLOCK_UL_DETAIL *>(detail);
                    flags = d.is_tight != 0;
                    aux0 = static_cast<unsigned char>(d.mark);
                    break;
                }
                case MD_BLOCK_OL: {
                    const auto & d = *static_cast<MD_BLOCK_OL_DETAIL *>(detail);
                    flags = d.is_tight != 0;
                    aux0 = static_cast<int32_t>(d.start);
                    aux1 = static_cast<unsigned char>(d.mark_delimiter);
                    break;
                }
                case MD_BLOCK_LI: {
                    const auto & d = *static_cast<MD_BLOCK_LI_DETAIL *>(detail);
                    flags = d.is_task != 0;
                    if (d.is_task) {
                        offset = static_cast<int32_t>(d.task_mark_offset);
                        aux0 = static_cast<unsigned char>(d.task_mark);
                    }
                    break;
                }
                case MD_BLOCK_H:
                    aux0 = static_cast<int32_t>(static_cast<MD_BLOCK_H_DETAIL *>(detail)->level);
                    break;
                case MD_BLOCK_CODE: {
                    const auto & d = *static_cast<MD_BLOCK_CODE_DETAIL *>(detail);
                    aux0 = static_cast<unsigned char>(d.fence_char);
                    text = attributeText(d.info);
                    text2 = attributeText(d.lang);
                    break;
                }
                case MD_BLOCK_TABLE: {
                    const auto & d = *static_cast<MD_BLOCK_TABLE_DETAIL *>(detail);
                    aux0 = static_cast<int32_t>(d.col_count);
                    aux1 = static_cast<int32_t>(d.head_row_count);
                    size = static_cast<int32_t>(d.body_row_count);
                    break;
                }
                case MD_BLOCK_TH:
                case MD_BLOCK_TD:
                    aux0 = static_cast<int32_t>(static_cast<MD_BLOCK_TD_DETAIL *>(detail)->align);
                    break;
                default:
                    break;
            }
        }

        static int enterBlock(MD_BLOCKTYPE type, void * detail, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            int32_t flags = 0, offset = -1, size = 0, aux0 = 0, aux1 = 0;
            std::string text, text2;
            blockDetail(type, detail, flags, offset, size, aux0, aux1, text, text2);
            emit(bridge, EnterBlock, static_cast<int32_t>(type), flags, offset,
                 size, aux0, aux1, text, text2);
            return 0;
        }

        static int leaveBlock(MD_BLOCKTYPE type, void * detail, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            int32_t flags = 0, offset = -1, size = 0, aux0 = 0, aux1 = 0;
            std::string text, text2;
            blockDetail(type, detail, flags, offset, size, aux0, aux1, text, text2);
            emit(bridge, LeaveBlock, static_cast<int32_t>(type), flags, offset,
                 size, aux0, aux1, text, text2);
            return 0;
        }

        static void spanDetail(MD_SPANTYPE type, void * detail, int32_t & flags,
                               std::string & text, std::string & text2) {
            if (!detail) return;
            switch (type) {
                case MD_SPAN_A: {
                    const auto & d = *static_cast<MD_SPAN_A_DETAIL *>(detail);
                    flags = d.is_autolink != 0;
                    text = attributeText(d.href);
                    text2 = attributeText(d.title);
                    break;
                }
                case MD_SPAN_IMG: {
                    const auto & d = *static_cast<MD_SPAN_IMG_DETAIL *>(detail);
                    text = attributeText(d.src);
                    text2 = attributeText(d.title);
                    break;
                }
                case MD_SPAN_WIKILINK:
                    text = attributeText(static_cast<MD_SPAN_WIKILINK_DETAIL *>(detail)->target);
                    break;
                default:
                    break;
            }
        }

        static int enterSpan(MD_SPANTYPE type, void * detail, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            int32_t flags = 0;
            std::string text, text2;
            spanDetail(type, detail, flags, text, text2);
            emit(bridge, EnterSpan, static_cast<int32_t>(type), flags,
                 -1, 0, 0, 0, text, text2);
            return 0;
        }

        static int leaveSpan(MD_SPANTYPE type, void * detail, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            int32_t flags = 0;
            std::string text, text2;
            spanDetail(type, detail, flags, text, text2);
            emit(bridge, LeaveSpan, static_cast<int32_t>(type), flags,
                 -1, 0, 0, 0, text, text2);
            return 0;
        }

        static int textEvent(MD_TEXTTYPE type, const MD_CHAR * text,
                             MD_SIZE size, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            const int32_t offset = sourceOffset(bridge, text, size);
            std::string value = text && size ? std::string(text, size) : std::string();
            if (type == MD_TEXT_ENTITY) {
                value = decodeEntity(value);
            } else if (type == MD_TEXT_NULLCHAR) {
                value = "\xef\xbf\xbd"; // U+FFFD
            }
            emit(bridge, Text, static_cast<int32_t>(type), 0, offset,
                 static_cast<int32_t>(size), 0, 0, value, {});
            return 0;
        }

        static void debugEvent(const char * message, void * userdata) {
            auto & bridge = *static_cast<MarkdownBridge *>(userdata);
            emit(bridge, Debug, 0, 0, -1, 0, 0, 0,
                 message ? std::string(message) : std::string(), {});
        }

    } // namespace

    int32_t MarkdownParseEvents(const char * source, int32_t source_size,
                                int32_t flags, const MarkdownEventBlock & block,
                                Context * context, LineInfoArg * at) {
        if (source_size < 0 || (!source && source_size != 0)) {
            return -2;
        }
        MarkdownBridge bridge;
        bridge.source = source ? source : "";
        bridge.source_size = source_size;
        bridge.block = &block;
        bridge.context = context;
        bridge.at = at;

        MD_PARSER parser = {};
        parser.abi_version = 0;
        parser.flags = static_cast<unsigned>(flags);
        parser.enter_block = enterBlock;
        parser.leave_block = leaveBlock;
        parser.enter_span = enterSpan;
        parser.leave_span = leaveSpan;
        parser.text = textEvent;
        parser.debug_log = debugEvent;
        return static_cast<int32_t>(md_parse(bridge.source,
            static_cast<MD_SIZE>(source_size), &parser, &bridge));
    }

} // namespace das

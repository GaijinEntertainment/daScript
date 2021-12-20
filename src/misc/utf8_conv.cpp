#include "daScript/misc/utf8_conv.h"
#include <codecvt>

namespace das {

    wstring utf8_to_wstring(const char * utf8_string) {
        if (!utf8_string)
            return wstring();
        wstring_convert<codecvt_utf8<wchar_t>> convert;
        return convert.from_bytes(string(utf8_string));
    }

    wstring utf8_to_wstring(const string & utf8_string) {
        wstring_convert<codecvt_utf8<wchar_t>> convert;
        return convert.from_bytes(utf8_string);
    }

    string wstring_to_utf8(const wstring & wide_string) {
        wstring_convert<codecvt_utf8<wchar_t>> convert;
        return convert.to_bytes(wide_string);
    }

}


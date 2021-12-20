#pragma once

#include "daScript/misc/platform.h"

namespace das
{
    wstring utf8_to_wstring(const char * utf8_string);
    wstring utf8_to_wstring(const string & utf8_string);
    string wstring_to_utf8(const wstring & wide_string);
}

options gen2

module text_lines shared public

require strings
require daslib/strings_boost

//! A program's output as stripped lines, blank ones dropped
def non_empty_lines(text : string) : array<string> => <- [
    for (raw in split(text, "\n")); strip(raw); where !empty(strip(raw))]

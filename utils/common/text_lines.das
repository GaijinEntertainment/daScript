options gen2

module text_lines shared public

require strings
require daslib/strings_boost

//! The pieces of `text` between `sep`s, stripped, empty ones dropped
def split_trimmed(text : string; sep : string) : array<string> => <- [
    for (raw in split(text, sep)); strip(raw); where !empty(strip(raw))]

//! A program's output as stripped lines, blank ones dropped
def non_empty_lines(text : string) : array<string> => <- split_trimmed(text, "\n")

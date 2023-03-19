options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module base64 shared private

require daslib/strings

let private BASE64_PAD = '='
let private BASE64DE_FIRST = '+'
let private BASE64DE_LAST = 'z'

/* ASCII order for BASE 64 decode, -1 in unused character */
let base64de <- [[int
    /* '+'; ';'; '-'; '.'; '/'; '0'; '1'; '2'; */
        62;  -1;  -1;  -1;  63;  52;  53;  54;
    /* '3'; '4'; '5'; '6'; '7'; '8'; '9'; ':'; */
        55;  56;  57;  58;  59;  60;  61;  -1;
    /* ';'; '<'; '='; '>'; '?'; '@'; 'A'; 'B'; */
        -1;  -1;  -1;  -1;  -1;  -1;   0;   1;
    /* 'C'; 'D'; 'E'; 'F'; 'G'; 'H'; 'I'; 'J'; */
         2;   3;   4;   5;   6;   7;   8;   9;
    /* 'K'; 'L'; 'M'; 'N'; 'O'; 'P'; 'Q'; 'R'; */
        10;  11;  12;  13;  14;  15;  16;  17;
    /* 'S'; 'T'; 'U'; 'V'; 'W'; 'X'; 'Y'; 'Z'; */
        18;  19;  20;  21;  22;  23;  24;  25;
    /* '['; '\'; ']'; '^'; '_'; '`'; 'a'; 'b'; */
        -1;  -1;  -1;  -1;  -1;  -1;  26;  27;
    /* 'c'; 'd'; 'e'; 'f'; 'g'; 'h'; 'i'; 'j'; */
        28;  29;  30;  31;  32;  33;  34;  35;
    /* 'k'; 'l'; 'm'; 'n'; 'o'; 'p'; 'q'; 'r'; */
        36;  37;  38;  39;  40;  41;  42;  43;
    /* 's'; 't'; 'u'; 'v'; 'w'; 'x'; 'y'; 'z'; */
        44;  45;  46;  47;  48;  49;  50;  51
]]

def BASE64_DECODE_OUT_SIZE(s:int)
    return s / 4 * 3

def public base64_decode ( _in:string; var _out:array<uint8> ) : int
    var inp : array<int>
    let inlen = _in |> length
    inp |> reserve(inlen)
    for ch in _in
        inp |> push(ch)
    var out : array<int>
    out |> resize(BASE64_DECODE_OUT_SIZE(inlen))
    var j = 0
    for i in range(inp |> length)
        var c = 0
        var s = i % 4
        if inp[i] == BASE64_PAD
            return j
        if inp[i] < BASE64DE_FIRST || inp[i] > BASE64DE_LAST
            return -1
        c = base64de[inp[i] - BASE64DE_FIRST]
        if c == -1
            return -1
        if s == 0
            out[j] = int((uint(c) << 2u) & 0xFF)
            continue
        elif s == 1
            out[j++] += int((uint(c) >> 4u) & 0x3)
            //  if not last char with padding
            if i < (inlen - 3) || inp[inlen - 2] != BASE64_PAD
                out[j] = int((uint(c) & 0xF) << 4u)
            continue
        elif s == 2
            out[j++] += int((uint(c) >> 2u) & 0xF)
            // if not last char with padding
            if i < (inlen - 2) || inp[inlen - 1] != BASE64_PAD
                out[j] =  int((uint(c) & 0x3) << 6u)
            continue
        elif s == 3
            out[j++] += c
    _out |> resize(out |> length)
    for d,b in out,_out
        b = uint8(d)
    return j

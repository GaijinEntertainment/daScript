// CodeMirror simple-mode for daslang.
// Token set is sourced from daslang-keywords.js (canonical lexer keyword list).
// Token classes match forge.css selectors (.cm-keyword, .cm-type, ...).

(function (global) {
    if (!global.CodeMirror) {
        console.warn('daslang-mode: CodeMirror not loaded');
        return;
    }
    const KW = global.daslangKeywords;
    if (!KW) {
        console.warn('daslang-mode: daslangKeywords not loaded; load daslang-keywords.js first');
        return;
    }

    function identToken(match) {
        const w = match[0];
        if (KW.KEYWORDS.has(w)) return 'keyword';
        if (KW.TYPES.has(w)) return 'type';
        if (KW.ATOMS.has(w)) return 'atom';
        return 'variable';
    }

    global.CodeMirror.defineSimpleMode('daslang', {
        start: [
            { regex: /\/\/.*/, token: 'comment' },
            { regex: /\/\*/, token: 'comment', push: 'comment' },

            // Attribute at line start: `[name]`, `[hint(arg)]`, `[!expect_ref(v)]`.
            // sol-anchored so inline `arr[i]` subscripts don't get miscolored.
            { sol: true, regex: /(\s*)(\[!?[a-zA-Z_][^\]\n]*\])/, token: [null, 'meta'] },

            { regex: /"/, token: 'string', push: 'string' },
            { regex: /'(?:\\.|[^'\\])'(?:u8|U8|u|U)?/, token: 'string' },

            { regex: /0[xX][0-9a-fA-F][0-9a-fA-F_]*(?:u8|U8|ul|UL|u|U|l|L)?/, token: 'number' },
            { regex: /(?:\d[\d_]*\.\d[\d_]*(?:[eE][+\-]?\d+)?|\.\d[\d_]*(?:[eE][+\-]?\d+)?|\d[\d_]*[eE][+\-]?\d+)(?:lf|d|f|F)?/, token: 'number' },
            { regex: /\d[\d_]*(?:lf|d|f|F|u8|U8|ul|UL|u|U|l|L)?/, token: 'number' },

            // Multi-char operators (longest-match first).
            { regex: /\|\||&&|\^\^|<<=|>>=|<<|>>|<-|->|<\||\|>|=>|::|\+\+|--|==|!=|<=|>=|\+=|-=|\*=|\/=|%=|\^\^=|\|\|=|&&=|\^=|&=|\|=|@@|\?\?|\?\.|\?\[|:=/, token: 'operator' },

            { regex: /[a-zA-Z_][a-zA-Z0-9_]*/, token: identToken },

            { regex: /[+\-*\/%=<>!&|^~@$:;,.()\[\]{}?]/, token: 'operator' },
        ],

        comment: [
            { regex: /\/\*/, token: 'comment', push: 'comment' },
            { regex: /\*\//, token: 'comment', pop: true },
            { regex: /[^*\/]+/, token: 'comment' },
            { regex: /./, token: 'comment' },
        ],

        string: [
            { regex: /\\./, token: 'string' },
            { regex: /\{/, token: 'string-2', push: 'interp' },
            { regex: /"/, token: 'string', pop: true },
            { regex: /[^\\"{]+/, token: 'string' },
            { regex: /./, token: 'string' },
        ],

        // String interpolation `{...}` — reuses start-state tokenizing for the body.
        interp: [
            { regex: /\}/, token: 'string-2', pop: true },
            { regex: /\/\/.*/, token: 'comment' },
            { regex: /"/, token: 'string', push: 'string' },
            { regex: /\|\||&&|<-|->|<\||\|>|=>|::|==|!=|<=|>=|@@|\?\?|\?\.|\?\[|:=/, token: 'operator' },
            { regex: /0[xX][0-9a-fA-F][0-9a-fA-F_]*[uUlL]?/, token: 'number' },
            { regex: /\d[\d_]*(?:\.\d[\d_]*)?(?:[eE][+\-]?\d+)?(?:lf|d|f|F|u8|U8|ul|UL|u|U|l|L)?/, token: 'number' },
            { regex: /[a-zA-Z_][a-zA-Z0-9_]*/, token: identToken },
            { regex: /[+\-*\/%=<>!&|^~@$:;,.()\[\]?]/, token: 'operator' },
            { regex: /\s+/, token: null },
        ],

        meta: {
            dontIndentStates: ['comment', 'string', 'interp'],
            lineComment: '//',
        },
    });
})(typeof window !== 'undefined' ? window : globalThis);

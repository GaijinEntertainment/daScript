// Token-based highlighter for blog code blocks.
// Used by markdown-fenced `<pre><code class="language-daslang">` produced by
// site/blog/build_blog.py. The hero + playground now use CodeMirror — this
// file no longer ships an editable variant.
//
// Keyword/type sets come from site/files/cm/daslang-keywords.js when loaded;
// otherwise the local fallback (smaller, but always present) takes over so
// pre-generated blog pages keep working if the keywords script is unavailable.

(function (global) {
    const FALLBACK_KEYWORDS = new Set([
        'def', 'let', 'var', 'for', 'in', 'if', 'else', 'elif', 'while', 'return',
        'struct', 'class', 'require', 'module', 'options', 'export', 'typeinfo',
        'static_if', 'true', 'false', 'null', 'as', 'is', 'cast',
        'yield',
    ]);
    const FALLBACK_TYPES = new Set([
        'int', 'int2', 'int3', 'int4', 'uint', 'uint2', 'uint3', 'uint4',
        'float', 'float2', 'float3', 'float4', 'double', 'bool', 'string',
        'void', 'auto', 'array', 'table', 'range', 'tuple', 'variant',
        'lambda', 'generator', 'block', 'iterator',
    ]);

    function resolveSets() {
        const ext = global.daslangKeywords;
        if (ext) {
            // Atoms collapse into 'kw' so they get amber via .tok-kw.
            const KW = new Set([...ext.KEYWORDS, ...ext.ATOMS]);
            return { KEYWORDS: KW, TYPES: ext.TYPES };
        }
        return { KEYWORDS: FALLBACK_KEYWORDS, TYPES: FALLBACK_TYPES };
    }

    function tokenize(src) {
        const { KEYWORDS, TYPES } = resolveSets();
        const out = [];
        let i = 0;
        const push = (t, k) => out.push({ t, k });
        while (i < src.length) {
            const c = src[i];
            if (c === '/' && src[i + 1] === '/') {
                let j = i;
                while (j < src.length && src[j] !== '\n') j++;
                push(src.slice(i, j), 'c'); i = j; continue;
            }
            if (c === '"') {
                let j = i + 1;
                while (j < src.length && src[j] !== '"') {
                    if (src[j] === '\\') j++;
                    j++;
                }
                j = Math.min(j + 1, src.length);
                push(src.slice(i, j), 's'); i = j; continue;
            }
            if (c === '[') {
                let j = i;
                while (j < src.length && src[j] !== ']') j++;
                j = Math.min(j + 1, src.length);
                push(src.slice(i, j), 'a'); i = j; continue;
            }
            if (/[A-Za-z_]/.test(c)) {
                let j = i;
                while (j < src.length && /[A-Za-z0-9_]/.test(src[j])) j++;
                const w = src.slice(i, j);
                const k = KEYWORDS.has(w) ? 'kw' : TYPES.has(w) ? 'ty' : 'id';
                push(w, k); i = j; continue;
            }
            if (/[0-9]/.test(c)) {
                let j = i;
                while (j < src.length && /[0-9.]/.test(src[j])) j++;
                push(src.slice(i, j), 'n'); i = j; continue;
            }
            if (c === '<') {
                let j = i + 1;
                while (j < src.length && src[j] !== '>' && src[j] !== '\n') j++;
                if (src[j] === '>') { push(src.slice(i, j + 1), 'ty'); i = j + 1; continue; }
            }
            push(c, /\s/.test(c) ? 'w' : 'p');
            i++;
        }
        return out;
    }

    function esc(s) {
        return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    }

    // Walk all `<pre><code class="language-daslang">` (or "language-das")
    // and replace their contents with tokenized HTML. Idempotent.
    function highlightAll() {
        const blocks = document.querySelectorAll(
            'code.language-daslang, code.language-das, pre code:not(.tok-id):not(.tok-kw)');
        blocks.forEach(code => {
            const cls = code.className || '';
            if (cls && !/language-(das|daslang)/.test(cls)) return;
            const src = code.textContent;
            const tokens = tokenize(src);
            code.innerHTML = tokens.map(t =>
                `<span class="tok-${t.k}">${esc(t.t)}</span>`
            ).join('');
        });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', highlightAll);
    } else {
        setTimeout(highlightAll, 0);
    }

    global.dasHighlight = { tokenize, highlightAll };
})(window);

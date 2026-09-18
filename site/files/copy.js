// Without the clipboard API (plain http, a denied clipboard permission) the click selects the
// command text instead, so a manual copy is one keystroke away.
(function () {
    'use strict';

    const CLIP  = '<svg viewBox="0 0 16 16" width="14" height="14" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="1.4" d="M5.5 3.5h6a1 1 0 0 1 1 1v8a1 1 0 0 1-1 1h-6a1 1 0 0 1-1-1v-8a1 1 0 0 1 1-1z"/><path fill="none" stroke="currentColor" stroke-width="1.4" d="M3.5 11.5v-8a1 1 0 0 1 1-1h5"/></svg>';
    const CHECK = '<svg viewBox="0 0 16 16" width="14" height="14" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="1.8" d="M3 8.5l3 3 7-7"/></svg>';

    function commandText(el) {
        const clone = el.cloneNode(true);
        clone.querySelectorAll('.prompt, .forge-copy').forEach(n => n.remove());
        return clone.textContent.replace(/\s+/g, ' ').trim();
    }

    function selectCommand(el, btn) {
        const range = document.createRange();
        const prompt = el.querySelector('.prompt');
        if (prompt) {
            range.setStartAfter(prompt);
        } else {
            range.setStart(el, 0);
        }
        range.setEndBefore(btn);
        const sel = window.getSelection && window.getSelection();
        if (!sel) return;
        sel.removeAllRanges();
        sel.addRange(range);
    }

    function addCopyButton(el) {
        const btn = document.createElement('button');
        btn.className = 'forge-copy';
        btn.type = 'button';
        btn.title = 'copy';
        btn.setAttribute('aria-label', 'copy command');
        btn.innerHTML = CLIP;
        btn.addEventListener('click', ev => {
            ev.preventDefault();
            const text = commandText(el);
            const done = () => {
                btn.innerHTML = CHECK;
                btn.classList.add('is-done');
                setTimeout(() => { btn.innerHTML = CLIP; btn.classList.remove('is-done'); }, 1200);
            };
            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(text).then(done, () => selectCommand(el, btn));
            } else {
                selectCommand(el, btn);
            }
        });
        appendToLastWord(el, btn);
    }

    // the tail span is white-space: nowrap, so a wrapping command keeps the icon on its last line;
    // the word inside it may still break anywhere, so a long URL wraps instead of clipping
    function appendToLastWord(el, btn) {
        let last = el.lastChild;
        while (last && last.nodeType === Node.TEXT_NODE && !last.textContent.trim()) last = last.previousSibling;
        const tail = document.createElement('span');
        tail.className = 'forge-copy-tail';
        const word = document.createElement('span');
        word.className = 'forge-copy-word';
        if (last && last.nodeType === Node.TEXT_NODE) {
            const text = last.textContent.replace(/\s+$/, '');
            const cut = text.lastIndexOf(' ');
            if (cut >= 0) {
                last.textContent = text.slice(0, cut + 1);
                word.appendChild(document.createTextNode(text.slice(cut + 1)));
            } else {
                word.appendChild(last);
            }
        } else if (last) {
            // an element tail (a link) is a flex row: the link shrinks and wraps inside, the icon stays beside it
            tail.classList.add('forge-copy-tail--flex');
            word.appendChild(last);
        }
        tail.appendChild(word);
        tail.appendChild(btn);
        el.appendChild(tail);
    }

    function init() {
        document.querySelectorAll('.forge-install__cmd, code.forge-cmd').forEach(addCopyButton);
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }
})();

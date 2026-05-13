// Multi-file state for the playground.
//
// pgState holds one CodeMirror.Doc per file, plus the name of the active one.
// editor.swapDoc swaps the visible buffer without re-creating the editor, so
// each file keeps its own undo history and modification state.
//
// Wiring:
//   - pageInit (main.js) creates `window.code`; this file polls until that's
//     true, then constructs pgState seeded with the editor's current buffer
//     as main.das.
//   - loadSample() (main.js) calls pgLoadFiles when a multi-file sample lands.
//   - runCode() (main.js) iterates pgState before calling Module.callMain.
//   - Autosave debounced into localStorage on every CM change.
//
// main.das is fixed name: cannot be deleted, cannot be renamed. Its tab still
// shows a × button for visual symmetry but it's disabled.

(function () {
    const STORAGE_KEY = 'daslang.playground.state.v1';
    const AUTOSAVE_DEBOUNCE_MS = 250;
    const ENTRY = 'main.das';

    let pgState = null;
    let tabsEl = null;
    let editorMode = 'daslang';
    let autosaveTimer = null;

    function $(sel) { return document.querySelector(sel); }

    // ─── State helpers ────────────────────────────────────────────────

    function makeDoc(text) {
        return new CodeMirror.Doc(text || '', editorMode);
    }

    function getStateJson() {
        return {
            files: Object.fromEntries(
                Object.entries(pgState.files).map(([k, doc]) => [k, doc.getValue()])
            ),
            active: pgState.active,
        };
    }

    function autosave() {
        if (!pgState) return;
        if (autosaveTimer) clearTimeout(autosaveTimer);
        autosaveTimer = setTimeout(() => {
            try {
                localStorage.setItem(STORAGE_KEY, JSON.stringify(getStateJson()));
            } catch (e) { /* quota / disabled — ignore */ }
        }, AUTOSAVE_DEBOUNCE_MS);
    }

    function attachAutosave(doc) {
        doc.on('change', autosave);
    }

    function loadAutosave() {
        try {
            const raw = localStorage.getItem(STORAGE_KEY);
            if (!raw) return null;
            const obj = JSON.parse(raw);
            if (!obj || typeof obj !== 'object' || !obj.files) return null;
            return obj;
        } catch (e) { return null; }
    }

    // ─── Tab strip rendering ──────────────────────────────────────────

    function renderTabs() {
        if (!tabsEl) return;
        const names = Object.keys(pgState.files);
        const html = names.map(name => {
            const isActive = name === pgState.active;
            const isEntry = name === ENTRY;
            return (
                '<div class="pg-tab' + (isActive ? ' is-active' : '') + (isEntry ? ' is-entry' : '') + '" data-file="' + escAttr(name) + '">' +
                    '<span class="pg-tab__name" data-file="' + escAttr(name) + '">' + esc(name) + '</span>' +
                    '<button class="pg-tab__close" type="button" data-file="' + escAttr(name) + '"' +
                        (isEntry ? ' disabled title="main.das is the entry point"' : ' title="close file"') + '>×</button>' +
                '</div>'
            );
        }).join('');
        tabsEl.innerHTML = html + '<button class="pg-tab__add" id="pg-tab-add" type="button" title="new file">+</button>';
    }

    function esc(s) { return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;'); }
    function escAttr(s) { return esc(s).replace(/"/g, '&quot;'); }

    // ─── Operations ───────────────────────────────────────────────────

    function pgSwitchFile(name) {
        if (!(name in pgState.files)) return;
        pgState.active = name;
        window.code.swapDoc(pgState.files[name]);
        // Toggle classes in place — rebuilding the tab DOM here breaks
        // dblclick rename (second click lands on a freshly-mounted node).
        if (tabsEl) {
            tabsEl.querySelectorAll('.pg-tab').forEach(t => {
                t.classList.toggle('is-active', t.dataset.file === name);
            });
        }
    }

    function pgAddFile(name) {
        if (!name) {
            let i = 1;
            while (('untitled' + i + '.das') in pgState.files) i++;
            name = 'untitled' + i + '.das';
        }
        if (name in pgState.files) return false;
        const doc = makeDoc('');
        attachAutosave(doc);
        pgState.files[name] = doc;
        renderTabs();
        pgSwitchFile(name);
        autosave();
        return true;
    }

    function pgDeleteFile(name) {
        if (name === ENTRY) return false;
        if (!(name in pgState.files)) return false;
        const doc = pgState.files[name];
        if (doc.getValue().trim().length > 0) {
            if (!window.confirm('Discard "' + name + '"?')) return false;
        }
        delete pgState.files[name];
        const switchTo = pgState.active === name ? ENTRY : pgState.active;
        renderTabs();
        if (switchTo !== pgState.active) pgSwitchFile(switchTo);
        autosave();
        return true;
    }

    function pgRenameFile(oldName, newName) {
        if (oldName === ENTRY) return false;
        newName = (newName || '').trim();
        if (!newName) return false;
        if (!/^[A-Za-z0-9_./-]+\.das$/.test(newName)) {
            window.alert('Filename must end in .das and contain only [A-Za-z0-9_./-].');
            return false;
        }
        if (newName === oldName) return true;
        if (newName in pgState.files) {
            window.alert('"' + newName + '" already exists.');
            return false;
        }
        // Preserve insertion order so the tab order stays stable.
        const next = {};
        for (const k of Object.keys(pgState.files)) {
            next[k === oldName ? newName : k] = pgState.files[k];
        }
        pgState.files = next;
        if (pgState.active === oldName) pgState.active = newName;
        renderTabs();
        autosave();
        return true;
    }

    function pgLoadFiles(filesByName, activeOverride) {
        if (!filesByName || typeof filesByName !== 'object') return;
        // Build fresh Doc per file. Preserve insertion order, but if main.das
        // is present, ensure it comes first.
        const names = Object.keys(filesByName);
        const ordered = names.includes(ENTRY)
            ? [ENTRY, ...names.filter(n => n !== ENTRY)]
            : names;
        const newFiles = {};
        for (const name of ordered) {
            const doc = makeDoc(filesByName[name]);
            attachAutosave(doc);
            newFiles[name] = doc;
        }
        pgState.files = newFiles;
        pgState.active = activeOverride && activeOverride in newFiles
            ? activeOverride
            : (ordered.includes(ENTRY) ? ENTRY : ordered[0]);
        window.code.swapDoc(pgState.files[pgState.active]);
        renderTabs();
        autosave();
    }

    // ─── Tab strip event delegation ───────────────────────────────────

    function bindTabsHandlers() {
        if (!tabsEl) return;

        tabsEl.addEventListener('click', (e) => {
            const closeBtn = e.target.closest('.pg-tab__close');
            const addBtn = e.target.closest('.pg-tab__add');
            const tab = e.target.closest('.pg-tab');
            if (closeBtn && !closeBtn.disabled) {
                pgDeleteFile(closeBtn.dataset.file);
                return;
            }
            if (addBtn) {
                pgAddFile();
                return;
            }
            if (tab) {
                pgSwitchFile(tab.dataset.file);
            }
        });

        tabsEl.addEventListener('dblclick', (e) => {
            const nameEl = e.target.closest('.pg-tab__name');
            if (!nameEl) return;
            const name = nameEl.dataset.file;
            if (name === ENTRY) return;
            startRename(nameEl, name);
        });
    }

    function startRename(nameEl, oldName) {
        const input = document.createElement('input');
        input.type = 'text';
        input.className = 'pg-tab__rename';
        input.value = oldName;
        input.spellcheck = false;

        let done = false;
        function commit(apply) {
            if (done) return;
            done = true;
            input.remove();
            if (apply) pgRenameFile(oldName, input.value);
            // Re-render either way to drop the input from the DOM cleanly.
            renderTabs();
        }
        input.addEventListener('blur', () => commit(true));
        input.addEventListener('keydown', (ev) => {
            if (ev.key === 'Enter') { ev.preventDefault(); input.blur(); }
            else if (ev.key === 'Escape') { ev.preventDefault(); commit(false); }
        });
        // Replace the name span with the input in-place.
        nameEl.replaceWith(input);
        input.focus();
        input.select();
    }

    // ─── Init ─────────────────────────────────────────────────────────

    function tryInit() {
        if (typeof CodeMirror === 'undefined' ||
            typeof window.code === 'undefined' || window.code === null ||
            typeof window.code.swapDoc !== 'function') {
            setTimeout(tryInit, 30);
            return;
        }
        // Use the mode the editor was constructed with.
        editorMode = window.code.getOption('mode') || 'daslang';
        tabsEl = document.getElementById('pg-tabs');
        if (!tabsEl) return;

        // Seed pgState. Priority: hash > pending sample bundle > autosave > current editor.
        const initialDoc = window.code.getDoc();
        pgState = {
            files: { [ENTRY]: initialDoc },
            active: ENTRY,
        };
        attachAutosave(initialDoc);

        // Expose globals before applying pending sources so handlers can use them.
        window.pgState = pgState;
        window.pgLoadFiles = pgLoadFiles;
        window.pgSwitchFile = pgSwitchFile;
        window.pgAddFile = pgAddFile;
        window.pgDeleteFile = pgDeleteFile;
        window.pgRenameFile = pgRenameFile;
        window.pgStateJson = getStateJson;

        bindTabsHandlers();
        renderTabs();

        // Priority: URL hash > autosave > default sample > empty editor.
        // When restored from hash or autosave we set pgRestoredFromState so
        // main.js skips its default selectSample("examples", 0) call (whose
        // async fetch would otherwise clobber the restored state ~200ms in).
        const hash = window.location.hash || '';
        const hasHash = hash.startsWith('#code=') || hash.startsWith('#z=');

        if (hasHash && window.__pendingSampleBundle) {
            const bundle = window.__pendingSampleBundle;
            window.__pendingSampleBundle = null;
            pgLoadFiles(bundle);
            window.pgRestoredFromState = true;
            return;
        }

        const saved = !hasHash ? loadAutosave() : null;
        if (saved && saved.files && Object.keys(saved.files).length > 0) {
            pgLoadFiles(saved.files, saved.active);
            window.pgRestoredFromState = true;
            return;
        }

        // No autosave: let the pending default sample (if any) drop in.
        if (window.__pendingSampleBundle) {
            const bundle = window.__pendingSampleBundle;
            window.__pendingSampleBundle = null;
            pgLoadFiles(bundle);
        }
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', tryInit);
    } else {
        tryInit();
    }
})();

options gen2
options indenting = 4
options no_aot   // no AOT gen exists for this module yet (module-move fallout) — test_aot interprets it

module clipboard shared

require clipboard_core public
require daslib/utf8_utils

//! Platform-neutral system clipboard access.
//!
//! Text crossing this boundary is UTF-8. Desktop operations are synchronous;
//! unsupported platforms and sessions without an available desktop report an
//! explicit status instead of falling back to a process-local clipboard.

enum public ClipboardStatus {
    ok
    no_text
    busy
    unavailable
    unsupported
    invalid_utf8
    failed
    no_content
    invalid_image
}

bitfield public ClipboardCapabilities {
    read_text
    write_text
    clear
    synchronous
    read_rich_text
    write_rich_text
    read_image
    write_image
}

bitfield public ClipboardFormats {
    plain_text
    markdown
    html
    uri_list
    image_rgba8
}

struct public ClipboardImage {
    //! Canonical transport image: straight-alpha RGBA8, top-to-bottom rows.
    width : int = 0
    height : int = 0
    stride : int = 0
    rgba8 : array<uint8>
}

struct public ClipboardContent {
    //! One atomic offer set. Only fields selected by `formats` participate.
    formats : ClipboardFormats
    plain_text : string
    markdown : string
    //! UTF-8 HTML fragment. Native transports add/remove platform wrappers.
    html : string
    uri_list : string
    image : ClipboardImage = ClipboardImage()
}

struct public ClipboardMemory {
    //! Deterministic in-memory transport for tests, tools, and applications
    //! that need clipboard semantics without touching the desktop session.
    content : ClipboardContent = ClipboardContent()
    has_content : bool = false
    revision : int = 0
}

def public clipboard_content_has_data(content : ClipboardContent const) : bool {
    let f = content.formats
    return f.plain_text || f.markdown || f.html || f.uri_list || f.image_rgba8
}

def public clipboard_validate_content(content : ClipboardContent const) : ClipboardStatus {
    let f = content.formats
    if ((f.plain_text && !is_utf8_string_valid(content.plain_text))
            || (f.markdown && !is_utf8_string_valid(content.markdown))
            || (f.html && !is_utf8_string_valid(content.html))
            || (f.uri_list && !is_utf8_string_valid(content.uri_list))) {
        return ClipboardStatus.invalid_utf8
    }
    if (f.image_rgba8) {
        if (content.image.width <= 0 || content.image.height <= 0
                || content.image.stride <= 0) {
            return ClipboardStatus.invalid_image
        }
        let row_bytes = int64(content.image.width) * 4l
        let data_bytes = int64(content.image.stride) * int64(content.image.height)
        if (int64(content.image.stride) < row_bytes
                || data_bytes != long_length(content.image.rgba8)) {
            return ClipboardStatus.invalid_image
        }
    }
    return ClipboardStatus.ok
}

def public clipboard_dispose_content(var content : ClipboardContent) {
    delete content.image.rgba8
    content = ClipboardContent()  // nolint:PERF030 - the only owned field is deleted above; strings are borrowed, so a whole-struct delete would double-free
}

def public clipboard_clone_content(content : ClipboardContent const) : ClipboardContent {
    let f = content.formats
    var result = ClipboardContent(formats = f)
    if (f.plain_text) {
        result.plain_text = content.plain_text
    }
    if (f.markdown) {
        result.markdown = content.markdown
    }
    if (f.html) {
        result.html = content.html
    }
    if (f.uri_list) {
        result.uri_list = content.uri_list
    }
    if (f.image_rgba8) {
        result.image = ClipboardImage(
            width = content.image.width,
            height = content.image.height,
            stride = content.image.stride)
        result.image.rgba8 := content.image.rgba8
    }
    return <- result
}

def public clipboard_memory_write(var memory : ClipboardMemory;
                                  content : ClipboardContent const) : ClipboardStatus {
    let status = clipboard_validate_content(content)
    if (status != ClipboardStatus.ok) return status
    clipboard_dispose_content(memory.content)
    memory.has_content = clipboard_content_has_data(content)
    if (memory.has_content) {
        memory.content <- clipboard_clone_content(content)
    }
    memory.revision++
    return ClipboardStatus.ok
}

def public clipboard_memory_read(memory : ClipboardMemory const;
                                 var content : ClipboardContent) : ClipboardStatus {
    clipboard_dispose_content(content)
    if (!memory.has_content) return ClipboardStatus.no_content
    content <- clipboard_clone_content(memory.content)  // nolint:PERF030 - released by clipboard_dispose_content above; the rule tracks only direct delete
    return ClipboardStatus.ok
}

def public clipboard_memory_clear(var memory : ClipboardMemory) {
    clipboard_dispose_content(memory.content)
    memory.has_content = false
    memory.revision++
}

def public clipboard_memory_dispose(var memory : ClipboardMemory) {
    clipboard_dispose_content(memory.content)
    memory = ClipboardMemory()  // nolint:PERF030 - its only owned field is released by clipboard_dispose_content above
}

def public clipboard_capabilities() : ClipboardCapabilities {
    return unsafe(reinterpret<ClipboardCapabilities>(_clipboard_capabilities()))
}

def public clipboard_set_text(text : string) : ClipboardStatus {
    //! Replace the system clipboard with UTF-8 text. Empty text clears it.
    if (!is_utf8_string_valid(text)) return ClipboardStatus.invalid_utf8
    return unsafe(reinterpret<ClipboardStatus>(
        _clipboard_set_text(text, length(text))))
}

def public clipboard_set_content(content : ClipboardContent const) : ClipboardStatus {
    //! Replace the system clipboard with all selected representations while
    //! holding one native clipboard lock. An empty offer set clears it.
    let validation = clipboard_validate_content(content)
    if (validation != ClipboardStatus.ok) return validation
    return unsafe(reinterpret<ClipboardStatus>(_clipboard_set_content(
        unsafe(reinterpret<int>(content.formats)),
        content.plain_text, length(content.plain_text),
        content.markdown, length(content.markdown),
        content.html, length(content.html),
        content.uri_list, length(content.uri_list),
        content.image.rgba8,
        content.image.width, content.image.height, content.image.stride)))
}

def public clipboard_get_content(var content : ClipboardContent) : ClipboardStatus {
    //! Take one native snapshot of every supported representation.
    clipboard_dispose_content(content)
    var raw_formats = 0
    let raw_status = _clipboard_get_content(
        raw_formats,
        content.plain_text, content.markdown, content.html, content.uri_list,
        content.image.rgba8,
        content.image.width, content.image.height, content.image.stride)
    let status = unsafe(reinterpret<ClipboardStatus>(raw_status))
    if (status != ClipboardStatus.ok) {
        clipboard_dispose_content(content)
        return status
    }
    content.formats = unsafe(reinterpret<ClipboardFormats>(raw_formats))
    let validation = clipboard_validate_content(content)
    if (validation != ClipboardStatus.ok) {
        clipboard_dispose_content(content)
        return validation
    }
    return ClipboardStatus.ok
}

def public clipboard_get_text(var text : string&) : ClipboardStatus {
    //! Read UTF-8 text into `text`. The output is empty for every non-ok result.
    var raw_status = int(ClipboardStatus.failed)
    text = _clipboard_get_text(raw_status)
    let status = unsafe(reinterpret<ClipboardStatus>(raw_status))
    if (status == ClipboardStatus.ok && !is_utf8_string_valid(text)) {
        text = ""
        return ClipboardStatus.invalid_utf8
    }
    return status
}

def public clipboard_clear() : ClipboardStatus {
    return unsafe(reinterpret<ClipboardStatus>(_clipboard_clear()))
}

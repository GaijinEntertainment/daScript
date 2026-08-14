options gen2
options indenting = 4

require dastest/testing_boost public
require clipboard/clipboard
require strings

def private equal_bytes(a, b : array<uint8> const) : bool {
    if (length(a) != length(b)) return false
    for (ea, eb in a, b) {
        if (ea != eb) return false
    }
    return true
}

def private make_all_formats() : ClipboardContent {
    var content = ClipboardContent(
        plain_text = "Plain Привет 日本語 👩‍💻\r\nsecond line",
        markdown = "**Bold Привет** and [文書](editor://workspace/文書)",
        html = "<p><strong>Bold Привет</strong> and <a href=\"editor://workspace/文書\">文書</a></p>",
        uri_list = "editor://workspace/文書\r\nhttps://example.com/naïve\r\n",
        image = ClipboardImage(width = 2, height = 2, stride = 12))
    content.formats.plain_text = true
    content.formats.markdown = true
    content.formats.html = true
    content.formats.uri_list = true
    content.formats.image_rgba8 = true
    content.image.rgba8 <- [
        255u8, 0u8, 0u8, 255u8, 0u8, 255u8, 0u8, 128u8, 9u8, 8u8, 7u8, 6u8,
        0u8, 0u8, 255u8, 64u8, 255u8, 255u8, 255u8, 0u8, 5u8, 4u8, 3u8, 2u8
    ]
    return <- content
}

[test]
def test_clipboard_memory_atomic_all_format_roundtrip(t : T?) {
    var memory = ClipboardMemory()
    var offered <- make_all_formats()
    let expected_plain = offered.plain_text
    let expected_markdown = offered.markdown
    let expected_html = offered.html
    let expected_uri_list = offered.uri_list
    var expected_image : array<uint8>
    expected_image := offered.image.rgba8

    t |> equal(clipboard_memory_write(memory, offered), ClipboardStatus.ok,
               "atomic offer set validates and writes")
    t |> equal(memory.revision, 1, "one atomic write has one revision")

    // The transport owns a deep snapshot, not aliases into client buffers.
    offered.plain_text = "mutated after write"
    offered.image.rgba8[0] = 0u8
    var actual = ClipboardContent()
    t |> equal(clipboard_memory_read(memory, actual), ClipboardStatus.ok,
               "in-memory transport reads the offer set")
    t |> success(actual.formats.plain_text && actual.formats.markdown
              && actual.formats.html && actual.formats.uri_list
              && actual.formats.image_rgba8,
                 "all canonical formats survive one transaction")
    t |> equal(actual.plain_text, expected_plain, "plain UTF-8 roundtrip")
    t |> equal(actual.markdown, expected_markdown, "Markdown UTF-8 roundtrip")
    t |> equal(actual.html, expected_html, "HTML UTF-8 roundtrip")
    t |> equal(actual.uri_list, expected_uri_list, "URI-list UTF-8 roundtrip")
    t |> equal(actual.image.width, 2, "image width roundtrip")
    t |> equal(actual.image.height, 2, "image height roundtrip")
    t |> equal(actual.image.stride, 12, "padded image stride roundtrip")
    t |> success(equal_bytes(actual.image.rgba8, expected_image),
                 "RGBA8 bytes and row padding roundtrip")

    // Reads are deep snapshots too.
    actual.image.rgba8[1] = 99u8
    var second = ClipboardContent()
    t |> equal(clipboard_memory_read(memory, second), ClipboardStatus.ok,
               "same stored revision can be read again")
    t |> success(equal_bytes(second.image.rgba8, expected_image),
                 "mutating one read cannot alter the transport")

    clipboard_dispose_content(offered)
    clipboard_dispose_content(actual)
    clipboard_dispose_content(second)
    delete expected_image
    clipboard_memory_dispose(memory)
}

[test]
def test_clipboard_memory_clear_and_empty_offer(t : T?) {
    var memory = ClipboardMemory()
    var content = ClipboardContent(plain_text = "ignored without a format bit")
    t |> equal(clipboard_memory_write(memory, content), ClipboardStatus.ok,
               "empty offer set is a successful clear")
    t |> success(!memory.has_content, "unselected fields do not become clipboard data")
    var actual = ClipboardContent(plain_text = "stale")
    actual.formats.plain_text = true
    t |> equal(clipboard_memory_read(memory, actual), ClipboardStatus.no_content,
               "cleared transport reports no content")
    t |> success(!clipboard_content_has_data(actual) && empty(actual.plain_text),
                 "non-ok read clears the output")

    content.formats.plain_text = true
    t |> equal(clipboard_memory_write(memory, content), ClipboardStatus.ok,
               "selected text writes after clear")
    let before_clear = memory.revision
    clipboard_memory_clear(memory)
    t |> equal(memory.revision, before_clear + 1, "explicit clear is one revision")
    t |> success(!memory.has_content, "explicit clear removes content")
    clipboard_dispose_content(content)
    clipboard_dispose_content(actual)
    clipboard_memory_dispose(memory)
}

[test]
def test_clipboard_clone_only_selected_formats(t : T?) {
    var content = ClipboardContent(
        plain_text = "",
        markdown = "unselected Markdown",
        html = "unselected HTML",
        uri_list = "unselected URI",
        image = ClipboardImage(width = 1, height = 1, stride = 4))
    content.formats.plain_text = true
    content.image.rgba8 <- [1u8, 2u8, 3u8, 4u8]

    var cloned <- clipboard_clone_content(content)
    t |> success(cloned.formats.plain_text && empty(cloned.plain_text),
                 "selected empty plain text remains an explicit offer")
    t |> success(empty(cloned.markdown) && empty(cloned.html) && empty(cloned.uri_list),
                 "unselected strings are not retained")
    t |> success(empty(cloned.image.rgba8) && cloned.image.width == 0
              && cloned.image.height == 0 && cloned.image.stride == 0,
                 "unselected image storage and metadata are not retained")

    clipboard_dispose_content(content)
    clipboard_dispose_content(cloned)
}

[test]
def test_clipboard_content_validation(t : T?) {
    let invalid_utf8 = string([uint8(0xC3), uint8(0x28)])
    var content = ClipboardContent()

    content.formats.plain_text = true
    content.plain_text = invalid_utf8
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_utf8,
               "plain text rejects invalid UTF-8")
    content.formats.plain_text = false
    content.formats.markdown = true
    content.markdown = invalid_utf8
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_utf8,
               "Markdown rejects invalid UTF-8")
    content.formats.markdown = false
    content.formats.html = true
    content.html = invalid_utf8
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_utf8,
               "HTML rejects invalid UTF-8")
    content.formats.html = false
    content.formats.uri_list = true
    content.uri_list = invalid_utf8
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_utf8,
               "URI list rejects invalid UTF-8")

    content.formats.uri_list = false
    content.formats.image_rgba8 = true
    content.image = ClipboardImage(width = 0, height = 2, stride = 8)
    content.image.rgba8 |> resize(16)
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_image,
               "image dimensions must be positive")
    content.image.width = 3
    content.image.stride = 8
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_image,
               "image stride must hold one RGBA8 row")
    content.image.stride = 12
    t |> equal(clipboard_validate_content(content), ClipboardStatus.invalid_image,
               "image buffer length must match stride times height")
    content.image.rgba8 |> resize(24)
    t |> equal(clipboard_validate_content(content), ClipboardStatus.ok,
               "valid padded RGBA8 image is accepted")

    clipboard_dispose_content(content)
}

[test]
def test_clipboard_native_capabilities_are_coherent(t : T?) {
    let capabilities = clipboard_capabilities()
    let has_base = (capabilities.read_text || capabilities.write_text
                 || capabilities.clear || capabilities.synchronous)
    t |> success(!has_base || (capabilities.read_text && capabilities.write_text
              && capabilities.clear && capabilities.synchronous),
                 "native text operations and synchronous semantics are one capability set")
    t |> success(capabilities.read_rich_text == capabilities.write_rich_text,
                 "rich-text read and write support are symmetric")
    t |> success(capabilities.read_image == capabilities.write_image,
                 "image read and write support are symmetric")
    t |> success(!capabilities.read_rich_text || has_base,
                 "rich-text support implies the native base transport")
    t |> success(!capabilities.read_image || has_base,
                 "image support implies the native base transport")
}

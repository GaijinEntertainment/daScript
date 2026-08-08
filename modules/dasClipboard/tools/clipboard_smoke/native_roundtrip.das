options gen2
options indenting = 4

require clipboard/clipboard

let PROBE_MARKER = "daScript clipboard smoke - Привет 日本語 👩‍💻"

def private equal_bytes(a, b : array<uint8> const) : bool {
    if (length(a) != length(b)) return false
    for (i in range(length(a))) {
        if (a[i] != b[i]) return false
    }
    return true
}

def private make_probe() : ClipboardContent {
    var probe = ClipboardContent(
        plain_text = PROBE_MARKER,
        markdown = "**daScript smoke** - [文書](editor://workspace/文書)",
        html = "<p><strong>daScript smoke</strong> - <a href=\"editor://workspace/文書\">文書</a></p>",
        uri_list = "editor://workspace/文書\r\nhttps://example.com/naïve\r\n",
        image = ClipboardImage(width = 2, height = 2, stride = 8))
    probe.formats.plain_text = true
    probe.formats.markdown = true
    probe.formats.html = true
    probe.formats.uri_list = true
    probe.formats.image_rgba8 = true
    probe.image.rgba8 <- [
        255u8, 0u8, 0u8, 255u8, 0u8, 255u8, 0u8, 128u8,
        0u8, 0u8, 255u8, 64u8, 255u8, 255u8, 255u8, 0u8
    ]
    return <- probe
}

def private same_probe(actual, expected : ClipboardContent const) : bool {
    return (actual.formats.plain_text &&
        actual.formats.markdown &&
        actual.formats.html &&
        actual.formats.uri_list &&
        actual.formats.image_rgba8 &&
        actual.plain_text == expected.plain_text &&
        actual.markdown == expected.markdown &&
        actual.html == expected.html &&
        actual.uri_list == expected.uri_list &&
        actual.image.width == expected.image.width &&
        actual.image.height == expected.image.height &&
        actual.image.stride == expected.image.stride &&
        equal_bytes(actual.image.rgba8, expected.image.rgba8))
}

[export]
def main() {
    var before = ClipboardContent()
    let before_status = clipboard_get_content(before)
    var probe <- make_probe()
    let write_status = clipboard_set_content(probe)
    var actual = ClipboardContent()
    let read_status = clipboard_get_content(actual)
    let passed = (write_status == ClipboardStatus.ok &&
        read_status == ClipboardStatus.ok &&
        same_probe(actual, probe))

    // Restore only while our marker still owns the clipboard. If another app
    // or the user copied something during the smoke, their newer data wins.
    var guard = ClipboardContent()
    let guard_status = clipboard_get_content(guard)
    let still_ours = (guard_status == ClipboardStatus.ok &&
        guard.formats.plain_text &&
        guard.plain_text == PROBE_MARKER)
    var restored = false
    if (still_ours) {
        if (before_status == ClipboardStatus.ok) {
            restored = clipboard_set_content(before) == ClipboardStatus.ok
        } elif (before_status == ClipboardStatus.no_content) {
            restored = clipboard_clear() == ClipboardStatus.ok
        }
    }

    clipboard_dispose_content(before)
    clipboard_dispose_content(probe)
    clipboard_dispose_content(actual)
    clipboard_dispose_content(guard)

    if (!passed) {
        panic("clipboard smoke failed (write={write_status}, read={read_status})")
    }
    if (!still_ours) {
        panic("clipboard smoke passed, but restore was skipped because clipboard ownership changed")
    }
    if (!restored) {
        panic("clipboard smoke passed, but supported-subset restore failed")
    }
    print("PASS: native plain/Markdown/HTML/URI/RGBA8 atomic roundtrip; clipboard restored\n")
}

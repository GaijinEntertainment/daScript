options gen2
options indenting = 4

require clipboard/clipboard
require daslib/clargs

let DAS_MARKER = "daScript -> .NET clipboard smoke - 文書"
let EXTERNAL_MARKER = ".NET -> daScript clipboard smoke - Привет"

def private expected_image() : array<uint8> {
    return <- [
        255u8, 0u8, 0u8, 255u8, 0u8, 255u8, 0u8, 255u8,
        0u8, 0u8, 255u8, 255u8, 255u8, 255u8, 255u8, 255u8
    ]
}

def private equal_bytes(a, b : array<uint8> const) : bool {
    if (length(a) != length(b)) return false
    for (i in range(length(a))) {
        if (a[i] != b[i]) return false
    }
    return true
}

def private write_for_external_consumer() {
    var content = ClipboardContent(
        plain_text = DAS_MARKER,
        markdown = "**daScript to .NET** - [文書](editor://workspace/文書)",
        html = "<p><strong>daScript to .NET</strong> - <a href=\"editor://workspace/文書\">文書</a></p>",
        uri_list = "editor://workspace/文書\r\nhttps://example.com/naïve\r\n",
        image = ClipboardImage(width = 2, height = 2, stride = 8))
    content.formats.plain_text = true
    content.formats.markdown = true
    content.formats.html = true
    content.formats.uri_list = true
    content.formats.image_rgba8 = true
    content.image.rgba8 <- expected_image()
    let status = clipboard_set_content(content)
    clipboard_dispose_content(content)
    if (status != ClipboardStatus.ok) panic("daScript external-offer write failed: {status}")
}

def private read_external_consumer_offer() {
    var content = ClipboardContent()
    let status = clipboard_get_content(content)
    var pixels <- expected_image()
    let valid = (status == ClipboardStatus.ok &&
        content.formats.plain_text && content.plain_text == EXTERNAL_MARKER &&
        content.formats.markdown && content.markdown == "**.NET to daScript** - Привет" &&
        content.formats.html && content.html == "<p><strong>.NET to daScript</strong> - Привет</p>" &&
        content.formats.uri_list && content.uri_list == "https://example.com/from-dotnet\r\n" &&
        content.formats.image_rgba8 &&
        content.image.width == 2 && content.image.height == 2 && content.image.stride == 8 &&
        equal_bytes(content.image.rgba8, pixels))
    clipboard_dispose_content(content)
    delete pixels
    if (!valid) panic("external consumer offer did not roundtrip through daScript (status={status})")
}

[export]
def main() {
    let args <- get_user_args()
    if (length(args) != 1) panic("usage: external_probe.das -- write|read")
    if (args[0] == "write") {
        write_for_external_consumer()
    } elif (args[0] == "read") {
        read_external_consumer_offer()
    } else {
        panic("unknown clipboard smoke mode '{args[0]}'")
    }
}

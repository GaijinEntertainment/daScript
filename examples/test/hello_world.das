// options log = true

require uriparser

[export]
def test


    let uri = "file:///d%3A/dev/cpp/daScript/cmake_temp/Debug/mod.das"
    let path = uri_to_windows_file_name(uri)
    print("uri = {uri}\npath = {path}\n")

    return true

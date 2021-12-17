require uriparser


[export]
def main

    print("uri size is {typeinfo(sizeof type<Uri>)}\n")

    var dir = "dir"
    print("\n{dir}\n")
    Uri("test.das") |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "/dir"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "./dir"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "file://dir"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "file:///dir"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "file://./dir"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()


    // dir = "file://dir/"
    // print("\n{dir}\n")
    // Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    // dir = "file:///dir/"
    // print("\n{dir}\n")
    // Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    // Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()

    dir = "file://./dir/"
    print("\n{dir}\n")
    Uri("test.das") |> add_base_uri <| Uri(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_unix_file_name(dir) |> to_file_name |> debug()
    Uri("test.das") |> add_base_uri <| uri_from_windows_file_name(dir) |> to_file_name |> debug()



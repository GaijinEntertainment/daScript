require daslib/extract_method

[export]
def main
    var t <- [{for x in 3..7; x*x}]
    var q = 13
    extract_method("foobar") <|
        q ++
        print("{q} {t}\n")
    print("upsipe\n")

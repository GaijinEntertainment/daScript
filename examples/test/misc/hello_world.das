require daslib/extract_method

[export]
def main
    var t <- [{for x in 3..7; x*x}]
    var q = 13
    extract_method("method") <|
        q ++
        print("{q} {t}\n")
    print("upsipe\n")
    var x = extract_expression("expression",q*3+27)
    print("x = {x}\n")
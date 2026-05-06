options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// Read-only mentions of `dst.X` (e.g. `if (dst.X == "")`) must NOT suppress the
// auto-derived `dst.X = old.X`. A previous broader visitor catching ANY ExprField
// under `dst` would silently leave dst.X at default — copying produced empty rows
// instead of the source data.

[sql_table(name = "users", legacy = true)]
struct OldUser {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[sql_table(name = "users")]
struct NewUser {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def convert_readonly_mention(old : OldUser; var dst : NewUser) {
    // Read-only mention of dst.Name — auto-fill MUST still emit `dst.Name = old.Name`.
    if (dst.Name == "") {
        // Path is unreachable in practice; the conditional exists to anchor the read mention.
    }
    // Read-only mention of dst.Email used as RHS — same: auto-fill MUST still copy.
    let _peek = dst.Email
}

[test]
def test_readonly_mentions_preserve_auto_fill(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Email TEXT NOT NULL)")
    db |> exec("INSERT INTO users (Id, Name, Email) VALUES (1, 'alice', 'alice@x')")
    db |> convert_and_rename(type<OldUser>, type<NewUser>)
    let landed_name = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    let landed_email = db |> query_scalar("SELECT Email FROM users WHERE Id=1", type<string>)
    t |> equal(landed_name, "alice", "Name preserved (read-only mention did NOT suppress auto-copy)")
    t |> equal(landed_email, "alice@x", "Email preserved (read-only RHS mention did NOT suppress auto-copy)")
}

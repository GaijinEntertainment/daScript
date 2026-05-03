options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 28 — `@sql_json` columns and `_sql` walker
// descent into JSON paths.
//
// Chain shapes (NEW vs earlier tutorials):
//   - `_where(_.Prefs.theme == "...")`             field descent in WHERE
//   - `_select(_.Prefs.theme)`                     field descent in SELECT
//   - `_select((Name = ..., Theme = _.Prefs.theme))` mixed projection
//   - `_where(_.Prefs.notify == true) |> count()`  JSON predicate + count
//   - `_where(_.Profile.Addr.City == "...")`        nested struct path
//
// In Mode 1 these lower to `json_extract("Col", '$.path')`. In Modes 2/3
// the field access is plain daslang struct/tuple access on the
// materialized values. Parity hinges on the SQL JSON round-trip
// preserving the values exactly (and on the nested path producing the
// same field as direct access).
//
// `@sql_blob` is opaque to `_sql` (compile error to descend); no chain
// to verify. Out of scope.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name  : string
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

def fixture_users_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Prefs = (theme = "dark",  lang = "en", notify = true)))
    db |> insert(User(Id = 2, Name = "bob",   Prefs = (theme = "light", lang = "ru", notify = false)))
    db |> insert(User(Id = 3, Name = "carol", Prefs = (theme = "dark",  lang = "fr", notify = false)))
}

def fixture_users_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "alice", Prefs = (theme = "dark",  lang = "en", notify = true)))
    arr |> emplace(User(Id = 2, Name = "bob",   Prefs = (theme = "light", lang = "ru", notify = false)))
    arr |> emplace(User(Id = 3, Name = "carol", Prefs = (theme = "dark",  lang = "fr", notify = false)))
    return <- arr
}

def user_eq(a : User; b : User) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Prefs.theme == b.Prefs.theme
        && a.Prefs.lang == b.Prefs.lang
        && a.Prefs.notify == b.Prefs.notify)
}

def users_eq(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!user_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

def sort_by_id(var users : array<User>) : array<User> {
    users |> sort() <| $(a, b) {
        return a.Id < b.Id
    }
    return <- users
}

[test]
def parity_json_where_descend(t : T?) {
    // _where(_.Prefs.theme == "dark") — keeps alice + carol.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User>) |> _where(_.Prefs.theme == "dark"))
        var m2 <- (db |> select_from(type<User>) |> _where(_.Prefs.theme == "dark"))
        var m3 <- (arr |> _where(_.Prefs.theme == "dark"))

        m1 <- sort_by_id(m1)
        m2 <- sort_by_id(m2)
        m3 <- sort_by_id(m3)

        t |> equal(length(m1), 2, "json where: 2 dark users")
        t |> success(users_eq(m1, m2), "json where: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "json where: M2 ≡ M3")
    }
}

[test]
def parity_json_select_scalar(t : T?) {
    // _select(_.Prefs.theme) — projects the json-path leaf as array<string>.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User>) |> _select(_.Prefs.theme))
        var m2 <- (db |> select_from(type<User>) |> _select(_.Prefs.theme))
        var m3 <- (arr |> _select(_.Prefs.theme))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 3, "json select scalar: 3 themes")
        t |> equal(length(m1), length(m2), "json select scalar: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "json select scalar: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "theme[{i}]: M1 ≡ M2")
            t |> equal(m2[i], m3[i], "theme[{i}]: M2 ≡ M3")
        }
    }
}

[test]
def parity_json_select_named_tuple(t : T?) {
    // _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify))
    // Tuple shape (string, string, bool) — distinct from earlier tests.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify)))
        var m2 <- (db |> select_from(type<User>)
                     |> _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify)))
        var m3 <- (arr |> _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify)))

        m1 |> sort() <| $(a, b) {
            return a.Name < b.Name
        }
        m2 |> sort() <| $(a, b) {
            return a.Name < b.Name
        }
        m3 |> sort() <| $(a, b) {
            return a.Name < b.Name
        }

        t |> equal(length(m1), 3, "json named-tuple: 3 rows")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,   m2[i].Name,   "row[{i}].Name:   M1 ≡ M2")
            t |> equal(m1[i].Theme,  m2[i].Theme,  "row[{i}].Theme:  M1 ≡ M2")
            t |> equal(m1[i].Notify, m2[i].Notify, "row[{i}].Notify: M1 ≡ M2")
            t |> equal(m2[i].Name,   m3[i].Name,   "row[{i}].Name:   M2 ≡ M3")
            t |> equal(m2[i].Theme,  m3[i].Theme,  "row[{i}].Theme:  M2 ≡ M3")
            t |> equal(m2[i].Notify, m3[i].Notify, "row[{i}].Notify: M2 ≡ M3")
        }
    }
}

[test]
def parity_json_where_count(t : T?) {
    // _where(_.Prefs.notify == true) |> count()
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        let m1 = _sql(db |> select_from(type<User>) |> _where(_.Prefs.notify == true) |> count())
        let m2 = db |> select_from(type<User>) |> _where(_.Prefs.notify == true) |> count()
        let m3 = arr |> _where(_.Prefs.notify == true) |> count()

        t |> equal(m1, 1,  "json where|count: 1 notifiable user (alice)")
        t |> equal(m1, m2, "json where|count: M1 ≡ M2")
        t |> equal(m2, m3, "json where|count: M2 ≡ M3")
    }
}

// --- Nested struct paths ---

struct Address {
    City : string
    Zip  : string
}
struct Profile {
    Addr : Address
    Bio  : string
}

[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile : Profile
}

def account_eq(a : Account; b : Account) : bool {
    return (a.Id == b.Id
        && a.Profile.Addr.City == b.Profile.Addr.City
        && a.Profile.Addr.Zip == b.Profile.Addr.Zip
        && a.Profile.Bio == b.Profile.Bio)
}

def accounts_eq(a : array<Account>; b : array<Account>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!account_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

def fixture_accounts_db(db : SqlRunner) {
    db |> create_table(type<Account>)
    db |> insert(Account(Id = 1, Profile = Profile(Addr = Address(City = "SF", Zip = "94000"), Bio = "engineer")))
    db |> insert(Account(Id = 2, Profile = Profile(Addr = Address(City = "NYC", Zip = "10000"), Bio = "writer")))
    db |> insert(Account(Id = 3, Profile = Profile(Addr = Address(City = "SF", Zip = "94100"), Bio = "designer")))
}

def fixture_accounts_array() : array<Account> {
    var arr : array<Account>
    arr |> emplace(Account(Id = 1, Profile = Profile(Addr = Address(City = "SF", Zip = "94000"), Bio = "engineer")))
    arr |> emplace(Account(Id = 2, Profile = Profile(Addr = Address(City = "NYC", Zip = "10000"), Bio = "writer")))
    arr |> emplace(Account(Id = 3, Profile = Profile(Addr = Address(City = "SF", Zip = "94100"), Bio = "designer")))
    return <- arr
}

def sort_accounts_by_id(var rows : array<Account>) : array<Account> {
    rows |> sort() <| $(a, b) {
        return a.Id < b.Id
    }
    return <- rows
}

[test]
def parity_json_nested_path(t : T?) {
    // _where(_.Profile.Addr.City == "SF") — 2-level nested path through
    // a JSON-stored struct. SQL emits json_extract("Profile", '$.Addr.City').
    with_sqlite(":memory:") $(db) {
        fixture_accounts_db(db)
        var arr <- fixture_accounts_array()

        var m1 <- _sql(db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "SF"))
        var m2 <- (db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "SF"))
        var m3 <- (arr |> _where(_.Profile.Addr.City == "SF"))

        m1 <- sort_accounts_by_id(m1)
        m2 <- sort_accounts_by_id(m2)
        m3 <- sort_accounts_by_id(m3)

        t |> equal(length(m1), 2, "nested json path: 2 SF accounts")
        t |> success(accounts_eq(m1, m2), "nested json path: M1 ≡ M2")
        t |> success(accounts_eq(m2, m3), "nested json path: M2 ≡ M3")
    }
}

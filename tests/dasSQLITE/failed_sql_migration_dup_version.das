options gen2
//! two migrations with the same version in one module must not
//! compile

// Two migrations with version=2 in the same module — must fail at
// compile time with the [sql_migration] dup-version error (error
// 30111).
expect 20800

require sqlite/sqlite_migrate

[sql_migration(version=2, description="first")]
def migration_a(db : SqlRunner) {
    db |> exec("SELECT 1")
}

[sql_migration(version=2, description="second — same version, should fail to compile")]
def migration_b(db : SqlRunner) {
    db |> exec("SELECT 2")
}

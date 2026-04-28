options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Posts"),
 sql_index(fields = "AuthorId"),
 sql_index(fields = ("AuthorId", "Published"), name = "idx_posts_author_published"),
 sql_index(fields = "Slug", unique = true)]
struct Post {
    @sql_primary_key Id : int
    AuthorId : int
    Slug : string
    Published : int64
    Title : string
}

[test]
def test_indexes_helper_concatenates(t : T?) {
    let sql = _sql_create_indexes_sql(type<Post>)
    t |> success(find(sql, "CREATE INDEX \"idx_Posts_AuthorId\"") >= 0, "auto-named single-col index")
    t |> success(find(sql, "CREATE INDEX \"idx_posts_author_published\"") >= 0, "explicit-named composite index")
    t |> success(find(sql, "CREATE UNIQUE INDEX \"idx_Posts_Slug\"") >= 0, "auto-named unique index")
}

[test]
def test_indexes_actually_created_in_sqlite(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Post>)
        let n = db |> query_scalar("SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='Posts' AND name LIKE 'idx_%'", type<int>)
        t |> equal(n, 3, "all three indexes registered with SQLite")
    }
}

[test]
def test_unique_index_rejects_duplicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Post>)
        db |> insert(Post(Id = 1, AuthorId = 1, Slug = "first", Published = 0l, Title = "First"))
        let r = db |> try_insert(Post(Id = 2, AuthorId = 1, Slug = "first", Published = 0l, Title = "Dup"))
        t |> success(r |> is_err, "duplicate Slug rejected by UNIQUE INDEX")
    }
}

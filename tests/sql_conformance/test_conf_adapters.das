options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: the sql_bind / sql_extract custom-type adapter rail — user pair
// next to the type, enum auto-round-trip, Option over a custom adapter, and the
// @sql_json / @sql_blob generated adapters (+ json-path descent in a predicate).

require dastest/testing_boost public

require _conformance_provider

struct ConfStamp {
    unix_seconds : int64
}

def sql_bind(v : ConfStamp) : int64 => v.unix_seconds

[unused_argument(wt)]
def sql_extract(v : int64; wt : type<ConfStamp>) : ConfStamp => ConfStamp(unix_seconds = v)

enum ConfStatus {
    Pending = 1
    Shipped = 2
}

struct ConfPrefs {
    theme : string
    notify : bool
}

[sql_table(name = "ConfShipments")]
struct ConfShipment {
    @sql_primary_key Id : int
    CreatedAt : ConfStamp
    Status : ConfStatus
    @safe_when_uninitialized DeliveredAt : Option<ConfStamp>
    @sql_json Prefs : ConfPrefs
}

[test(description = "custom types, enums, options over custom types and sql_json columns round-trip through the sql_bind and sql_extract adapter rail, also as bound predicate values", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_adapter_rail(t : T?) {
    t |> run("custom type + enum + Option-over-custom round-trip") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfShipment>)
            db |> insert(ConfShipment(
                Id = 0,
                CreatedAt = ConfStamp(unix_seconds = 1000l),
                Status = ConfStatus.Shipped,
                DeliveredAt = some(ConfStamp(unix_seconds = 2000l)),
                Prefs = ConfPrefs(theme = "dark", notify = true)))
            db |> insert(ConfShipment(
                Id = 0,
                CreatedAt = ConfStamp(unix_seconds = 3000l),
                Status = ConfStatus.Pending,
                DeliveredAt = none(type<ConfStamp>),
                Prefs = ConfPrefs(theme = "light", notify = false)))
            var rows <- db |> select_from(type<ConfShipment>)
            t |> equal(length(rows), 2)
            for (r in rows) {
                if (r.Status == ConfStatus.Shipped) {
                    t |> equal(r.CreatedAt.unix_seconds, 1000l)
                    t |> success(r.DeliveredAt |> is_some, "Option over custom adapter reads back some")
                    t |> equal((r.DeliveredAt |> unwrap).unix_seconds, 2000l)
                    t |> equal(r.Prefs.theme, "dark")
                } else {
                    t |> success(r.DeliveredAt |> is_none, "Option over custom adapter reads back none")
                    t |> success(!r.Prefs.notify, "@sql_json struct round-trips")
                }
            }
        }
    }
    t |> run("adapter-backed column filters in _sql via bound custom value") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfShipment>)
            db |> insert(ConfShipment(
                Id = 0, CreatedAt = ConfStamp(unix_seconds = 500l), Status = ConfStatus.Pending,
                DeliveredAt = none(type<ConfStamp>), Prefs = ConfPrefs(theme = "t", notify = false)))
            let shipped <- _sql(db |> select_from(type<ConfShipment>)
                |> _where(_.Status == ConfStatus.Pending))
            t |> equal(length(shipped), 1)
        }
    }
    t |> run("@sql_json path descent in a predicate") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfShipment>)
            db |> insert(ConfShipment(
                Id = 0, CreatedAt = ConfStamp(unix_seconds = 1l), Status = ConfStatus.Pending,
                DeliveredAt = none(type<ConfStamp>), Prefs = ConfPrefs(theme = "dark", notify = true)))
            db |> insert(ConfShipment(
                Id = 0, CreatedAt = ConfStamp(unix_seconds = 2l), Status = ConfStatus.Pending,
                DeliveredAt = none(type<ConfStamp>), Prefs = ConfPrefs(theme = "light", notify = true)))
            let dark <- _sql(db |> select_from(type<ConfShipment>)
                |> _where(_.Prefs.theme == "dark"))
            t |> equal(length(dark), 1)
            t |> equal(dark[0].CreatedAt.unix_seconds, 1l)
        }
    }
}

options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: [sql_function] client-side UDFs — CAPABILITY-GATED (caps.client_udfs).
// A provider without client UDFs (e.g. PostgreSQL) drops this FILE from its copy of
// the suite — see README.md.

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfEnemies")]
struct ConfEnemy {
    @sql_primary_key Id : int
    Hp : float
    Armor : int
}

[sql_function(deterministic = true)]
def conf_score(hp : float; armor : int) : double => double(hp) * (1.0lf - double(armor) / 100.0lf)

[test(description = "a sql_function is visible to _sql chains", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_sql_function(t : T?) {
    t |> run("[sql_function] UDF is visible to _sql chains") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfEnemy>)
            db |> insert([
                ConfEnemy(Id = 0, Hp = 100.0f, Armor = 0),
                ConfEnemy(Id = 0, Hp = 100.0f, Armor = 50),
                ConfEnemy(Id = 0, Hp = 10.0f, Armor = 90)])
            let strong <- _sql(db |> select_from(type<ConfEnemy>)
                |> _where(conf_score(_.Hp, _.Armor) >= 50.0lf)
                |> _order_by(_.Armor))
            t |> equal(length(strong), 2)
            t |> equal(strong[0].Armor, 0)
        }
    }
}

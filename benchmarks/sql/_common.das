options gen2
options persistent_heap

require daslib/sql public
require daslib/linq_boost public
require sqlite/sqlite_boost public
require sqlite/sqlite_linq public
require dastest/testing_boost public
require daslib/fio public

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name : string
    price : int
}

def public fixture_db(db : SqlRunner; n : int) {
    db |> create_table(type<Car>)
    var rows : array<Car>
    rows |> resize(n)
    for (i in range(n)) {
        rows[i] = Car(id = i + 1, name = "Car{i}", price = (i * 37) % 1000)
    }
    db |> insert(rows)
}

def public fixture_array(n : int) : array<Car> {
    var arr : array<Car>
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = Car(id = i + 1, name = "Car{i}", price = (i * 37) % 1000)
    }
    return <- arr
}

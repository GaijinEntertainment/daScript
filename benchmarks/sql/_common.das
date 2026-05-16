options gen2
options persistent_heap

require daslib/sql public
require daslib/linq_boost public
require sqlite/sqlite_boost public
require sqlite/sqlite_linq public
require dastest/testing_boost public
require daslib/fio public

let public BRAND_COUNT = 5
let public DEALER_COUNT = 100

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name : string
    price : int
    brand : int
    year : int
    dealer_id : int
}

[sql_table(name = "Dealers")]
struct Dealer {
    @sql_primary_key id : int
    name : string
}

def public fixture_db(db : SqlRunner; n : int) {
    db |> create_table(type<Car>)
    db |> create_table(type<Dealer>)
    var cars : array<Car>
    cars |> resize(n)
    for (i in range(n)) {
        cars[i] = Car(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        )
    }
    db |> insert(cars)
    var dealers : array<Dealer>
    dealers |> resize(DEALER_COUNT)
    for (i in range(DEALER_COUNT)) {
        dealers[i] = Dealer(id = i + 1, name = "Dealer{i}")
    }
    db |> insert(dealers)
}

def public fixture_array(n : int) : array<Car> {
    var arr : array<Car>
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = Car(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        )
    }
    return <- arr
}

def public fixture_dealers_array() : array<Dealer> {
    var arr : array<Dealer>
    arr |> resize(DEALER_COUNT)
    for (i in range(DEALER_COUNT)) {
        arr[i] = Dealer(id = i + 1, name = "Dealer{i}")
    }
    return <- arr
}

options gen2
options persistent_heap

require daslib/sql public
require daslib/linq_boost public
require sqlite/sqlite_boost public
require sqlite/sqlite_linq public
require dastest/testing_boost public
require daslib/fio public
require daslib/decs_boost public
require pugixml/PUGIXML_boost public
require strings public

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

// m5_xml lane: same Car schema, rows materialized from XML child elements via
// from_xml_node. Attribute names match Car's fields; same deterministic row
// generator as fixture_array so the XML lane is directly comparable. Built with
// a string writer (not `+`) to keep fixture construction O(n).
def public fixture_xml_string(n : int) : string {
    return build_string() $(var w) {
        w |> write("<cars>")
        for (i in range(n)) {
            w |> write("<car id=\"{i + 1}\" name=\"Car{i}\" price=\"{(i * 37) % 1000}\" brand=\"{i % BRAND_COUNT}\" year=\"{2010 + (i * 7) % 16}\" dealer_id=\"{(i % DEALER_COUNT) + 1}\"/>")
        }
        w |> write("</cars>")
    }
}

def public fixture_dealers_array() : array<Dealer> {
    var arr : array<Dealer>
    arr |> resize(DEALER_COUNT)
    for (i in range(DEALER_COUNT)) {
        arr[i] = Dealer(id = i + 1, name = "Dealer{i}")
    }
    return <- arr
}

// m4_decs_fold lane: same Car schema, decs-archetype-backed. Same row generator as fixture_array so fold splice perf is directly comparable across SQL / array / decs.
[decs_template(prefix = "car_")]
struct public DecsCar {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

[decs_template(prefix = "dealer_")]
struct public DecsDealer {
    id   : int
    name : string
}

def public fixture_decs(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, DecsCar(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        ))
    }
}

// Cars + Dealers together for the decs join lane. Both archetypes co-exist in the same decs world; plan_decs_join builds an in-query hash from the smaller side (dealers) then walks the larger side (cars).
def public fixture_decs_cars_and_dealers(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, DecsCar(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        ))
    }
    create_entities(DEALER_COUNT) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, DecsDealer(
            id = i + 1,
            name = "Dealer{i}"
        ))
    }
}

// Duplicate-prone single-field template for decs distinct/order benches.
// Populated with `i % BRAND_COUNT` so n=100k has dense duplicates per value,
// matching the array-side `make_brands(n)` helper in order_distinct_take.das.
[decs_template(prefix = "brand_")]
struct public DecsBrand {
    value : int
}

def public fixture_decs_brands(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, DecsBrand(value = i % BRAND_COUNT))
    }
}

// SQL-side mirror of DecsBrand / make_brands. n rows of `value = i % BRAND_COUNT`
// gives dense duplicates so the SQL DISTINCT lane has meaningful work to do.
[sql_table(name = "Brands")]
struct public Brand {
    @sql_primary_key id : int
    value : int
}

def public fixture_db_brands(db : SqlRunner; n : int) {
    db |> create_table(type<Brand>)
    var brands : array<Brand>
    brands |> resize(n)
    for (i in range(n)) {
        brands[i] = Brand(id = i + 1, value = i % BRAND_COUNT)
    }
    db |> insert(brands)
}

// Same fixture as fixture_decs, but returns the eid of the n/2-th entity for indexed-lookup benches.
// The decs O(1) lookup path keys on EntityId (not the Car.id column), so the bench needs an actual eid
// captured at creation time. fixture_decs has no return so call sites can't reuse it for this.
def public fixture_decs_capture_mid(n : int) : EntityId {
    restart()
    var target_eid : EntityId
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        if (i == n / 2) {
            target_eid = eid
        }
        apply_decs_template(cmp, DecsCar(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        ))
    }
    return target_eid
}

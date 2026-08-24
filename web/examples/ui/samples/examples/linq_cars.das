require daslib/linq_boost

struct Car {
    brand : string
    price : float
}

[export]
def main {
    var cars <- [Car(brand = "Ka", price = 900.0),
                 Car(brand = "Vaz", price = 1200.0)]
    let stats <- _fold(each(cars)
        ._group_by(_.brand)
        ._select((Brand = _._0,
                  Avg = _._1 |> select($(c : Car) => c.price)
                             |> average()))
        .to_array())
    for (s in stats) {
        print("{s.Brand}: avg {s.Avg}\n")
    }
}

This macro converts a DECS query into an iterator<tuple<...>>. For example::
    let it = from_decs($(index:int; text:string){})
    for (item in it) {
        // process item
        print("Entity {item.index}: {item.text}\n")
    }
Internally it generates the following code::
    let it = invoke($() {
        var res : array<tuple<int,string>>
        query($(index:int; text:string) {
            res |> push((index, text))
        })
        return res.to_sequence()

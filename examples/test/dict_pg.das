require fio

options string_heap=4000000     // big gc hint

[export,unsafe]
def test
    profile(1,"freq dictionary PG") <|
        fopen("c:/users/boris/work/pg/pg.txt", "rb") <| $( f )
            if f != null
                var dict : table<string; int>
                fmap(f) <| $(data)
                    var offset, length, index = 0
                    for ch,index in data,range(0,INT_MAX)
                        if is_alpha(ch)
                            if length == 0
                                offset = index
                            length ++
                        elif length != 0
                            var token = chop(data,offset,length) |> to_lower_in_place() // unsafe
                            if dict[token]++ != 0
                                delete token        // gc hint
                            length = 0
                var arr : array<tuple<string;int>>
                reserve(arr,length(dict))
                for k,v in keys(dict),values(dict)
                    push(arr,[[auto k,v]])
                delete dict                         // gc hint
                sort(arr) <| $(a,b)
                    return a._1 != b._1 ? a._1 > b._1 : a._0 < b._0
                fopen("c:/users/boris/work/pg/das.freq", "wb") <| $( fw )
                    if fw != null
                        for t in arr
                            var ts = "{t._1} {t._0}\n"
                            fwrite(fw,ts)
                            delete ts               // gc hint
                delete arr                          // gc hint
    return true


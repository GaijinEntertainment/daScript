require fio

options string_heap=400000000
options heap=10000000

[export]
def test
	profile(1,"freq dictionary PG") <|
		fopen("c:/users/boris/work/pg/pg.txt", "rb") <| $( f )
			if f != null
				var data = fread(f)
				var dict : table<string; int>
				var offset, length = 0
				for ch,index in data,range(0,INT_MAX)
					if is_alpha(ch)
						if length == 0
							offset = index
						length ++
					elif length != 0
						var subs = slice(data,offset,offset+length)
						var token = to_lower(subs)
						delete subs
						if (dict[token]++)!=0
							delete token
						length = 0
					/*
					if index % (10*1024*1024) == 0
						print("{index/(1024*1024)} mb, table {capacity(dict)}, string heap {int(string_heap_bytes_allocated())}, heap {int(heap_bytes_allocated())}\n")
					*/
				delete data
				var arr : array<tuple<string;int>>
				reserve(arr,length(dict))
				for k,v in keys(dict),values(dict)
					push(arr,[[auto k,v]])
				delete dict
				sort(arr) <| $(a,b)
					return a._1 != b._1 ? a._1 > b._1 : a._0 < b._0
				fopen("c:/users/boris/work/pg/das.freq", "wb") <| $( fw )
					if fw != null
						for t in arr
							fwrite(fw,"{t._1} {t._0}\n")
				delete arr
    return true


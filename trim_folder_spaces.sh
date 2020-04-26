# solution from https://stackoverflow.com/a/10120431
while IFS= read -r -d '' -u 9
do
    if [[ "$(file -bs --mime-type -- "$REPLY")" = text/* ]]
    then
        sed -i 's/[ \t]\+\(\r\?\)$/\1/' -- "$REPLY"
    else
        echo "Skipping $REPLY" >&2
    fi
done 9< <(find $1 -type f -print0)

Appends a child element with *name*, invokes *blk* with it, and
returns the new child. Use with pipe+block for nested XML:

  ``node |> tag("book") <| $(var book) { book |> tag("title", "...") }``

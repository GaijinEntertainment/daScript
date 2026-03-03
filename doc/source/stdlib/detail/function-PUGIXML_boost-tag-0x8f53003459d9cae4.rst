Appends a child element to a document, invokes *blk*, returns it.
Allows ``doc |> tag("root") <| $(var root) { ... }`` without manual
``doc as xml_node``.

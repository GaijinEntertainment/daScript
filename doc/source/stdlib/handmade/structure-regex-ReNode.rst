Regular expression node.
Regex operation
Unique node identifier
Matchig function
Generator function
Source range
Text fragment
Length of text fragment
All child nodes
Left child node
Right child node
Subexpression node
Next node in the list
Character set for character class matching
Index for character class matching
Minimum repetition count for counted quantifiers
Maximum repetition count for counted quantifiers (-1 means unlimited)
Whether this quantifier uses lazy matching (``*?``, ``+?``, ``??``, ``{n,m}?``)
Where this node's match ended on the path being tried; a group takes its body's end
Quantifier: the node that closes one pass of its subexpression; its ``next`` points back here
Quantifier: whether the pass now running is probed, collected, or must reach ``reach_target``
Quantifier: the stop a reaching pass must hit
Quantifier: every stop a collected pass can end at, best first
Quantifier: tries the rest of the pattern before another pass
A quantifier's ``sub_end``: reaching it ends one pass through the subexpression
Quantifier: its subexpression holds no choice, so one set of stops is all there is

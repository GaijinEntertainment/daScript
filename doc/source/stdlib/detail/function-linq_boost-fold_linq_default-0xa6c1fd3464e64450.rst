fold sequence into
  invoke ( top, $ ( it ) : auto {
     var pass_0 = call0(it,...) // or call0_to_array(it,...)
     var pass_1 = call1(pass_0,...)
     ...
     return <- pass_N // or pass_N.to_sequence() if expr is iterator
  } )
  skip var if call is inplace
  skip delete if call is not first
  if call is first and source is iterator, then call is call_to_array
  if call is last and expr is iterator, then return is pass_N.to_sequence()
  if expr is not linq call, return null

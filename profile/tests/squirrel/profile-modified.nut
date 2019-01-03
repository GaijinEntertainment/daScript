function profile_it(cnt, f)//for modified version
{
  local system = require("system")
  local res = 0
  for (local i = 0; i < cnt; ++i)
  {
    local start = system.clock()
    f()
    local measured = system.clock() - start
    if (i == 0 || measured < res)
      res = measured;
  }
  return res
}

local clock_func = ("require" in this) ? require("system").clock : clock //we can also check existence of system module and clock in roottable, but currently it's ok

function profile_it(cnt, f)//for modified version
{
  local res = 0
  for (local i = 0; i < cnt; ++i)
  {
    local start = clock_func()
    f()
    local measured = clock_func() - start
    if (i == 0 || measured < res)
      res = measured;
  }
  return res
}

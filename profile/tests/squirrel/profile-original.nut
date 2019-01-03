function profile_it(cnt, f)//for original
{
  local res = 0
  for (local i = 0; i < cnt; ++i)
  {
    local start = clock()
    f()
    local measured = clock() - start
    if (i == 0 || measured < res)
      res = measured;
  }
  return res
}

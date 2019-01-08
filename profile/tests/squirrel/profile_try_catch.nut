particles <- []
for (local i = 0; i < 1000; ++i)
  particles.push(i)

local function try_catch_loop(fails_count)
{
  fails_count = fails_count + 1000
  local fails = 0
  local cnt = 0
  for (local j = 0; j < 100; ++j)
  {
    for (local i = 0; i < fails_count; ++i)
    {
       try
       {
         cnt = cnt + particles[i]
       } catch(e)
       {
         fails = fails+1
       }
    }
  }
}

loadfile("profile.nut")()
print("try-catch loop: " + profile_it(20, function() {try_catch_loop(1000)}) + "\n");

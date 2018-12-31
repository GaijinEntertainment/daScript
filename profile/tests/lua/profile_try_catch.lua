particles = {}
for i = 1, 1000 do
	table.insert(particles, i)
end
fails = 0
cnt = 0
start = os.clock()
for j = 1, 1000 do
  for i = 1, 2000 do
     if not pcall(function ()
             cnt = cnt + particles[i]
             end)
      then
          fails = fails+1
      end
  end
end
print("took"..(os.clock()-start))

function update_particle(p)
  p.pos.x=p.pos.x+p.vel.x
  p.pos.y=p.pos.y+p.vel.y
  p.pos.z=p.pos.z+p.vel.z
end

function update(particles)
	for i,p in ipairs(particles) do
	  update_particle(p)
	end
end

function update_several_times(particles, count)
  for i = 0, count  do
     update(particles)
  end
end


particles = {}
for i = 0, 1000000 do
	table.insert(particles,
		{
			pos = {x = i + 0.1, y = i + 0.2, z = i + 0.3},
			vel = {x = 1.1, y = 2.1,  z = 3.1}
		})
end

---
start = os.clock()
update_several_times(particles, 100)
print("took " .. (os.clock()-start))
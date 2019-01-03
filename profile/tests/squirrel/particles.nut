function update_particle(p)
{
  p.pos.x=p.pos.x+p.vel.x
  p.pos.y=p.pos.y+p.vel.y
  p.pos.z=p.pos.z+p.vel.z
}

function update(particles)
{
  foreach(p in particles) {
    update_particle(p)
  }
}

function update_several_times(particles, count)
{
  for (local i = 0; i < count; ++i)
  {
    update(particles)
  }
}

function updateI(particles)
{
  foreach(p in particles) {
    p.pos.x=p.pos.x+p.vel.x
    p.pos.y=p.pos.y+p.vel.y
    p.pos.z=p.pos.z+p.vel.z
  }
}

function update_several_timesI(particles, count)
{
  for (local i = 0; i < count; ++i)
  {
    updateI(particles)
  }
}


particles <- []
for (local i = 0; i < 50000; ++i)
{
  particles.push(
  	{
  		pos = {x = i + 0.1, y = i + 0.2, z = i + 0.3},
  		vel = {x = 1.1, y = 2.1,  z = 3.1}
  	})
}

loadfile("profile.nut")()

print("particles kinematics: " + profile_it(20, function () {update_several_times(particles, 100) }) + "\n")
print("particles kinematics inline: " + profile_it(20, function () {update_several_timesI(particles, 100)}) + "\n")

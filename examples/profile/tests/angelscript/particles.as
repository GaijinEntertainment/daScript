class Particle
{
  float x,y,z;
  float vx,vy,vz;
  void update()
  {
    x+=vx;y+=vy;z+=vz;
  }
};

int main(int unused)
{
    print("particles: ");
    array<Particle> particles(50000);
    for (uint i = 0; i < particles.length(); ++i)
    {
      particles[i].x = float(i);
      particles[i].y = float(i+1);
      particles[i].z = float(i+2);
      particles[i].vx = 1;
      particles[i].vy = 2;
      particles[i].vz = 3;
    }
    int ctime = GetSystemTime();
    for (uint j = 0; j < 100; ++j)
    {
      for (uint i = 0,e =particles.length(); i < e; ++i)
      {
         //particles[i].update();
         Particle@ p = particles[i];
         p.x += p.vx;
         p.y += p.vy;
         p.z += p.vz;
       }
     }
    int time = GetSystemTime()-ctime;
    print("\n");
    print("particles inlined: time taken (msec): ");
    print (time);
    print("\n");
    return 0;
}

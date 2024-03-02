float PI = 3.141592653589793;
float SOLAR_MASS = 4 * PI * PI;
float DAYS_PER_YEAR = 365.24;

class Body {
  float x, y, z, vx, vy, vz, mass;

  void offsetMomentum(float px, float py, float pz){
    vx = -px / SOLAR_MASS;
    vy = -py / SOLAR_MASS;
    vz = -pz / SOLAR_MASS;
  }
};

Body jupiter(){
  Body p = Body();
  p.x = 4.84143144246472090e+00;
  p.y = -1.16032004402742839e+00;
  p.z = -1.03622044471123109e-01;
  p.vx = 1.66007664274403694e-03 * DAYS_PER_YEAR;
  p.vy = 7.69901118419740425e-03 * DAYS_PER_YEAR;
  p.vz = -6.90460016972063023e-05 * DAYS_PER_YEAR;
  p.mass = 9.54791938424326609e-04 * SOLAR_MASS;
  return p;
}

Body saturn(){
  Body p = Body();
  p.x = 8.34336671824457987e+00;
  p.y = 4.12479856412430479e+00;
  p.z = -4.03523417114321381e-01;
  p.vx = -2.76742510726862411e-03 * DAYS_PER_YEAR;
  p.vy = 4.99852801234917238e-03 * DAYS_PER_YEAR;
  p.vz = 2.30417297573763929e-05 * DAYS_PER_YEAR;
  p.mass = 2.85885980666130812e-04 * SOLAR_MASS;
  return p;
}

Body uranus(){
  Body p = Body();
  p.x = 1.28943695621391310e+01;
  p.y = -1.51111514016986312e+01;
  p.z = -2.23307578892655734e-01;
  p.vx = 2.96460137564761618e-03 * DAYS_PER_YEAR;
  p.vy = 2.37847173959480950e-03 * DAYS_PER_YEAR;
  p.vz = -2.96589568540237556e-05 * DAYS_PER_YEAR;
  p.mass = 4.36624404335156298e-05 * SOLAR_MASS;
  return p;
}

Body neptune(){
  Body p = Body();
  p.x = 1.53796971148509165e+01;
  p.y = -2.59193146099879641e+01;
  p.z = 1.79258772950371181e-01;
  p.vx = 2.68067772490389322e-03 * DAYS_PER_YEAR;
  p.vy = 1.62824170038242295e-03 * DAYS_PER_YEAR;
  p.vz = -9.51592254519715870e-05 * DAYS_PER_YEAR;
  p.mass = 5.15138902046611451e-05 * SOLAR_MASS;
  return p;
}

Body sun(){
  Body p = Body();
  p.mass = SOLAR_MASS;
  return p;
}


array<Body> bodies;

void init_bodies()
{
  bodies.resize(5);
  bodies[0] = sun();
  bodies[1] = jupiter();
  bodies[2] = saturn();
  bodies[3] = uranus();
  bodies[4] = neptune();

  float px = 0.0;
  float py = 0.0;
  float pz = 0.0;
  for(uint i=0; i < bodies.length(); ++i) {
    px += bodies[i].vx * bodies[i].mass;
    py += bodies[i].vy * bodies[i].mass;
    pz += bodies[i].vz * bodies[i].mass;
  }
  bodies[0].offsetMomentum(px,py,pz);
}

void advance_bodies(int nbody)
{
  float dx, dy, dz, dist2, mag, bm;
  float bix, biy, biz, bimass, bivx, bivy, bivz;
  for(int i=0; i < nbody; ++i) {
    Body@ bi = bodies[i];
    bix = bi.x;
    biy = bi.y;
    biz = bi.z;
    bimass = bi.mass;
    bivx = bi.vx;
    bivy = bi.vy;
    bivz = bi.vz;
    for(int j=i+1; j < nbody; ++j) {
      Body@ bj = bodies[j];

      dx = bix - bj.x;
      dy = biy - bj.y;
      dz = biz - bj.z;

      dist2 = dx*dx + dy*dy + dz*dz;
      mag = 1 / (sqrt(dist2) * dist2);

      bm = bj.mass * mag;
      bivx -= dx * bm;
      bivy -= dy * bm;
      bivz -= dz * bm;

      bm = bimass * mag;
      bj.vx += dx * bm;
      bj.vy += dy * bm;
      bj.vz += dz * bm;
    }
    bi.vx = bivx;
    bi.vy = bivy;
    bi.vz = bivz;
    bi.x = bix + bivx;
    bi.y = biy + bivy;
    bi.z = biz + bivz;
  }
}

float calc_energy(int nbody){
  float dx, dy, dz, distance, vx, vy, vz, bix, biy, biz, bim;
  float e = 0.0;

  for(int i=0; i < nbody; ++i) {
    Body@ bi = bodies[i];

    bix = bi.x;
    biy = bi.y;
    biz = bi.z;
    vx = bi.vx;
    vy = bi.vy;
    vz = bi.vz;
    bim = bi.mass;
    e += 0.5 * bim * ( vx*vx + vy*vy + vz*vz );

    for(int j=i+1; j < nbody; ++j) {
      Body@ bj = bodies[j];

      dx = bix - bj.x;
      dy = biy - bj.y;
      dz = biz - bj.z;

      distance = sqrt(dx*dx + dy*dy + dz*dz);
      e -= (bim * bj.mass) / distance;
    }
  }
  return e;
}

void scale_bodies(int nbody, float scale) {
  for(int i=0; i < nbody; ++i) {
    Body@ bi = bodies[i];
    bi.mass *= scale*scale;
    bi.vx *= scale;
    bi.vy *= scale;
    bi.vz *= scale;
  }
}

int main(int u)
{
  //int n = 5000;
  int n = 500000;
  init_bodies();
  int nbody = bodies.length();
  scale_bodies(nbody, 0.01);
  int ctime = GetSystemTime();
  for (int j=0; j!=10; ++j ) {
    for (int i=0; i<n; ++i) {
      advance_bodies(nbody);
    }
  }
  int time = (GetSystemTime()-ctime)/100;
  print("\"n-bodies\", ");
  print (time/1000);
  print (".");
  print(time%1000);
  print(", 10\n");
  return 0;
}
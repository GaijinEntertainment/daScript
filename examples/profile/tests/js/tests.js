// primes test

function Node(x) {
    this.x = x
    this.y = Math.random()
    this.left = null
    this.right = null
}


function merge(lower, greater) {
    if (lower === null) {
    	return greater
    }

    if (greater === null) {
    	return lower
    }

    if (lower.y < greater.y) {
        lower.right = merge(lower.right, greater)
        return lower
    } else {
        greater.left = merge(lower, greater.left)
        return greater
    }
}

function splitBinary(orig, value) {
    if (orig === null) {
    	return [null, null]
    }

    if (orig.x < value) {
        const splitPair = splitBinary(orig.right, value)
        orig.right = splitPair[0]
        return [orig, splitPair[1]]
    } else {
        const splitPair = splitBinary(orig.left, value)
        orig.left = splitPair[1]
        return [splitPair[0], orig]
    }
}

function merge3(lower, equal, greater) {
    return merge(merge(lower, equal), greater)
}


function SplitResult(lower, equal, greater) {
    this.lower = lower
    this.equal = equal
    this.greater = greater
}


function split(orig, value) {
    var lower_eqg = splitBinary(orig, value)
    var eq_greater = splitBinary(lower_eqg[1], value + 1)
    return new SplitResult(lower_eqg[0], eq_greater[0], eq_greater[1])
}


function Tree()
{
    this.root = null
}

Tree.prototype.has_value = function(x) {
    var splited = split(this.root, x)
    var res = splited.equal !== null
    this.root = merge3(splited.lower, splited.equal, splited.greater)
    return res
}

Tree.prototype.insert = function(x) {
    var splited = split(this.root, x)
    if (splited.equal === null) {
        splited.equal = new Node(x)
    }
    this.root = merge3(splited.lower, splited.equal, splited.greater)
}

Tree.prototype.erase = function(x) {
    var splited = split(this.root, x)
    this.root = merge(splited.lower, splited.greater)
}

function testTree() {
    var tree = new Tree()
    var cur = 5;
    var res = 0;

    for (var i = 1; i < 1000000; ++i) {
        var a = i % 3;
        cur = (cur * 57 + 43) % 10007;
        if (a === 0) {
            tree.insert(cur);
        } else if (a === 1) {
            tree.erase(cur);
        } else if (a == 2) {
            res += tree.has_value(cur) ? 1 : 0;
        }
    }
	return res;
}

function isprime(n) {
	var m = n - 1;
	for(var i=2; i!=m; ++i ) {
		if ( n % i ==0 ) {
			return false;
		}
	}
	return true;
}

function primes(n) {
	var count = 0;
	for ( var i=2; i!=n; ++i ) {
		if ( isprime(i) ) {
			count ++;
		}
	}
	return count;
}

// particle test

function update_particle(p) {
	p.pos.x += p.vel.x;
	p.pos.y += p.vel.y;
	p.pos.z += p.vel.z;
}

function update_particles(particles) {
	var n = particles.length;
	for ( var i=0; i!=n; ++i ) {
		update_particle(particles[i]);
	}
}

function multi_update_particles(particles,count) {
	while ( count ) {
		update_particles(particles);
		count--;
	}
}

function update_particles_i(particles) {
	var n = particles.length;
	for ( var i=0; i!=n; ++i ) {
		var p = particles[i];
		p.pos.x += p.vel.x;
		p.pos.y += p.vel.y;
		p.pos.z += p.vel.z;
	}
}

function multi_update_particles_i(particles,count) {
	while ( count ) {
		update_particles_i(particles);
		count--;
	}
}

function make_particles() {
	var particles = []
	var n = 50000;
	for ( var i=0; i!=n; ++i ) {
		var p = {
			pos : {x : i + 0.1, y : i + 0.2, z : i + 0.3},
			vel : {x : 1.1, y : 2.1,  z : 3.1}
		};
		particles.push(p);
	}
	return particles;
}

// dictionary test

function dict_makeSrc(){
	var src = [];
	var n = 500000;
	var modn = n;
	for (var i=0; i != n; ++i ) {
		var num = (271828183 ^ i*119) % modn;
		src.push('_' + num);
	}
	return src;
}

function dict(src) {
	var tab = {}
	var n = src.length;
	var max = 1
	for (var i=0; i != n; ++i ) {
		var l = src[n];
		if ( tab.hasOwnProperty(l) ) {
			max = Math.max(++tab[l],max);
		} else {
			tab[l] = 1;
		}
	}
	return max;
}

// exponent test

function expLoop(n) {
	var sum = 0;
	for ( var i=0; i!=n; ++i ) {
		sum += Math.exp(1.0/(1.0+i));
	}
	return sum;
}

// fibonacii test

function fibR(n) {
	if ( n<2 ) return n;
	return fibR(n-2) + fibR(n-1);
}

function fibI(n) {
	var last = 0
	var cur = 1
	n = n - 1
	while ( n>0 ) {
		n = n - 1
		var tmp = cur
		cur = last + cur
		last = tmp
	}
	return cur
}

var PI = 3.141592653589793;
var SOLAR_MASS = 4 * PI * PI;
var DAYS_PER_YEAR = 365.24;

function Body(x,y,z,vx,vy,vz,mass){
   this.x = x;
   this.y = y;
   this.z = z;
   this.vx = vx;
   this.vy = vy;
   this.vz = vz;
   this.mass = mass;
}

Body.prototype.offsetMomentum = function(px,py,pz) {
   this.vx = -px / SOLAR_MASS;
   this.vy = -py / SOLAR_MASS;
   this.vz = -pz / SOLAR_MASS;
   return this;
}

function Jupiter(){
   return new Body(
      4.84143144246472090e+00,
      -1.16032004402742839e+00,
      -1.03622044471123109e-01,
      1.66007664274403694e-03 * DAYS_PER_YEAR,
      7.69901118419740425e-03 * DAYS_PER_YEAR,
      -6.90460016972063023e-05 * DAYS_PER_YEAR,
      9.54791938424326609e-04 * SOLAR_MASS
   );
}

function Saturn(){
   return new Body(
      8.34336671824457987e+00,
      4.12479856412430479e+00,
      -4.03523417114321381e-01,
      -2.76742510726862411e-03 * DAYS_PER_YEAR,
      4.99852801234917238e-03 * DAYS_PER_YEAR,
      2.30417297573763929e-05 * DAYS_PER_YEAR,
      2.85885980666130812e-04 * SOLAR_MASS
   );
}

function Uranus(){
   return new Body(
      1.28943695621391310e+01,
      -1.51111514016986312e+01,
      -2.23307578892655734e-01,
      2.96460137564761618e-03 * DAYS_PER_YEAR,
      2.37847173959480950e-03 * DAYS_PER_YEAR,
      -2.96589568540237556e-05 * DAYS_PER_YEAR,
      4.36624404335156298e-05 * SOLAR_MASS
   );
}

function Neptune(){
   return new Body(
      1.53796971148509165e+01,
      -2.59193146099879641e+01,
      1.79258772950371181e-01,
      2.68067772490389322e-03 * DAYS_PER_YEAR,
      1.62824170038242295e-03 * DAYS_PER_YEAR,
      -9.51592254519715870e-05 * DAYS_PER_YEAR,
      5.15138902046611451e-05 * SOLAR_MASS
   );
}

function Sun(){
   return new Body(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, SOLAR_MASS);
}


function NBodySystem(bodies){
   this.bodies = bodies;
   var px = 0.0;
   var py = 0.0;
   var pz = 0.0;
   var size = this.bodies.length;
   for (var i=0; i<size; i++){
      var b = this.bodies[i];
      var m = b.mass;
      px += b.vx * m;
      py += b.vy * m;
      pz += b.vz * m;
   }
   this.bodies[0].offsetMomentum(px,py,pz);
}

NBodySystem.prototype.advance = function(dt){
   var dx, dy, dz, distance, mag;
   var size = this.bodies.length;

   for (var i=0; i<size; i++) {
      var bodyi = this.bodies[i];
      for (var j=i+1; j<size; j++) {
         var bodyj = this.bodies[j];
         dx = bodyi.x - bodyj.x;
         dy = bodyi.y - bodyj.y;
         dz = bodyi.z - bodyj.z;

         distance = Math.sqrt(dx*dx + dy*dy + dz*dz);
         mag = dt / (distance * distance * distance);

         bodyi.vx -= dx * bodyj.mass * mag;
         bodyi.vy -= dy * bodyj.mass * mag;
         bodyi.vz -= dz * bodyj.mass * mag;

         bodyj.vx += dx * bodyi.mass * mag;
         bodyj.vy += dy * bodyi.mass * mag;
         bodyj.vz += dz * bodyi.mass * mag;
      }
   }

   for (var i=0; i<size; i++) {
      var body = this.bodies[i];
      body.x += dt * body.vx;
      body.y += dt * body.vy;
      body.z += dt * body.vz;
   }
}

NBodySystem.prototype.energy = function(){
   var dx, dy, dz, distance;
   var e = 0.0;
   var size = this.bodies.length;

   for (var i=0; i<size; i++) {
      var bodyi = this.bodies[i];

      e += 0.5 * bodyi.mass *
         ( bodyi.vx * bodyi.vx
         + bodyi.vy * bodyi.vy
         + bodyi.vz * bodyi.vz );

      for (var j=i+1; j<size; j++) {
         var bodyj = this.bodies[j];
         dx = bodyi.x - bodyj.x;
         dy = bodyi.y - bodyj.y;
         dz = bodyi.z - bodyj.z;

         distance = Math.sqrt(dx*dx + dy*dy + dz*dz);
         e -= (bodyi.mass * bodyj.mass) / distance;
      }
   }
   return e;
}

// infrastructure

function timeStamp() {
    if (typeof Duktape == 'object')
        return Date.now();
	var timeStampInMs =
		window.performance &&
		window.performance.now &&
		window.performance.timing &&
		window.performance.timing.navigationStart
			? window.performance.now() + window.performance.timing.navigationStart : Date.now();
	return timeStampInMs;
}


function profile(tname,cnt,testFn) {
	var t = 100500;
	var count = cnt
	while ( count>0 ) {
		var t0 = timeStamp();
		testFn();
		var t1 = timeStamp();
		var dt = t1 - t0;
		t = Math.min(dt, t);
		count --;
	}
	t /= 1000.0;
	var msg = '"'+tname+'", '+t+', '+cnt;
    if (typeof Duktape == 'object')
    {
        print(msg)
	} else
	{
	    msg+='\n'
        console.log(msg);
      	document.body.innerHTML += msg + "<br>";
  	}
}

function performance_tests() {
	profile("tree",1,function(){
		testTree();
	});
	profile("primes",20,function(){
		primes(14000);
	});
	profile("exponent",20,function(){
		expLoop(1000000);
	});
	profile("fibonacci loop",20,function(){
		fibI(6511134);
	});
	profile("fibonacci recursive",20,function(){
		fibR(31)
	});
	{
		var src = dict_makeSrc();
		profile("dictionary",20,function(){
			dict(src);
		});
	}
	{
		var particles = make_particles();
		profile("particles",20,function(){
			multi_update_particles(particles,100);
		});
		profile("particles, inline",20,function(){
			multi_update_particles_i(particles,100);
		});
	}
	{
		var bodies = new NBodySystem( Array(
		   Sun(),Jupiter(),Saturn(),Uranus(),Neptune()
		));
		profile("n-bodies",20,function(){
			for (var i=0; i<500000; i++) {
				bodies.advance(0.01);
			}
		});
	}
	timeStamp();
}

performance_tests();

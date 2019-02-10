// primes test

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
	var msg = '"'+tname+'",'+cnt+','+t;
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
	timeStamp();
}
performance_tests();


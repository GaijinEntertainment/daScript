// primes test

function isprime(n) {
	let m = n - 1;
	for(let i=2; i!=m; ++i ) {
		if ( n % i ==0 ) {
			return false;
		}
	}
	return true;
}

function primes(n) {
	let count = 0;
	for ( let i=2; i!=n; ++i ) {
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
	let n = particles.length;
	for ( let i=0; i!=n; ++i ) {
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
	let n = particles.length;
	for ( let i=0; i!=n; ++i ) {
		let p = particles[i];
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
	let particles = []
	let n = 50000;
	for ( let i=0; i!=n; ++i ) {
		let p = {
			pos : {x : i + 0.1, y : i + 0.2, z : i + 0.3},
			vel : {x : 1.1, y : 2.1,  z : 3.1}
		};
		particles.push(p);
	}
	return particles;
}

// dictionary test

function dict_makeSrc(){
	let src = [];
	let n = 500000;
	let modn = n;
	for (let i=0; i != n; ++i ) {
		let num = (271828183 ^ i*119) % modn;
		src.push('_' + num);
	}
	return src;
}

function dict(src) {
	let tab = {}
	let n = src.length;
	let max = 1
	for (let i=0; i != n; ++i ) {
		let l = src[n];
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
	let sum = 0;
	for ( let i=0; i!=n; ++i ) {
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
	let last = 0
	let cur = 1
	n = n - 1
	while ( n>0 ) {
		n = n - 1
		let tmp = cur
		cur = last + cur
		last = tmp
	}
	return cur
}

// infrastructure

function timeStamp() {
	var timeStampInMs = 
		window.performance && 
		window.performance.now && 
		window.performance.timing && 
		window.performance.timing.navigationStart 
			? window.performance.now() + window.performance.timing.navigationStart : Date.now();
	return timeStampInMs;
}


function profile(tname,cnt,testFn) {
	let t = 100500;
	let count = cnt
	while ( count>0 ) {
		let t0 = timeStamp();
		testFn();
		let t1 = timeStamp();
		let dt = t1 - t0;
		t = Math.min(dt, t);
		count --;
	}
	t /= 1000.0;
	let msg = '"'+tname+'",'+cnt+','+t+'\n';
	console.log(msg);
	document.body.innerHTML += msg + "<br>";
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
		let src = dict_makeSrc();
		profile("dictionary",20,function(){
			dict(src);
		});	
	}
	{
		let particles = make_particles();
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


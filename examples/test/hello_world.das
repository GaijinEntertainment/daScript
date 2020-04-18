options log = true
options log_infer_passes = true

require functional

def forward ( var src : iterator<auto(QQ)> )
	return <- generator<QQ const &> () <| $ ()
		for w in src
			yield w
		return false

[export]
def test
	(	[[for x in range(0,10); x]]
	|>	forward()
	)
	return true

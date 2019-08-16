require math

options logCpp=true

[sideeffects]
def cf(a)
	return ceili(a)

[export]
def test
	print("ceili(1.2)={cf(1.2)}")
    return true

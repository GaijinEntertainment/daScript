options gen2

module test_module_identity_a public

require test_module_identity_b public

def identity_a() : int => identity_b() + 1

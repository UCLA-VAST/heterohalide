import heterocl as hcl
hcl.init()
__expo = hcl.local(init = 2.71828, dtype = hcl.Float())

def _all(_input, ):
    def test(y, x, _input):
        # return hcl.power(2.71828, _input[y, x])
        # return hcl.power(__expo[0], _input[y, x])
        return 1 + hcl.call_pure_intrin("float", "pow", 2.718280, _input[y, x])
    _output = hcl.compute((4, 4), lambda y, x : test(y, x, _input, ), name = "test_local", dtype = hcl.Float())
    return _output

_input = hcl.placeholder((4, 4, ), name = "_input")
s = hcl.create_schedule([_input, ], _all)
f = hcl.build(s)
print(hcl.lower(s))
print(hcl.build(s, target = "soda"))
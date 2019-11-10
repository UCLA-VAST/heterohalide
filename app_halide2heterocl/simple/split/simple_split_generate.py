import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        def _output_0(_output_s0_x, _output_s0_y, _input, ):
            out = hcl.local(init = 0)
            out[0] = (_input[((_output_s0_y * 10) + ((_output_min_0 + _output_s0_x_x_inner_base_s) + _output_s0_x_x_inner))] * 2)
            return out[0]
        _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

        _output = hcl.compute((10, 10, ), lambda _output_s1_x, _output_s1_y: _output_0(_output_s1_x, _output_s1_y, _input, _output, ), name = "_output_0")

        return _output

    _input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
_x_x_outer, _x_x_inner = s[s_0].split(s_0.axis[0], 2)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))
import heterocl as hcl
hcl.init()


def _output(_input):
    def _output_0(_output_s0_x, _output_s0_y, _input, ):
        out = hcl.local(init = 0)
        out[0] = (_input[((_output_s0_y * 10) + _output_s0_x)] * 2)
        return out[0]
    _output = hcl.compute((10, 10, ), lambda _output_s0_x, _output_s0_y: _output_0(_output_s0_x, _output_s0_y, _input, ), name = "_output_0")

    return _output

_input = hcl.placeholder((10 * 10, ), name = "_input")
s = hcl.create_schedule([_input, ], _output)
f = hcl.build(s)
print(hcl.lower(s))

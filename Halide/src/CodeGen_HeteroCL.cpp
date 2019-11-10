#include <iostream>
#include <limits>

#include "CodeGen_HeteroCL.h"
#include "CodeGen_Internal.h"
#include "Deinterleave.h"
#include "IROperator.h"
#include "Lerp.h"
#include "Param.h"
#include "Simplify.h"
#include "Substitute.h"
#include "Var.h"
#include "fstream"
#include "Debug.h"
#include "FindCalls.h"

namespace Halide {
namespace Internal {

using std::endl;
using std::map;
using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

extern "C" unsigned char halide_internal_initmod_inlined_c[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntime_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeCuda_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeHexagonHost_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeMetal_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeOpenCL_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeOpenGLCompute_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeOpenGL_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeQurt_h[];
extern "C" unsigned char halide_internal_runtime_header_HalideRuntimeD3D12Compute_h[];

/*
namespace {

const string headers =
    "#include <iostream>\n"
    "#include <math.h>\n"
    "#include <float.h>\n"
    "#include <assert.h>\n"
    "#include <string.h>\n"
    "#include <stdio.h>\n"
    "#include <stdint.h>\n";

// We now add definitions of things in the runtime which are
// intended to be inlined into every module but are only expressed
// in .ll. The redundancy is regrettable (FIXME).
const string globals = R"INLINE_CODE(
extern "C" {
int64_t halide_current_time_ns(void *ctx);
void halide_profiler_pipeline_end(void *, void *);
}

#ifdef _WIN32
__declspec(dllimport) float __cdecl roundf(float);
__declspec(dllimport) double __cdecl round(double);
#else
inline float asinh_f32(float x) {return asinhf(x);}
inline float acosh_f32(float x) {return acoshf(x);}
inline float atanh_f32(float x) {return atanhf(x);}
inline double asinh_f64(double x) {return asinh(x);}
inline double acosh_f64(double x) {return acosh(x);}
inline double atanh_f64(double x) {return atanh(x);}
#endif
inline float sqrt_f32(float x) {return sqrtf(x);}
inline float sin_f32(float x) {return sinf(x);}
inline float asin_f32(float x) {return asinf(x);}
inline float cos_f32(float x) {return cosf(x);}
inline float acos_f32(float x) {return acosf(x);}
inline float tan_f32(float x) {return tanf(x);}
inline float atan_f32(float x) {return atanf(x);}
inline float atan2_f32(float x, float y) {return atan2f(x, y);}
inline float sinh_f32(float x) {return sinhf(x);}
inline float cosh_f32(float x) {return coshf(x);}
inline float tanh_f32(float x) {return tanhf(x);}
inline float hypot_f32(float x, float y) {return hypotf(x, y);}
inline float exp_f32(float x) {return expf(x);}
inline float log_f32(float x) {return logf(x);}
inline float pow_f32(float x, float y) {return powf(x, y);}
inline float floor_f32(float x) {return floorf(x);}
inline float ceil_f32(float x) {return ceilf(x);}
inline float round_f32(float x) {return roundf(x);}

inline double sqrt_f64(double x) {return sqrt(x);}
inline double sin_f64(double x) {return sin(x);}
inline double asin_f64(double x) {return asin(x);}
inline double cos_f64(double x) {return cos(x);}
inline double acos_f64(double x) {return acos(x);}
inline double tan_f64(double x) {return tan(x);}
inline double atan_f64(double x) {return atan(x);}
inline double atan2_f64(double x, double y) {return atan2(x, y);}
inline double sinh_f64(double x) {return sinh(x);}
inline double cosh_f64(double x) {return cosh(x);}
inline double tanh_f64(double x) {return tanh(x);}
inline double hypot_f64(double x, double y) {return hypot(x, y);}
inline double exp_f64(double x) {return exp(x);}
inline double log_f64(double x) {return log(x);}
inline double pow_f64(double x, double y) {return pow(x, y);}
inline double floor_f64(double x) {return floor(x);}
inline double ceil_f64(double x) {return ceil(x);}
inline double round_f64(double x) {return round(x);}

inline float nan_f32() {return NAN;}
inline float neg_inf_f32() {return -INFINITY;}
inline float inf_f32() {return INFINITY;}
inline bool is_nan_f32(float x) {return x != x;}
inline bool is_nan_f64(double x) {return x != x;}

template<typename A, typename B>
inline A reinterpret(const B &b) {
    #if __cplusplus >= 201103L
    static_assert(sizeof(A) == sizeof(B), "type size mismatch");
    #endif
    A a;
    memcpy(&a, &b, sizeof(a));
    return a;
}
inline float float_from_bits(uint32_t bits) {
    return reinterpret<float, uint32_t>(bits);
}

template<typename T>
inline int halide_popcount(T a) {
    int bits_set = 0;
    while (a != 0) {
        bits_set += a & 1;
        a >>= 1;
    }
    return bits_set;
}

template<typename T>
inline int halide_count_leading_zeros(T a) {
    int leading_zeros = 0;
    int bit = sizeof(a) * 8 - 1;
    while (bit >= 0 && (a & (((T)1) << bit)) == 0) {
        leading_zeros++;
        bit--;
    }
    return leading_zeros;
}

template<typename T>
inline int halide_count_trailing_zeros(T a) {
    int trailing_zeros = 0;
    constexpr int bits = sizeof(a) * 8;
    int bit = 0;
    while (bit < bits && (a & (((T)1) << bit)) == 0) {
        trailing_zeros++;
        bit++;
    }
    return trailing_zeros;
}

template<typename T>
inline T halide_cpp_max(const T &a, const T &b) {return (a > b) ? a : b;}

template<typename T>
inline T halide_cpp_min(const T &a, const T &b) {return (a < b) ? a : b;}

template<typename A, typename B>
const B &return_second(const A &a, const B &b) {
    (void) a;
    return b;
}

template<typename A, typename B>
inline auto quiet_div(const A &a, const B &b) -> decltype(a / b) {
    return b == 0 ? static_cast<decltype(a / b)>(0) : (a / b);
}

template<typename A, typename B>
inline auto quiet_mod(const A &a, const B &b) -> decltype(a % b) {
    return b == 0 ? static_cast<decltype(a % b)>(0) : (a % b);
}

namespace {
class HalideFreeHelper {
    typedef void (*FreeFunction)(void *user_context, void *p);
    void * user_context;
    void *p;
    FreeFunction free_function;
public:
    HalideFreeHelper(void *user_context, void *p, FreeFunction free_function)
        : user_context(user_context), p(p), free_function(free_function) {}
    ~HalideFreeHelper() { free(); }
    void free() {
        if (p) {
            // TODO: do all free_functions guarantee to ignore a nullptr?
            free_function(user_context, p);
            p = nullptr;
        }
    }
};
} // namespace

)INLINE_CODE";
}  // namespace

class TypeInfoGatherer : public IRGraphVisitor {
private:
    using IRGraphVisitor::include;
    using IRGraphVisitor::visit;

    void include_type(const Type &t) {
        if (t.is_vector()) {
            if (t.is_bool()) {
                // bool vectors are always emitted as uint8 in the C++ backend
                // TODO: on some architectures, we could do better by choosing
                // a bitwidth that matches the other vectors in use; EliminateBoolVectors
                // could be used for this with a bit of work.
                vector_types_used.insert(UInt(8).with_lanes(t.lanes()));
            } else if (!t.is_handle()) {
                // Vector-handle types can be seen when processing (e.g.)
                // require() statements that are vectorized, but they
                // will all be scalarized away prior to use, so don't emit
                // them.
                vector_types_used.insert(t);
                if (t.is_int()) {
                    // If we are including an int-vector type, also include
                    // the same-width uint-vector type; there are various operations
                    // that can use uint vectors for intermediate results (e.g. lerp(),
                    // but also Mod, which can generate a call to abs() for int types,
                    // which always produces uint results for int inputs in Halide);
                    // it's easier to just err on the side of extra vectors we don't
                    // use since they are just type declarations.
                    vector_types_used.insert(t.with_code(halide_type_uint));
                }
            }
        }
    }

    void include_lerp_types(const Type &t) {
        if (t.is_vector() && t.is_int_or_uint() && (t.bits() >= 8 && t.bits() <= 32)) {
            Type doubled = t.with_bits(t.bits() * 2);
            include_type(doubled);
        }
    }

protected:
    void include(const Expr &e) override {
        include_type(e.type());
        IRGraphVisitor::include(e);
    }

    // GCC's __builtin_shuffle takes an integer vector of
    // the size of its input vector. Make sure this type exists.
    void visit(const Shuffle *op) override {
        vector_types_used.insert(Int(32, op->vectors[0].type().lanes()));
        IRGraphVisitor::visit(op);
    }

    void visit(const For *op) override {
        for_types_used.insert(op->for_type);
        IRGraphVisitor::visit(op);
    }

    void visit(const Ramp *op) override {
        include_type(op->type.with_lanes(op->lanes));
        IRGraphVisitor::visit(op);
    }

    void visit(const Broadcast *op) override {
        include_type(op->type.with_lanes(op->lanes));
        IRGraphVisitor::visit(op);
    }

    void visit(const Cast *op) override {
        include_type(op->type);
        IRGraphVisitor::visit(op);
    }

    void visit(const Call *op) override {
        include_type(op->type);
        if (op->is_intrinsic(Call::lerp)) {
            // lower_lerp() can synthesize wider vector types.
            for (auto &a : op->args) {
                include_lerp_types(a.type());
            }
        } else if (op->is_intrinsic(Call::absd)) {
            // absd() can synthesize a new type
            include_type(op->type.with_code(op->type.is_int() ? Type::UInt : op->type.code()));
        }

        IRGraphVisitor::visit(op);
    }

public:
    std::set<ForType> for_types_used;
    std::set<Type> vector_types_used;
};
*/


class TypeFinder : public IRVisitor {
public:
    // ScopeFinder();
    // ~ScopeFinder();
    enum InputType {
        Float,
        Int,
        UInt,
    } input_type;

    int input_bits;

    void compile(const Module &module) {
        debug(0) << "TypeFinder begin compile module...\n";
        for (const auto &f : module.functions()) {
            compile(f);
        }
    }

protected:
    using IRVisitor::visit;

    void visit(const Call *op) override {
        debug(0) << "TypeFinder visit Call...\n";
        if (op->call_type == Internal::Call::CallType::Image) { // ignore other type of call
            input_bits = op->image.type().bits();

            if (op->image.defined()) { // only call input Image will be True. Call other intermediate Func won't go into this branch
                if (op->image.type().is_float()) {
                    input_type = InputType::Float;
                } else if (op->image.type().is_int()) {
                    input_type = InputType::Int;
                } else if (op->image.type().is_uint()) {
                    input_type = InputType::UInt;
                } else {
                    internal_error << "Input Type is not one of (Float, Int, UInt)\n";
                    
                }
            }
        }
    }

    virtual void compile(const LoweredFunc &f) {
        f.body.accept(this); 
    }

};





CodeGen_HeteroCL::CodeGen_HeteroCL(ostream &s, const std::vector<Function> &out_funcs, const std::vector<int> &out_shape, Target t, OutputKind output_kind, const std::string &guard) :
    IRPrinter(s), id("$$ BAD ID $$"), output_funcs(out_funcs), output_shape(out_shape), target(t), output_kind(output_kind) {
        for (Function f : out_funcs) {
            Internal::populate_environment(f, env);
        }
    }

CodeGen_HeteroCL::~CodeGen_HeteroCL() {
    }





string CodeGen_HeteroCL::print_name(const string &name) {
    ostringstream oss;

    // Prefix an underscore to avoid reserved words (e.g. a variable named "while")
    // if (isalpha(name[0])) {
    //     oss << '_';
    // }

    for (size_t i = 0; i < name.size(); i++) {
        if (name[i] == '.') {
            oss << '_';
        } else if (name[i] == '$') {
            oss << "__";
        } else if (name[i] != '_' && !isalnum(name[i])) {
            oss << "___";
        }
        else oss << name[i];
    }
    return oss.str();
}


void CodeGen_HeteroCL::compile(const Module &input) {
    // TypeInfoGatherer type_info;
    // for (const auto &f : input.functions()) {
    //     if (f.body.defined()) {
    //         f.body.accept(&type_info);
    //     }
    // }

    // stream << "\n";

    // ignore buffers for now
    // for (const auto &b : input.buffers()) {
    //     compile(b);
    // }
    TypeFinder tf;
    tf.compile(input);
    debug(1) << "TypeFinder type: " << tf.input_type << "\n";
    if (tf.input_type == TypeFinder::InputType::Float) {
        input_data_type = "hcl.Float(bits = " + std::to_string(tf.input_bits) + ")";
    } else if (tf.input_type == TypeFinder::InputType::Int) {
        input_data_type = "hcl.Int(bits = " + std::to_string(tf.input_bits) + ")";
    } else if (tf.input_type == TypeFinder::InputType::UInt) {
        input_data_type = "hcl.UInt(bits = " + std::to_string(tf.input_bits) + ")";
    }



    debug(0) << "begin compile module...\n";

    do_indent();
    stream << "import heterocl as hcl" << "\n";
    do_indent();
    // stream << "hcl.init(" << input_data_type << ")" << "\n"; // dont use it for now
    stream << "hcl.init()\n";
    
    // pre-defined variable: e.g. e
    // do_indent();
    // stream << "__expo = hcl.local(init = 2.71828, dtype = hcl.Float())\n";




    // do_indent();
    // stream << "def _all(";
    // for (auto iter = input_name.begin(); iter != input_name.end(); ++iter) { // bug here: dont have input_name info now. Collect it in the function compiling level
    //     stream << *iter << ", ";
    // }
    // stream << "):\n";
    // indent += 4;


    // std::ofstream tmp_file("/curr/jiajieli/app_halide2heterocl/tmp");
    // CodeGen_HeteroCL find_name(tmp_file, input.target());
    // for (const auto &f : input.functions()) {
    //     input_name = find_name.find_input_name(f);
    // }
    debug(0) << "vector<LoweredFunc> size: " << input.functions().size() << "\n";
    for (const auto &f : input.functions()) {
        compile(f);
    }
    // do_indent();
    // stream << "\n" << "return " << output_name << "\n";

    // indent -= 4;

    // do_indent();
    // print_placeholder();

    do_indent();
    schedule_name = "s";
    stream << schedule_name << " = hcl.create_schedule([";

    for(auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
        stream << *iter << ", ";
    }
    stream << "], " << "top" << ")\n";
    
    schedule();

    stream << "f = hcl.build(s)\n";
    stream << "print(hcl.lower(s))\n"; // for convenience to see HeteroCL IR   

    instantiate();
}

void CodeGen_HeteroCL::instantiate() {
    do_indent();
    stream << "import numpy as np\n";

    for (auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
    // for (auto iter = input_info.begin(); iter != input_info.end(); ++iter) {
        auto iter_info = input_info.find(*iter);

        string np_input_name = "np_" + iter_info->first;
        string hcl_input_name = "hcl_" + iter_info->first;
        hcl_input_name_group.push_back(hcl_input_name);
        do_indent();

        stream << np_input_name << " = " << "np.load(\"" << iter_info->first << ".npy\")\n";

        do_indent();
        stream << hcl_input_name << " = " << "hcl.asarray(" << np_input_name << ", ";
        stream << "dtype = " << input_type[iter_info->first].first << "(bits = " << std::to_string(input_type[iter_info->first].second) << ")";
        stream << ")\n";
    }

    do_indent();
    stream << "output_shape = (";
    for (size_t i = 0; i < output_shape.size(); i++) {
        stream << output_shape[i] << ", ";
    }
    stream << ")\n";

    do_indent();
    stream << "hcl_out = hcl.asarray(np.zeros(output_shape), " << "dtype = " << stage_type.first << "(bits = " << std::to_string(stage_type.second) << "))\n";
    do_indent();
    stream << "f(";
    for (size_t i = 0; i < hcl_input_name_group.size(); i++) {
        stream << hcl_input_name_group[i] << ", ";
    }
    stream << "hcl_out)\n";
    do_indent();
    stream << "np_out = hcl_out.asnumpy()\n";
    do_indent();
    stream << "np.save(\"output_heterocl.npy\", np_out)" << "\n";
    stream << "print(hcl.build(s, target = \"soda\"))" << "\n";

}

void CodeGen_HeteroCL::schedule() {
    // unroll
    for (auto iter = unroll_info.begin(); iter != unroll_info.end(); ++iter) {
        string stage_name = iter->first;
        string schedule_stage_name = "s_" + iter->first;
        do_indent();
        stream << schedule_stage_name << " = " << "top." << stage_name << "\n";

        for (auto iter_pair = iter->second.begin(); iter_pair != iter->second.end(); ++iter_pair) {
            do_indent();
            stream << "s[" << schedule_stage_name << "].unroll(" << schedule_stage_name << ".axis[" << iter_pair->first << "], " << iter_pair->second << ")\n";
        }
    }

    // parallel
    for (auto iter = parallel_info.begin(); iter != parallel_info.end(); ++iter) {
        string stage_name = iter->first;
        string schedule_stage_name = "s_" + iter->first;
        do_indent();
        stream << schedule_stage_name << " = " << "top." << stage_name << "\n";

        for (auto iter_axis = iter->second.begin(); iter_axis != iter->second.end(); ++iter_axis) {
            do_indent();
            stream << "s[" << schedule_stage_name << "].parallel(" << schedule_stage_name << ".axis[" << *iter_axis << "])\n";
        }
    }



    // Set Default order of Halide, because HeteroCL order is the opposit of Halide default order   
    
    // Dont need that now

    // string schedule_stage_name;
    // for (int i = 0; i < (int)stage_name_group.size(); i++) {
    //     schedule_stage_name = "s" + stage_name_group[i];
    //     do_indent();
    //     stream << schedule_stage_name << " = " << "_all." << stage_name_group[i] << "\n";
    //     do_indent();
    //     stream << "s[" << schedule_stage_name << "].reorder(";
    //     for (int dim = stage_dim_group[i] - 1; dim >= 0; dim--) {
    //         stream << schedule_stage_name << ".axis[" << dim << "], ";
    //     }
    //     // print axis depending on the dim of the stage
    //     stream << ")\n";
    // }



    // some test on output_funcs


    debug(1) << "env in HeteroCL: \n"; // only a "output"
    for (auto iter = env.begin(); iter != env.end(); ++iter) {
        debug(1) << iter->first << ":\n ";
        vector<Dim> dim_group = iter->second.definition().schedule().dims();
        for (auto iter_dim = dim_group.begin(); iter_dim != dim_group.end(); ++iter_dim ) {
            debug(1) << iter_dim->var << ", " << iter_dim->unroll_factor << "\n";
        }
    }
    debug(1) << "\n";
    // print as followed: 
    // blur_x:
    // x, 2
    // y, 0
    // __outermost, 0
    // blur_y:
    // x, 4
    // y, 0
    // __outermost, 0


    // debug(0) << "print dim order in HeteroCL CodeGen:\n"; // here already have order scheduling: _outermost, x, y
    // string var_name;
    // debug(0) << "output_funcs size: " << output_funcs.size() << "\n";
    // for (auto iter = output_funcs.rbegin(); iter != output_funcs.rend(); iter++) {
    //     const auto &f = *iter; // f: class LoweredFunc
    //     Definition def = f.definition();
    //     StageSchedule stage_s = def.schedule();
    //     debug(0) << "dim size: " << stage_s.dims().size() << "\n";
    //     for (int i = (int)stage_s.dims().size() - 1; i >= 0; i--) {
    //         const Dim &dim = stage_s.dims()[i];
    //         var_name = print_name(dim.var);
    //         debug(0) << var_name << "\n";
    //     }

    //     vector<Split> splits = stage_s.splits();
    //     debug(0) << "vector splits size: " << splits.size() << "\n"; // 1

    //     string old_var, outer, inner;
    //     for (auto iter = splits.begin(); iter != splits.end(); ++iter) {
    //         debug(0) << print_name(iter->old_var) << ", " << iter->outer << ", " << iter->inner << "\n"; // x, x.x_outer, x.x_inner
    //         debug(0) << "split type: " << iter->split_type << "\n"; // 0:SplitVar
    //         debug(0) << "factor: " << iter->factor << "\n"; // class Expr, but in this sample can be printed directly: 2
            
    //         old_var = get_axis(iter->old_var);
    //         outer = print_name(iter->outer);
    //         inner = print_name(iter->inner);
    //         do_indent();
    //         stream << outer << ", " << inner << " = " << "s[s_0].split(" << "s_0." << old_var << ", ";
    //         iter->factor.accept(this);
    //         stream << ")\n";
            



    //     }

    // }

}

    




string CodeGen_HeteroCL::get_axis(string Halide_axis) {
    string HeteroCL_axis;
    if (Halide_axis.compare("x") == 0) {
        HeteroCL_axis = "axis[0]";
    } else if (Halide_axis.compare("y") == 0) {
        HeteroCL_axis = "axis[1]";
    } else {
        HeteroCL_axis = "not_determined_yet";
    }
    return HeteroCL_axis;
}

void CodeGen_HeteroCL::print_output_extent(string func_name) {
    int output_dim = output_shape.size();
    string output_extent_name;
    for (int i = 0; i < output_dim; i++) {
        output_extent_name = func_name + "_extent_" + std::to_string(i);
        do_indent();
        stream << output_extent_name << " = " << output_shape[i] << "\n";
    }
}

void CodeGen_HeteroCL::print_output_min(string func_name) {
    int output_dim = output_shape.size();
    string output_min_name;
    for (int i = 0; i < output_dim; i++) {
        output_min_name = func_name + "_min_" + std::to_string(i);
        do_indent();
        stream << output_min_name << " = 0\n";
    }
}

void CodeGen_HeteroCL::compile(const LoweredFunc &f) {
    function_name = print_name(f.name); // it is also the last stage's
    print_output_extent(function_name);
    print_output_min(function_name); // simply set all min = 0

    const std::vector<LoweredArgument> &args = f.args;
    for (size_t i = 0; i < args.size() - 1; i++) {
        input_name.push_back(print_name(args[i].name)); // the frontest one is _output, which we don't want. so the for loop end at args.size()-2
    }

    do_indent();
    stream << "def top(";
    for (auto iter = input_name.begin(); iter != input_name.end(); ++iter) { // bug here: dont have input_name info now. Collect it in the function compiling level
        stream << *iter << ", ";
    }
    stream << "):\n";

    indent += 4;
    
    // // claim output stage's compute function in advance, because the last stage doesn't have a Realize node
    // stream << function_name << " = " << "hcl.compute(";
    // for(size_t i = 0; i < output_shape.size(); i++) {
    //     stream << output_shape[i];
    //     if (i < output_shape.size() - 1) {
    //         stream << ", ";
    //     }
    // }
    // stream << ", lambda: ";
    // print_index(output_shape.size());
    // stream << ": 0, " << "name = \"" << function_name << "\", "

    print(f.body);  

    do_indent();

    stream << "return " << function_name << "\n";

    /* Version 1
    stream << "return " << stage_name << "\n";
    */

    indent -= 4;

    do_indent();
    print_placeholder(); // bug here. input_info is collected in Load visitor, but now only Call
}

string CodeGen_HeteroCL::print_expr(Expr e) {
    id = "$$ BAD ID $$";
    e.accept(this);
    std::cout << id << endl;
    return id;
}

void CodeGen_HeteroCL::print_stmt(Stmt s) {
    s.accept(this);
}

// string CodeGen_HeteroCL::print_type(Type type, AppendSpaceIfNeeded space_option) {
//     return type_to_c_type(type, space_option == AppendSpace);
// }

// string CodeGen_HeteroCL::print_assignment(Type t, const std::string &rhs) {
//     auto cached = cache.find(rhs);
//     if (cached == cache.end()) {
//         id = unique_name('_');
//         do_indent();
//         stream << print_type(t, AppendSpace) << (output_kind == CPlusPlusImplementation ? "const " : "") << id << " = " << rhs << ";\n";
//         cache[rhs] = id;
//     } else {
//         id = cached->second;
//     }
//     return id;
// }

// void CodeGen_HeteroCL::open_scope() {
//     cache.clear();
//     do_indent();
//     indent++;
//     stream << "{\n";
// }

// void CodeGen_HeteroCL::close_scope(const std::string &comment) {
//     cache.clear();
//     indent--;
//     do_indent();
//     if (!comment.empty()) {
//         stream << "} // " << comment << "\n";
//     } else {
//         stream << "}\n";
//     }
// }


void CodeGen_HeteroCL::visit(const Variable *op) {
    id = print_name(op->name);
    auto iter = inter_var.find(id);
    if (iter == inter_var.end()) { // can't find it
        stream << id;
    } else { // found it
        inter_var[id].accept(this);
    }
}


void CodeGen_HeteroCL::visit(const Cast *op) {
    debug(0) << "visit Cast...\n";

    string hcl_type = "";
    int bits = 0;
    enum CastType {
        Float,
        Int,
        UInt,
    } cast_type = CastType::Int;


    if (op->type.is_float()) {
        cast_type = CastType::Float;
    } else if (op->type.is_int()) {
        cast_type = CastType::Int;
    } else if (op->type.is_uint()) {
        cast_type = CastType::UInt;
    } else {
        internal_error << "Cast Type is not one of (Float, Int, UInt)\n";
    }
    bits = op->type.bits();
    
    switch (cast_type)
    {
    case CastType::Float:
        hcl_type = "hcl.Float(bits = " + std::to_string(bits) + ")";
        break;
    case CastType::Int:
        hcl_type = "hcl.Int(bits = " + std::to_string(bits) + ")";
        break;
    case CastType::UInt:
        hcl_type = "hcl.UInt(bits = " + std::to_string(bits) + ")";
        break;
    default:
        internal_error << "Cast Type is not one of (Float, Int, UInt)\n";
        break;
    }

    stream << "hcl.cast(dtype = " << hcl_type;
    stream << ", " << "expr = ";
    op->value.accept(this);
    stream << ")"; 
}

void CodeGen_HeteroCL::visit_binop(Type t, Expr a, Expr b, const char * op) {
    // string sa = print_expr(a);
    // string sb = print_expr(b);
    // stream << "(" + sa + " " + op + " " + sb + ")";
    debug(0) << "visit binop\n";
    debug(0) << "op: " << op << "\n";

    if (!reduction_info.empty()) { // if it's a reduction operation, didn't show the initial value, thus to eliminate an unnecessary stage. But of course this optimization is not general
        // TODO: need to determine the scope using reduction domain index, might need class ScopeFinder
        stream << "(";
        a.accept(this);
        debug(0) << "finish Expr a of this binop\n";
        stream << " " << op << " ";
        stream << reducer_name << "(\n";
        indent += 4;
        do_indent();
        stream << "axis = [";        
        for (auto iter = reduction_info.rbegin(); iter != reduction_info.rend(); ++iter) { // need to print it in the opposite order: z, y, x. So that its order loop is z,y,x (outer to inner), which is the same as Halide default 
            stream << iter->first << ", ";
        }
        stream << "],\n";
        do_indent();
        reduction_info.clear();

        stream << "expr = ";
        b.accept(this);
        stream << "\n";
        indent -= 4;
        do_indent();
        stream << "))";
    } else { // normal op
        stream << "(";
        a.accept(this);
        stream << " " << op << " ";
        b.accept(this);
        stream << ")";
    }
}


void CodeGen_HeteroCL::visit(const Add *op) {
    visit_binop(op->type, op->a, op->b, "+");
}

void CodeGen_HeteroCL::visit(const Sub *op) {
    visit_binop(op->type, op->a, op->b, "-");
}

void CodeGen_HeteroCL::visit(const Mul *op) {
    visit_binop(op->type, op->a, op->b, "*");
}

// void CodeGen_HeteroCL::visit(const Div *op) {
//     int bits;
//     if (is_const_power_of_two_integer(op->b, &bits)) {
//         visit_binop(op->type, op->a, make_const(op->a.type(), bits), ">>");
//     } else if (op->type.is_int()) {
//         print_expr(lower_euclidean_div(op->a, op->b));
//     } else {
//         visit_binop(op->type, op->a, op->b, "/");
//     }
// }

/*
void CodeGen_HeteroCL::visit(const Mod *op) {
    int bits;
    if (is_const_power_of_two_integer(op->b, &bits)) {
        visit_binop(op->type, op->a, make_const(op->a.type(), (1 << bits)-1), "&");
    } else if (op->type.is_int()) {
        print_expr(lower_euclidean_mod(op->a, op->b));
    } else if (op->type.is_float()) {
        string arg0 = print_expr(op->a);
        string arg1 = print_expr(op->b);
        ostringstream rhs;
        rhs << "fmod(" << arg0 << ", " << arg1 << ")";
        print_assignment(op->type, rhs.str());
    } else {
        visit_binop(op->type, op->a, op->b, "%");
    }
}
*/

void CodeGen_HeteroCL::visit(const Max *op) { // Max and Min need to be modified. Re-visit the nodes
    // use hcl.select(a > b, a, b) to replace max(a, b)
    stream << "hcl.select(";
    op->a.accept(this);
    stream << " > ";
    op->b.accept(this);
    stream << ", ";
    op->a.accept(this);
    stream << ", ";
    op->b.accept(this);
    stream << ")";
}


void CodeGen_HeteroCL::visit(const Min *op) {
    // use hcl.select(a < b, a, b) to replace min(a, b)
    stream << "hcl.select(";
    op->a.accept(this);
    stream << " < ";
    op->b.accept(this);
    stream << ", ";
    op->a.accept(this);
    stream << ", ";
    op->b.accept(this);
    stream << ")";
}    


/*
void CodeGen_HeteroCL::visit(const EQ *op) {
    visit_binop(op->type, op->a, op->b, "==");
}

void CodeGen_HeteroCL::visit(const NE *op) {
    visit_binop(op->type, op->a, op->b, "!=");
}

void CodeGen_HeteroCL::visit(const LT *op) {
    visit_binop(op->type, op->a, op->b, "<");
}

void CodeGen_HeteroCL::visit(const LE *op) {
    visit_binop(op->type, op->a, op->b, "<=");
}

void CodeGen_HeteroCL::visit(const GT *op) {
    visit_binop(op->type, op->a, op->b, ">");
}

void CodeGen_HeteroCL::visit(const GE *op) {
    visit_binop(op->type, op->a, op->b, ">=");
}
*/

void CodeGen_HeteroCL::visit(const Or *op) {
    stream << "hcl.or_(";
    op->a.accept(this);
    stream << ", ";
    op->b.accept(this);
    stream << ")";
}


void CodeGen_HeteroCL::visit(const And *op) {
    stream << "hcl.and_(";
    op->a.accept(this);
    stream << ", ";
    op->b.accept(this);
    stream << ")";
}

/*
void CodeGen_HeteroCL::visit(const Not *op) {
    print_assignment(op->type, "!(" + print_expr(op->a) + ")");
}
*/

void CodeGen_HeteroCL::visit(const IntImm *op) {
    // if (op->type == Int(32)) {
    //     id = std::to_string(op->value);
    // } else {
    //     print_assignment(op->type, "(" + print_type(op->type) + ")(" + std::to_string(op->value) + ")");
    // }
    // id = std::to_string(op->value);
    if (op->type == Int(32)) {
        stream << op->value;
    } else {
    stream << "hcl.cast(dtype = hcl.";
    switch(op->type.code()) {
    case Type::Int:
        stream << "Int";
        break;
    case Type::UInt:
        stream << "UInt";
        break;
    case Type::Float:
        stream << "Float";
        break;
    default:
        internal_error << "IntImm Type not included in (Int, Uint, Float)\n";
        break;
    }
    stream << "(bits = " << op->type.bits() << "), ";
    stream << "expr = " << op->value << ")";
    }
}

void CodeGen_HeteroCL::visit(const UIntImm *op) {
    stream << "hcl.cast(dtype = hcl.";
    switch(op->type.code()) {
    case Type::Int:
        stream << "Int";
        break;
    case Type::UInt:
        stream << "UInt";
        break;
    case Type::Float:
        stream << "Float";
        break;
    default:
        internal_error << "IntImm Type not included in (Int, Uint, Float)\n";
        break;
    }
    stream << "(bits = " << op->type.bits() << "), ";
    stream << "expr = " << op->value << ")";
}

/*
void CodeGen_HeteroCL::visit(const StringImm *op) {
    ostringstream oss;
    oss << Expr(op);
    id = oss.str();
}

// NaN is the only float/double for which this is true... and
// surprisingly, there doesn't seem to be a portable isnan function
// (dsharlet).
template <typename T>
static bool isnan(T x) { return x != x; }

template <typename T>
static bool isinf(T x)
{
    return std::numeric_limits<T>::has_infinity && (
        x == std::numeric_limits<T>::infinity() ||
        x == -std::numeric_limits<T>::infinity());
}
*/

void CodeGen_HeteroCL::visit(const FloatImm *op) { // need to deal with bit type maybe. float in Python is float64 by default
    // stream << "float(" << op->value << ")";
    stream << "hcl.cast(dtype = hcl.";
    switch(op->type.code()) {
    case Type::Int:
        stream << "Int";
        break;
    case Type::UInt:
        stream << "UInt";
        break;
    case Type::Float:
        stream << "Float";
        break;
    default:
        internal_error << "IntImm Type not included in (Int, Uint, Float)\n";
        break;
    }
    stream << "(bits = " << op->type.bits() << "), ";
    stream << "expr = " << op->value << ")";
}

void CodeGen_HeteroCL::print_list_as_last_stage_order(vector<Expr> exprs) {
    debug(0) << "Check if call args vector size = last stage's for loop size...\n";
    // if (!last_func_axis_call_order.empty()) {
        // internal_assert(exprs.size() == last_func_axis_call_order.size()); // important
        // if (exprs.size() == last_func_axis_call_order.size()) { // no split 
    for (int i = 0; i < (int)last_func_axis_call_order.size(); i++) {
        exprs[ last_func_axis_call_order[i] ].accept(this);
        if (i < (int)last_func_axis_call_order.size() - 1) {
            stream << ", ";
        }
    }
        // } else { // split
        //     internal_error << "size error\n";
        // }
    // }
    


    // for (int i = (int)exprs.size() - 1; i >= 0; i--) {
    //     exprs[i].accept(this);
    //     if (i > 0) {
    //         stream << ", ";
    //     }
    // }
}

void CodeGen_HeteroCL::print_list_reverse(vector<Expr> exprs) {
    for (int i = (int)exprs.size() - 1; i >= 0; i--) {
        exprs[i].accept(this);
        if (i > 0) {
            stream << ", ";
        }
    }
}

void CodeGen_HeteroCL::print_data_cast() {
    stream << "hcl.cast(dtype = " << stage_type.first << "(bits = " << std::to_string(stage_type.second) << "), " << "expr = ";
}

void CodeGen_HeteroCL::visit(const Call *op) {
    // // copy from IRPrinter. 
    // // Need more "if" for other Call Function
    // stream << op->name << "[";

    // for (size_t i = 0; i < op->args.size(); i++) {
    //     op->args[i].accept(this);
    //     if (i < op->args.size() - 1) {
    //         stream << ", ";
    //     }
    // }

    // stream << "]";



    debug(0) << "visit call...\n";
    debug(0) << "name: " << op->name << "\n";
    debug(0) << "CallType: " << op->call_type << "\n";
    debug(0) << "if image defined?: " << op->image.defined() << "\n";

    if (op->call_type == Internal::Call::CallType::Image || op->call_type == Internal::Call::CallType::PureExtern || op->call_type == Internal::Call::CallType::Halide || op->call_type == Internal::Call::CallType::PureIntrinsic) { // ignore other type of call. 
    // PureExtern(3) include pow_f32, exp_f32; PureIntrinsic include absd(absolute difference)
        if (op->image.defined()) { // only call input Image will be True. Call other intermediate Func won't go into this branch
            debug(0) << "input dimensions: " << op->image.dimensions() << "\n";
            debug(0) << "input extent 0: " << op->image.extent(0) << "\n";
            // debug(0) << "input extent 1: " << op->image.extent(1) << "\n";
            // debug(0) << "input stride 0 : " << op->image.stride(0) << "\n";
            // debug(0) << "input stride 1: " << op->image.stride(1) << "\n";
            debug(0) << "input name: " << op->image.name() << "\n"; // input
            debug(0) << "input type UInt: " << op->image.type().is_uint() << "\n";
            debug(0) << "input type bits: " << op->image.type().bits() << "\n";
            string name = print_name(op->image.name());

            auto input_iter = input_info.find(name);
            if (input_iter == input_info.end()) { // can't find it
                std::vector<int> input_extents;
                for(int i = 0; i < op->image.dimensions(); i++) {
                    input_extents.push_back(op->image.extent(i));
                }
                input_info[name] = input_extents;
            } else { // find it
                debug(0) << "already save this input info\n";
            }
            
            auto input_type_iter = input_type.find(name);
            if (input_type_iter == input_type.end()) { // can't find it
                std::pair<string, int> type_bits;
                if (op->image.type().is_float()) {
                    type_bits.first = "hcl.Float";
                } else if (op->image.type().is_int()) {
                    type_bits.first = "hcl.Int";
                } else if (op->image.type().is_uint()) {
                    type_bits.first = "hcl.UInt";
                } else {
                    internal_error << "Realize Type is not one of {float, int, uint}\n";
                }
                type_bits.second = op->image.type().bits();
                input_type[name] = type_bits;
            } else { // find it
                debug(0) << "already save this input type info\n";
            }
        }
        
        string call_name;
        if (op->name == "absd") {
            stream << "absd(";
            print_list(op->args);
            stream << ", ";
        } else if (op->name == "pow_f32") {
            call_name = "hcl.power";
            stream << call_name << "(";
            print_list(op->args);
            stream << ")";
        } else if (op->name == "exp_f32") {
            stream << "hcl.call_pure_intrin(\"float\", \"pow\", 2.718280, ";
            // stream << "hcl.power(__expo[0], ";
            print_list(op->args);
            stream << ")";
        } else if (op->call_type == Internal::Call::CallType::Halide) { // Load multi-dimensional. Maybe need to check if call_name = one of the input or output name?

            stream << op->name << "[";
            for (size_t i = 0; i < op->args.size(); i++) {
                op->args[i].accept(this);
                if (i < op->args.size() - 1) {
                    stream << ", ";
                }
            }
            stream << "]";

        } else if (op->call_type == Internal::Call::CallType::Image) {

            stream << op->name << "[";
            for (size_t i = 0; i < op->args.size(); i++) {
                op->args[i].accept(this);
                if (i < op->args.size() - 1) {
                    stream << ", ";
                }
            }
            stream << "]";
            
        }
    }








    }

    /* Version 1 - Call
    debug(0) << "visit call...\n";
    debug(0) << "name: " << op->name << "\n";
    debug(0) << "CallType: " << op->call_type << "\n";
    debug(0) << "if image defined?: " << op->image.defined() << "\n";

    if (op->call_type == Internal::Call::CallType::Image || op->call_type == Internal::Call::CallType::PureExtern || op->call_type == Internal::Call::CallType::Halide || op->call_type == Internal::Call::CallType::PureIntrinsic) { // ignore other type of call. 
    // PureExtern(3) include pow_f32, exp_f32; PureIntrinsic include absd(absolute difference)
        if (op->image.defined()) { // only call input Image will be True. Call other intermediate Func won't go into this branch
            debug(0) << "input dimensions: " << op->image.dimensions() << "\n";
            debug(0) << "input extent 0: " << op->image.extent(0) << "\n";
            // debug(0) << "input extent 1: " << op->image.extent(1) << "\n";
            // debug(0) << "input stride 0 : " << op->image.stride(0) << "\n";
            // debug(0) << "input stride 1: " << op->image.stride(1) << "\n";
            debug(0) << "input name: " << op->image.name() << "\n"; // input
            debug(0) << "input type UInt: " << op->image.type().is_uint() << "\n";
            debug(0) << "input type bits: " << op->image.type().bits() << "\n";
            string name = print_name(op->image.name());

            auto input_iter = input_info.find(name);
            if (input_iter == input_info.end()) { // can't find it
                std::vector<int> input_extents;
                for(int i = 0; i < op->image.dimensions(); i++) {
                    input_extents.push_back(op->image.extent(i));
                }
                input_info[name] = input_extents;
            } else { // find it
                debug(0) << "already save this input info\n";
            }
            
            auto input_type_iter = input_type.find(name);
            if (input_type_iter == input_type.end()) { // can't find it
                std::pair<string, int> type_bits;
                if (op->image.type().is_float()) {
                    type_bits.first = "hcl.Float";
                } else if (op->image.type().is_int()) {
                    type_bits.first = "hcl.Int";
                } else if (op->image.type().is_uint()) {
                    type_bits.first = "hcl.UInt";
                } else {
                    internal_error << "Realize Type is not one of {float, int, uint}\n";
                }
                type_bits.second = op->image.type().bits();
                input_type[name] = type_bits;
            } else { // find it
                debug(0) << "already save this input type info\n";
            }
        }
        
        string call_name;
        if (op->name == "absd") {
            stream << "absd(";
            print_list(op->args);
            stream << ", ";
        } else if (op->name == "pow_f32") {
            call_name = "hcl.power";
            stream << call_name << "(";
            print_list(op->args);
            stream << ")";
        } else if (op->name == "exp_f32") {
            stream << "hcl.call_pure_intrin(\"float\", \"pow\", 2.718280, ";
            // stream << "hcl.power(__expo[0], ";
            print_list(op->args);
            stream << ")";
        } else if (op->call_type == Internal::Call::CallType::Halide) { // Load multi-dimensional. Maybe need to check if call_name = one of the input or output name?
            call_name = print_name(op->name);

            stream << call_name << "[";
            print_list_as_last_stage_order(op->args);
            stream << "]";
            // }
        } else if (op->call_type == Internal::Call::CallType::Image) {
            debug(1) << "if op->image.type().is_float()?: " << op->image.type().is_float() << "\n";
            call_name = print_name(op->name);
            // if (stage_type.first == "hcl.Float") {
            //     print_data_cast(); // equals to sth like: stream << "hcl.cast(dtype = hcl.Float(bits = 32), expr = "
            //     stream << call_name << "[";
            //     print_list_reverse(op->args);
            //     stream << "]";
            //     stream << ")";
            // } else {
            stream << call_name << "[";
            print_list_reverse(op->args);
            stream << "]";
            // }
        }
    }
    */










    /* Very Old Version: Load
    // debug(0) << "visit load...\n";

    
    // if (op->image.defined()) {
    //     debug(0) << "input dimensions: " << op->image.dimensions() << "\n";
    //     debug(0) << "input extent 0: " << op->image.extent(0) << "\n";
    //     // debug(0) << "input extent 1: " << op->image.extent(1) << "\n";
    //     // debug(0) << "input stride 0 : " << op->image.stride(0) << "\n";
    //     // debug(0) << "input stride 1: " << op->image.stride(1) << "\n";
    //     debug(0) << "input name: " << op->image.name() << "\n"; // input
    //     string name = print_name(op->image.name());
    //     auto input_iter = input_info.find(name);
    //     if (input_iter == input_info.end()) { // can't find it
    //         std::vector<int> input_extents;
    //         for(int i = 0; i < op->image.dimensions(); i++) {
    //             input_extents.push_back(op->image.extent(i));
    //         }
    //         input_info[name] = input_extents;
    //     } else { // find it
    //         debug(0) << "already save this input info\n";
    //     }
    // }

    // if (print_name(op->name) == output_name) { // Load the output from last stage
    //     stream << "out[0]";
    // } else {
    // stream << print_name(op->name)
    //        << "[";
    // op->index.accept(this);
    // stream << "]";
    // }
    // print_load_index(shape_dim);
    */











/*
string CodeGen_HeteroCL::print_scalarized_expr(Expr e) {
    Type t = e.type();
    internal_assert(t.is_vector());
    string v = unique_name('_');
    do_indent();
    stream << print_type(t, AppendSpace) << v << ";\n";
    for (int lane = 0; lane < t.lanes(); lane++) {
        Expr e2 = extract_lane(e, lane);
        string elem = print_expr(e2);
        ostringstream rhs;
        rhs << v << ".replace(" << lane << ", " << elem << ")";
        v = print_assignment(t, rhs.str());
    }
    return v;
}

string CodeGen_HeteroCL::print_extern_call(const Call *op) {
    if (op->type.is_vector()) {
        // Need to split into multiple scalar calls.
        return print_scalarized_expr(op);
    }
    ostringstream rhs;
    vector<string> args(op->args.size());
    for (size_t i = 0; i < op->args.size(); i++) {
        args[i] = print_expr(op->args[i]);
        // This substitution ensures const correctness for all calls
        if (args[i] == "__user_context") {
            args[i] = "_ucon";
        }
    }
    if (function_takes_user_context(op->name)) {
        args.insert(args.begin(), "_ucon");
    }
    rhs << op->name << "(" << with_commas(args) << ")";
    return rhs.str();
}
*/

void CodeGen_HeteroCL::print_load_index(int shape_dim) {
    switch (shape_dim)
    {
        case 1:
            stream << "[x]";
            break;
        case 2:
            stream << "[x, y]";
            break;
        case 3:
            stream << "[x, y, z]";
            break;
        case 4:
            stream << "[x, y, z, w]";
            break;
        default:
            break;
    }
}

void CodeGen_HeteroCL::visit(const Load *op) {
    debug(0) << "visit load...\n";
    

    // formal: dont use for now
    // if (op->image.defined()) {
    //     debug(0) << "input dimensions: " << op->image.dimensions() << "\n";
    //     debug(0) << "input extent 0: " << op->image.extent(0) << "\n";
    //     // debug(0) << "input extent 1: " << op->image.extent(1) << "\n";
    //     // debug(0) << "input stride 0 : " << op->image.stride(0) << "\n";
    //     // debug(0) << "input stride 1: " << op->image.stride(1) << "\n";
    //     debug(0) << "input name: " << op->image.name() << "\n"; // input
    //     string name = print_name(op->image.name());
    //     auto input_iter = input_info.find(name);
    //     if (input_iter == input_info.end()) { // can't find it
    //         std::vector<int> input_extents;
    //         for(int i = 0; i < op->image.dimensions(); i++) {
    //             input_extents.push_back(op->image.extent(i));
    //         }
    //         input_info[name] = input_extents;
    //     } else { // find it
    //         debug(0) << "already save this input info\n";
    //     }
    // }

    // if (print_name(op->name) == output_name) { // Load the output from last stage
    //     stream << "out[0]";
    // } else {
    // stream << print_name(op->name)
    //        << "[";
    // op->index.accept(this);
    // stream << "]";
    // }
    // // print_load_index(shape_dim);
}


void CodeGen_HeteroCL::visit(const Store *op) {
    op->value.accept(this);





    // formal：dont use it for now. be replaced by Provide
    /*

    // string name = print_name(op->name);
    // string id_index = print_expr(op->index);
    // string id_value = print_expr(op->value);
    debug(0) << "visit store...\n";
    // debug(0) << "store buffer dimensions: " << op->param.dimensions() << "\n";
    // debug(0) << "is output a buffer: " << op->param.is_buffer() << "\n";
    // debug(0) << "is output defined: " << op->param.defined() << "\n";
    // debug(0) << "is output buffer defined: " << op->param.buffer().defined() << "\n";
    // debug(0) << "output extent 0 constraint: " << op->param.extent_constraint(0) << "\n";
    // debug(0) << "output extent 0 constraint estimate: " << op->param.extent_constraint_estimate(0) << "\n";
    // // debug(0) << "output max value: " << op->param.max_value() << "\n";
    // debug(0) << "output raw buffer extent 0: " << op->param.raw_buffer()->dim[0].extent << "\n";
    // debug(0) << "Try the output extent 0: " << op->param.buffer().extent(0) << "\n";
    
    string init; // for now, we just set the init = output from last stage, if for_count != 0
    if (for_count == 0) {
        init = "0";
    } else {
        string param = print_param(output_shape.size(), for_count);
        init = output_name + "[" + param + "]";
    }

    do_indent();
    stream << "out = hcl.local(";
    // if ((op->value.node_type() == IRNodeType::Load) && (print_name(op->value.as<Load>()->name) == output_name)) { // lots of branch here. Just include some possibilities here
    //     init = "0";
    // } else if (op->value.node_type() == IRNodeType::) //other operations... Normally, the operation node -> a is the last stage's output. maybe we can leave a flag to the next node?

    stream << "init = " << init << ")\n";

    // claim reduction domain
    if (!reduction_info.empty()) {
        do_indent();
        reducer_name = "_sum"; // for now, it's given "_sum" directly
        stream << reducer_name << " = hcl.reducer(0, lambda x, y: x + y)\n"; // only use sum reduction for now, so just print it directly
        for (auto iter = reduction_info.begin(); iter != reduction_info.end(); ++iter) {
            do_indent();
            stream << iter->first << " = " << "hcl.reduce_axis(0, " << iter->second << ")\n"; // TODO: maybe also need the min information? maybe sometimes min != 0
        }
    }

    do_indent();
    stream << "out[0] = ";

    // want to find the scope of using reduction index



    op->value.accept(this);


    stream << "\n";
    do_indent();
    stream << "return out[0]\n";
    indent -= 4;
    
    // ignore the output name and index, because this is a specific example with hcl.compute
    */
}


void CodeGen_HeteroCL::visit(const Let *op) {   
    do_indent();
    stream << print_name(op->name) << " = ";
    debug(0) << "Let name: " << op->name << "\n";
    op->value.accept(this);
    stream << "\n";
    
    
    if (op->body.node_type() == IRNodeType::Let) {
        debug(0) << "visit Let: branch 1\n";
        op->body.accept(this);
    } else {
        debug(0) << "visit Let: branch 2\n";
        do_indent();
        stream << "return ";
        op->body.accept(this);
    }
}


void CodeGen_HeteroCL::visit(const Select *op) {
    stream << "hcl.select(";
    op->condition.accept(this);
    stream << ", ";
    op->true_value.accept(this);
    stream << ", ";
    op->false_value.accept(this);
    stream << ")";
}

void CodeGen_HeteroCL::visit(const LetStmt *op) {
    // to clean the code, just ignore it...
    
    if(letstmt_validate) {
        string var_name = print_name(op->name);
        inter_var[var_name] = op->value;

        // auto iter = inter_var.find(var_name);
        // if (iter == inter_var.end()) { // can't find it
        //     inter_var[var_name] = op->value;
        // }


        // stream << "let " << op->name << " = ";
        // op->value.accept(this);
        // stream << "\n";
    }



    // do_indent();
    // stream << print_name(op->name) << " = ";
    // op->value.accept(this);
    // stream << "\n";
    op->body.accept(this);
}

/*
// Halide asserts have different semantics to C asserts.  They're
// supposed to clean up and make the containing function return
// -1, so we can't use the C version of assert. Instead we convert
// to an if statement.
void CodeGen_HeteroCL::create_assertion(const string &id_cond, const string &id_msg) {
    if (target.has_feature(Target::NoAsserts)) return;

    do_indent();
    stream << "if (!" << id_cond << ")\n";
    open_scope();
    do_indent();
    stream << "return " << id_msg << ";\n";
    close_scope("");
}

void CodeGen_HeteroCL::create_assertion(const string &id_cond, Expr message) {
    internal_assert(!message.defined() || message.type() == Int(32))
        << "Assertion result is not an int: " << message;

    if (target.has_feature(Target::NoAsserts)) return;

    // don't call the create_assertion(string, string) version because
    // we don't want to force evaluation of 'message' unless the condition fails
    do_indent();
    stream << "if (!" << id_cond << ") ";
    open_scope();
    string id_msg = print_expr(message);
    do_indent();
    stream << "return " << id_msg << ";\n";
    close_scope("");
}

void CodeGen_HeteroCL::create_assertion(Expr cond, Expr message) {
    create_assertion(print_expr(cond), message);
}
*/

void CodeGen_HeteroCL::visit(const AssertStmt *op) {
    // simply ignore it...
}


void CodeGen_HeteroCL::print_input_name() {
    int input_num = input_name.size();
    if (input_num == 0) {
        stream << "():\n";
        indent += 4;
    } else if (input_num == 1) {
        stream << "(" << input_name.back() << "):\n";
        indent += 4;
    } else {
        int i = 0;
        for(auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
            if (i == 0) {
                stream << "(";
            }
            stream << *iter << ", ";
            if (i == input_num - 1) {
                stream << "):\n";
                indent += 4;
            }
            i++;
        }
    }
}

void CodeGen_HeteroCL::print_placeholder() {
    int input_dim; // define it for now. to be general, have to load this information from buffer. Don't know how. 
    int dim_count; // count the dim loop, used to know to print '*' or not

    // for(auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
    //     if (input_dim == 1) {
    //             dim_name = "_" + *iter + "_extent_0";
    //             stream << *iter << " = " << "hcl.placeholder((" << dim_name << ", )," << "name = \"" << *iter << "\")\n"; 
    //     } else {
    //         for (int dim = 0; dim < input_dim; dim++) {
    //             if (dim == 0) {
    //                 dim_name = "_" + *iter + "_extent_" + std::to_string(dim);
    //                 stream << *iter << " = " << "hcl.placeholder((" << dim_name;
    //             } else if (dim == input_dim - 1) {
    //                 dim_name = "_" + *iter + "_extent_" + std::to_string(dim);
    //                 stream << " * " << dim_name << ", ), " << "name = \"" << *iter << "\")\n"; 
    //             } else {
    //                 dim_name = "_" + *iter + "_extent_" + std::to_string(dim);
    //                 stream << " * " << dim_name;
    //             }
    //         }
    //     }
    // }
    for (auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
        auto iter_info = input_info.find(*iter);
        do_indent();
        input_dim = iter_info->second.size();
        dim_count = 0;

        debug(0) << "input dim: " << input_dim << "\n";
        debug(0) << "Wrong here?!" << "\n";

        stream << iter_info->first << " = " << "hcl.placeholder((";
        for (auto iter_ext = iter_info->second.begin(); iter_ext != iter_info->second.end(); ++iter_ext) {
            if (dim_count == input_dim - 1) {
                stream << *iter_ext;
                dim_count ++;
            } else {
                stream << *iter_ext << ", ";
                dim_count ++;
            }
        }
        stream << ", ), name = \"" << iter_info->first << "\", ";
        stream << "dtype = " << input_type[iter_info->first].first << "(bits = " << std::to_string(input_type[iter_info->first].second) << ")";
        stream << ")\n";
    }
}



void CodeGen_HeteroCL::visit(const ProducerConsumer *op) {
    letstmt_validate = 1;



    if (op->is_producer) {

        // To deal with "Final stage", one solution is: we collect information in "Realize", and print hcl.compute(lambda: 0) in Producer
        if (op->name == function_name) {
            do_indent();
            stream << op->name << " = " << "hcl.compute(";
            stream << "(";
            for (size_t i = 0; i < output_shape.size(); i++) {
                stream << output_shape[i];
                if (i < output_shape.size() - 1) {
                    stream << ", ";
                }
            }
            stream << "), lambda: ";
            print_index(output_shape.size());
            stream << ": 0, name = \"" << op->name << "\", dtype = " << stage_type.first << "(bits = " << std::to_string(stage_type.second) << "))\n";
        }
        //


        do_indent();
        stream << "with hcl.Stage(\"" << op->name << "\"):\n";
        indent += 4;
        op->body.accept(this);
        indent -= 4;
    } else {
        op->body.accept(this);
    }
}




    /* Version 1
    if (op->is_producer) {
        debug(0) << "visit produce...\n";
        stage_num = 0; // maybe this equation have to be moved under if ?...
        if_consume = 0;
        original_output_name = op->name;
        output_name = print_name(op->name);

        op->body.accept(this);

        produce_stage_num[output_name] = stage_num - 1;
        consume_name_group.clear(); // clear vector consume_name_group in the end of Producer visitor 
    } else {
        if_consume = 1;
        string consume_name = print_name(op->name);
        consume_name_group.push_back(consume_name);
        op->body.accept(this);
    }
    */



    /* Very Old Version
    // output_name = print_name(op->name);
    // for_count = 0;
    // do_indent();
    // if (op->is_producer) {
    //     // stream << "// produce " << op->name << '\n';
    //     stream << "def " << output_name;
    //     print_input_name();
    //     // stream << output_name << " = hcl.compute(";
    // } else {
    //     stream << "// consume " << op->name << '\n';
    // }
    // print_stmt(op->body);
    
    // // stream << ")" << '\n';
    // do_indent();
    // stream << "return " << output_name << '\n';
    // indent -= 4;
    // stream << "\n";
    // print_placeholder();
    */










/*
void CodeGen_HeteroCL::visit(const Fork *op) {
    // TODO: This doesn't actually work with nested tasks
    do_indent();
    stream << "#pragma omp parallel\n";
    open_scope();
    do_indent();
    stream << "#pragma omp single\n";
    open_scope();
    do_indent();
    stream << "#pragma omp task\n";
    open_scope();
    print_stmt(op->first);
    close_scope("");
    do_indent();
    stream << "#pragma omp task\n";
    open_scope();
    print_stmt(op->rest);
    close_scope("");
    do_indent();
    stream << "#pragma omp taskwait\n";
    close_scope("");
    close_scope("");
}

void CodeGen_HeteroCL::visit(const Acquire *op) {
    string id_sem = print_expr(op->semaphore);
    string id_count = print_expr(op->count);
    open_scope();
    do_indent();
    stream << "while (!halide_semaphore_try_acquire(" << id_sem << ", " << id_count << "))\n";
    open_scope();
    do_indent();
    stream << "#pragma omp taskyield\n";
    close_scope("");
    op->body.accept(this);
    close_scope("");
}
*/




string CodeGen_HeteroCL::print_param(int dim, int stage) {
    string param;
    string x = output_name + "_s" + std::to_string(stage) + "_x";
    string y = output_name + "_s" + std::to_string(stage) + "_y";
    string z = output_name + "_s" + std::to_string(stage) + "_z";  
    string n = output_name + "_s" + std::to_string(stage) + "_n"; 
    switch (dim)
    {
    case 1:
        param = x;
        break;
    case 2:
        param = x + ", " + y;
        break;
    case 3:  
        param = x + ", " + y + ", " + z;
        break;
    case 4:
        param = x + ", " + y + ", " + z + ", " + n;
        break;
    default:
        break;
    }
    return param;
}

// void CodeGen_HeteroCL::print_stage_def(string stage_name, int dim, int for_count) { // formal
//     /** if for_count = 0, parameter don't need the output of last stage. Otherwise, need it. */
//     stream << stage_name << "(";
//     string param = print_param(dim, for_count);
//     stream << param << ", ";
//     for(auto iter = input_name.begin(); iter != input_name.end(); ++iter) {
//         stream << *iter << ", ";
//     }

//     if (for_count != 0) {
//         stream << output_name << ", ";
//     }
    
//     stream <<")";
// }

void CodeGen_HeteroCL::print_param_name() {
    for (auto iter = for_name.begin(); iter != for_name.end(); ++iter) {
        stream << print_name(*iter) << ", ";
    }
    // for (auto iter = for_name.begin(); iter != for_name.end(); ++iter) {
    //     stream << print_name(*iter) << ", ";
    // }
}

void CodeGen_HeteroCL::print_stage_shape() {
    stream << "(";
    for (size_t i = 0; i < for_extent.size(); i++) {
        debug(0) << "i index = " << i << "\n";
        for_extent[i].accept(this);
        stream << ", ";
    }
    stream << ")";
}

void CodeGen_HeteroCL::print_stage_def() {
    stream << stage_name << "(";
    print_param_name();
    for (auto iter = input_name.begin(); iter != input_name.end(); ++iter) { // print input name
        stream << print_name(*iter) << ", ";
    }

    
    // if (if_consume) { // don't use if_consume to judge for now, because if there's 2 For loop in a Producer, the second need to include the first stage_name(actualy in def statement, it need to be output_name of this Producer)
    
    // if (last_output_name != "") {
    //     stream << last_output_name << ", ";
    // }

    if (stage_num != 0) { // if this is stage 1 or more in a Producer loop, need to include the last stage's name
        stream << last_output_name << ", ";
    }

    for (auto iter = consume_name_group.begin(); iter != consume_name_group.end(); ++iter) {
        stream << *iter << ", "; // already go through print_name function when push_back in the vector
    }

    stream << ")";
}

void CodeGen_HeteroCL::print_using_stage_def_when_compute() {
    stream << stage_name << "(";
    print_param_name();
    for(auto iter = input_name.begin(); iter != input_name.end(); ++iter) { // print input name
        stream << print_name(*iter) << ", ";
    }

    // if (last_stage_name != "") {
    //     stream << last_stage_name << ", ";
    // }

    if (stage_num != 0) {
        stream << last_stage_name << ", ";
    }
    for (auto iter = consume_name_group.begin(); iter != consume_name_group.end(); ++iter) {
        string consume_name_add = *iter + "_" + std::to_string(produce_stage_num[*iter]);
        stream << consume_name_add << ", ";
    }

    stream << ")";
}

void CodeGen_HeteroCL::visit(const For *op) {


    debug(0) << "visit for...\n";

    do_indent();
    stream << "with hcl.for_" << "(";
    op->min.accept(this); 
    stream << ", ";
    op->extent.accept(this);
    stream << ", name = " << "\"" << print_name(op->name) << "\") as " << print_name(op->name) << ":\n";
    indent += 4;

    op->body.accept(this);
    
    indent -= 4;
}
 
 
    /* Version 1 - For

    std::size_t found = op->name.find("$"); //"$" seems to be the character of reduction domain loop's name
    if (found != std::string::npos) {
        debug(0) << "This is a reduction loop...\n";
        debug(0) << "reduction loop name: " << print_name(op->name) << "\n";
        debug(0) << "reduction extent: " << op->extent.as<IntImm>()->value << "\n";
        reduction_info[print_name(op->name)] = op->extent.as<IntImm>()->value; // need to be cleared
    } else {
        debug(0) << "This is a normal loop...\n";          
        for_extent.push_back(op->extent); // extent2, extent1, extent0 ...
        for_name.push_back(op->name); // c, y, x
        // extract axis order from op->min, because op->min seem always to be a Variable...
        debug(0) << "extracting axis order from op->min...\n";
        int axis;
        if (op->min.node_type() == IRNodeType::Variable) {
            axis = (int)op->min.as<Variable>()->name.back() - (int)'0';
        } else if (op->min.node_type() == IRNodeType::IntImm) { // not right to count like this
            axis = op->min.as<IntImm>()->value;
        } else {
            internal_error << "op->min is neither Variable nor IntImm...\n";
        }
        func_axis_call_order.push_back(axis); 

        if (op->for_type == ForType::Unrolled) {
            // order in env's key(output name, namely intermediate func name) is the same as Halide order: from inner to outer. Without reorder, it's like x, y, __outermost
            // So we can simply use this order (stage_axis) to collect unroll factor
            if_stage_unroll = 1;
            int unroll_factor;
            vector<Dim> dim_group = env[original_output_name].definition().schedule().dims();
            int dim_size = dim_group.size();
            unroll_factor = dim_group[dim_size - 2 - stage_axis].unroll_factor;
            axis_and_factor.push_back(std::make_pair(stage_axis, unroll_factor));

            // unroll_axis.push_back(stage_axis);
            

            // vector<Dim> dim_group = env[original_output_name].definition().schedule().dims();
            // unroll_factor_group.push_back(dim_group[stage_axis].unroll_factor);

        } else if (op->for_type == ForType::Parallel) {
            if_stage_parallel = 1;
            parallel_axis.push_back(stage_axis);
        }

        stage_axis ++;
    }



    
    if (op->body.node_type() == IRNodeType::For || op->body.node_type() == IRNodeType::LetStmt) { // if there's letstmt inserted into for loop, let it go to make the perfection for loop
        // shape.push_back(id_extent);
        // output_size += id_extent + " * ";
        debug(0) << "for branch 1...\n";
        op->body.accept(this);
    } else {
        debug(0) << "for branch 2...\n";
        last_stage_name = stage_name;

        stage_name = output_name + "_" + std::to_string(stage_num);
        stage_name_group.push_back(stage_name);

        // store in unroll_info map
        if (if_stage_unroll) {
            unroll_info[stage_name] = axis_and_factor;
        }
        if (if_stage_parallel) {
            parallel_info[stage_name] = parallel_axis;
        }

        do_indent();
        stream << "def ";
        print_stage_def();
        stream << ":\n";
        indent += 4;

        op->body.accept(this);

        indent -= 4;
        do_indent();
        stream << stage_name << " = hcl.compute(";
        print_stage_shape();
        stream << ", lambda ";
        print_param_name();
        stream << ": ";
        print_using_stage_def_when_compute();
        stream << ", name = \"" << stage_name << "\", " << "dtype = " << stage_type.first << "(bits = " << std::to_string(stage_type.second) << "))";
        stream << "\n\n";


        last_func_axis_call_order = func_axis_call_order;
        stage_dim_group.push_back(for_extent.size());
        for_extent.clear();
        for_name.clear();
        func_axis_call_order.clear();
        axis_and_factor.clear();
        parallel_axis.clear();

        last_output_name = output_name;
        stage_num ++;
        stage_axis = 0;
        if_stage_unroll = 0;
        if_stage_parallel = 0;
    }
    */








    /* Very Old Version (probably not storage_flatten version? So I still remain it here)
    string stage_name;

    string id_min;
    string id_extent;

    std::size_t found = op->name.find("r$"); //"r$" seems to be the character of reduction domain loop's name
    if (found != std::string::npos) {
        debug(0) << "This is a reduction loop...\n";
        debug(0) << "reduction loop name: " << print_name(op->name) << "\n";
        debug(0) << "reduction extent: " << op->extent.as<IntImm>()->value << "\n";
        reduction_info[print_name(op->name)] = op->extent.as<IntImm>()->value; // need to be cleared
    } else {
        debug(0) << "This is a normal loop...\n";
    }

    // id_min and id_extent sometimes are Expr instead of IntImm or Variable
    // if (op->min.node_type() == IRNodeType::IntImm) {
    //     id_min = std::to_string(op->min.as<IntImm>()->value);// valuetype is int64_t
    // } else {
    //     id_min = print_name(op->min.as<Variable>()->name);
    // }

    // if (op->extent.node_type() == IRNodeType::IntImm) {
    //     id_extent = std::to_string(op->extent.as<IntImm>()->value);// valuetype is int64_t
    // } else {
    //     id_extent = print_name(op->extent.as<Variable>()->name);
    // }

    

    if (op->body.node_type() == IRNodeType::For) {
        // shape.push_back(id_extent);
        // output_size += id_extent + " * ";
        debug(0) << "for branch 1...\n";
        op->body.accept(this);
    } else {
        debug(0) << "for branch 2...\n";
        do_indent();
        stage_name = output_name + "_" + std::to_string(for_count);
        stream << "def ";
        print_stage_def(stage_name, output_shape.size(), for_count);
        stream << ":\n";
        indent += 4;

        op->body.accept(this); // then come the visitor Store. Only support For loop with a next store node for now.

        do_indent();
        stream << output_name << " = " << "hcl.compute((";
        for (auto iter = output_shape.begin(); iter != output_shape.end(); ++iter) {
            stream << *iter << ", ";
        }
        stream << "), ";
        // shape.push_back(id_extent);
        // output_size += id_extent;
        // shape_dim = shape.size(); // 2
        // if (shape_dim == 1) {
        //     stream << "("
        //            << shape.back()
        //            << ", ), ";
        //     shape.pop_back();
        // } else {
        //     for (int i = 0; i < shape_dim; i++) {
        //         if (i == 0) {
        //             stream << "("
        //                 << shape.back()
        //                 << ", ";
        //             shape.pop_back();
        //         } else if (i == shape_dim - 1) {
        //             stream << shape.back()
        //                 << "), ";
        //             shape.pop_back();
        //         } else {
        //             stream << shape.back()
        //                 << ", ";
        //             shape.pop_back();
        //         }
        //     }
        // }

        string param = print_param(output_shape.size(), for_count);
        stream << "lambda " << param << ": ";
        print_stage_def(stage_name, output_shape.size(), for_count);
        stream << ", name = \"" << stage_name << "\")\n\n"; // \n two times between stages would probably make code generated more beautiful...

        for_count += 1;
    }
    */














/*
void CodeGen_HeteroCL::visit(const Ramp *op) {
    Type vector_type = op->type.with_lanes(op->lanes);
    string id_base = print_expr(op->base);
    string id_stride = print_expr(op->stride);
    print_assignment(vector_type, print_type(vector_type) + "::ramp(" + id_base + ", " + id_stride + ")");
}

void CodeGen_HeteroCL::visit(const Broadcast *op) {
    Type vector_type = op->type.with_lanes(op->lanes);
    string id_value = print_expr(op->value);
    string rhs;
    if (op->lanes > 1) {
        rhs = print_type(vector_type) + "::broadcast(" + id_value + ")";
    } else {
        rhs = id_value;
    }

    print_assignment(vector_type, rhs);
}
*/

void CodeGen_HeteroCL::visit(const Provide *op) {
    debug(0) << "visit Provide...\n";

    do_indent();
    stream << op->name << "[";
    print_list(op->args);
    stream << "] = ";
    // if (op->values.size() > 1) {
    //     stream << "{";
    // }

    print_list(op->values);

    // if (op->values.size() > 1) {
    //     stream << "}";
    // }

    stream << '\n';

}
    /* Version 1 - Provide
    if (!reduction_info.empty()) {
        do_indent();
        reducer_name = "_sum"; // for now, it's given "_sum" directly
        stream << reducer_name << " = hcl.reducer(0, lambda x, y: x + y)\n"; // only use sum reduction for now, so just print it directly
        for (auto iter = reduction_info.begin(); iter != reduction_info.end(); ++iter) {
            do_indent();
            stream << iter->first << " = " << "hcl.reduce_axis(0, " << iter->second << ")\n"; // TODO: maybe also need the min information? maybe sometimes min != 0
        }
    }

    internal_assert(op->values.size() == 1); // assume op->values only have one Expr
    if (op->values[0].node_type() != IRNodeType::Let) {
        do_indent();
        // stream << "out[0] = ";
        stream << "return ";
    }

    print_list(op->values);

    stream << "\n";
    */







/*
void CodeGen_HeteroCL::visit(const Allocate *op) {
    open_scope();

    string op_name = print_name(op->name);
    string op_type = print_type(op->type, AppendSpace);

    // For sizes less than 8k, do a stack allocation
    bool on_stack = false;
    int32_t constant_size;
    string size_id;
    Type size_id_type;

    if (op->new_expr.defined()) {
        Allocation alloc;
        alloc.type = op->type;
        allocations.push(op->name, alloc);
        heap_allocations.push(op->name);
        stream << op_type << "*" << op_name << " = (" << print_expr(op->new_expr) << ");\n";
    } else {
        constant_size = op->constant_allocation_size();
        if (constant_size > 0) {
            int64_t stack_bytes = constant_size * op->type.bytes();

            if (stack_bytes > ((int64_t(1) << 31) - 1)) {
                user_error << "Total size for allocation "
                           << op->name << " is constant but exceeds 2^31 - 1.\n";
            } else {
                size_id_type = Int(32);
                size_id = print_expr(make_const(size_id_type, constant_size));

                if (op->memory_type == MemoryType::Stack ||
                    (op->memory_type == MemoryType::Auto &&
                     can_allocation_fit_on_stack(stack_bytes))) {
                    on_stack = true;
                }
            }
        } else {
            // Check that the allocation is not scalar (if it were scalar
            // it would have constant size).
            internal_assert(op->extents.size() > 0);

            size_id = print_assignment(Int(64), print_expr(op->extents[0]));
            size_id_type = Int(64);

            for (size_t i = 1; i < op->extents.size(); i++) {
                // Make the code a little less cluttered for two-dimensional case
                string new_size_id_rhs;
                string next_extent = print_expr(op->extents[i]);
                if (i > 1) {
                    new_size_id_rhs =  "(" + size_id + " > ((int64_t(1) << 31) - 1)) ? " + size_id + " : (" + size_id + " * " + next_extent + ")";
                } else {
                    new_size_id_rhs = size_id + " * " + next_extent;
                }
                size_id = print_assignment(Int(64), new_size_id_rhs);
            }
            do_indent();
            stream << "if ((" << size_id << " > ((int64_t(1) << 31) - 1)) || ((" << size_id <<
              " * sizeof(" << op_type << ")) > ((int64_t(1) << 31) - 1)))\n";
            open_scope();
            do_indent();
            // TODO: call halide_error_buffer_allocation_too_large() here instead
            // TODO: call create_assertion() so that NoAssertions works
            stream << "halide_error(_ucon, "
                   << "\"32-bit signed overflow computing size of allocation " << op->name << "\\n\");\n";
            do_indent();
            stream << "return -1;\n";
            close_scope("overflow test " + op->name);
        }

        // Check the condition to see if this allocation should actually be created.
        // If the allocation is on the stack, the only condition we can respect is
        // unconditional false (otherwise a non-constant-sized array declaration
        // will be generated).
        if (!on_stack || is_zero(op->condition)) {
            Expr conditional_size = Select::make(op->condition,
                                                 Variable::make(size_id_type, size_id),
                                                 make_const(size_id_type, 0));
            conditional_size = simplify(conditional_size);
            size_id = print_assignment(Int(64), print_expr(conditional_size));
        }

        Allocation alloc;
        alloc.type = op->type;
        allocations.push(op->name, alloc);

        do_indent();
        stream << op_type;

        if (on_stack) {
            stream << op_name
                   << "[" << size_id << "];\n";
        } else {
            stream << "*"
                   << op_name
                   << " = ("
                   << op_type
                   << " *)halide_malloc(_ucon, sizeof("
                   << op_type
                   << ")*" << size_id << ");\n";
            heap_allocations.push(op->name);
        }
    }

    if (!on_stack) {
        create_assertion(op_name, "halide_error_out_of_memory(_ucon)");

        do_indent();
        string free_function = op->free_function.empty() ? "halide_free" : op->free_function;
        stream << "HalideFreeHelper " << op_name << "_free(_ucon, "
               << op_name << ", " << free_function << ");\n";
    }

    op->body.accept(this);

    // Should have been freed internally
    internal_assert(!allocations.contains(op->name));

    close_scope("alloc " + print_name(op->name));
}

void CodeGen_HeteroCL::visit(const Free *op) {
    if (heap_allocations.contains(op->name)) {
        do_indent();
        stream << print_name(op->name) << "_free.free();\n";
        heap_allocations.pop(op->name);
    }
    allocations.pop(op->name);
}
*/

void CodeGen_HeteroCL::print_index(int number) {
    switch (number)
    {
        case 1: 
            stream << "x";
            break;
        case 2:
            stream << "x, y";
            break;
        case 3:
            stream << "x, y, z";
            break;
        case 4: 
            stream << "x, y, z, w";
            break; 
        default: 
            internal_error << "number is not in 1-4\n";
    }
}

void CodeGen_HeteroCL::visit(const Realize *op) {
    // check Realize type, record in stage_type
    internal_assert(op->types.size() == 1); // seems always to be 1 for now
    stage_type.second = op->types[0].bits();
    if (op->types[0].is_float()) {
        stage_type.first = "hcl.Float";
    } else if (op->types[0].is_int()) {
        stage_type.first = "hcl.Int";
    } else if (op->types[0].is_uint()) {
        stage_type.first = "hcl.UInt";
    } else {
        internal_error << "Realize Type is not one of {float, int, uint}\n";
    }

    do_indent();
    stream << op->name << " = " << "hcl.compute(";
    stream << "(";
    for (size_t i = 0; i < op->bounds.size(); i++) {
        op->bounds[i].extent.accept(this);
        if (i < op->bounds.size() - 1) {
            stream << ", ";
        }
    }
    stream << "), lambda: ";
    print_index(op->bounds.size());
    stream << ": 0, name = \"" << op->name << "\", dtype = " << stage_type.first << "(bits = " << std::to_string(stage_type.second) << "))\n";

    op->body.accept(this); 


    /** Version 1
    debug(0) << "visit Realize...\n";
    debug(0) << "realize type vector size: " << op->types.size() << "\n";
    debug(0) << "is_float: " << op->types[0].is_float() << ", " << "is_int: " << op->types[0].is_int() << ", " << "is_uint: " << op->types[0].is_uint() << "\n";
    debug(0) << "bits: " << op->types[0].bits() << "\n";

    internal_assert(op->types.size() == 1); // seems always to be 1 for now
    stage_type.second = op->types[0].bits();
    if (op->types[0].is_float()) {
        stage_type.first = "hcl.Float";
    } else if (op->types[0].is_int()) {
        stage_type.first = "hcl.Int";
    } else if (op->types[0].is_uint()) {
        stage_type.first = "hcl.UInt";
    } else {
        internal_error << "Realize Type is not one of {float, int, uint}\n";
    }

    op->body.accept(this);
    // do nothing
    **/
}

/*
void CodeGen_HeteroCL::visit(const Prefetch *op) {
    internal_error << "Cannot emit prefetch statements to C\n";
}
*/

void CodeGen_HeteroCL::visit(const IfThenElse *op) {
    // don't show the condition, only show the then_case ...
    
    op->then_case.accept(this);
}


void CodeGen_HeteroCL::visit(const Evaluate *op) {
    op->value.accept(this);
    //do nothing
}

/*
void CodeGen_HeteroCL::visit(const Shuffle *op) {
    internal_assert(op->vectors.size() >= 1);
    internal_assert(op->vectors[0].type().is_vector());
    for (size_t i = 1; i < op->vectors.size(); i++) {
        internal_assert(op->vectors[0].type() == op->vectors[i].type());
    }
    internal_assert(op->type.lanes() == (int) op->indices.size());
    const int max_index = (int) (op->vectors[0].type().lanes() * op->vectors.size());
    for (int i : op->indices) {
        internal_assert(i >= -1 && i < max_index);
    }

    std::vector<string> vecs;
    for (Expr v : op->vectors) {
        vecs.push_back(print_expr(v));
    }
    string src = vecs[0];
    if (op->vectors.size() > 1) {
        ostringstream rhs;
        string storage_name = unique_name('_');
        do_indent();
        stream << "const " << print_type(op->vectors[0].type()) << " " << storage_name << "[] = { " << with_commas(vecs) << " };\n";

        rhs << print_type(op->type) << "::concat(" << op->vectors.size() << ", " << storage_name << ")";
        src = print_assignment(op->type, rhs.str());
    }
    ostringstream rhs;
    if (op->type.is_scalar()) {
        rhs << src << "[" << op->indices[0] << "]";
    } else {
        string indices_name = unique_name('_');
        do_indent();
        stream << "const int32_t " << indices_name << "[" << op->indices.size() << "] = { " << with_commas(op->indices) << " };\n";
        rhs << print_type(op->type) << "::shuffle(" << src << ", " << indices_name << ")";
    }
    print_assignment(op->type, rhs.str());
}
*/

// void CodeGen_C::test() {
//     LoweredArgument buffer_arg("buf", Argument::OutputBuffer, Int(32), 3, ArgumentEstimates{});
//     LoweredArgument float_arg("alpha", Argument::InputScalar, Float(32), 0, ArgumentEstimates{});
//     LoweredArgument int_arg("beta", Argument::InputScalar, Int(32), 0, ArgumentEstimates{});
//     LoweredArgument user_context_arg("__user_context", Argument::InputScalar, type_of<const void*>(), 0, ArgumentEstimates{});
//     vector<LoweredArgument> args = { buffer_arg, float_arg, int_arg, user_context_arg };
//     Var x("x");
//     Param<float> alpha("alpha");
//     Param<int> beta("beta");
//     Expr e = Select::make(alpha > 4.0f, print_when(x < 1, 3), 2);
//     Stmt s = Store::make("buf", e, x, Parameter(), const_true(), ModulusRemainder());
//     s = LetStmt::make("x", beta+1, s);
//     s = Block::make(s, Free::make("tmp.stack"));
//     s = Allocate::make("tmp.stack", Int(32), MemoryType::Stack, {127}, const_true(), s);
//     s = Block::make(s, Free::make("tmp.heap"));
//     s = Allocate::make("tmp.heap", Int(32), MemoryType::Heap, {43, beta}, const_true(), s);
//     Expr buf = Variable::make(Handle(), "buf.buffer");
//     s = LetStmt::make("buf", Call::make(Handle(), Call::buffer_get_host, {buf}, Call::Extern), s);

//     Module m("", get_host_target());
//     m.append(LoweredFunc("test1", args, s, LinkageType::External));

//     ostringstream source;
//     {
//         CodeGen_C cg(source, Target("host"), CodeGen_C::CImplementation);
//         cg.compile(m);
//     }

//     string src = source.str();
//     string correct_source =
//         headers +
//         globals +
//         string((const char *)halide_internal_runtime_header_HalideRuntime_h) + '\n' +
//         string((const char *)halide_internal_initmod_inlined_c) + R"GOLDEN_CODE(
// #ifndef HALIDE_FUNCTION_ATTRS
// #define HALIDE_FUNCTION_ATTRS
// #endif



// #ifdef __cplusplus
// extern "C" {
// #endif

// int test1(struct halide_buffer_t *_buf_buffer, float _alpha, int32_t _beta, void const *__user_context) HALIDE_FUNCTION_ATTRS {
//  void * const _ucon = const_cast<void *>(__user_context);
//  void *_0 = _halide_buffer_get_host(_buf_buffer);
//  void * _buf = _0;
//  {
//   int64_t _1 = 43;
//   int64_t _2 = _1 * _beta;
//   if ((_2 > ((int64_t(1) << 31) - 1)) || ((_2 * sizeof(int32_t )) > ((int64_t(1) << 31) - 1)))
//   {
//    halide_error(_ucon, "32-bit signed overflow computing size of allocation tmp.heap\n");
//    return -1;
//   } // overflow test tmp.heap
//   int64_t _3 = _2;
//   int32_t *_tmp_heap = (int32_t  *)halide_malloc(_ucon, sizeof(int32_t )*_3);
//   if (!_tmp_heap)
//   {
//    return halide_error_out_of_memory(_ucon);
//   }
//   HalideFreeHelper _tmp_heap_free(_ucon, _tmp_heap, halide_free);
//   {
//    int32_t _tmp_stack[127];
//    int32_t _4 = _beta + 1;
//    int32_t _5;
//    bool _6 = _4 < 1;
//    if (_6)
//    {
//     char b0[1024];
//     snprintf(b0, 1024, "%lld%s", (long long)(3), "\n");
//     char const *_7 = b0;
//     halide_print(_ucon, _7);
//     int32_t _8 = 0;
//     int32_t _9 = return_second(_8, 3);
//     _5 = _9;
//    } // if _6
//    else
//    {
//     _5 = 3;
//    } // if _6 else
//    int32_t _10 = _5;
//    float _11 = float_from_bits(1082130432 /* 4 */);
//    bool _12 = _alpha > _11;
//    int32_t _13 = (int32_t)(_12 ? _10 : 2);
//    ((int32_t *)_buf)[_4] = _13;
//   } // alloc _tmp_stack
//   _tmp_heap_free.free();
//  } // alloc _tmp_heap
//  return 0;
// }

// #ifdef __cplusplus
// }  // extern "C"
// #endif

// )GOLDEN_CODE";

//     if (src != correct_source) {
//         int diff = 0;
//         while (src[diff] == correct_source[diff]) diff++;
//         int diff_end = diff + 1;
//         while (diff > 0 && src[diff] != '\n') diff--;
//         while (diff_end < (int)src.size() && src[diff_end] != '\n') diff_end++;

//         internal_error
//             << "Correct source code:\n" << correct_source
//             << "Actual source code:\n" << src
//             << "Difference starts at:\n"
//             << "Correct: " << correct_source.substr(diff, diff_end - diff) << "\n"
//             << "Actual: " << src.substr(diff, diff_end - diff) << "\n";
//     }

//     std::cout << "CodeGen_C test passed\n";
// }


}  // namespace Internal
}  // namespace Halide

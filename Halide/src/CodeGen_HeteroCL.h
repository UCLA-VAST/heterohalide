#ifndef HALIDE_CODEGEN_HETEROCL_H
#define HALIDE_CODEGEN_HETEROCL_H

/** \file
 *
 * Defines an IRPrinter that emits HeteroCL code equivalent to a halide stmt
 */

#include "IRVisitor.h"
#include "IRPrinter.h"
#include "Module.h"
#include "Scope.h"

namespace Halide {

struct Argument;

namespace Internal {

/** This class emits HeteroCL code equivalent to a halide Stmt. It's
 * mostly the same as an IRPrinter, but it's wrapped in a function
 * definition, and some things are handled differently to be valid
 * C++.
 */
class CodeGen_HeteroCL : public IRPrinter {
public:
    enum OutputKind {
        CHeader,
        CPlusPlusHeader,
        CImplementation,
        CPlusPlusImplementation,
    };

    /** Initialize a HeteroCL code generator pointing at a particular output
     * stream (e.g. a file, or std::cout) */
    CodeGen_HeteroCL(std::ostream &dest,
              const std::vector<Function> &output_funcs,
              const std::vector<int> &output_shape, 
              Target target,
              OutputKind output_kind = CImplementation,
              const std::string &include_guard = "");
    ~CodeGen_HeteroCL() override;

    /** Emit the declarations contained in the module as HeteroCL code. */
    void compile(const Module &module);

    /** The target we're generating code for */
    const Target &get_target() const { return target; }

    static void test();

protected:
    enum AppendSpaceIfNeeded {
        DoNotAppendSpace,
        AppendSpace,
    };
    
    /** Emit a declaration. */
    // @{
    virtual void compile(const LoweredFunc &func);
    // virtual void compile(const Buffer<> &buffer);
    // @}

    /** env stores all the intermediate func, and related info. Use populate_environment to get it */
    std::map<std::string, Function> env;

    /** An ID for the most recently generated ssa variable */
    std::string id;

    /** from Pipeline::contents->outputs. It stores lots of informatioin, including scheduling.  */
    std::vector<Function> output_funcs;

    /** output_shape, required in Produce visitor: hcl.compute */
    std::vector<int> output_shape;

    /** The target being generated for. */
    Target target;

    /** Controls whether this instance is generating declarations or
     * definitions and whether the interface us extern "C" or C++. */
    OutputKind output_kind;



    // /** A cache of generated values in scope */
    // std::map<std::string, std::string> cache;

    /** Use output_funcs to print HeteroCL scheduling code */
    void schedule();

    /** Emit an expression as an assignment, then return the id of the
     * resulting var */
    std::string print_expr(Expr);

    /** int is the output shape of compute function, return the function index parameter, used in stage_def and lambda function */
    std::string print_param(int, int);

    /** int is the output shape of compute function, emit the input index. eg: shape_dim = 2: emit: [x, y] */
    void print_load_index(int);

    /** print all the input name within a bracket using the vector input_name, e.g. (input1, input2): */
    void print_input_name();

    /** print all the input placeholder using vector input_name*/
    void print_placeholder();

    // /** print the stage def, including def name and def parameter for each For loop. Notice: don't print "def " here */
    // void print_stage_def(std::string, int, int); // formal

    /** print the stage def, including def name and def parameter for each For loop. Notice: don't print "def " here */
    void print_stage_def(); // new

    /** print it when using stage def. Used in hcl.compute's lambda definition. It's different from print_stage_def() because the parameter is the last stage_name instead of output_name(produce_name) */
    void print_using_stage_def_when_compute();

    /** Emit a statement */
    void print_stmt(Stmt);

    /** Emit a version of a string that is a valid identifier in C (. is replaced with _) */
    virtual std::string print_name(const std::string &);

    /** translate from Halide axis (x, y ...) to HeteroCL axis (axis[0], axis[1] ...) */
    std::string get_axis(std::string);

    /** print param name, like "_linear_s0_x, _linear_s0_y, _linear_s0_c" */
    void print_param_name();

    /** Use for_extent information to print each stage's output shape */
    void print_stage_shape();

    /** to print output shape at the very first of HeteroCL code. */
    void print_output_extent(std::string);

    /** print output min index at the very first of HeteroCL code: simply set all min = 0 */
    void print_output_min(std::string);

    /** print Halide image axis in last stage shape's order. Because HeteroCL For loop order is related to its shape of stage, so in reorder scheduling, need to deal with it. # Maybe need to have a map for specific Halide Func. For now, it only supports the stage only use last stages's Func */
    void print_list_as_last_stage_order(std::vector<Expr>);

    /** print Halide input image in reverse order. */
    void print_list_reverse(std::vector<Expr>);

    /** print those numpy() and execute code... */
    void instantiate();

    /** print sth like: hcl.cast(dtype = hcl.Float(bits = 32), expr = . It's used in every stage's calling. We get the data type from Realize visitor in every stages.*/
    void print_data_cast();

    // std::string find_input_name(const LowredFunc &func);

    //     /** Emit the C name for a halide type. If space_option is AppendSpace,
    //  *  and there should be a space between the type and the next token,
    //  *  one is appended. (This allows both "int foo" and "Foo *foo" to be
    //  *  formatted correctly. Otherwise the latter is "Foo * foo".)
    //  */
    // virtual std::string print_type(Type, AppendSpaceIfNeeded space_option = DoNotAppendSpace);

    // /** Emit an SSA-style assignment, and set id to the freshly generated name. Return id. */
    // virtual std::string print_assignment(Type t, const std::string &rhs);
    
    /** input_data_type, used in hcl.init(input_data_type) */
    std::string input_data_type;

    /** Record For Loop Shape */
    std::vector<std::string> shape;

    /** Record For Output Total Size */
    std::string output_size = "";// but it is used as input size for now...


    /** Record the output_name, which is extracted in each Produce. It's each stage's definition name */
    std::string output_name = "";

    // /** Record all the produce name (formal only record a output name) */
    // std::vector<std::string> produce_name_group; // seems not work very well, because stage def statement parameter need to correspond to the def using, and when using def we need to give specific tensor

    /** Record the for_bound in the for_extent */
    std::vector<Expr> for_extent;

    /** Record each axis's name in a for loop */
    std::vector<std::string> for_name;

    /** Record the input_name, get by vector<LoweredArguments> in the Module */
    std::vector<std::string> input_name;

    // /** input shape: record input extent */
    // std::vector<int> input_extents;

    /** key is input name, value is input_extents. Generated in Load visitor */
    std::map<std::string, std::vector<int> > input_info;

    /** to count the number of For loop. Each For loop corresponds to a Stage */
    int for_count;

    /** Record the reduction domain loop. key is reduction loop name, value is reduction index's extent */
    std::map<std::string, int> reduction_info;

    /** name of reducer. for now, it is "_sum" */
    std::string reducer_name;

    /** name of schedule */
    std::string schedule_name;

    /** record if there is a consumer. If True, need to include the consumer name in the param of def */
    bool if_consume;

    /** Record the consume names in a Producer visitor loop */
    std::vector<std::string> consume_name_group;

    /** Record the stage_num for every intermediate tensor being produced. This is needed when calling functions after hcl.compute(...) */
    std::map<std::string, int> produce_stage_num;

    /** Record the stage num in a Produce loop. First For loop under a produce have stage num 0, the next For loop have stage num 1... */
    int stage_num;

    /** Record the stage name = output_name + "_" + stage_num, e.g: "f_conv_0" */
    std::string stage_name = "";

    /** Record the last stage name, if it's not the very first stage */
    std::string last_stage_name;

    /** Record the last output name, if it's not the very first Produce */
    std::string last_output_name = "";

    /** Collect all the stage_name, use in the scheduling part: To set default order of Halide */
    std::vector<std::string> stage_name_group;

    /** Collect all the dimensions of the stage, used in the scheduling part. This doesn't have relationship wiith stage_name for now, just push back in order. Maybe need to create a map for stage_name and stage_dim in the future */
    std::vector<int> stage_dim_group;

    /** intermediate variable stored here. It's collected in LetStmt visitor, and in Variable visitor we check if the string name is in the inter_var. If it's true, replace it with the Stmt(value) stored */
    std::map<std::string, Expr> inter_var;

    /** make LetStmt validate. At first it's not, to ignore lots of useless information. It's validated after first Produce */
    bool letstmt_validate = 0;

    /** halide func's axis call order: generated from last stage's for_name (For loop order), which is the stage's output shape order */
    std::vector<int> func_axis_call_order;

    /** store the last func_axis_call_order, used in Call visitor's print_list_as_last_stage_order */
    std::vector<int> last_func_axis_call_order;

    /** collect all the hcl input name */
    std::vector<std::string> hcl_input_name_group;

    /** collect all the unroll info. Use map to record: key is string: stage_name, value is a vector with pair(two int) : unroll axis and unroll factor group */
    std::map< std::string, std::vector< std::pair<int, int> > > unroll_info;

    /** collect all the parallel info. Use map to record: key is string: stage_name, value is a vector of int: unroll axis. Parallel just need axis info, don't need a factor */
    std::map< std::string, std::vector<int> > parallel_info;

    /** vector of pair: contains unroll axis and unroll factor group */
    std::vector< std::pair<int, int> > axis_and_factor;

    /** vector of int: contains parallel axis of one stage*/
    std::vector<int> parallel_axis;

    // /** collect the unroll axis in a stage, stored in unroll_info */
    // std::vector<int> unroll_axis;

    // /** collect the unroll factor of each axis */
    // std::vector<int> unroll_factor_group;

    /** record the stage_axis in a For loop. Count from 0, 1, 2, ... */
    int stage_axis = 0;

    /** record if this stage(for loop) has unrolled axis */
    bool if_stage_unroll = 0;

    /** record if this stage(for loop) has parallel axis */
    bool if_stage_parallel = 0;

    /** original output name(func name) without print_name. Generated in Producer visitor */
    std::string original_output_name;

    /** record every stage's output data type, including data type and bits. This info is extracted in Realize visitor, and used in hcl.compute() */
    std::pair <std::string, int> stage_type;

    /** record input data type: input_type->first is input name, input_type->second is a pair including (type, bits) */
    std::map< std::string, std::pair <std::string, int> > input_type;


    // New Transformation Rules
    // Function
    void print_index(int number);
    





    // Variables
    /** function_name, collected in compile(const LoweredFunc &f); it is also the last stage's name */
    std::string function_name;


    using IRVisitor::visit;

    void visit(const Variable *) override;
    void visit(const IntImm *) override; // ignore the type for code cleaning
    void visit(const UIntImm *) override; // ignore the type for code cleaning
    // void visit(const StringImm *) override;
    void visit(const FloatImm *) override; // need to deal with float bits maybe. Maybe just print op->value is ok
    void visit(const Cast *) override;
    void visit(const Add *) override;
    void visit(const Sub *) override;
    void visit(const Mul *) override;
    // void visit(const Div *) override;
    // void visit(const Mod *) override;
    void visit(const Max *) override;
    void visit(const Min *) override;
    // void visit(const EQ *) override;
    // void visit(const NE *) override;
    // void visit(const LT *) override;
    // void visit(const LE *) override;
    // void visit(const GT *) override;
    // void visit(const GE *) override;
    void visit(const And *) override;
    void visit(const Or *) override;
    // void visit(const Not *) override;
    void visit(const Call *) override; // for cleaning the code, ignore it
    void visit(const Select *) override;
    void visit(const Load *) override; // dont use it for now. Be replaced by Call for multi dimension
    void visit(const Store *) override; // dont use it for now. Be replaced by Provide for multi dimensiona
    void visit(const Let *) override; // used in linear_blur app
    void visit(const LetStmt *) override; // for cleaning the code, ignore it, just print(op->body)
    void visit(const AssertStmt *) override; // for cleaning the code, ignore it
    void visit(const ProducerConsumer *) override;
    void visit(const For *) override;

    // void visit(const Block *) override;
    
    // void visit(const Ramp *) override;
    // void visit(const Broadcast *) override;
    void visit(const Provide *) override; // used in multi-dimensional store
    // void visit(const Allocate *) override;
    // void visit(const Free *) override;
    void visit(const Realize *) override; // figure out output type of each stage
    void visit(const IfThenElse *) override; // for cleaning the code, have problems with its meaning
    void visit(const Evaluate *) override;
    // void visit(const Shuffle *) override;
    // void visit(const Prefetch *) override;
    // void visit(const Fork *) override;
    // void visit(const Acquire *) override;

    void visit_binop(Type t, Expr a, Expr b, const char *op);

    template<typename T>
    static std::string with_sep(const std::vector<T> &v, const std::string &sep) {
        std::ostringstream o;
        for (size_t i = 0; i < v.size(); ++i) {
            if (i > 0) {
                o << sep;
            }
            o << v[i];
        }
        return o.str();
    }

    template<typename T>
    static std::string with_commas(const std::vector<T> &v) {
        return with_sep<T>(v, ", ");
    }
};


}  // namespace Internal
}  // namespace Halide

#endif

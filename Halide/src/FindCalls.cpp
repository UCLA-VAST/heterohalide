#include "FindCalls.h"
#include "IRVisitor.h"

namespace Halide {
namespace Internal {

using std::map;
using std::pair;
using std::string;
using std::vector;

namespace {
/* Find all the internal halide calls in an expr */
class FindCalls : public IRVisitor {
public:
    map<string, Function> calls;

    using IRVisitor::visit;

    void include_function(Function f) {
        map<string, Function>::iterator iter = calls.find(f.name());
        if (iter == calls.end()) {
            calls[f.name()] = f;
        } else {
            user_assert(iter->second.same_as(f))
                << "Can't compile a pipeline using multiple functions with same name: "
                << f.name() << "\n";
        }
    }

    void visit(const Call *call) override {
        IRVisitor::visit(call);

        if (call->call_type == Call::Halide && call->func.defined()) {
            Function f(call->func);
            include_function(f);
        }
    }
};

void populate_environment_helper(Function f, map<string, Function> &env,
                                 bool recursive = true, bool include_wrappers = false) {
    map<string, Function>::const_iterator iter = env.find(f.name());
    if (iter != env.end()) {
        user_assert(iter->second.same_as(f))
            << "Can't compile a pipeline using multiple functions with same name: "
            << f.name() << "\n";
        return;
    }

    FindCalls calls;
    f.accept(&calls);
    debug(2) << "if f has extern definition?: " << f.has_extern_definition() << "\n"; // False
    if (f.has_extern_definition()) {
        for (ExternFuncArgument arg : f.extern_arguments()) {
            if (arg.is_func()) {
                Function g(arg.func);
                calls.calls[g.name()] = g;
            }
        }
    }
    debug(2) << "if include wrappers?: " << include_wrappers << "\n"; // True
    if (include_wrappers) {
        for (auto it : f.schedule().wrappers()) {
            Function g(it.second);
            calls.calls[g.name()] = g;
        }
    }
    debug(2) << "if recursive?: " << recursive << "\n"; // True. !recursive = False
    if (!recursive) {
        env.insert(calls.calls.begin(), calls.calls.end());
    } else {
        env[f.name()] = f;

        for (pair<string, Function> i : calls.calls) {
            populate_environment_helper(i.second, env, recursive, include_wrappers);
        }
    }
}

}  // namespace

void populate_environment(Function f, map<string, Function> &env) {
    populate_environment_helper(f, env, true, true);
}

map<string, Function> find_transitive_calls(Function f) {
    map<string, Function> res;
    populate_environment_helper(f, res, true, false);
    return res;
}

map<string, Function> find_direct_calls(Function f) {
    map<string, Function> res;
    populate_environment_helper(f, res, false, false);
    return res;
}

}  // namespace Internal
}  // namespace Halide


#include "verify_program.hpp"
#include <migraphx/program.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/operators.hpp>

struct test_concat_transpose2 : verify_program<test_concat_transpose2>
{
    migraphx::program create_program() const
    {
        migraphx::program p;
        int axis = 1;
        migraphx::shape s0{migraphx::shape::int32_type, {2, 2}};
        migraphx::shape s1{migraphx::shape::int32_type, {2, 3}};
        migraphx::shape s2{migraphx::shape::int32_type, {5, 2}};
        auto l0  = p.add_parameter("x", s0);
        auto l1  = p.add_parameter("y", s1);
        auto lp2 = p.add_parameter("z", s2);
        auto l2  = p.add_instruction(migraphx::op::transpose{{1, 0}}, lp2);
        p.add_instruction(migraphx::op::concat{axis}, l0, l1, l2);
        return p;
    }
};
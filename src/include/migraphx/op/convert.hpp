#ifndef MIGRAPHX_GUARD_OPERATORS_CONVERT_HPP
#define MIGRAPHX_GUARD_OPERATORS_CONVERT_HPP

#include <array>
#include <migraphx/op/unary.hpp>
#include <migraphx/operation.hpp>
#include <migraphx/check_shapes.hpp>
#include <migraphx/stringutils.hpp>
#include <migraphx/streamutils.hpp>
#include <migraphx/literal.hpp>
#include <migraphx/shape_for_each.hpp>
#include <migraphx/config.hpp>
#include <cmath>
#include <utility>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace op {

struct convert : unary<convert>
{
    shape::type_t target_type = shape::half_type;
    float scale               = 1.0f;
    float shift               = 0.0f;

    template <class Self, class F>
    static auto reflect(Self& self, F f)
    {
        return pack(
            f(self.target_type, "target_type"), f(self.scale, "scale"), f(self.shift, "shift"));
    }

    shape compute_shape(std::vector<shape> inputs) const
    {
        check_shapes{inputs, *this}.has(1);
        return {target_type, inputs.at(0).lens(), inputs.at(0).strides()};
    }

    auto apply() const
    {
        return [&](auto x) {
            float res = scale * x + shift;
            if(target_type == shape::int8_type)
            {
                if(res > 127.0 or res < -128.0)
                {
                    std::cout << "res = " << res << std::endl;
                }
                int factor = (res > 0) ? 1 : -1;
                res        = res + factor * 0.5f;
                res        = res > 127.0 ? 127.0 : res;
                res        = res < -128.0 ? -128.0 : res;
            }

            return res;
        };
    }

    convert(shape::type_t t) : target_type{t} {}
    convert(shape::type_t t, float sle, float sft) : target_type{t}, scale{sle}, shift{sft} {}
    convert() {}
};

} // namespace op
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif
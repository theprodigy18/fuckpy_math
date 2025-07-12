#include "vector/vec2.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/operators.h"

namespace py = pybind11;

PYBIND11_MODULE(fuckpy_math, m)
{
    m.doc() = "Math module.";

    py::class_<SIMD_Vec2>(m, "vec2", "A vector with 2 floating point components.")
        .def(py::init<f32, f32>(), "Need to specify x and y.")
        .def("__repr__", [](const SIMD_Vec2& self)
             {
            std::ostringstream oss;
            oss << "vec2(" << self.X() << ", " << self.Y() << ")";
            return oss.str(); }, "Returns the string representation of the vector.")
        .def_static("zero", &SIMD_Vec2::Zero, "Returns vector (0, 0).")
        .def_static("one", &SIMD_Vec2::One, "Returns vector (1, 1).")
        .def_static("right", &SIMD_Vec2::Right, "Returns vector (1, 0).")
        .def_static("left", &SIMD_Vec2::Left, "Returns vector (-1, 0).")
        .def_static("up", &SIMD_Vec2::Up, "Returns the vector (0, 1).")
        .def_static("down", &SIMD_Vec2::Down, "Returns the vector (0, -1).")
        .def_property_readonly("x", &SIMD_Vec2::X, "The x component of the vector.")
        .def_property_readonly("y", &SIMD_Vec2::Y, "The y component of the vector.")
        .def("set_x", &SIMD_Vec2::SetX, "Sets the x component of the vector.")
        .def("set_y", &SIMD_Vec2::SetY, "Sets the y component of the vector.")
        .def("length", &SIMD_Vec2::Length, "Returns the length of the vector.")
        .def("squared_length", &SIMD_Vec2::SquaredLength, "Returns the squared length of the vector.")
        .def("dot", &SIMD_Vec2::Dot, "Returns the dot product of the this vector and another.")
        .def("normalized", &SIMD_Vec2::Normalized, "Returns the normalized vector.")
        .def(py::self + py::self, "Add two vectors.")
        .def(py::self - py::self, "Subtract two vectors.")
        .def(py::self * f32(), "Multiply vector by a scalar.")
        .def(py::self / f32(), "Divide vector by a scalar.")
        .def_static("sum_normalized", &SIMD_Vec2::SumNormalized, "Returns the sum of normalized vectors.");

    py::class_<SIMD_IVec2>(m, "ivec2", "A vector with 2 integer components.")
        .def(py::init<i32, i32>(), "Need to specify x and y.")
        .def("__repr__", [](const SIMD_IVec2& self)
             {
            std::ostringstream oss;
            oss << "ivec2(" << self.X() << ", " << self.Y() << ")";
            return oss.str(); }, "Returns the string representation of the vector.")
        .def_static("zero", &SIMD_IVec2::Zero, "Returns vector (0, 0).")
        .def_static("one", &SIMD_IVec2::One, "Returns vector (1, 1).")
        .def_static("right", &SIMD_IVec2::Right, "Returns vector (1, 0).")
        .def_static("left", &SIMD_IVec2::Left, "Returns vector (-1, 0).")
        .def_static("up", &SIMD_IVec2::Up, "Returns the vector (0, 1).")
        .def_static("down", &SIMD_IVec2::Down, "Returns the vector (0, -1).")
        .def_property_readonly("x", &SIMD_IVec2::X, "The x component of the vector.")
        .def_property_readonly("y", &SIMD_IVec2::Y, "The y component of the vector.")
        .def("set_x", &SIMD_IVec2::SetX, "Sets the x component of the vector.")
        .def("set_y", &SIMD_IVec2::SetY, "Sets the y component of the vector.")
        .def("dot", &SIMD_IVec2::Dot, "Returns the dot product of the this vector and another.")
        .def(py::self + py::self, "Add two vectors.")
        .def(py::self - py::self, "Subtract two vectors.")
        .def(py::self * i32(), "Multiply vector by a scalar.")
        .def(py::self / i32(), "Divide vector by a scalar.");
}
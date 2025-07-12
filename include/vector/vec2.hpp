#pragma once

#include "common_header.hpp"

inline static __m128 SimdDot2(__m128 a, __m128 b)
{
    __m128 mul = _mm_mul_ps(a, b);                         // [x*x, y*y, ?, ?]
    __m128 sum = _mm_add_ps(mul, _mm_movehl_ps(mul, mul)); // [x*x + y*y, ?, ?, ?]
    return _mm_move_ss(mul, sum);                          // [x*x + y*y, ?, ?, ?]
}

struct SIMD_Vec2
{
    __m128 data;

    SIMD_Vec2(f32 x, f32 y) : data(_mm_setr_ps(x, y, 0.0f, 0.0f)) { }

    f32 X() const { return _mm_cvtss_f32(data); }
    f32 Y() const
    {
        __m128 shuf {_mm_shuffle_ps(data, data, _MM_SHUFFLE(0, 0, 0, 1))};
        return _mm_cvtss_f32(shuf);
    }

    void SetX(f32 x) { data = _mm_move_ss(data, _mm_set_ss(x)); }
    void SetY(f32 y)
    {
        data = _mm_insert_ps(data, _mm_set_ss(y), 0x10);
    }

    // Some value declaration.
    static SIMD_Vec2 Zero() { return SIMD_Vec2(0.0f, 0.0f); }
    static SIMD_Vec2 One() { return SIMD_Vec2(1.0f, 1.0f); }
    static SIMD_Vec2 Right() { return SIMD_Vec2(1.0f, 0.0f); }
    static SIMD_Vec2 Left() { return SIMD_Vec2(-1.0f, 0.0f); }
    static SIMD_Vec2 Up() { return SIMD_Vec2(0.0f, 1.0f); }
    static SIMD_Vec2 Down() { return SIMD_Vec2(0.0f, -1.0f); }

    SIMD_Vec2 operator+(const SIMD_Vec2& other) const { return SIMD_Vec2(_mm_add_ps(data, other.data)); }
    SIMD_Vec2 operator-(const SIMD_Vec2& other) const { return SIMD_Vec2(_mm_sub_ps(data, other.data)); }
    SIMD_Vec2 operator*(const f32 scalar) const { return SIMD_Vec2(_mm_mul_ps(data, _mm_set1_ps(scalar))); }
    SIMD_Vec2 operator/(const f32 scalar) const { return SIMD_Vec2(_mm_div_ps(data, _mm_set1_ps(scalar))); }

    f32 Length() const
    {
        __m128 sum {SimdDot2(data, data)};
        __m128 sqrt {_mm_sqrt_ss(sum)}; // [sqrt(x2 + y2), x2, x2, x2]
        return _mm_cvtss_f32(sqrt);     // [sqrt(x2 + y2)]
    }

    f32 SquaredLength() const
    {
        __m128 sum {SimdDot2(data, data)};
        return _mm_cvtss_f32(sum);
    }

    f32 Dot(const SIMD_Vec2& other) const
    {
        i32 x = _mm_extract_epi32(data, 0) * _mm_extract_epi32(other.data, 0);
        i32 y = _mm_extract_epi32(data, 1) * _mm_extract_epi32(other.data, 1);
        return x + y;
    }

    SIMD_Vec2 Normalized() const
    {
        __m128 sum {SimdDot2(data, data)};
        __m128 sqrt {_mm_sqrt_ss(sum)};
        __m128 lenFull {_mm_shuffle_ps(sqrt, sqrt, _MM_SHUFFLE(0, 0, 0, 0))};

        if (_mm_cvtss_f32(lenFull) <= 0.0f)
            return SIMD_Vec2(0.0f, 0.0f);

        return SIMD_Vec2(_mm_div_ps(data, lenFull));
    }

    static SIMD_Vec2 SumNormalized(const SIMD_Vec2& v, size_t count)
    {
        SIMD_Vec2 sum = SIMD_Vec2::Zero();
        for (size_t i = 0; i < count; ++i)
            sum = sum + v.Normalized();
        return sum;
    }

private:
    SIMD_Vec2(__m128 data) : data(data) { } // Private constructor for internal use.
};

struct SIMD_IVec2
{
    __m128i data;

    SIMD_IVec2(i32 x, i32 y) : data(_mm_setr_epi32(x, y, 0, 0)) { }

    i32 X() const { return _mm_extract_epi32(data, 0); }
    i32 Y() const { return _mm_extract_epi32(data, 1); }

    void SetX(i32 x) { data = _mm_insert_epi32(data, x, 0); }
    void SetY(i32 y) { data = _mm_insert_epi32(data, y, 1); }

    // Some value declaration.
    static SIMD_IVec2 Zero() { return SIMD_IVec2(0, 0); }
    static SIMD_IVec2 One() { return SIMD_IVec2(1, 1); }
    static SIMD_IVec2 Right() { return SIMD_IVec2(1, 0); }
    static SIMD_IVec2 Left() { return SIMD_IVec2(-1, 0); }
    static SIMD_IVec2 Up() { return SIMD_IVec2(0, 1); }
    static SIMD_IVec2 Down() { return SIMD_IVec2(0, -1); }

    SIMD_IVec2 operator+(const SIMD_IVec2& other) const { return SIMD_IVec2(_mm_add_epi32(data, other.data)); }
    SIMD_IVec2 operator-(const SIMD_IVec2& other) const { return SIMD_IVec2(_mm_sub_epi32(data, other.data)); }
    SIMD_IVec2 operator*(const i32 scalar) const { return SIMD_IVec2(_mm_mullo_epi32(data, _mm_set1_epi32(scalar))); }
    SIMD_IVec2 operator/(const i32 scalar) const
    {
        __m128i res {_mm_setzero_si128()};
        i32     x {_mm_extract_epi32(data, 0) / scalar};
        i32     y {_mm_extract_epi32(data, 1) / scalar};
        res = _mm_insert_epi32(res, x, 0);
        res = _mm_insert_epi32(res, y, 1);
        return SIMD_IVec2(res);
    }

    i32 Dot(const SIMD_IVec2& other) const
    {
        __m128i sum {_mm_mullo_epi32(data, other.data)};
        __m128i shuf {_mm_shuffle_epi32(sum, _MM_SHUFFLE(0, 0, 0, 1))};
        __m128i add {_mm_add_epi32(sum, shuf)};
        return _mm_cvtsi128_si32(add);
    }

private:
    SIMD_IVec2(__m128i data) : data(data) { }
};

inline SIMD_Vec2  IVec2ToVec2(const SIMD_IVec2& v) { return SIMD_Vec2(v.X(), v.Y()); }
inline SIMD_IVec2 Vec2ToIVec2(const SIMD_Vec2& v) { return SIMD_IVec2((i32) v.X(), (i32) v.Y()); }
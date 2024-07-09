#include "Color.h"
#include <algorithm>
#include <cassert>
#include <cmath>

int iRgbSqrt(int num) {
    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    assert("sqrt input should be non-negative" && num >= 0);
    assert("sqrt input should no exceed 16 bits" && num <= 0xFFFF);
    int res = 0;
    int bit = 1 << 16;
    while (bit > num)
        bit >>= 2;
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        } else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

Rgb Rgb::operator+(const Rgb& in) const {
    auto copy = *this;
    copy += in;
    return copy;
}

Rgb& Rgb::operator+=(const Rgb& in) {
    unsigned int red = r + in.r;
    r = (red < 255) ? red : 255;
    unsigned int green = g + in.g;
    g = (green < 255) ? green : 255;
    unsigned int blue = b + in.b;
    b = (blue < 255) ? blue : 255;
    return *this;
}

Rgb Rgb::operator-(const Rgb& in) const {
    auto copy = *this;
    copy -= in;
    return copy;
}

Rgb& Rgb::operator-=(const Rgb& in) {
    r = (in.r > r) ? 0 : r - in.r;
    g = (in.g > g) ? 0 : g - in.g;
    b = (in.b > b) ? 0 : b - in.b;
    return *this;
}

Rgb& Rgb::blend(const Rgb& in) {
    unsigned int inAlpha = in.a * (255 - a);
    unsigned int alpha = a + inAlpha;
    r = iRgbSqrt(((r * r * a) + (in.r * in.r * inAlpha)) / alpha);
    g = iRgbSqrt(((g * g * a) + (in.g * in.g * inAlpha)) / alpha);
    b = iRgbSqrt(((b * b * a) + (in.b * in.b * inAlpha)) / alpha);
    a = alpha;
    return *this;
}

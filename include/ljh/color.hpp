
//          Copyright Jared Irwin 2022-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// color.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include <tuple>
#include <cstdint>
#include <cmath>
#include <algorithm>

namespace ljh
{
    // Converts an HSL color value to RGB. Conversion formula
    // adapted from http://en.wikipedia.org/wiki/HSL_color_space.
    // Assumes h, s, and l are contained in the set [0, 1] and
    // returns r, g, and b in the set [0, 255].
    //
    // @param h The hue
    // @param s The saturation
    // @param l The lightness
    // @return tuple of uint8_t
    LJH_MODULE_MATH_EXPORT [[nodiscard]] auto hsl_to_rgb(double h, double s, double l) -> std::tuple<uint8_t, uint8_t, uint8_t>
    {
        auto hue_to_rgb = [](double p, double q, double t) {
            if (t < 0)
                t += 1;
            if (t > 1)
                t -= 1;
            if (t < 1 / 6)
                return p + (q - p) * 6 * t;
            if (t < 1 / 2)
                return q;
            if (t < 2 / 3)
                return p + (q - p) * (2 / 3 - t) * 6;
            return p;
        };

        double r, g, b;

        if (s == 0)
        {
            r = g = b = l; // achromatic
        }
        else
        {
            auto q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            auto p = 2 * l - q;
            r      = hue_to_rgb(p, q, h + 1 / 3);
            g      = hue_to_rgb(p, q, h);
            b      = hue_to_rgb(p, q, h - 1 / 3);
        }

        return std::make_tuple((uint8_t)std::round(r * 255), (uint8_t)std::round(g * 255), (uint8_t)std::round(b * 255));
    }

    // Converts an RGB color value to HSL. Conversion formula
    // adapted from http://en.wikipedia.org/wiki/HSL_color_space.
    // Assumes r, g, and b are contained in the set [0, 255] and
    // returns h, s, and l in the set [0, 1].
    //
    // @param r The red color value
    // @param g The green color value
    // @param b The blue color value
    // @return tuple of doubles
    LJH_MODULE_MATH_EXPORT [[nodiscard]] auto rgb_to_hsl(uint8_t r, uint8_t g, uint8_t b) -> std::tuple<double, double, double>
    {
        double dr = r / 255.;
        double dg = g / 255.;
        double db = b / 255.;

        auto max = std::max({dr, dg, db});
        auto min = std::min({dr, dg, db});

        double h, s, l;
        l = (max + min) / 2.0f;

        if (max == min)
        {
            h = s = 0.0f;
        }
        else
        {
            double d = max - min;
            s        = d / ((l > 0.5f) ? (2.0f - max - min) : (max + min));

            if (dr > dg && dr > db)
                h = (dg - db) / d + (dg < db ? 6.0f : 0.0f);
            else if (dg > db)
                h = (db - dr) / d + 2.0f;
            else
                h = (dr - dg) / d + 4.0f;

            h /= 6.0f;
        }

        return std::make_tuple(h, s, l);
    }
} // namespace ljh
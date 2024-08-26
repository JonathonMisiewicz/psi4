/*
 * @BEGIN LICENSE
 *
 * Psi4: an open-source quantum chemistry software package
 *
 * Copyright (c) 2007-2024 The Psi4 Developers.
 *
 * The copyrights for code used from other parties are included in
 * the corresponding files.
 *
 * This file is part of Psi4.
 *
 * Psi4 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * Psi4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with Psi4; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @END LICENSE
 */

/*! \file
    \ingroup DETCI
    \brief Enter brief description of file here
*/

/*
** ODOMETER.CC
**
** C. David Sherrill
** Center for Computational Quantum Chemistry
** University of Georgia
*/

#include "psi4/psi4-dec.h"
#include "psi4/libpsi4util/PsiOutStream.h"

#include "psi4/detci/odometer.h"

#include <algorithm>
#include <cstdio>
#include <numeric>

namespace psi {
namespace detci {

// #define STANDALONE
//#ifdef STANDALONE
//#include <iostream>
//#endif

Odometer::Odometer() {
    length = 0;
}

void Odometer::size(size_t n) {
    outfile->Printf("size in\n");
    length = n;
    max = std::vector<int>(n, 0);
    min = std::vector<int>(n, 0);
    value = std::vector<int>(n, 0);
    outfile->Printf("size out\n");
}

void Odometer::set_max(int m) {
    outfile->Printf("set_max in\n");
    std::fill(max.begin(), max.end(), m);
    outfile->Printf("set_max out\n");
}

void Odometer::set_max_lex(int m) {
    outfile->Printf("set_max_lex in\n");
    std::iota(std::rbegin(max), std::rend(max), m - length + 1);
    outfile->Printf("set_max_lex out\n");
}

void Odometer::set_max(const std::vector<int>& m) {
    outfile->Printf("set_max2 in\n");
    max = m;
    outfile->Printf("set_max2 out\n");
}

void Odometer::set_min(int m) {
    outfile->Printf("set_min in\n");
    std::fill(min.begin(), min.end(), m);
    outfile->Printf("set_min out\n");
}

void Odometer::set_min_lex(int m) {
    outfile->Printf("set_min_lex in\n");
    std::iota(std::rbegin(min), std::rend(min), m);
    outfile->Printf("set_min_lex out\n");
}

void Odometer::set_min(const std::vector<int>& m) {
    outfile->Printf("set_min2 in\n");
    min = m;
    outfile->Printf("set_min2 out\n");
}

void Odometer::set_value(int m) {
    outfile->Printf("set_value in\n");
    std::fill(value.begin(), value.end(), m);
    outfile->Printf("set_value out\n");
}

void Odometer::get_value(std::vector<int>& m) {
    outfile->Printf("get_value in\n");
    m = value;
    outfile->Printf("get_value out\n");
}

void Odometer::set_value(const std::vector<int>& m) {
    outfile->Printf("set_value2 in\n");
    value = m;
    outfile->Printf("set_value2 out\n");
}

void Odometer::reset() {
    outfile->Printf("reset in\n");
    value = min;
    outfile->Printf("reset out\n");
}

void Odometer::increment() {
    outfile->Printf("increment in\n");
    for (size_t i = 0; i < length; i++) {
        if (value[i] < max[i]) {
            value[i] += 1;
            return;
        } else {
            value[i] = min[i];
        }
    }
    outfile->Printf("increment out\n");
}

void Odometer::increment_lex() {
    outfile->Printf("increment_lex in: %d %d %d %d\n", value.size(), max.size(), min.size(), length);
    for (size_t i = 0; i < length; i++) {
        outfile->Printf("iter %d", i);
        if (value[i] < max[i]) {
            outfile->Printf("true branch");
            value[i] += 1;
            for (size_t j = i; j >= 1; j--) {
                if (value[j] + 1 >= min[j - 1])
                    value[j - 1] = value[j] + 1;
                else
                    value[j - 1] = min[j - 1];
            }
            break;
        } else {
            outfile->Printf("false branch");
            value[i] = min[i];
        }
    }
    outfile->Printf("increment_lex out\n");
}

void Odometer::print() {
    for (size_t i = length - 1; i >= 0; i--) {
        outfile->Printf("%d ", value[i]);
    }
    outfile->Printf("\n");
}

bool Odometer::at_max() {
    outfile->Printf("at_max in\n");
    for (size_t i = 0; i < length; i++) {
        if (value[i] != max[i]) return false;
    }
    return true;
}

bool Odometer::at_min() {
    outfile->Printf("at_min in\n");
    for (size_t i = 0; i < length; i++) {
        if (value[i] != min[i]) return false;
    }
    return true;
}

bool Odometer::boundscheck() {
    outfile->Printf("boundscheck in\n");
    for (size_t i = 0; i < length; i++)
        if (max[i] < min[i]) return false;

    return true;
}

}
}  // namespace psi

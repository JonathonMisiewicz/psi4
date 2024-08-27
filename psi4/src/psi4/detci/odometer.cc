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
    length = n;
    max = std::vector<int>(n, 0);
    min = std::vector<int>(n, 0);
    value = std::vector<int>(n, 0);
}

void Odometer::set_max(int m) {
    std::fill(max.begin(), max.end(), m);
}

void Odometer::set_max_lex(int m) {
    std::iota(std::rbegin(max), std::rend(max), m - length + 1);
}

void Odometer::set_max(const std::vector<int>& m) {
    max = m;
}

void Odometer::set_min(int m) {
    std::fill(min.begin(), min.end(), m);
}

void Odometer::set_min_lex(int m) {
    std::iota(std::rbegin(min), std::rend(min), m);
}

void Odometer::set_min(const std::vector<int>& m) {
    min = m;
}

void Odometer::set_value(int m) {
    std::fill(value.begin(), value.end(), m);
}

void Odometer::get_value(std::vector<int>& m) {
    m = value;
}

void Odometer::set_value(const std::vector<int>& m) {
    value = m;
}

void Odometer::reset() {
    value = min;
}

void Odometer::increment() {
    for (size_t i = 0; i < length; i++) {
        if (value[i] < max[i]) {
            value[i] += 1;
            return;
        } else {
            value[i] = min[i];
        }
    }
}

void Odometer::increment_lex() {
    for (size_t i = 0; i < length; i++) {
        if (value[i] < max[i]) {
            value[i] += 1;
            for (size_t j = i; j >= 1; j--) {
                if (value[j] + 1 >= min[j - 1])
                    value[j - 1] = value[j] + 1;
                else
                    value[j - 1] = min[j - 1];
            }
            break;
        } else {
            value[i] = min[i];
        }
    }
}

void Odometer::print() {
    for (size_t i = length - 1; i >= 0; i--) {
        outfile->Printf("%d ", value[i]);
    }
    outfile->Printf("\n");
}

bool Odometer::at_max() {
    for (size_t i = 0; i < length; i++) {
        if (value[i] != max[i]) return false;
    }
    return true;
}

bool Odometer::at_min() {
    for (size_t i = 0; i < length; i++) {
        if (value[i] != min[i]) return false;
    }
    return true;
}

bool Odometer::boundscheck() {
    for (size_t i = 0; i < length; i++)
        if (max[i] < min[i]) return false;

    return true;
}

}
}  // namespace psi

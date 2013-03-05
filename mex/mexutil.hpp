/**
	Copyright 2012 Institute for Communications and Navigation, TUM

	This file is part of uhdinterface.

    uhdinterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    uhdinterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uhdinterface.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "uhdinterface.hpp"

#ifndef MEXUTIL_HPP_
#define MEXUTIL_HPP_

/**
 * convert mxArray to a real valued double and abort with a Matlab error if not possible
 */
double getDouble(const mxArray *prhs);

/**
 * convert mxArray to integer and abort with a Matlab error if not possible
 */
int getInt(const mxArray *prhs);

/**
 * convert mxArray to integer and abort with a Matlab error if not possible or value is negative
 */
int getUInt(const mxArray *prhs);

/**
 * convert mxArray to integer and abort with a Matlab error if not possible or value is negative
 */
size_t getSize_t(const mxArray *prhs);

/**
 * convert mxArray to a string and abort with a Matlab error if not possible
 */
std::string getString(const mxArray *prhs);

/**
 * convert mxArray of size Nx1 to vector<cdouble> of size N and abort with a Matlab error if not possible
 */
std::vector<cdouble>* getCVector(const mxArray *prhs);

/**
 * return a pointer to a mxArray containing a single real double value
 */
mxArray* retDouble(double d);

/**
 * return a pointer to a mxArray containing a single integer value
 */
mxArray* retInt(int i);

/**
 * return a pointer to a Nx1 mxArray containing the elements of a vector of type cdouble
 */
mxArray* retCVector(std::vector<cdouble> vec);

/**
 * return a pointer to a mxArray containing a string
 */
mxArray* retString(std::string s);

/**
 * check if variable is an unsigned integer value
 */
bool isUInt(const mxArray *prhs);

/**
 * check if variable is a string with at least two chars
 */
bool isString(const mxArray *prhs);

#endif /* MEXUTIL_HPP_ */

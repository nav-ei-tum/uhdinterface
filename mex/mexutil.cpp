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

#include "mexutil.hpp"

void checkForScalar(const mxArray *prhs) {
	if((mxGetM(prhs) * mxGetN(prhs)) != 1) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotScalar", "Input must be a scalar.");
	}
}


double getDouble(const mxArray *prhs){
	checkForScalar(prhs);
	if(!mxIsDouble(prhs)) {
			mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotDouble", "Input must be a double.");
		}
	return *(mxGetPr(prhs));
}

int getInt(const mxArray *prhs){
	return int(getDouble(prhs));
}

int getUInt(const mxArray *prhs){
	double d = getDouble(prhs);
	if(d < 0) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotPositive", "Input must be a positive number.");
	}
	return int(d);
}

size_t getSize_t(const mxArray *prhs){
	double d = getDouble(prhs);
	if(d < 0) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotPositive", "Input must be a positive number.");
	}
	return size_t(d);
}

std::string getString(const mxArray *prhs) {
	if ( mxIsChar(prhs) != 1) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotString", "Input must be a string.");
	}

	/* input must be a row vector */
	if (mxGetM(prhs)!=1){
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotVector", "Input must be a row vector.");
	}
	std::string str(mxArrayToString(prhs));

	if(str.empty()){
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:conversionFailed", "Could not convert input to string.");
	}

	return str;
}

std::vector<cdouble>* getCVector(const mxArray *prhs) {
	size_t n = mxGetN(prhs)*mxGetM(prhs);

	if(n == 0) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputEmpty", "Input vector is empty.");
	}
	if(!mxIsDouble(prhs)) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:inputNotDouble", "Input must be a double.");
	}

	std::vector<cdouble> *buff = new std::vector<cdouble>();
	buff->reserve(n);
	double *rebuf = mxGetPr(prhs);
	double *imbuf = mxGetPi(prhs);

	size_t i;
	std::complex<double> c;
	for(i=0;i<n;i++) {
		buff->push_back(std::complex<double>(rebuf[i],(mxIsComplex(prhs) ? imbuf[i] : 0)));
	}
	return buff;
}

mxArray* retDouble(double d) {
	mxArray *plhs = mxCreateDoubleMatrix(1,1,mxREAL);
	double *buf = mxGetPr(plhs);
	*buf = d;
	return plhs;
}

mxArray* retInt(int i) {
	mxArray *plhselement = mxCreateDoubleMatrix(1,1,mxREAL);
	double *buf = mxGetPr(plhselement);
	*buf = double(i);

	return plhselement;
}

mxArray* retString(std::string s) {
	return mxCreateString(s.c_str());
}

mxArray* retCVector(std::vector<cdouble> vec) {
	mxArray *plhselement = mxCreateDoubleMatrix(vec.size(),1,mxCOMPLEX);

	double *rebuf = mxGetPr(plhselement);
		double *imbuf = mxGetPi(plhselement);

	size_t i;
	for(i=0; i<vec.size(); i++) {
		rebuf[i] = std::real(vec.at(i));
		imbuf[i] = std::imag(vec.at(i));
		if(DEBUG) {
			if(i%10 == 0) {
				printf("\n");
			}
			printf("i:%3d %5f+j%5f = %5f+j%5f    ",i,rebuf[i],imbuf[i],std::real(vec.at(i)),std::imag(vec.at(i)));
		}
	}
	return plhselement;
}

bool isUInt(const mxArray *prhs) {
	if((mxGetM(prhs) * mxGetN(prhs)) != 1) {
		return false;
	}
	if(!mxIsDouble(prhs)) {
		return false;
	}

	double d = *(mxGetPr(prhs));;
	if(d-double(int(d)) < 1e-10) {
		return true;
	}
	return false;
}


bool isString(const mxArray *prhs) {
	if ( mxIsChar(prhs) != 1) {
		return false;
	}

	if (mxGetM(prhs)!=1){
		return false;
	}
	if (mxGetN(prhs)<=1){
		return false;
	}
	std::string str(mxArrayToString(prhs));

	if(!str.empty()){
		return true;
	}

	return false;
}

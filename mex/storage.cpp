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

#include "storage.hpp"

// init map
std::map<int,uhdstruct> Storage::uhdstructs = std::map<int,uhdstruct>();

// init testvalue
int Storage::test=0;

// no objhects stored yet, first free index is zero
int Storage::indexcount=0;


bool Storage::hasElement(int index){
	return uhdstructs.find(index) != uhdstructs.end();
}

uhdstruct* Storage::getUHDStruct(int index){
	if(hasElement(index)) {
		return &uhdstructs[index];
	}
	mexErrMsgIdAndTxt( "MATLAB:uhdInterface:noUSRP", "No valid USRP object associated with that index");
	return NULL;
}

bool Storage::removeUHDStruct(int index){
	if(hasElement(index)) {
		// remove the pointers
		getUHDStruct(index)->rx_stream.reset(); // NULL
		getUHDStruct(index)->tx_stream.reset(); // NULL
		getUHDStruct(index)->usrp.reset(); // NULL
		getUHDStruct(index)->init = false;

		return uhdstructs.erase(index) == 1;
	}
	return false;
}
int Storage::addUHDStruct(std::string arg){
	// check if the next free index is really free
	if(hasElement(indexcount)) {
		mexErrMsgIdAndTxt( "MATLAB:uhdInterface:indexFailure", "The assigned index is already in use");
		return -1;
	}

	// take the next free index
	size_t newindex = indexcount;

	// initialize and store the uhdstruct
	uhdstructs[newindex] = uhdstruct(uhd::usrp::multi_usrp::make(arg));

	// increase the index counter
	indexcount++;

	// return the index
	return newindex;
}

std::vector<int> Storage::getIndices(void) {
	std::vector<int> ret;

	std::map<int, uhdstruct>::iterator iter;

	for (iter = uhdstructs.begin(); iter != uhdstructs.end(); ++iter) {
		ret.push_back(iter->first);
	}

	return ret;
}

size_t Storage::getNumberOfElements(void) {
	return uhdstructs.size();
}

int Storage::getTest(void) {
	return test;
}

void Storage::setTest(int newtest){
	test = newtest;
	return;
}


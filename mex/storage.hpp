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
#include <string>
#include <map>

#ifndef STORAGE_HPP_
#define STORAGE_HPP_

class Storage{
public:

	/**
	 * Returns uhdstruct if it exists
	 */
	static uhdstruct* getUHDStruct(int index);

	/**
	 * Deinitialize uhdstruct and remove it from Storage if it exists. Returns true if removal was successful
	 */
	static bool removeUHDStruct(int index);

	/**
	 * Initialize a new uhdstruct and add it to Storage and return the index. Return -1 if initialization failed.
	 */
	static int addUHDStruct(std::string arg);

	/**
	 * Return a vector of all indices that are stored in Storage
	 */
	static std::vector<int> getIndices(void);

	/**
	 * Return the test value
	 */
	static int getTest(void);

	/**
	 * Replace the test value
	 */
	static void setTest(int newtest);

	/**
	 * Return the number of elements that are stored in Storage
	 */
	static size_t getNumberOfElements(void);

	/**
	 * Check if specified element is stored in Storage
	 */
	static bool hasElement(int index);

private:
	/**
	 * Map where all uhdstructs are stored
	 */
	static std::map<int,uhdstruct> uhdstructs;

	/**
	 * Value if the next free index
	 */
	static int indexcount;

	/**
	 * testvalue
	 */
	static int test;
};

#endif /* STORAGE_HPP_ */

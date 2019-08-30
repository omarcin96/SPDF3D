#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <map>
#include <vector>

namespace pdf3d
{

	/*
	Generic function to find an element in vector and also its position.
	It returns a pair of bool & int i.e.

	bool : Represents if element is present in vector or not.
	int : Represents the index of element in vector if its found else -1

	*/
	template < typename T>
	std::pair<bool, int > findInVector(const std::vector<T>& vecOfElements, const T& element)
	{
		std::pair<bool, int > result;

		// Find given element in vector
		auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

		if (it != vecOfElements.end())
		{
			result.second = std::distance(vecOfElements.begin(), it);
			result.first = true;
		}
		else
		{
			result.first = false;
			result.second = -1;
		}

		return result;
	}
}

#endif
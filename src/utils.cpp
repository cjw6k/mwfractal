#include "utils.hpp"

bool JS::findmin(const double &a, const double &b){
	if(a == -1) return false;
	if(b == -1) return true;

	if(a < b){
		return true;
	}

	return false;
}

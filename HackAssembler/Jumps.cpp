#include "Jumps.h"
#include <boost/algorithm/string.hpp>

Jumps::Jumps()
{
	jumps.insert(pair<string, string>("NULL", "000"));
	jumps.insert(pair<string, string>("JGT", "001"));
	jumps.insert(pair<string, string>("JEQ", "010"));
	jumps.insert(pair<string, string>("JGE", "011"));
	jumps.insert(pair<string, string>("JLT", "100"));
	jumps.insert(pair<string, string>("JNE", "101"));
	jumps.insert(pair<string, string>("JLE", "110"));
	jumps.insert(pair<string, string>("JMP", "111"));
}

string Jumps::find(string str)
{
	boost::to_upper(str);

	map<string, string>::iterator it;
	it = jumps.find(str);

	if (it != jumps.end())
	{
		//return value associated with str key
		return it->second;
	}
	else
	{
		return "Destination code not found: " + str;
	}
}
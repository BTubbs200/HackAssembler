#pragma once
#include <map>
#include <string>

using namespace std;

class Computations
{

private: map<string, string> computations;

public:
	Computations();
	string find(string);
};

class IllegalComps : public exception
{
private:
	string message;

public:
	IllegalComps(const string& msg)
	{
		message = msg;
	}

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};


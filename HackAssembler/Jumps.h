#pragma once
#include <map>
#include <string>

using namespace std;

class Jumps
{

private: map<string, string> jumps;

public:
	Jumps();
	string find(string);
};

class IllegalJumps : public exception
{
private:
	string message;

public:
	IllegalJumps(const string& msg)
	{
		message = msg;
	}

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};


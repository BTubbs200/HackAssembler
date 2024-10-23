#include "Destinations.h"
#include "Jumps.h"
#include "Computations.h"
#include "Bin2Hex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	string inFileName, outFileName, inStr;
	vector<string> instructions;
	map<string, int> labels;
	map<string, int>::iterator itLabels;
	int lineCount = 0;
	
	if (argc < 2) //user provided one or no file names
	{
		cout << "You must enter two parameters, input file and output file names." << endl;
	}
	else
	{
		//get file names from command line arguments
		inFileName = "C:\\mydata\\" + string(argv[1]) + ".asm";
		outFileName = "C:\\mydata\\" + string(argv[2]);
	}

	ifstream inFile(inFileName); //open input file

	//iterate through & display all items in input file
	while (getline(inFile, inStr, '\n'))
	{
		int commentLoc = inStr.find("//");

		//delete comments
		if (commentLoc != string::npos)
		{
			inStr = inStr.substr(0, commentLoc);
		}

		if (inStr.size() == 0)
		{
			continue;
		}

		//parse labels
		if (inStr.find("(") != string::npos && inStr.find(")") != string::npos)
		{
			int leftLoc = inStr.find("("); int rightLoc = inStr.find(")");
			string label = inStr.substr(leftLoc + 1, rightLoc - (leftLoc + 1));

			itLabels = labels.find(label);

			//if label doesn't already exist, add to labels map
			if (itLabels == labels.end())
			{
				labels.insert(pair<string, int>(label, lineCount));
			}
			else
			{
				cout << "Duplicate Label at Line: " + lineCount;
			}
		}
		//save label to instructions map
		else
		{
			instructions.push_back(inStr);
			lineCount++;
		}
	}
}
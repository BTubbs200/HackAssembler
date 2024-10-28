#include "Destinations.h"
#include "Jumps.h"
#include "Computations.h"
#include "Bin2Hex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	Destinations destTbl;
	Computations compTbl;
	string inFileName, outFileName, inStr;
	string comp, dest, jump, prefix;
	vector<string> instructions, binaryInstructions;
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


	//////////// PASS 1 ////////////
	/// Clean up human input into acceptable Assembly instructions ///

	//iterate through & display all items in input file
	while (getline(inFile, inStr, '\n'))
	{
		boost::erase_all(inStr, " ");
		boost::to_upper(inStr);

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

	//////////// PASS 2 ////////////
	/// Process C and A instructions into binary ///

	for (string inst : instructions)
	{
		if (inst.substr(0, 1) != "@")
		{
			// C instruction
			// D=M
			// D=M;jgt
			// D;jgt

			int eqLoc = inst.find("=");
			int scLoc = inst.find(";");

			// found = but not ;
			if (eqLoc != string::npos && scLoc == string::npos)
			{
				dest = inst.substr(0, eqLoc);
				comp = inst.substr(eqLoc + 1, inst.length() - eqLoc);
				jump = "000";
			}

			// if M, set bit 12 for mem instruction
			if (comp.find("M") != string::npos)
			{
				prefix = "1001";
			}
			else
			{
				prefix = "1000";
			}

			binaryInstructions.push_back(prefix + compTbl.find(comp) + destTbl.find(dest) + jump);
		}
		else
		{
			// A instruction
		}
	}
}
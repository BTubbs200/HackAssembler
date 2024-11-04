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
	Jumps jmpTbl;
	string inFileName, outFileName, inStr;
	string comp, dest, jump, prefix;
	vector<string> instructions, binaryInstructions;
	map<string, int> labels;
	map<string, int>::iterator itLabels;
	map<string, int> symbols;
	map<string, int>::iterator itSymbols;
	int lineCount = 0, memoryAddr = 100;
	
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
	/// Clean up human input into acceptable Assembly instruction ///

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
	/// Process C and A instruction into binary ///

	for (string inst : instructions)
	{
		// C instruction
		if (inst.substr(0, 1) != "@")
		{
			int eqLoc = inst.find("=");
			int scLoc = inst.find(";");

			// found = but not ; ('D=M' format)
			if (eqLoc != string::npos && scLoc == string::npos)
			{
				dest = inst.substr(0, eqLoc);
				comp = inst.substr(eqLoc + 1, inst.length() - eqLoc);
				jump = "null";
			}

			// found = and ; ('D=M;jgt' format)
			if (eqLoc != string::npos && scLoc != string::npos)
			{
				dest = inst.substr(0, eqLoc);
				comp = inst.substr(eqLoc + 1, scLoc - (eqLoc + 1));
				jump = inst.substr(scLoc + 1, inst.length() - (scLoc + 1));
			}

			//found ; but not = ('D;jgt' format)
			if (eqLoc == string::npos && scLoc != string::npos)
			{
				dest = "null";
				comp = inst.substr(0, scLoc);
				jump = inst.substr(scLoc + 1, inst.length() - (scLoc + 1));
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

			binaryInstructions.push_back(prefix + compTbl.find(comp) + destTbl.find(dest) + jmpTbl.find(jump));
		}
		// A instruction
		else
		{
			//@Loop (loop target)
			//@X (variable)

			string addrInst = inst.substr(1, inst.length() - 1);
			char symb[16] = { 0 };

			itLabels = labels.find(addrInst);
			itSymbols = symbols.find(addrInst);

			//if address exists in neither
			if (itLabels == labels.end() && itSymbols == symbols.end())	
			{
				symbols.insert(pair<string, int>(addrInst, memoryAddr));

				_itoa_s(memoryAddr, symb, 2); //int to bin
				string str(symb);
				string binStr = string(16 - str.length(), '0') + str;

				//add binary representation of addr to binary instructions table
				binaryInstructions.push_back(binStr);

				memoryAddr++;
			}
			//if address exists only in the lables table
			else if (itLabels != labels.end() && itSymbols == symbols.end())	
			{

			}
			//if address exists only in symbols table
			else if (itLabels == labels.end() && itSymbols != symbols.end())
			{
				int addr = itSymbols->second;

				_itoa_s(addr, symb, 2); //int to bin
				string str(symb);	
				string binStr = string(16 - str.length(), '0') + str;	

				//add binary representation of addr to binary instructions table
				binaryInstructions.push_back(binStr);
			}
			//if address exists in both tables, throw error
			else	
			{

			}
		}
	}

	//test
	for (string inst : binaryInstructions)
	{
		cout << inst << endl;
	}
}
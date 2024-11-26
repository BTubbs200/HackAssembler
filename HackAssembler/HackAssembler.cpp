#include "Destinations.h"
#include "Jumps.h"
#include "Computations.h"
#include "Bin2Hex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <boost/algorithm/string.hpp>

using namespace std;

string convertToBinary(int value);
vector<string> instructions, binaryInstructions;
map<string, int> labels;
map<string, int>::iterator itLabels;
map<string, int> symbols;
map<string, int>::iterator itSymbols;

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();

	Destinations destTbl;
	Computations compTbl;
	Bin2Hex b2hTbl;
	Jumps jmpTbl;
	string inFileName, outFileName, inStr;
	string comp, dest, jump, prefix;
	int lineCount = 0, memoryAddr = 100;

	if (argc < 2) //user provided one or no file names
	{
		std::cout << "You must enter two parameters, input file and output file names." << endl;
	}
	else
	{
		//get file names from command line arguments
		inFileName = "C:\\mydata\\" + string(argv[1]) + ".asm";
		outFileName = "C:\\mydata\\" + string(argv[2]);
	}

	ifstream inFile(inFileName); //open input file
	ofstream outFile(outFileName);	//open output file

	if (!inFile)
	{
		cerr << "Error: Input file '" << inFileName << "' not found.";
		return 1;
	}

	if (!outFile)
	{
		cerr << "Error: Output file '" << outFileName << "' not found.";
		return 1;
	}

#pragma region Pass1
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
				std::cout << "Duplicate Label at Line: " + lineCount;
			}
		}
		//save label to instructions map
		else
		{
			instructions.push_back(inStr);
			lineCount++;
		}
	}

#pragma endregion Pass1

#pragma region Pass2
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

			try
			{
				binaryInstructions.push_back(prefix + compTbl.find(comp) + destTbl.find(dest) + jmpTbl.find(jump));
			}
			///FIXME///
			catch (const exception& e)
			{
				std::cerr << e.what() << endl;
			}
		}
		// A instruction
		else
		{
			//@Loop (loop target)
			//@X (variable)
			//@2 (address)

			string addrInst = inst.substr(1, inst.length() - 1);

			//if a number instead of variable name
			if (isdigit(addrInst.at(0)))
			{
				int value = stoi(addrInst);	//convert to int

				binaryInstructions.push_back(convertToBinary(value));
			}
			else
			{
				itLabels = labels.find(addrInst);
				itSymbols = symbols.find(addrInst);

				//if address exists in neither
				if (itLabels == labels.end() && itSymbols == symbols.end())
				{
					//save address of variable to symbols table
					symbols.insert(pair<string, int>(addrInst, memoryAddr));

					binaryInstructions.push_back(convertToBinary(memoryAddr));

					//increment memory addr for next variable
					memoryAddr++;
				}
				//if address exists only in the labels table
				else if (itLabels != labels.end() && itSymbols == symbols.end())
				{
					//get linNum of label from labels table
					int lineNum = itLabels->second;

					binaryInstructions.push_back(convertToBinary(lineNum));
				}
				//if address exists only in symbols table
				else if (itLabels == labels.end() && itSymbols != symbols.end())
				{
					//get mem addr from symbols table
					int addr = itSymbols->second;

					binaryInstructions.push_back(convertToBinary(addr));
				}
				//if address exists in both tables, throw error
				else
				{

				}
			}
		}
	}

#pragma endregion Pass2

#pragma region Pass3
//Generate output file

	outFile << "v2.0 raw" << endl;

	for (string inst : binaryInstructions)
	{
		//convert to hex and write to outFile
		outFile << b2hTbl.Convert16Bin2Hex(inst) << endl;
	}

	outFile.close();

	//generate debug list file
	ofstream lstFile(outFileName + ".lst");
	lstFile << "List File for: " << inFileName << endl;

	lstFile << endl << endl << "**** Code ****" << endl;
	for (int i = 0; i < binaryInstructions.size(); i++)
	{
		lstFile << binaryInstructions[i]
			+ "\t" + b2hTbl.Convert16Bin2Hex(binaryInstructions[i])
			+ "\t\t" + instructions[i] << endl;
	}

	lstFile << endl << endl << "**** Symbols ****" << endl;
	if (symbols.size() != 0)
	{
		for (pair<string, int> symbol : symbols)
		{
			lstFile << symbol.second << "\t\t" << symbol.first << endl;
		}
	}
	else
	{
		lstFile << "No Symbols Found" << endl;
	}

	lstFile << endl << endl << "**** Labels ****" << endl;
	if (labels.size() != 0)
	{
		for (pair<string, int> label : labels)
		{
			lstFile << label.second << "\t\t" << label.first << endl;
		}
	}
	else
	{
		lstFile << "No Labels Found" << endl;
	}

	lstFile.close();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	std::cout << "\nAssembly Complete\n\t" << "Look for files: " << "\n\t"
		<< outFileName << "\n\t" << outFileName << ".lst\n" << endl;
	std::cout << "Elapsed Time: " << elapsed.count() << endl;

#pragma endregion Pass3

}

string convertToBinary(int value)
{
	char symb[16] = { 0 };
	_itoa_s(value, symb, 2); //int to bin
	string str(symb);
	string binStr = string(16 - str.length(), '0') + str;

	return binStr;
}
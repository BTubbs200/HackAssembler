// HackAssembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Destinations.h";
#include "Jumps.h";
#include "Computations.h";
#include "Bin2Hex.h";
#include <iostream>
#include <fstream>;
#include <string>;
#include <map>;

using namespace std;

int main(int argc, char* argv[])
{
	/*
	Destinations dstTbl;
	Computations compTbl;
	Jumps jmpTbl;
	Bin2Hex bin2HexTbl;

	cout << dstTbl.find("MD") << endl;
	cout << compTbl.find("0") << endl;
	cout << jmpTbl.find("JGE") << endl;
	cout << bin2HexTbl.find("1010") << endl;
	cout << bin2HexTbl.Convert16Bin2Hex("1010111111000101");
	*/

	string inFileName, outFileName, inStr;
	
	if (argc < 2)
	{
		cout << "You must enter two parameters, input file and output file names." << endl;
	}
	else
	{
		//find user-specified .asm file in C:\\mydata
		inFileName = "C:\\mydata\\" + string(argv[1]) + ".asm";

		//create user-specified name for save file in C:\\mydata
		outFileName = "C:\\mydata\\" + string(argv[2]);
	}

	ifstream inFile(inFileName);

	//iterate through all items in file
	while (getline(inFile, inStr, '\r'))
	{
		cout << inStr << endl;
	}
}
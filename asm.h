#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <bits/stdc++.h> 
#include <stdio.h>
using namespace std;

class instruction {
    public:
        string name;
        int code;
        int argType;
};

class section {
    public:
        string name;
        int position;
        vector<int> header;
    
};


class assembler {
    
    public:
        int exportToBin (vector<int> assembledCode, string path);
        int createInstruction(string name, int code, int argType);
        int createSection(string name, vector<int> header);
        bool checkIfInstructionExists ( string name );
        void init();
        string assemblyToString(vector<int> code);
        vector<int> assemble (string code);
    private:
        int getInstructionCode ( string instructionName ); 
        int getInstructionArgType (string instruction);
        int getAddress ( string variable );
        string getSectionName (string line);
        string removeSpaces( string line );
        section getSection ( string name );
        vector<int> getSectionHeader ( string name );
        vector<int> getDataArgs (string line, string instruction);
        vector<int> generatePointer (int address);
        vector<int> instructionTo (int address);
        vector<int> convertInstruction ( string line );
        vector<string> getInstructionData (string rawLine);
        
        
        
    
};


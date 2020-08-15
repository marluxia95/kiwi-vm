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
class cpu {
    public:  
        int running;
        int load ( vector<int> code) ;
        int run (int address);
        int acc = 0x0010;
        int memory[UINT16_MAX];
        int registers = 0x0011; 
        int codePos = 0x0100;
        int videoMem = 0xF000;
        void init ();
        void memoryDump (int bufferSize);
        
    private:
        int getAddress( int hex1, int hex2 );
};

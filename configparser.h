#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>



using namespace std;

class confvalue {
    public:
        string name;
        string value;
        
};

class configParser {
    public:
        void init ();
        
    private:
        int TotalValues;
        void readConfig(string path);
        void createValue ( string name);
        bool updateValue ( string name, string value );
        confvalue values[0xFF];
        vector<string> configData;
        
        
        
};

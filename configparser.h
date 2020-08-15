#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#define CONFIG_PATH "/etc/kiwi/vm.conf"


using namespace std;

class config {
    public:
        void init ();
        const char* rom_path;
        const char* loader_path;
        int ramAmmount;
        
    private:
        vector<string> readFile(string path);
        vector<string> configData;
};

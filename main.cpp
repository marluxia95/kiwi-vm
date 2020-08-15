#include <iostream>
#include <fstream>
#include <stdio.h>
#include <asm.h>
#include <cpu.h>
#include <configparser.h>


#define VER 0.2

#define MAIN_CONFIG_PATH "vm.conf"

bool debug = false;


typedef unsigned char BYTE;


template< typename T >
string int_to_hex_1byte( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)/2) 
         << std::hex << i;
  return stream.str();
}

void showHelp (string cmd) {
    cout << " Kiwi VM v" << VER << endl;
    cout << " Usage : ./kiwi_vm " << endl;
    cout << " -r [FILE] : Runs a binary file ( MUST BE BINARY ) " << endl;
    cout << " -c [FILE] [OUT] : Assembles a file and exports it into a binary file" << endl;
    cout << " --debug : Enables debug mode " << endl;
}
/*
vector<int> loadBin ( string path ){
    vector<int> code;

    ifstream in(path, ios::out | ios::binary);
    if( in.fail() ) throw 0;
    vector<char> const charCode(
        (istreambuf_iterator<char>(in)),
        (istreambuf_iterator<char>())
        );
    cout << charCode.size() << " bytes." << endl;
    code.resize(charCode.size());
    int converted;
    for(std::vector<char>::size_type i = 0; i != charCode.size(); i++) {  
        cout << int_to_hex_1byte(code[i]) << endl;
    }
    return code;
}
*/
std::vector<BYTE> loadBin(string path){
    // open the file:
    std::ifstream file(path, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<BYTE> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<BYTE>(file),
               std::istream_iterator<BYTE>());
    
    return vec;
}
void assembleToBin( string code, string path ) {
    assembler kasm;
    kasm.init();
    kasm.debug = debug;
    vector<int> hexCode = kasm.assemble( code );
    kasm.exportToBin(hexCode, path);
    
}

void run (cpu machine, vector<int> bin){
    cout << "Loading virtual machine ... " << endl;
    machine.init();
    cout << "Uploading code to ram .. " << endl;
    machine.load(bin);
        
    cout << "Running .. " << endl;
    machine.run(0x0100); // default code position
        
    if( machine.debug == true ) {
        cout << "Memory dump : " << endl;
        machine.memoryDump(32); // 32 bytes per line
    }
    
}

string readFile (string path) {
    string code;
    ifstream codeFile (path);
    string line;
    if (codeFile.is_open())
    {
        while ( getline (codeFile,line) )
        {
            if(debug){cout << line << endl;}
            code.append(line);
            code.append("\n");
        }
        codeFile.close();
    }
    else{cout << "Couldnt Open File !" << endl; return code;}
    
    return code;
    
}

int main (int argc, char* argv[]) {
    configParser config;
    config.init();
    
    string runFile;
    string compileIn;
    string compileOut;
    
    cpu virtualMachine;


    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--debug") {
            virtualMachine.debug = true;
            debug = true;
        } 
    }
    
    if (std::string(argv[1]) == "-r" && argc>=3) {
        
        runFile = argv[2];
        vector<BYTE> rawCode = loadBin( runFile );
        vector<int> machineCode;
        for ( int x = 0; x != rawCode.size(); x++ ) {
            int hexCode = (int)rawCode[x];
            machineCode.push_back(hexCode);
        }
        
        
        run(virtualMachine, machineCode);
        
        
    } 

    if (std::string(argv[1]) == "--loopexample") {
        
        //vector<int> code = { 0xFF, 0x1B, 0x01, 0x01, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0x81, 0x06 , 0x01,0x6, 0xFF  };
        vector<int> code = { 0xFF, 0x1B, 0x01, 0x01, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0xff };
        
        run(virtualMachine, code);
        
        
    } 
    
    if (std::string(argv[1]) == "-c") {
        compileIn = argv[2];
        compileOut = argv[3];
        
        string code = readFile(compileIn);
        
        assembleToBin( code, compileOut );
        
        
    } 
    
    return 1;


}




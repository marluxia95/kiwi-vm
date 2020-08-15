#include <iostream>
#include <fstream>
#include <stdio.h>
#include <asm.h>
#include <cpu.h>

#define VER 0.1

// 0xFF, 0x1B, 0x02, 0x00, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0xFF ( 2 + 2 ) 
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
    cout << " Usage : " << cmd << " " << endl;
    cout << " -r [FILE] : Runs a binary file ( MUST BE BINARY ) " << endl;
    cout << " -c [FILE] [OUT] : Assembles a file and exports it into a binary file" << endl;
    cout << " --dump : Shows a memory dump at end of code " << endl;
}

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
void assembleToBin( string code, string path ) {
    assembler kasm;
    kasm.init();
    cout << "Assembling ..." << endl;
    vector<int> hexCode = kasm.assemble( code );
    cout << "Exporting ...." << endl;
    kasm.exportToBin(hexCode, path);
    
}


string readFile (string path) {
    string code;
    ifstream codeFile (path);
    string line;
    if (codeFile.is_open())
    {
        while ( getline (codeFile,line) )
        {
            code.append(line);
            code.append("\n");
            cout << line << endl;
        }
        codeFile.close();
    }
    else{cout << "Couldnt Open File !" << endl; return code;}
    
    return code;
    
}

int main (int argc, char* argv[]) {
    
    int dumpMode = 0;
    string runFile;
    string compileIn;
    string compileOut;
    
    cout << "EE" << endl;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--dump") {
            dumpMode = 1;
        } 
    }
    
    if (std::string(argv[1]) == "-r" && argc>=3) {
        
        runFile = argv[2];
        vector<int> machineCode = loadBin( runFile );
        
        cout << "Loading virtual machine ... " << endl;
        cpu virtualMachine;
        virtualMachine.init();
        cout << "Uploading code to ram .. " << endl;
        virtualMachine.load(machineCode);
        
        cout << "Running .. " << endl;
        virtualMachine.run(0x0100);
        
        if( dumpMode ) {
            cout << "Memory dump : " << endl;
            virtualMachine.memoryDump(32);
        }
        
        
    } 

    if (std::string(argv[1]) == "--loopexample") {
        
        runFile = argv[2];
        vector<int> code = { 0xFF, 0x1B, 0x01, 0x01, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0x81, 0x06 , 0x01,0x6, 0xFF  };

        
        cout << "Loading virtual machine ... " << endl;
        cpu virtualMachine;
        virtualMachine.init();
        cout << "Uploading code to ram .. " << endl;
        virtualMachine.load(code);
        
        cout << "Running .. " << endl;
        virtualMachine.run(0x0100);
        
        if( dumpMode ) {
            cout << "Memory dump : " << endl;
            virtualMachine.memoryDump(32);
        }
        
        
    } 
    
    if (std::string(argv[1]) == "-c") {
        cout << "Compile Mode selected" << endl;
        compileIn = argv[2];
        compileOut = argv[3];
        
        string code = readFile(compileIn);
        
        assembleToBin( code, compileOut );
        
        
    } 
    
    return 1;


}




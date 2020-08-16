#include "SDL.h"
#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <bits/stdc++.h> 
#include <stdio.h>

#include <cpu.h>
#include <asm.h>
#include <configparser.h>

#define VER 0.2
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

using namespace std;

int running;
SDL_Window *window;
SDL_Renderer *renderer;

#define MAIN_CONFIG_PATH "vm.conf"
typedef unsigned char BYTE;
 
bool debug = false;

template< typename T >
string int_to_hex_1byte( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)/2) 
         << std::hex << i;
  return stream.str();
}

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

void showHelp (string cmd) {
    cout << " Kiwi VM v" << VER << endl;
    cout << " Usage : ./kiwi_vm " << endl;
    cout << " -r [FILE] : Runs a binary file ( MUST BE BINARY ) " << endl;
    cout << " -c [FILE] [OUT] : Assembles a file and exports it into a binary file" << endl;
    cout << " --debug : Enables debug mode " << endl;
}

void run (cpu machine, vector<int> bin) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) >= 0) {
        window = SDL_CreateWindow("Kiwi_VM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        if (window) {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            if (renderer) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0 , 255);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0 , 255);
                int i;
                for (i = 0; i < WINDOW_WIDTH; ++i)
                    SDL_RenderDrawPoint(renderer, i, i);
                cout << "Loading virtual machine ... " << endl;
                machine.init();
                cout << "Uploading code to ram .. " << endl;
                machine.load(bin);
                    
                cout << "Running .. " << endl;
                machine.run(0x0100); // default code position
                running = 1;
                if( machine.debug == true ) {
                    cout << "Memory dump : " << endl;
                    machine.memoryDump(32); // 32 bytes per line
                }
                while (running) {
                    SDL_RenderDrawPoint(renderer, i, i);
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                            case SDL_QUIT: {
                                running = 0;
                            } break;
                        }
                    }
                    
                    SDL_RenderPresent(renderer);
                    
                }
                
            }
        }
    }
}

int main (int argc, char* argv[]) {
    configParser config;
    config.init();
    
    string runFile;
    string compileIn;
    string compileOut;
    
    cpu machine;


    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--debug") {
            machine.debug = true;
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
        
        run(machine, machineCode);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_SUCCESS;
        
    } 

    if (std::string(argv[1]) == "--loopexample") {
        
        //vector<int> code = { 0xFF, 0x1B, 0x01, 0x01, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0x81, 0x06 , 0x01,0x6, 0xFF  };
        vector<int> code = { 0xFF, 0x1B, 0x01, 0x01, 0x40, 0x02, 0x01, 0x41, 0x02, 0x20, 0x01, 0xff };
        
        run(machine, code);
        
        
    } 
    
    if (std::string(argv[1]) == "-c") {
        compileIn = argv[2];
        compileOut = argv[3];
        
        string code = readFile(compileIn);
        
        assembleToBin( code, compileOut );
        
        
    } 
    
    return 1;


}




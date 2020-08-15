#include <configparser.h>



void configParser::readConfig(string path){
    ifstream cFile (path);
    
    if (cFile.is_open())
    {
        string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                                 line.end());
            if(line[0] == '#' || line.empty()) // ignore comments
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            updateValue ( name , value );
        }
        
    }
    else {
        cerr << "Couldn't open config file for reading.\n";
    }
    
}


void configParser::createValue(string name) {
    TotalValues++;
    values[TotalValues].name = name;
    
}

bool configParser::updateValue ( string name, string value ) {
    for ( int x = 0; x != TotalValues; x++ ) {
        if ( name == values[x].name ) {
            values[x].value = value;
            return true;
        }
        
    } 
    
    return false;
    
}


void configParser::init() { 
    
    createValue("SysRomPath");
    

    
    
    
}

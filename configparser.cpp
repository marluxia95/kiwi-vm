#include <configparser.h>



vector<string> config::readFile(string path){
    vector<string> data;
    ifstream configFile (path);
    string line;
    if (configFile.is_open())
    {
        while ( getline (configFile,line) )
        {
            data.push_back(line);
            cout << line << endl;
        }
        configFile.close();
    }
    else{cerr << "Error : Couldnt Open Config File ! ";}
    
    return data;
    
}

void config::init() { 
    readFile(CONFIG_PATH);
    
    
}

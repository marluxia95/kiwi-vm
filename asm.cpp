#include <asm.h>

/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */


using namespace std;




string currentSection;

int acc = 0x0010;
int registers = 0x0011; 
int codePos = 0x0100;
int videoMem = 0xF000;
int writtenInstructions = 0;

int writtenSections = 0;

instruction instructions[0xFF];
section sections[2];

vector<string> splitter(string in_pattern, string& content){
    vector<string> split_content;

    regex pattern(in_pattern);
    copy( sregex_token_iterator(content.begin(), content.end(), pattern, -1),
    sregex_token_iterator(),back_inserter(split_content));  
    return split_content;
}

vector<string> splitSpaces(string instruction) 
{ 

   	vector<string> arr;
	
	istringstream ss(instruction);
	do { 
		string word; 
		ss >> word;
		arr.push_back(word);
	} while (ss); 	


   	return arr;
} 

template< typename T >
string int_to_hex( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)) 
         << std::hex << i;
  return stream.str();
}

instruction assembler::createInstruction(string name, int code, vector<int> arguments) {
    writtenInstructions++;
    instructions[writtenInstructions].name = name;
    instructions[writtenInstructions].code = code;
    instructions[writtenInstructions].arguments = arguments;
    return instructions[writtenInstructions];
}

int assembler::createSection(string name, vector<int> header) {
    writtenSections++;
    sections[writtenSections].name = name;
    sections[writtenSections].header = header;
    return writtenSections;
}

void assembler::init(){
    /*
     *      Instruction declaration
     * 
     *      createInstuction( Name, Code, { } -> Arguments are :
     *                                              1 for integer
     *                                              2 for register
     *                                              3 for pointer
     *                                              4 for bytearray
     */
     
    createInstruction("ADD", 0x20, {1});
    createInstruction("SUB", 0x21, {1});
    createInstruction("MUL", 0x22, {1});
    createInstruction("DIV", 0x23, {1});
    
    createInstruction("MOV", 0x40, {1,2});/*
    createInstruction("MOV", 0x41, {1,3});
    createInstruction("MOV", 0x42, {2,2});
    createInstruction("MOV", 0x43, {2,3});
    createInstruction("MOV", 0x44, {3,3});
    createInstruction("MOV", 0x45, {3,2});*/
    createInstruction("ACC", 0x46, {1});
    
    createInstruction("JMP", 0x80, {2});
    createInstruction("JNE", 0x81, {1,2});
    createInstruction("JEQ", 0x82, {1,2});

    
    createInstruction("HLT", 0xFF, {});
    
    createInstruction("NULL", 0x00, {});

    
    /*
     *      Section Declaration
     */
    
    
    sections[0].name = "code";
    sections[0].header = { 0xFF, 0x1B };

    sections[1].name = "data";
    sections[1].header = { 0xFF, 0x1C };
    
}

bool assembler::checkIfInstructionExists ( string instructionName ) {
    
    for ( int x = 0; x != writtenInstructions; x++ ) {

        if ( instructionName == instructions[x].name ) {
            
            return true;
            
            
        }
        
    } 
    
    return false;
}

int assembler::getInstructionCode ( string instructionName ) {
    
    for ( int x = 1; x != writtenInstructions; x++ ) {
        if ( instructionName == instructions[x].name ) {
            cout << "CORRECT " << instructions[x].code << endl;
            return instructions[x].code;
            
        }
        
    } 
    
    return 0x00;
    
}

int assembler::getInstructionArgType ( string instructionName, int argument ) {
    
    for ( int x = 1; x <= writtenInstructions; x++ ) {
        if ( instructionName == instructions[x].name ) {

            return instructions[x].arguments[argument];
            
        }
        
    } 
    
    return 0x00;
    
}

int assembler::getInstructionArgCount ( string instructionName ) {
    
    for ( int x = 1; x <= writtenInstructions; x++ ) {
        
        if ( instructionName == instructions[x].name ) {
            cout << "CORRECT " << instructions[x].arguments.size() << endl;
            return instructions[x].arguments.size();
            
        }
        
    } 
    
    return 0;
    
}

section assembler::getSection( string name ) {
    for ( int x = 0; x != writtenSections; x++ ) {
        if ( name == sections[x].name ) {

            return sections[x];
            
        }
        
    } 
    
    return sections[0];

    
}

vector<int> assembler::getSectionHeader( string name ) {
    for ( int x = 0; x != writtenSections; x++ ) {
        if ( name == sections[x].name ) {

            return sections[x].header;
            
        }
        
    } 
    
    return sections[0].header;

    
}

string assembler::removeSpaces ( string line ) {
    int size = line.length();
    int removedSpaces = 0;
    string lineOut;
    
    for (std::vector<int>::size_type x = 0; x != size; x++) {
        
		if( line.at(x) != ' ' && removedSpaces == 0 ) {
            removedSpaces = 1;
		}
		if( removedSpaces == 1 ) {
            lineOut.push_back(line.at(x));
        }
		
	}
	return lineOut;
    
}

vector<string> assembler::getInstructionData ( string rawLine ) {
    
	
    vector<string> result = splitSpaces(rawLine);
    
	return result;
    
}

vector<int> assembler::getDataArgs ( string line, string instruction ) {

    vector<string> rawArgs = splitter(R"( )", line);

    vector<int> args;
    
    
    int argumentCount = getInstructionArgCount(instruction);
    if(debug){cout << "Count " << argumentCount << endl;}
    for(std::vector<string>::size_type i = 1; i != argumentCount+1; i++) {  
            if(rawArgs[i]==";"){break;} // ignore comments
            int argumentType = getInstructionArgType(instruction, i-1);
            
            switch(argumentType){
                case 1:{ // Integer 
                    int arg;
                    
                    istringstream(rawArgs[i]) >> arg;
                    
                    args.push_back(arg);
                    break;
                }
                case 2:{ // Memory Address, pointer
                    int rawAddress;
                    vector<int> hexPointer;
                    istringstream(rawArgs[i]) >> rawAddress;
                    
                    hexPointer = generatePointer(rawAddress);
                    
                    args.push_back(hexPointer[0]);
                    args.push_back(hexPointer[1]);
                    break;
                }
                default:{
                    break;
                }
                
            }
    }
    
    return args;
}

int assembler::getAddress ( string variable ) {
    int size = variable.length(); 
    unsigned int rawAddress;
    cout << size << endl;
	string name; 
	for (int y = 1; y != size; y++) {
        name.push_back(variable.at(y));
		
	}
	cout << name << endl;
    std::stringstream hex;
    hex << std::hex << name;
    hex >> rawAddress;
    
    int address = (int) rawAddress;
    
	return address;

    
}

vector<int> assembler::convertInstruction ( string line ) {
    vector<int> converted;
    
    string lineNoSpaces = removeSpaces(line);
    
    vector<string> data = getInstructionData ( lineNoSpaces );
    string instruction = data[0];
    
    vector<int> args = getDataArgs(lineNoSpaces,instruction);

    if( currentSection == "code" ) {
        bool isInstruction = checkIfInstructionExists(instruction);
        
        
        
        if( isInstruction == true) { 
            
            int instructionCode = getInstructionCode(data[0]);

            converted.push_back(instructionCode);
            cout << "code: " << instructionCode << endl;
            // Convert Arguments into hexcode
            for(std::vector<int>::size_type i = 0; i != args.size(); i++) {  
                if(data[i] == ";"){break;}
                if(debug){cout << "Argument : " << data[i] << endl;}
                converted.push_back(args[i]);

            }
        }else{
            if(data[0].at(0) == '.'){
                /*
                string name = getSectionName(line);
                
                currentSection = sections[1];
                
                currentSection.position = getAddress(data[1]);
                
                vector<int> header = currentSection.header;
                
                vector<int> memoryPosition = generatePointer(currentSection.position );
                for(std::vector<int>::size_type i = 0; i != header.size(); i++) {  
                    converted.push_back(header[i]);
                    
                }
                for(std::vector<int>::size_type i = 0; i != memoryPosition.size(); i++) {  
                    converted.push_back(memoryPosition[i]);
                    
                }*/
            }else if(data[0].at(0) == 'x'){
                int address = getAddress(data[0]);
                
                if(debug){cout << "Variable Address : " << address << endl;}
                
            }
            
        }
        
    
    
        
    }else{
        if(data[0].at(0) == '.'){
            string name = getSectionName(line);
            
            if( name == "code" ) {
                currentSection = name;
                
                int address;
                
                if(data[1] != "" ) {
                    int address = getAddress(data[1]);
                }else{
                    int address = codePos;
                }
                vector<int> memoryPosition = generatePointer(address);
                
                
                converted.push_back(0xFF);
                converted.push_back(0x1B);
                
                for(std::vector<int>::size_type i = 0; i != memoryPosition.size(); i++) {  
                    converted.push_back(memoryPosition[i]);
                        
                }
                
            }
        }

            
    }
    
    return converted;
    
}

string assembler::assemblyToString ( vector<int> code ) {
    for(std::vector<int>::size_type i = 0; i != code.size(); i++) { 
        cout << i << " | " << code[i] << endl;
        
    }
    
    
}

vector<int> assembler::assemble ( string code ) {
    vector<int> assembled;
    vector<string> instructions = splitter(R"(\n)", code);
    if(debug){cout << instructions.size() << endl;}
    for(std::vector<int>::size_type i = 0; i != instructions.size(); i++) { 
        if(debug){cout << "Instruction " << instructions[i] << endl;}
        vector<int> assembledInstruction = convertInstruction ( instructions[i] );
        for(std::vector<int>::size_type d = 0; d != assembledInstruction.size(); d++) { 
            //if(debug){cout << "Argument " << assembledInstruction[d] << endl;}
            assembled.push_back(assembledInstruction[d]);
        }
        
    }
    return assembled;
}

vector<int> assembler::generatePointer (int address) {
	vector<int> result;
    string hexAddress = int_to_hex(codePos + address);
 	
    unsigned int hex1; 
    unsigned int hex2;  
    std::stringstream ss1;
    std::stringstream ss2;
    ss1 << std::hex << hexAddress.at(0) << hexAddress.at(1);
    ss1 >> hex1;
    ss2 << std::hex << hexAddress.at(2) << hexAddress.at(3);
    ss2 >> hex2;
    
    result.push_back(hex1);
    result.push_back(hex2);
    if(debug){cout << "Hexadecimal number for address : " << hexAddress << " " << " " << endl;}
    return result;
}

string getPointerValue ( string argument ) {
    string pointerVal;
    if(argument.at(0)!="$"){return "";}
    for(x=1, 4 ) {
        pointerVal.push_back(argument.at(x));
    }
    return pointerVal;
}

int assembler::exportToBin (vector<int> assembledCode, string path) {
    ofstream bin(path, ios::out | ios::binary);
    if(!bin) {
        cerr << "Cannot open file!";
        return 0;
    }
    for(std::vector<int>::size_type i = 0; i < assembledCode.size(); i++) {
        bin.write ((char*)&assembledCode[i],1);
        if(debug){cout << "Write : " << assembledCode[i] << endl;}
    }
    bin.close();
    if(!bin.good()) {
      cerr << "Error occurred at writing to binary file !";
      return 0;
    }
    cout << "Successfully exported to binary file " << endl;
    return 1;
}


string assembler::getSectionName ( string line ) {
	int size = line.length(); 
    cout << size << endl;
	string name; 
	for (int y = 1; y != size; y++) {
        if(line.at(y) == ' ') { break; }
        name.push_back(line.at(y));
		
	}
	cout << name << endl;
	return name;

}




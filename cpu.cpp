#include <asm.h>
#include <cpu.h>

template< typename T >
string int_to_hex( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)) 
         << std::hex << i;
  return stream.str();
}
template< typename T >
string int_to_hex_1byte( T i )
{
  stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)/2) 
         << std::hex << i;
  return stream.str();
}

int getPointerAddress ( vector<int> pointer ) {
	string hexPart1 = int_to_hex_1byte(pointer[0]);
	string hexPart2 = int_to_hex_1byte(pointer[1]);
	string hexAddress;
	int address;
	
	hexAddress = hexPart1 + hexPart2;
	address = stoi(hexAddress,nullptr, 16);
	//cout << "Pointer address : " << hexAddress << "Result : " << address << endl;
	return address;
}

/*
 * FF 1B 01 00 -> Code Header
 * 
 */



int cpu::getAddress( int hex1, int hex2 ) {
    string hexStr1 = int_to_hex_1byte(hex1);
    string hexStr2 = int_to_hex_1byte(hex2);
    
    string hexAddress = hexStr1 + hexStr2;
    int address;
    
    std::stringstream hexCode;
    hexCode << std::hex << hexAddress;
    hexCode >> address;
    
    return address;
}

void cpu::init(){
    //cout << "Reserving machine memory . . . " << endl;
    //memory.reserve(0xFFFF);
}

int cpu::load ( vector<int> code ) { // load assembly code to RAM 
    vector<int> dataSector; 
    vector<int> codeSector; 
    int size = code.size();
    int currentSector = 0; // 1 for code 2 for data
    int codeAddress;
    int dataAddress;
    for ( std::vector<int>::size_type x = 0; x != size; x++) {
        if ( code[x] > 0xFF ) { cerr << "Error ! Hexcode exceeded limit ( 0xFF )"; return 0; }
        if ( code[x] == 0xFF && code[x+1] == 0x1B ) {
            currentSector = 1;
            codeAddress = getAddress( code[x+2], code[x+3] );
            if(debug){cout << "Got code address : " << codeAddress << endl;}
            x += 3;
        }else if ( code[x] == 0xFF && code[x+1] == 0x1A ) {
            currentSector = 2;
            dataAddress = getAddress( code[x+2], code[x+3] );
            x += 3;
        }
        
        if( currentSector == 1 ) {
            
            memory[codeAddress + x - 3] = code[x];
            if(debug){cout << "Copying code" << int_to_hex_1byte(code[x]) << " to " << codeAddress + x << endl;}
        }else if( currentSector == 2 ) {
            memory[dataAddress + x] = code[x];
            if(debug){cout << "Copying data to " << dataAddress + x << endl;}
            
        }
    }
    
    
    
}

void cpu::memoryDump (int bufferSize) {
    cout << "Generating memory dump " << endl;
    for ( int x = 0; x < 0xFFF; x+=bufferSize ) {
        string addr = int_to_hex(x);
        
        cout << "0x" << addr << " ";
        for ( int i = 0; i < bufferSize; i++ ) {
            string dump;
            if(memory[x+i] > 0xFF){dump = int_to_hex_1byte(0);}else{dump = int_to_hex_1byte(memory[x+i]);}
            
            cout << dump <<  " "; 
            
        }
        cout << endl;
        
    }
    
}
int cpu::run (int address) {
    int instruction = 0;
    cout << "Running " << address << endl;
    int currentAddress;
    cpu::running = 1;
    while(address < 0x01FF){
        //currentAddress = instruction+address;
        //cout << instruction << "( " << currentAddress << " ) " << " -> ";
        if(debug){cout << address <<  " -> ";}
        switch( memory[address] ) {
            case 0x20:{ // add
                int number = memory[address+1];
                memory[cpu::acc] = memory[cpu::acc] + memory[cpu::registers+number];
                if(debug){cout << "ADD " << memory[cpu::registers+number] << " result " << memory[cpu::acc];}
            }break;
            case 0x21:{ // sub
                int number = memory[address+1];
                memory[cpu::acc] = memory[cpu::acc] - memory[cpu::registers+number];
                if(debug){cout << "SUB " << memory[cpu::registers+number] << " to " << memory[cpu::acc];}
            }break;
            case 0x22:{ // mul
                int number = memory[address+1];
                memory[cpu::acc] = memory[cpu::acc] * memory[cpu::registers+number];
                if(debug){cout << "MUL " << memory[cpu::registers+number] << " to " << memory[cpu::acc];}
            }break;
            case 0x23:{ // div
                int number = memory[address+1];
                memory[cpu::acc] = memory[cpu::acc] / memory[cpu::registers+number];
                if(debug){cout << "DIV " << memory[cpu::registers+number] << " to " << memory[cpu::acc];}
            }break;
            case 0x40:{ // mov
                int number = memory[address+1];
                int target = memory[address+2];
                if(debug){cout << "MOV " << number << " to " << target;}
                memory[cpu::registers+target] = number;
                //instruction+=2;
                address+=2;
            }break;
            case 0x41:{ // acc
                int number = memory[address+1];
                memory[cpu::acc] = number;
                if(debug){cout << "ACC " << number;}
                address++;
                
            }break;
            case 0x80:{ // jmp
                vector<int> pointer = { memory[address+1], memory[address+2] };
                int target = getPointerAddress(pointer);
                
                if(debug){cout << "JMP " << target;}
                //instruction = target;
                address = target;
            }break;
            case 0x81:{ // jne
                int registerNumber = memory[address+1];
                int value = memory[cpu::registers+registerNumber];
                vector <int> pointer = { memory[address+2], memory[address+3] };
                //cout << pointer[0] << " " << pointer[1];
                int target = getPointerAddress(pointer);
                if(debug){cout << "JNE " << value << " " << target;}
                if(memory[cpu::acc]!=value){
                    if(debug){cout << endl << "Not equal" << endl;}
                    //instruction = target;
                    address = target;
                }else{
                    //instruction+=2;
                    address +=2;
                    
                }
            }break;
            case 0xFF:{ // hlt
                return 1;
            }break;
            case 0x00:{ // null
            }break;
            
        }
        if(debug){cout << endl;}
        address++;
        
    }
    
}

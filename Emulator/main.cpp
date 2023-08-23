#include <iostream>
#include <fstream>

uint8_t RAM[64];
uint8_t pc = 0;
uint8_t ROM[192];
uint8_t ROMInstr[192];
uint8_t acc = 0;
bool carry, shouldClose;

uint8_t add(uint8_t in, bool carryIn) {
    uint8_t mask[8] = {0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};
    uint8_t out = 0;
    uint8_t carryInternal = carryIn != 0 ? 0b11111111 : 0;
    for(unsigned char i : mask) {
        uint8_t accMasked = acc & i;
        uint8_t inMasked = in & i;
        out |= (accMasked ^ inMasked) ^ carryInternal & i;
        carryInternal = accMasked & inMasked | carryInternal & i & accMasked & inMasked != 0 ? 0b11111111 : 0;
    }
    carry = carryInternal;
    return out;
}

uint8_t resolve(uint8_t address);
void halt();
int execute(uint8_t opcode, uint8_t data);

int main() {
    std::ifstream executable;
    executable.open("out.bin", std::ios::in | std::ios::binary);
    int i = 0;
    while(!executable.eof()) {
        char a,b;
        executable.read(reinterpret_cast<char *>(&a), 1);
        executable.read(reinterpret_cast<char *>(&b), 1);
        ROMInstr[i] = a;
        ROM[i] = b;
        i++;
    }
    while(!shouldClose && pc < 192) {
        if(execute(ROMInstr[pc],ROM[pc])) {
            return -1;
        }
    }
    return 0;
}

int execute(uint8_t opcode, uint8_t data) {
    switch (opcode) {
        default:
            return -1;
        case 0x0: //jmp
            pc = data - 1;
            break;
        case 0x2: //jiz
            if(acc == 0)
                pc = data - 1;
            break;
        case 0x3: //jnz
            if(acc != 0)
                pc = data - 1;
            break;
        case 0x4: //jic
            if(carry)
                pc = data - 1;
            break;
        case 0x5: //jnc
            if(!carry)
                pc = data - 1;
            break;
        case 0x6: //jin
            if((acc & 0b10000000) == 0b10000000)
                pc = data - 1;
            break;
        case 0x7: //jnn
            if((acc & 0b10000000) != 0b10000000)
                pc = data - 1;
            break;
        case 0x9: //and
            acc &= data;
            break;
        case 0xA: //lor
            acc |= data;
            break;
        case 0xB: //xor
            acc ^= data;
            break;
        case 0xC: //inc
            acc++;
            break;
        case 0xD: //dec;
            acc--;
            break;
        case 0xE: //bsl;
            acc = acc << 1;
            break;
        case 0xF: //bsr
            acc = acc >> 1;
            break;
        case 0x18: //add literal
            acc = add(data, false);
            break;
        case 0x10: //add by address
            acc = add(resolve(data), false);
            break;
        case 0x19: //sub literal
            acc = add(~data, true);
            break;
        case 0x11: //sub by address
            acc = add(~resolve(data), true);
            break;
        case 0x1A: //adc literal
            acc = add(data, carry);
            break;
        case 0x12: //adc by address
            acc = add(resolve(data), carry);
            break;
        case 0x1E: //lda literal
            acc = data;
            break;
        case 0x16: //lda
            acc = resolve(data);
            break;
        case 0x17: //sta
            if((data & 0b11000000) == 0b11000000) {
                RAM[data ^ 0b11000000] = acc;
            } else {return -2;}
            break;
        case 0x1F:
            halt();
            break;

    }
    pc++;
    return 0;
}

void halt() {
    std::cout << "Program Counter: " << (int) pc << "\nRAM: " << (char *) RAM << std::endl;
    shouldClose = true;
}

uint8_t resolve(uint8_t address) {
    if((address & 0b11000000) == 0b11000000) {
        return RAM[address ^ 0b11000000];
    }
    return ROM[address];
}
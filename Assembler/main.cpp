#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

int main() {
    std::ifstream input("in.txt");
    std::ofstream output;
    output.open("out.bin", std::ios::out | std::ios::binary);

    for( std::string line; getline( input, line ); ) {
        std::stringstream stream(line);
        std::string segment;
        std::vector<std::string> list;

        while(std::getline(stream, segment, ' '))
        {
            std::transform(segment.begin(), segment.end(), segment.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            list.push_back(segment);
        }

        // big block of suffering
        uint8_t a[24] = {0x0, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x18, 0x10, 0x19, 0x11, 0x1A, 0x12, 0x1E, 0x16, 0x17, 0x1F};
        if(list[0] == "jmp") { output.write(reinterpret_cast<const char *>(&a[0]), sizeof(uint8_t));}
        else if(list[0] == "jiz") { output.write(reinterpret_cast<const char *>(&a[1]), sizeof(uint8_t));}
        else if(list[0] == "jnz") { output.write(reinterpret_cast<const char *>(&a[2]), sizeof(uint8_t));}
        else if(list[0] == "jic") { output.write(reinterpret_cast<const char *>(&a[3]), sizeof(uint8_t));}
        else if(list[0] == "jnc") { output.write(reinterpret_cast<const char *>(&a[4]), sizeof(uint8_t));}
        else if(list[0] == "jin") { output.write(reinterpret_cast<const char *>(&a[5]), sizeof(uint8_t));}
        else if(list[0] == "jnn") { output.write(reinterpret_cast<const char *>(&a[6]), sizeof(uint8_t));}
        else if(list[0] == "and") { output.write(reinterpret_cast<const char *>(&a[7]), sizeof(uint8_t));}
        else if(list[0] == "lor") { output.write(reinterpret_cast<const char *>(&a[8]), sizeof(uint8_t));}
        else if(list[0] == "xor") { output.write(reinterpret_cast<const char *>(&a[9]), sizeof(uint8_t));}
        else if(list[0] == "inc") { output.write(reinterpret_cast<const char *>(&a[10]), sizeof(uint8_t));}
        else if(list[0] == "dec") { output.write(reinterpret_cast<const char *>(&a[11]), sizeof(uint8_t));}
        else if(list[0] == "bsl") { output.write(reinterpret_cast<const char *>(&a[12]), sizeof(uint8_t));}
        else if(list[0] == "bsr") { output.write(reinterpret_cast<const char *>(&a[13]), sizeof(uint8_t));}
        else if(list[0] == "add" && list[1].c_str()[0] == '#') { output.write(reinterpret_cast<const char *>(&a[14]), sizeof(uint8_t));} //lit
        else if(list[0] == "add") { output.write(reinterpret_cast<const char *>(&a[15]), sizeof(uint8_t));}
        else if(list[0] == "sub" && list[1].c_str()[0] == '#') { output.write(reinterpret_cast<const char *>(&a[16]), sizeof(uint8_t));} // lit
        else if(list[0] == "sub") { output.write(reinterpret_cast<const char *>(&a[17]), sizeof(uint8_t));}
        else if(list[0] == "adc" && list[1].c_str()[0] == '#') { output.write(reinterpret_cast<const char *>(&a[18]), sizeof(uint8_t));} //lit
        else if(list[0] == "adc") { output.write(reinterpret_cast<const char *>(&a[19]), sizeof(uint8_t));}
        else if(list[0] == "lda" && list[1].c_str()[0] == '#') { output.write(reinterpret_cast<const char *>(&a[20]), sizeof(uint8_t));} //
        else if(list[0] == "lda") { output.write(reinterpret_cast<const char *>(&a[21]), sizeof(uint8_t));}
        else if(list[0] == "sta") { output.write(reinterpret_cast<const char *>(&a[22]), sizeof(uint8_t));}
        else if(list[0] == "hlt") { output.write(reinterpret_cast<const char *>(&a[23]), sizeof(uint8_t));}

        list[1].erase(std::remove(list[1].begin(), list[1].end(), '#'), list[1].end());
        list[1].erase(std::remove(list[1].begin(), list[1].end(), '$'), list[1].end());
        size_t pos;
        uint8_t data = std::stoi(list[1], &pos, 16);
        output.write(reinterpret_cast<const char *>(&data), sizeof(uint8_t));
    }
    output.close();
    return 0;
}




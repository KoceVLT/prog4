#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>

// sizeof(void*) - kolku bajti ima edna memoriska adresa (pokazuvac) na sistem (32-bit ili 64-bit)

class GermanString{
public:
    uint32_t len;

    char prefix[4];

    union Data{
        const char* ptr;
        char suffix[sizeof(void*)];
    } data;

    GermanString(const char* input_str, uint32_t length) {
        len = length;

        uint32_t kolkMoze = 4 + sizeof(void*);

        if(len <= kolkMoze) {
            std::memset(prefix, 0, 4); 
            std::memcpy(prefix, input_str, std::min(len, (uint32_t)4));

            if(len > 4) {
                std::memcpy(data.suffix, input_str + 4, len - 4);
            }
        } 
        else{
            std::memcpy(prefix, input_str, 4);

            data.ptr = input_str; 
        }
    }
    void print() {
        uint32_t max = 4 + sizeof(void*);
        
        if(len <= max) {
            for(int i = 0; i < len && i < 4; i++) {
                std::cout << prefix[i];
            }
            if(len > 4) {
                for(int i = 0; i < len - 4; i++) {
                    std::cout << data.suffix[i];
                }
            }
        } else {
            for(int i = 0; i < len; i++) {
                std::cout << data.ptr[i];
            }
        }
        std::cout << std::endl;
    }

    bool equals(GermanString& other) {
        if(len != other.len) {
            return false;
        }
        
        if(std::memcmp(prefix, other.prefix, 4) != 0) {
            return false;
        }

        uint32_t max = 4 + sizeof(void*);

        if(len <= max) {
            if(len > 4) {
                return std::memcmp(data.suffix, other.data.suffix, len - 4) == 0;
            }
            return true;
        } else {
            return std::memcmp(data.ptr, other.data.ptr, len) == 0;
        }
    }

    void modify(const char* s, uint32_t l) {
        len = l;
        uint32_t max = 4 + sizeof(void*);

        if(len <= max) {
            std::memset(prefix, 0, 4); 
            std::memcpy(prefix, s, std::min(len, (uint32_t)4));

            if(len > 4) {
                std::memcpy(data.suffix, s + 4, len - 4);
            }
        } else {
            std::memcpy(prefix, s, 4);
            data.ptr = s; 
        }
    }
};
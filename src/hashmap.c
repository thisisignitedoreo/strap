
#include "hashmap.h"

uint32_t hash_crc32(String string) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < string.size; i++) {
        crc ^= (uint8_t)(string.bytes[i]);
        for (int i = 0; i < 8; i++) {
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
            else crc >>= 1;
        }
    }
    return crc;
}

hashmap_implement(I32HashMap, I32HashPair, int32_t)
hashmap_implement(U32HashMap, U32HashPair, uint32_t)
hashmap_implement(FloatHashMap, FloatHashPair, float)
hashmap_implement(DoubleHashMap, DoubleHashPair, double)
hashmap_implement(StringHashMap, StringHashPair, String)
hashmap_implement(SBHashMap, SBHashPair, StringBuilder*)

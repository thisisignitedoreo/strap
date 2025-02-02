
#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "string.h"
#include "arena.h"
#include "array.h"

uint32_t hash_crc32(String string);

#define hashmap_define(hashmap_name, hashpair_name, hashmap_type) \
    typedef struct { \
        bool empty; \
        bool used; \
        String key; \
        hashmap_type value; \
    } hashpair_name; \
    \
    typedef struct { \
        hashpair_name* table; \
        size_t capacity; \
        Arena* allocator; \
    } hashmap_name; \
    \
    hashmap_name* hashmap_name##_new(Arena* arena, size_t capacity); \
    void hashmap_name##_set(hashmap_name* hashmap, String key, hashmap_type value); \
    hashmap_type hashmap_name##_get(hashmap_name* hashmap, String key); \
    bool hashmap_name##_contains(hashmap_name* hashmap, String key); \
    void hashmap_name##_remove(hashmap_name* hashmap, String key);

#define hashmap_implement(hashmap_name, hashpair_name, hashmap_type) \
    hashmap_name* hashmap_name##_new(Arena* arena, size_t capacity) { \
        hashmap_name* hashmap = arena_allocate(arena, sizeof(hashmap_name)); \
        hashmap->table = arena_allocate(arena, sizeof(hashpair_name) * capacity); \
        memset(hashmap->table, 0, sizeof(hashpair_name) * capacity); \
        for (size_t i = 0; i < capacity; i++) hashmap->table[i].empty = true; \
        hashmap->capacity = capacity; \
        hashmap->allocator = arena; \
        return hashmap; \
    } \
    void hashmap_name##_set(hashmap_name* hashmap, String key, hashmap_type value) { \
        size_t index = hash_crc32(key) % hashmap->capacity; \
        size_t old_index = index; \
        \
        while (!hashmap->table[index].empty && !sv_compare(hashmap->table[index].key, key)) { \
            index = (index + 1) % hashmap->capacity; \
            if (old_index == index) return; \
        } \
        \
        hashmap->table[index].empty = false; \
        hashmap->table[index].used = true; \
        hashmap->table[index].key = key; \
        hashmap->table[index].value = value; \
    } \
    hashmap_type hashmap_name##_get(hashmap_name* hashmap, String key) { \
        size_t index = hash_crc32(key) % hashmap->capacity; \
        size_t old_index = index; \
        \
        if (!hashmap->table[index].used) return (hashmap_type) {0}; \
        \
        while (hashmap->table[index].used && !sv_compare(hashmap->table[index].key, key)) { \
        index = (index + 1) % hashmap->capacity; \
            if (index == old_index) return (hashmap_type) {0}; \
        } \
        \
        return hashmap->table[index].value; \
    } \
    bool hashmap_name##_contains(hashmap_name* hashmap, String key) { \
        size_t index = hash_crc32(key) % hashmap->capacity; \
        size_t old_index = index; \
        \
        if (!hashmap->table[index].used) return false; \
        \
        while (!hashmap->table[index].empty && !sv_compare(hashmap->table[index].key, key)) { \
            index = (index + 1) % hashmap->capacity; \
            if (index == old_index) return false; \
        } \
        \
        return true; \
    } \
    void hashmap_name##_remove(hashmap_name* hashmap, String key) { \
        size_t index = hash_crc32(key) % hashmap->capacity; \
        size_t old_index = index; \
        \
        if (!hashmap->table[index].used) return; \
        \
        while (!hashmap->table[index].empty && !sv_compare(hashmap->table[index].key, key)) { \
            index = (index + 1) % hashmap->capacity; \
            if (index == old_index) return; \
        } \
        \
        hashmap->table[index].key = (String) {0}; \
        hashmap->table[index].value = (hashmap_type) {0}; \
        hashmap->table[index].empty = true; \
        /* .used is not set for linear probing after deleting an item to work */ \
    }

hashmap_define(I32HashMap, I32HashPair, int32_t)
hashmap_define(U32HashMap, U32HashPair, uint32_t)
hashmap_define(FloatHashMap, FloatHashPair, float)
hashmap_define(DoubleHashMap, DoubleHashPair, double)
hashmap_define(StringHashMap, StringHashPair, String)
hashmap_define(SBHashMap, SBHashPair, StringBuilder*)

#endif // HASHMAP_H_

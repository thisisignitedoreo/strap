
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "arena.h"
#include "array.h"

#include <stdint.h>
#include <stddef.h>

#define linked_list_define(list_name, list_type) \
    typedef struct list_name { \
        struct list_name* first_element; \
        struct list_name* next_element; \
        list_type element; \
        Arena* allocator; \
    } list_name; \
    \
    list_name* list_name##_new(Arena* arena, list_type item); \
    list_name* list_name##_get(list_name* array, size_t index); \
    list_name* list_name##_get_start(list_name* array); \
    list_name* list_name##_get_end(list_name* array); \
    list_name* list_name##_get_previous(list_name* array); \
    list_name* list_name##_get_next(list_name* array); \
    void list_name##_insert_at_node(list_name* node, list_type item); \
    void list_name##_insert_at_index(list_name* array, size_t index, list_type item); \
    void list_name##_push(list_name* array, list_type item); \
    void list_name##_remove_at_node(list_name* node); \
    void list_name##_remove_at_index(list_name* array, size_t index); \
    void list_name##_pop(list_name* array); \
    void list_name##_swap(list_name* a, list_name* b); \
    void list_name##_swap_at_indexes(list_name* array, size_t a, size_t b);

#define linked_list_implement(list_name, list_type) \
    list_name* list_name##_new(Arena* arena, list_type item) { \
        list_name* node = arena_malloc(arena, sizeof(list_name)); \
        node->first_element = node; \
        node->next_element = NULL; \
        node->element = item; \
        node->allocator = arena; \
        return node; \
    } \
    list_name* list_name##_get(list_name* array, size_t index) { \
        list_name* node = array->first_element; \
        for (size_t i = 0; i < index; i++) { \
            if (node->next_element == NULL) return NULL; \
            node = node->next_element; \
        } \
        return node; \
    } \
    list_name* list_name##_get_start(list_name* array) { return array->first_element; } \
    list_name* list_name##_get_end(list_name* array) { \
        list_name* node = array->first_element; \
        while (node->next_element) \
            node = node->next_element; \
        return node; \
    } \
    list_name* list_name##_get_previous(list_name* array) { \
        list_name* node = array->first_element; \
        if (node == array) return NULL; \
        while (node != NULL && node->next_element != array) \
            node = node->next_element; \
        return node; \
    } \
    list_name* list_name##_get_next(list_name* array) { return array->next_element; } \
    void list_name##_insert_at_node(list_name* node, list_type item) { \
        list_name* new_node = arena_malloc(node->allocator, sizeof(list_name)); \
        new_node->first_element = node->first_element; \
        new_node->next_element = node->next_element; \
        node->next_element = new_node; \
        new_node->element = item; \
        new_node->allocator = node->allocator; \
    } \
    void list_name##_insert_at_index(list_name* array, size_t index, list_type item) { \
        list_name##_insert_at_node(list_name##_get(array, index), item); \
    } \
    void list_name##_push(list_name* array, list_type item) { \
        list_name##_insert_at_node(list_name##_get_end(array), item); \
    } \
    void list_name##_remove_at_node(list_name* node) { \
        list_name* previous = list_name##_get_previous(node); \
        if (previous == NULL) { \
            list_name* new_start = node->next_element; \
            while (node->next_element) { \
                node->first_element = new_start; \
                node = node->next_element; \
            } \
        } else previous->next_element = node->next_element; \
    } \
    void list_name##_remove_at_index(list_name* array, size_t index) { \
        list_name##_remove_at_node(list_name##_get(array, index)); \
    } \
    void list_name##_pop(list_name* array) { \
        list_name##_remove_at_node(list_name##_get_end(array)); \
    } \
    void list_name##_swap(list_name* a, list_name* b) { \
        if (a == b) return; \
        list_name* prev_a = list_name##_get_previous(a); \
        list_name* next_b = list_name##_get_next(b); \
        if (prev_a == NULL) { \
            for (list_name* node = a->first_element; node != NULL; node = node->next_element) { \
                node->first_element = b; \
            } \
        } else { \
            prev_a->next_element = b; \
        } \
        a->next_element = next_b; \
        b->next_element = a; \
    } \
    void list_name##_swap_at_indexes(list_name* array, size_t a, size_t b) { \
        list_name##_swap(list_name##_get(array, a), list_name##_get(array, b)); \
    }

linked_list_define(I32LinkedNode, int32_t)
linked_list_define(U32LinkedNode, uint32_t)
linked_list_define(FloatLinkedNode, float)
linked_list_define(DoubleLinkedNode, double)
linked_list_define(StringLinkedNode, String)
linked_list_define(SBLinkedNode, StringBuilder*)

#endif // LINKED_LIST_H_

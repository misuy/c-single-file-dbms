#ifndef UTIL_H_
#define UTIL_H_

#include "inttypes.h"
#include "stdlib.h"
#include <glib-object.h>

#define MAYBE(type, type_name)                                                                  \
typedef struct Maybe_##type_name                                                                \
{                                                                                               \
    uint8_t ok;                                                                                 \
    union {                                                                                     \
        type value;                                                                             \
        char* exception;                                                                        \
    };                                                                                          \
} Maybe_##type_name;                                                                            \

typedef struct Nothing {
    uint8_t nothing;
} Nothing;

MAYBE(Nothing, Nothing)
MAYBE(uint8_t, uint8_t)
MAYBE(uint32_t, uint32_t)

#define RESULT(type, res) return (Maybe_##type) { .ok = 1, .value = res }

#define OK(type) return (Maybe_##type) { .ok = 1 }

#define EXCEPTION(type, msg) return (Maybe_##type) { .ok = 0, .exception = msg }

#define HANDLE(type, result_type, result)                                                                       \
(                                                                                                               \
{                                                                                                               \
    Maybe_##result_type _util_handle_result = result;                                                           \
    if (!_util_handle_result.ok) return (Maybe_##type) { .ok = 0, .exception = _util_handle_result.exception }; \
    _util_handle_result.value;                                                                                  \
}                                                                                                               \
)                                                                                                               \

#define TRY(result_type, result)                                                                                \
(                                                                                                               \
{                                                                                                               \
    Maybe_##result_type _util_handle_result = result;                                                           \
    if (!_util_handle_result.ok)                                                                                \
    {                                                                                                           \
        printf("exception: %s\n", _util_handle_result.exception);                                               \
        exit(1);                                                                                                \
    }                                                                                                           \
    _util_handle_result.value;                                                                                               \
}                                                                                                               \
)

#define HANDLE_NOTHING(type, result)                                                                                                \
{                                                                                                                                   \
    Maybe_Nothing _util_handle_nothing_result = result;                                                                             \
    if (!_util_handle_nothing_result.ok) return (Maybe_##type) { .ok = 0, .exception = _util_handle_nothing_result.exception };     \
}                                                                                                                                   \

#define LINKED_LIST_HEADER(type)                                                                \
                                                                                                \
MAYBE(type, type)                                                                               \
                                                                                                \
typedef struct type##_LinkedListNode                                                            \
{                                                                                               \
    type value;                                                                                 \
    struct type##_LinkedListNode* next;                                                         \
} type##_LinkedListNode;                                                                        \
                                                                                                \
typedef struct type##_LinkedList                                                                \
{                                                                                               \
    uint32_t length;                                                                            \
    struct type##_LinkedListNode* head;                                                         \
    struct type##_LinkedListNode* tail;                                                         \
} type##_LinkedList;                                                                            \
                                                                                                \
                                                                                                \
uint8_t type##_linked_list_is_empty(struct type##_LinkedList* list);                            \
struct type##_LinkedListNode* type##_linked_list_node_new(type value);                          \
void type##_linked_list_node_free(struct type##_LinkedListNode* node);                          \
struct type##_LinkedList type##_linked_list_new(void);                                          \
void type##_linked_list_push_front(struct type##_LinkedList* list, type value);                 \
void type##_linked_list_push_back(struct type##_LinkedList* list, type value);                  \
struct Maybe_##type type##_linked_list_pop_front(struct type##_LinkedList* list);               \
struct Maybe_##type type##_linked_list_get(struct type##_LinkedList* list, uint32_t idx);       \
void type##_linked_list_remove(struct type##_LinkedList* list, uint32_t idx);                   \
void type##_linked_list_free(struct type##_LinkedList *list);                                   \


#define LINKED_LIST(type)                                                                       \
                                                                                                \
uint8_t type##_linked_list_is_empty(struct type##_LinkedList* list)                             \
{                                                                                               \
    return list->length == 0;                                                                   \
}                                                                                               \
                                                                                                \
struct type##_LinkedListNode* type##_linked_list_node_new(type value)                           \
{                                                                                               \
    struct type##_LinkedListNode* node = malloc(sizeof(struct type##_LinkedListNode));          \
    node->value = value;                                                                        \
    node->next = 0;                                                                             \
    return node;                                                                                \
}                                                                                               \
                                                                                                \
void type##_linked_list_node_free(struct type##_LinkedListNode* node)                           \
{                                                                                               \
    if (node != 0) free(node);                                                                  \
    node = 0;                                                                                   \
}                                                                                               \
                                                                                                \
struct type##_LinkedList type##_linked_list_new(void)                                           \
{                                                                                               \
    struct type##_LinkedList list;                                                              \
    list.length = 0;                                                                            \
    list.head = 0;                                                                              \
    list.tail = 0;                                                                              \
    return list;                                                                                \
}                                                                                               \
                                                                                                \
                                                                                                \
void type##_linked_list_push_front(struct type##_LinkedList* list, type value)                  \
{                                                                                               \
    struct type##_LinkedListNode* node = type##_linked_list_node_new(value);                    \
    node->next = list->head;                                                                    \
    if (list->length == 0) list->tail = node;                                                   \
    list->head = node;                                                                          \
    list->length++;                                                                             \
}                                                                                               \
                                                                                                \
                                                                                                \
void type##_linked_list_push_back(struct type##_LinkedList* list, type value)                   \
{                                                                                               \
    struct type##_LinkedListNode* node = type##_linked_list_node_new(value);                    \
    if (list->length == 0)                                                                      \
    {                                                                                           \
        list->head = node;                                                                      \
        list->tail = list->head;                                                                \
    }                                                                                           \
    else                                                                                        \
    {                                                                                           \
        list->tail->next = node;                                                                \
        list->tail = node;                                                                      \
    }                                                                                           \
    list->length++;                                                                             \
}                                                                                               \
                                                                                                \
struct Maybe_##type type##_linked_list_pop_front(struct type##_LinkedList* list)                \
{                                                                                               \
    if (list->length == 0) EXCEPTION(type, "pop from 0 length list");                           \
    struct type##_LinkedListNode* head = list->head;                                            \
    if (list->length == 1) list->tail = 0;                                                      \
    list->head = head->next;                                                                    \
    type value = head->value;                                                                   \
    type##_linked_list_node_free(head);                                                         \
    list->length--;                                                                             \
    RESULT(type, value);                                                                        \
}                                                                                               \
                                                                                                \
                                                                                                \
                                                                                                \
                                                                                                \
struct Maybe_##type type##_linked_list_get(struct type##_LinkedList* list, uint32_t idx)        \
{                                                                                               \
    struct Maybe_##type maybe_value;                                                            \
    maybe_value.ok = 0;                                                                         \
    if (idx >= list->length) return maybe_value;                                                \
                                                                                                \
    struct type##_LinkedListNode* node = list->head;                                            \
    for (uint32_t i=0; i<idx; i++)                                                              \
    {                                                                                           \
        if (node == 0) return maybe_value;                                                      \
        node = node->next;                                                                      \
    }                                                                                           \
    if (node == 0) return maybe_value;                                                          \
    maybe_value.ok = 1;                                                                         \
    maybe_value.value = node->value;                                                            \
    return maybe_value;                                                                         \
}                                                                                               \
                                                                                                \
void type##_linked_list_remove(struct type##_LinkedList* list, uint32_t idx)                    \
{                                                                                               \
    if (idx >= list->length) return;                                                            \
    struct type##_LinkedListNode* node = list->head;                                            \
    struct type##_LinkedListNode* prev = 0;                                                     \
                                                                                                \
    for (uint32_t i=0; i<idx; i++)                                                              \
    {                                                                                           \
        if (node == 0) return;                                                                  \
        prev = node;                                                                            \
        node = node->next;                                                                      \
    }                                                                                           \
                                                                                                \
    if (node == 0) return;                                                                      \
    if (node == list->head) list->head = node->next;                                            \
    else                                                                                        \
    {                                                                                           \
        if (node == list->tail) list->tail = prev;                                              \
        prev->next = node->next;                                                                \
    }                                                                                           \
    type##_linked_list_node_free(node);                                                         \
    list->length--;                                                                             \
}                                                                                               \
                                                                                                \
void type##_linked_list_free(struct type##_LinkedList *list)                                    \
{                                                                                               \
    struct type##_LinkedListNode *it = list->head;                                              \
    while (it != 0)                                                                             \
    {                                                                                           \
        struct type##_LinkedListNode *it_copy = it;                                             \
        it = it->next;                                                                          \
        type##_linked_list_node_free(it_copy);                                                  \
    }                                                                                           \
}                                                                                               \

LINKED_LIST_HEADER(uint16_t)

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "arraylist.h"

#define START_BUCKETS 64


struct Node {
    struct Node* next;
    char* key;
    char* value;
};

struct Bucket {
    struct Node* head;
};

struct Bucket* Bucket_init(void) {
    struct Bucket* bucket = (struct Bucket*)malloc(sizeof(struct Bucket));
    bucket->head = NULL;
    return bucket;
}

void Bucket_free(struct Bucket* bucket) {
    // Step 1: Clean up nodes
    struct Node* node = bucket->head;
    while (node != NULL) {
        struct Node* nextNode = node->next;
        free(node);
        node = nextNode;
    }
    // Step 2: Free bucket
    free(bucket);
}

void Bucket_freeKeyChars(struct Bucket* bucket) {
    struct Node* node = bucket->head;
    while (node != NULL) {
        free(node->key);
        node = node->next;
    }
}

void Bucket_freeValueChars(struct Bucket* bucket) {
    struct Node* node = bucket->head;
    while (node != NULL) {
        free(node->value);
        node = node->next;
    }
}

void Bucket_print(struct Bucket* bucket) {
    struct Node* node = bucket->head;
    printf("==>");
    while (node != NULL) {
        printf("(%s, %s) ==> ", node->key, node->value);
        node = node->next;
    }
    printf("\n");
}

void Bucket_addFirst(struct Bucket* bucket, char* key, char* value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = bucket->head;
    bucket->head = newNode;
}

int Bucket_put(struct Bucket* bucket, char* key, char* value) {
    struct Node* node = bucket->head;
    int added = 1;
    while (node != NULL && added == 1) {
        if (strcmp(key, node->key) == 0) {
            // Key is already there, so just update value
            added = 0;
            node->value = value;
        }
        if (added == 1) {
            node = node->next;
        }
    }
    if (node == NULL) {
        Bucket_addFirst(bucket, key, value);
    }
    return added;
}

char* Bucket_get(struct Bucket* bucket, char* key) {
    char* ret = NULL;
    struct Node* node = bucket->head;
    int found = 0;
    while (node != NULL && found == 0) {
        if (strcmp(key, node->key) == 0) {
            // Key is already there, so just update value
            found = 1;
            ret = node->value;
        }
        node = node->next;
    }
    return ret;
}


void Bucket_removeFirst(struct Bucket* bucket, int remKey, int remValue) {
    if (bucket->head != NULL) {
        if (remKey == 1) {
            free(bucket->head->key);
        }
        if (remValue == 1) {
            free(bucket->head->value);
        }
        void* tmp = bucket->head;
        bucket->head = bucket->head->next;
        free(tmp);
    }
}

int Bucket_remove(struct Bucket* bucket, char* key, int remKey, int remValue) {
    int ret = 0;
    if (bucket->head != NULL) {
        struct Node* node = bucket->head;
        if (strcmp(node->key, key) == 0) {
            Bucket_removeFirst(bucket, remKey, remValue);
            ret = 1;
        }
        else {
            while (node->next != NULL) {
                if (strcmp(node->next->key, key) == 0) {
                    if (remKey) {
                        free(node->next->key);
                    }
                    if (remValue) {
                        free(node->next->value);
                    }
                    struct Node* temp = node->next;
                    node->next = node->next->next;
                    ret = 1;
                    free(temp);
                    break;
                }
                else {
                    node = node->next;
                }
            }
        }
    }
    return ret;
}



/**
 * @brief Dynamically allocate memory for a hashmap and all of its buckets
 * 
 * @return struct HashMap* 
 */
void HashMap_init(struct HashMap* map) {
    map->NBuckets = START_BUCKETS;
    map->N = 0;
    map->buckets = malloc(sizeof(struct Bucket*)*map->NBuckets);
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        buckets[i] = Bucket_init();
    }
}

/**
 * @brief Free all linked lists associated to a hash map
 * 
 * @param map 
 */
void HashMap_free(struct HashMap* map) {
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        Bucket_free(buckets[i]);
    }
    free(buckets);
}

/**
 * @brief Free all of the key strings
 * 
 * @param map 
 */
void HashMap_freeKeyChars(struct HashMap* map) {
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        Bucket_freeKeyChars(buckets[i]);
    }
}

/**
 * @brief Free all of the value strings
 * 
 * @param map 
 */
void HashMap_freeValueChars(struct HashMap* map) {
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        Bucket_freeValueChars(buckets[i]);
    }
}

/**
 * @brief Return the hash code for a string
 * 
 * @param s String of which to compute hash code
 * @return long Hash code
 */
long charHash(char* s) {
    long hash = 0;
    while ((*s) != '\0') {
        hash = (31*hash + *s) % 1000000;
        s++;
    }
    return hash;
}

/**
 * @brief Put a key/value pair in a hash map, or update
 * the value associated to a key if it's already there
 * 
 * @param key Key
 * @param value Value
 */
int HashMap_put(struct HashMap* map, char* key, char* value) {
    long i = charHash(key) % map->NBuckets;
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    int added = Bucket_put(buckets[i], key, value);
    map->N += added;
    return added;
}

/**
 * @brief Return the value associated to a key, or NULL
 * if the key does not exist in the map
 * 
 * @param map 
 * @param key 
 * @return char* 
 */
char* HashMap_get(struct HashMap* map, char* key) {
    long i = charHash(key) % map->NBuckets;
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    return Bucket_get(buckets[i], key);
}

/**
 * @brief Remove the key/value pair associated to a key if
 * that key exists in the hash map
 * 
 * @param map 
 * @param key Key to remove
 * @param remKey If 1, remove dynamic memory associated to key in the map
 * @param remValue If 1, remove dynamic memory associated to value in the map
 * @return 1 if the item was there, 0 otherwise
 */
int HashMap_remove(struct HashMap* map, char* key, int remKey, int remValue) {
    long i = charHash(key) % map->NBuckets;
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    return Bucket_remove(buckets[i], key, remKey, remValue);
}

/**
 * @brief Print out a string representation of the hashmap for debugging
 * 
 * @param map 
 */
void HashMap_print(struct HashMap* map) {
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        Bucket_print(buckets[i]); 
    }
}

/**
 * @brief Write all pairs to a string buffer, in no particular order
 *        Assumes both keys and values are char*
 * 
 * @param map Map object
 * @param buf Buffer to which to write result
 * @param kvsep String separator between keys and values
 * @param pairsep String separator between pairs
 */
void HashMap_writePairs(struct HashMap* map, struct ArrayListBuf* buf, char* kvsep, char* pairsep) {
    int kvsepLen = strlen(kvsep);
    int pairsepLen = strlen(pairsep);
    struct Bucket** buckets = (struct Bucket**)map->buckets;
    for (int i = 0; i < map->NBuckets; i++) {
        struct Node* node = buckets[i]->head;
        while (node != NULL) {
            ArrayListBuf_push(buf, node->key, strlen(node->key));
            ArrayListBuf_push(buf, kvsep, kvsepLen);
            ArrayListBuf_push(buf, (char*)node->value, strlen((char*)node->value));
            ArrayListBuf_push(buf, pairsep, pairsepLen);
            node = node->next;
        }
    }
}

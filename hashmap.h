#ifndef hashmap_h
#define hashmap_h

#include "arraylist.h"

struct HashMap {
    void* buckets;
    long NBuckets;
    int N;
};


/**
 * @brief Dynamically allocate memory for a hashmap and all of its buckets
 * 
 * @param struct HashMap* Map to initialize
 */
void HashMap_init(struct HashMap* map);

/**
 * @brief Free all linked lists associated to a hash map,
 * and then finally the hash map itself
 * 
 * @param map 
 */
void HashMap_free(struct HashMap* map);

/**
 * @brief Free all of the key/value pairs, assuming they are 
 * all chars
 * 
 * @param map 
 */
void HashMap_freeKeyValChars(struct HashMap* map);


/**
 * @brief Put a key/value pair in a hash map, or update
 * the value associated to a key if it's already there
 * 
 * @param key Key
 * @param value Value
 *
 * @return 1 if this is a new key, or 0 if not
 */
int HashMap_put(struct HashMap* map, char* key, void* value);

/**
 * @brief Return the value associated to a key, or NULL
 * if the key does not exist in the map
 * 
 * @param map 
 * @param key 
 * @return void* 
 */
void* HashMap_get(struct HashMap* map, char* key);

/**
 * @brief Remove the key/value pair associated to a key if
 * that key exists in the hash map
 * 
 * @param map 
 * @param key Key to remove
 * @param remKey If 1, remove dynamic memory associated to key in the map
 * @return 1 if the item was there, 0 otherwise
 */
int HashMap_remove(struct HashMap* map, char* key, int remKey);

/**
 * @brief Print out a string representation of the hashmap for debugging
 * 
 * @param map 
 */
void HashMap_print(struct HashMap* map);


/**
 * @brief Write all pairs to a string buffer, in no particular order
 *        Assumes both keys and values are char*
 * 
 * @param map Map object
 * @param buf Buffer to which to write result
 * @param kvsep String separator between keys and values
 * @param pairsep String separator between pairs
 */
void HashMap_writePairs(struct HashMap* map, struct ArrayListBuf* buf, char* kvsep, char* pairsep);

#endif
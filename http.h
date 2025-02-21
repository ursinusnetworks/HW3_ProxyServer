#ifndef HTTP_H
#define HTTP_H

#include "hashmap.h"
#include "arraylist.h"

// A struct general enough to encompass
// both HTTP request and HTTP response headers
struct HTTPHeader {
    char type[6];
    char host[1024];
    char port[6];
    char path[1024];
    int v1, v2;
    int code;
    struct HashMap fields;
};

/**
 * @brief Initialize the dynamic memory associated to the fields
 * 
 * @param header
 */
void HTTPHeader_init(struct HTTPHeader* header);
/**
 * @brief Free the dynamic memory associated to the fields
 * 
 * @param header
 */
void HTTPHeader_free(struct HTTPHeader* header);

/**
 * @brief Put a field into the header, or update the value if the key
 * already exists
 * 
 * @param header
 * @param key Key to put. Should be null terminated
 * @param value Value to associate to key. Should be null terminated
 *
 * @return 1 if this is a new key, or 0 if not
 */
int HTTPHeader_putField(struct HTTPHeader* header, char* key, char* value);

/**
 * @brief Get a value associated to a key, or return NULL if
 * the key is not in the fields
 * 
 * @param header
 * @param key Key to get. Should be null terminated
 * @param value Value to associate to key. Should be null terminated
 * 
 * @return Value associated to the key, or NULL if the key is not in
 * the fields
 */
char* HTTPHeader_getField(struct HTTPHeader* header, char* key);

/**
 * @brief Remove a key/value pair if it exists
 * 
 * @param header
 * @param key Key to remove. Should be null terminated
 * 
 * @return 1 if the item was there, 0 otherwise
 */
int HTTPHeader_removeField(struct HTTPHeader* header, char* key);

/**
 * @brief Parse a request and fill in information into the header object
 * 
 * @param header
 * @param request Request string to parse
 * 
 * @return 0 if successfully parsed, -1 otherwise
 */
int HTTPHeader_parseRequest(struct HTTPHeader* header, char* request);


/**
 * @brief Print request header information as a header string
 * to an arraylist buffer
 * 
 * @param header Header information
 * @param buf buffer to which to print information
 */
void HTTPHeader_printRequest(struct HTTPHeader* header, struct ArrayListBuf* buf);

#endif
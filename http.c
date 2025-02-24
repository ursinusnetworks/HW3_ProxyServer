#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http.h"
#include "arraylist.h"
#include "hashmap.h"

/**
 * @brief Initialize the dynamic memory associated to the fields
 * 
 * @param header
 */
void HTTPHeader_init(struct HTTPHeader* header) {
    HashMap_init(&header->fields);
    memset(header->type, 0, 1024);
    memset(header->host, 0, 1024);
    memset(header->port, 0, 6);
    memset(header->path, 0, 1024);
    strcpy(header->port, "80");
}

/**
 * @brief Free the dynamic memory associated to the fields
 * 
 * @param header
 */
void HTTPHeader_free(struct HTTPHeader* header) {
    // Step 1: Free all of the key/value strings that have been
    // dynamically allocated
    HashMap_freeKeyChars(&header->fields);
    HashMap_freeValueChars(&header->fields);

    // Step 2: Free the hashmap itself
    HashMap_free(&header->fields);
}


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
int HTTPHeader_putField(struct HTTPHeader* header, char* key, char* value) {
    int keyLen = strlen(key);
    char* keyPut = (char*)malloc(keyLen+1);
    memcpy(keyPut, key, keyLen);
    keyPut[keyLen] = '\0';

    int valueLen = strlen(value);
    char* valuePut = (char*)malloc(valueLen+1);
    memcpy(valuePut, value, valueLen);
    valuePut[valueLen] = '\0';

    char* valueBefore = (char*)HashMap_get(&header->fields, keyPut);
    int added = HashMap_put(&header->fields, keyPut, valuePut);
    if (added == 0) {
        free(keyPut);
    }
    if (valueBefore != NULL) {
        free(valueBefore);
    }
    return added;
}


/**
 * @brief Get a value associated to a key, or return NULL if
 * the key is not in the fields
 * 
 * @param header
 * @param key Key to get. Should be null terminated
 * 
 * @return Value associated to the key, or NULL if the key is not in
 * the fields
 */
char* HTTPHeader_getField(struct HTTPHeader* header, char* key) {
    int keyLen = strlen(key)+1;
    char* keyGet = (char*)malloc(keyLen+1);
    memcpy(keyGet, key, keyLen);
    keyGet[keyLen] = '\0';

    char* ret = HashMap_get(&header->fields, keyGet);
    free(keyGet);
    return ret;
}

/**
 * @brief Remove a key/value pair if it exists
 * 
 * @param header
 * @param key Key to remove. Should be null terminated
 * 
 * @return 1 if the item was there, 0 otherwise
 */
int HTTPHeader_removeField(struct HTTPHeader* header, char* key) {
    int ret = 0;
    int keyLen = strlen(key)+1;
    char* keyGet = (char*)malloc(keyLen+1);
    memcpy(keyGet, key, keyLen);
    keyGet[keyLen] = '\0';
    ret = HashMap_remove(&header->fields, keyGet, 1, 1);
    free(keyGet);
    return ret;
}


/**
 * @brief Parse a request and fill in information into the header object
 * 
 * @param header
 * @param request Request string to parse
 * 
 * @return 0 if successfully parsed, -1 otherwise
 */
int HTTPHeader_parseRequest(struct HTTPHeader* header, char* request) {
    // Step 0: Copy request, since it will be mutated
    int N = strlen(request)+1;
    char* requestCopy = (char*)malloc(N);
    memcpy(requestCopy, request, N);
       
    // Step 1: Parse top two lines of the header
    int nargs = sscanf(requestCopy, "%5s %1023s HTTP/%i.%i\r\nHost:%s", header->type, header->path, &header->v1, &header->v2, header->host);
    if (nargs < 4) {
        fprintf(stderr, "Error: HTML header line, nargs = %i\n%s", nargs, requestCopy);
        free(requestCopy);
        return -1;
    }
    char* idx = strstr(header->host, ":");
    if (idx != NULL) {
        sscanf(idx+1, "%5s", header->port);
        *idx = '\0';
    }
    else {
        strcpy(header->port, "80");
    }

    // Step 2: Parse the rest of the key/value pairs
    char* linestart = strstr(requestCopy, "\r\n");
    if (linestart == NULL) {
        free(requestCopy);
        return 0;
    }
    linestart = strstr(linestart+2, "\r\n");
    if (linestart == NULL) {
        free(requestCopy);
        return 0;
    }
    linestart = linestart + 2;
    while (linestart != NULL) {
        char* colon = strstr(linestart, ":");
        if (colon == NULL) {
            linestart = NULL;
        }
        else {
            char* end = strstr(linestart, "\r\n");
            if (end == NULL) {
                linestart = NULL;
            }
            else {
                *colon = '\0';
                *end = '\0';
                colon++;
                while (*colon == ' ') {
                    colon++;
                }
                HTTPHeader_putField(header, linestart, colon);
                linestart = end + 2;
            }
        }
    }
    free(requestCopy);
    return 0;
}

/**
 * @brief Print request header information as a header string
 * to an arraylist buffer
 * 
 * @param header Header information
 * @param buf buffer to which to print information
 */
void HTTPHeader_printRequest(struct HTTPHeader* header, struct ArrayListBuf* buf) {
    ArrayListBuf_push(buf, "GET ", strlen("GET "));
    ArrayListBuf_push(buf, header->path, strlen(header->path));
    char HTTPVer[1024];
    sprintf(HTTPVer, " HTTP/%i.%i\r\n", header->v1, header->v2);
    ArrayListBuf_push(buf, HTTPVer, strlen(HTTPVer));
    ArrayListBuf_push(buf, "Host: ", strlen("Host: "));
    ArrayListBuf_push(buf, header->host, strlen(header->host));
    ArrayListBuf_push(buf, ":", 1);
    ArrayListBuf_push(buf, header->port, strlen(header->port));
    ArrayListBuf_push(buf, "\r\n", 2);
    HashMap_writePairs(&header->fields, buf, ": ", "\r\n");
    ArrayListBuf_push(buf, "\r\n", 2);
    ArrayListBuf_push(buf, "\0\0\0", 3);
}

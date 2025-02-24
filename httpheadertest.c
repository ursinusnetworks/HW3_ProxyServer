#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "arraylist.h"
#include "http.h"

int main(int argc, char** argv) {
    char* req=  "GET /cool/pr0.html HTTP/1.1\r\n"
                "Host: 127.0.0.1:60000\r\n"
                "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:135.0) Gecko/20100101 Firefox/135.0\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                "Accept-Language: en-US,en;q=0.5\r\n"
                "Accept-Encoding: gzip, deflate, br, zstd\r\n"
                "DNT: 1\r\n"
                "Sec-GPC: 1\r\n"
                "Connection: keep-alive\r\n"
                "Upgrade-Insecure-Requests: 1\r\n"
                "Sec-Fetch-Dest: document\r\n"
                "Sec-Fetch-Mode: navigate\r\n"
                "Sec-Fetch-Site: none\r\n"
                "Sec-Fetch-User: ?1\r\n"
                "Priority: u=0, i\r\n\r\n";

    struct ArrayListBuf buf;
    ArrayListBuf_init(&buf);

    struct HTTPHeader header;
    HTTPHeader_init(&header);
    HTTPHeader_parseRequest(&header, req);
    HTTPHeader_putField(&header, "Connection", "close"); // Change connection to close
    printf("Connection value: %s\n", HTTPHeader_getField(&header, "Connection"));
    printf("Nonexistent value: %p\n", HTTPHeader_getField(&header, "nonexistent"));
    printf("Removing Sec-GPC: %i\n", HTTPHeader_removeField(&header, "Sec-GPC"));
    printf("Removing nonexistent field: %i\n\n\n", HTTPHeader_removeField(&header, "nonexistent field"));

    HTTPHeader_printRequest(&header, &buf);
    printf("%s\n", buf.buff);

    
    HTTPHeader_free(&header);
    ArrayListBuf_free(&buf);
}

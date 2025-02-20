#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "arraylist.h"

#define BUFSIZE 1024

struct myargs {
    char* pathget; // Path of GET message
    char* pathout; // Path to which to save result
    char* port; // Port of proxy server (60000 by default)
    struct timeval timeout; // 10 (second) by default
};


struct myargs parseArgs(int argc, char** argv) {
    struct myargs ret;
    // Step 1: Setup default values
    ret.port = "60000";
    ret.timeout.tv_sec = 10;
    ret.timeout.tv_usec = 0;
    ret.pathget = "";
    ret.pathout = "";


    // Step 2: Parse user specified values
    // Advance to the next element
    argv++; 
    argc--; 
    while (argc > 0) {
        if((*argv)[0] == '-') {
            if (strcmp(*argv, "--help") == 0) {
                printf("Usage: ./tester --pathget <path to text file with get request> ");
                printf("--pathout <path to file to which to write result>");
                printf(" [--port <port number>] [--timeout <timeout>]\n");
                exit(0);
            }
            else if (strcmp(*argv, "--port") == 0) {
                argv++; argc--;
                if (argc > 0) {
                    ret.port = *argv;
                }
                else {
                    printf("Error: Expecting field after --port\n");
                    exit(0);
                }
            }
            else if (strcmp(*argv, "--pathget") == 0) {
                argv++; argc--;
                if (argc > 0) {
                    ret.pathget = *argv;
                }
                else {
                    printf("Error: Expecting field after --pathget\n");
                    exit(0);
                }
            }
            else if (strcmp(*argv, "--pathout") == 0) {
                argv++; argc--;
                if (argc > 0) {
                    ret.pathout = *argv;
                }
                else {
                    printf("Error: Expecting field after --pathout\n");
                    exit(0);
                }
            }
            else if (strcmp(*argv, "--timeout") == 0) {
                argv++; argc--;
                if (argc > 0) {
                    ret.timeout.tv_sec = atol(*argv);
                }
                else {
                    printf("Error: Expecting field after --timeout\n");
                    exit(0);
                }
            }

        }
        else {
            printf("Warning: Unrecognized field %s\n", *argv);
        }
        argv++; argc--;
    }

    // Step 3: Check for required values
    if (strcmp(ret.pathget, "") == 0) {
        printf("Error: Require a --pathget to be specified\n");
        exit(1);
    }
    if (strcmp(ret.pathout, "") == 0) {
        printf("Error: Require a --pathout to be specified\n");
        exit(1);
    }

    return ret;
}


int main(int argc, char** argv) {
    // Step 1: Parse Parameters and initialize variables
    struct myargs args = parseArgs(argc, argv);
    struct addrinfo hints;
    struct addrinfo* info;
    ArrayListBuf buff;
    char chunk[BUFSIZE];

    // Step 2: Find address information of domain and attempt to open socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // OK to use either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //Using TCP
    getaddrinfo("127.0.0.1", args.port, &hints, &info);
    int sockfd = -1;
    struct addrinfo* node = info;
    while (node != NULL && sockfd == -1) {
        printf("Trying socket\n");
        sockfd = socket(node->ai_family, node->ai_socktype, node->ai_protocol);
        if (sockfd == -1) {
            node = node->ai_next;
        }
    }
    if (sockfd == -1) {
        fprintf(stderr, "ERROR: Error number %i on opening socket\n", errno);
    }
    else {
        printf("Connecting to socket...");
        // Step 3: Make HTTP request to proxy and read back the result
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &args.timeout, sizeof(args.timeout));
        if (connect(sockfd, node->ai_addr, node->ai_addrlen) != -1) {
            printf("Connected successfully!\n");
            // Step 3a: Make request
            FILE* fin = fopen(args.pathget, "r");
            ArrayListBuf_init(&buff);
            int ch;
            do {
                ch = fgetc(fin);
                if (ch != EOF) {
                    char uch = (char)ch;
                    ArrayListBuf_push(&buff, &uch, 1);                    
                }
            }
            while(ch != EOF);

            int sentLen = 0;
            int thisLen = 0;
            do {
                thisLen = send(sockfd, buff.buff+sentLen, buff.N-sentLen, 0);
                sentLen += thisLen;
            }
            while(sentLen < buff.N && thisLen != -1);
            ArrayListBuf_free(&buff);

            if (thisLen == -1) {
                fprintf(stderr, "Error number %i sending", errno);
            }
            else {
                // Step 3b: Read back result
                FILE* fout = fopen(args.pathout, "wb");
                int res = -1;
                do {
                    res = recv(sockfd, chunk, BUFSIZE, 0);
                    printf("%i bytes received\n", res);
                    if (res >= 0) {
                        fwrite(chunk, 1, res, fout);
                    }
                }
                while(res > 0);
                fclose(fout);
                printf("Finished serving GET request\n");
            }
        }
        else {
            fprintf(stderr, "ERROR: Error number %i when connecting to socket\n", errno);
        }
    }
    freeaddrinfo(info);
}
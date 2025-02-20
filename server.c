#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <regex.h>
#include "arraylist.h"
#include "util.h"
#include "http.h"

struct cmdArgs {
    char* port; // 60000 by default
    char* blockpattern; // "" by default
    struct timeval timeout; // 10 (second) by default
};


struct cmdArgs parseArgs(int argc, char** argv) {
    struct cmdArgs ret;
    // Step 1: Setup default values
    ret.port = "60000";
    ret.timeout.tv_sec = 10;
    ret.timeout.tv_usec = 0;
    ret.blockpattern = "";

    // Step 2: Parse user specified values
    // Advance to the next element
    argv++; 
    argc--; 
    while (argc > 0) {
        if((*argv)[0] == '-') {
            if (strcmp(*argv, "--help") == 0) {
                printf("Usage: ./server");
                printf(" ");
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
            else if (strcmp(*argv, "--blockpattern") == 0) {
                argv++; argc--;
                if (argc > 0) {
                    ret.blockpattern = *argv;
                }
                else {
                    printf("Error: Expecting field after --blockpattern\n");
                    exit(0);
                }
            }
        }
        else {
            printf("Warning: Unrecognized field %s\n", *argv);
        }
        argv++; argc--;
    }

    return ret;
}

int main(int argc, char** argv) {
    // Step 1: Parse Parameters and initialize variables
    struct cmdArgs args = parseArgs(argc, argv);
    // TODO: Fill this in
}
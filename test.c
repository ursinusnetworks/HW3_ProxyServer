#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "arraylist.h"
#include "hashmap.h"


int main() {
    // Testing simple md5 hash
    char* s = "www.ctralie.com:/index.html";
    char md5[33];
    bytes2md5String(s, md5);
    printf("md5 = %s\n", md5);

    // Testing date parsing
    struct HTTPDate date;
    int ret = parseDate("Wed, 12 Feb 2025 04:31:34 GMT", &date);
    printf("ret = %i\n", ret);
    printf("Day = %i, Month = %i, Year = %i, Hour = %i, Minute = %i, Second = %i\n", date.day, date.month, date.year, date.hour, date.minute, date.second);

    // TODO: Test out other simple things here


}
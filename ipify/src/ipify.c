#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fujinet-network.h"

char *devspec = "N1:HTTPS://api.ipify.org/?format=json";
char *ip_query = "/ip";
char result[256];
char query[256];

void handle_err(uint8_t err, char *reason) {
    if (err) {
        printf("  Error: %d %s\n  Press any key\n", err, reason);
        cgetc();
        exit(1);
    }
}

void main(void) {
    uint8_t err = 0;
    printf("Fetching IP v1.3\n");
    err = network_open(devspec, 12, 0);
    handle_err(err, "open");

    err = network_json_parse(devspec);
    handle_err(err, "parse");

    sprintf(query, "N1:%s%c", ip_query, 0x9b);
    err = network_json_query(devspec, query, result);
    handle_err(err, "query");

    printf("  IP: %s\n", result);
    err = network_close(devspec);
    handle_err(err, "close");
    exit(0);
}

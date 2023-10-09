#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fujinet-network.h"
#include "debug.h"

char *ipify_json = "N1:HTTPS://api.ipify.org/?format=json";
char *ipify_text = "N1:HTTPS://api.ipify.org/";
char *ip_query = "/ip";
char result[256];
char query[32];

void handle_err(uint8_t err, char *reason) {
    if (err) {
        printf("  Error: %d %s\n  Press any key\n", err, reason);
        cgetc();
        exit(1);
    }
}

void main(void) {
    uint8_t err = 0;

    ////////////////////////////////////////////////////////////
    // USING JSON ENDPOINT
    ////////////////////////////////////////////////////////////
    printf("Fetching IP v1.4.4\n");
    err = network_open(ipify_json, 12, 0);
    handle_err(err, "open json");

    err = network_json_parse(ipify_json);
    handle_err(err, "parse");

    sprintf(query, "N1:%s%c", ip_query, 0);
    err = network_json_query(ipify_json, query, result);
    handle_err(err, "query");

    printf("  IP (json): %s\n", result);
    err = network_close(ipify_json);
    handle_err(err, "close json");

    ////////////////////////////////////////////////////////////
    // TEXT END POINT
    ////////////////////////////////////////////////////////////
    err = network_open(ipify_text, 4, 0);
    handle_err(err, "open text");
    bzero(result, 16);
    network_read(ipify_text, (uint8_t *)result, 16); // aaa.bbb.ccc.ddd + 0 is longest string
    handle_err(err, "read");
    printf("  IP (text): %s\n", result);
    network_close(ipify_text);
    handle_err(err, "close text");

    exit(0);
}

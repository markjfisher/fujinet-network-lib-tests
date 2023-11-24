#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fujinet-network.h"
#include "debug.h"

char *ipify_json = "N:HTTP://api.ipify.org/?format=json";
char *ipify_text = "N:HTTP://api.ipify.org/";
char *ip_query = "/ip";
char result[256];
char query[32];

void handle_err(uint8_t err, char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        exit(1);
    }
}

void main(void) {
    uint8_t err = 0;

    ////////////////////////////////////////////////////////////
    // USING JSON ENDPOINT
    ////////////////////////////////////////////////////////////
    gotox(0);
    printf("Fetching IP v1.5.6\n\n[%s]\n", ipify_json);
    err = network_open(ipify_json, 4, 0);
    handle_err(err, "open json");

    err = network_json_parse(ipify_json);
    handle_err(err, "parse");

    sprintf(query, "N:%s%c", ip_query, 0);
    err = network_json_query(ipify_json, query, result);
    handle_err(err, "query");

    printf("IP (json): %s\n", result);
    err = network_close(ipify_json);
    handle_err(err, "close json");

    ////////////////////////////////////////////////////////////
    // USING TEXT END POINT
    ////////////////////////////////////////////////////////////
    printf("\n[%s]\n", ipify_text);
    err = network_open(ipify_text, 4, 0);
    handle_err(err, "open text");

    err = network_read(ipify_text, (uint8_t *)result, 15); // aaa.bbb.ccc.ddd is longest string
    printf("read %d [%s]\n", fn_bytes_read, ipify_text);

    // it isn't the library's responsibility to set nuls, or return "strings" when doing a normal READ
    // so application has to take care of that, by adding 0 at the end of the data.
    if (fn_bytes_read < 15) {
        result[fn_bytes_read] = 0x00;
    }

    printf("IP (text): %s\n", result);
    network_close(ipify_text);
    handle_err(err, "close text");

    exit(0);
}

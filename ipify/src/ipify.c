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
    // set some trash in the result to check we are overwriting it.
    strcpy(result, "123456789012345678901234567890");

    ////////////////////////////////////////////////////////////
    // USING JSON ENDPOINT
    ////////////////////////////////////////////////////////////
    printf("Fetching IP v1.4.5\n");
    err = network_open(ipify_json, 12, 0);
    handle_err(err, "open json");

    err = network_json_parse(ipify_json);
    handle_err(err, "parse");
#ifdef BUILD_ATARI
    // uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, [uint8_t dstats, uint16_t dbuf, uint16_t dbyt]);
    err = network_ioctl(0xFB, 0, 1, ipify_json, 0, 0, 0);
    handle_err(err, "query param");

    err = network_ioctl(0xFB, 1, 0x00, ipify_json, 0, 0, 0);
    handle_err(err, "line ending");
#endif
    sprintf(query, "N1:%s%c", ip_query, 0);
    debug();
    err = network_json_query(ipify_json, query, result);
    handle_err(err, "query");

    // The library should handle JSON strings by nul terminating them.
    printf("  IP (json): %s\n", result);
    err = network_close(ipify_json);
    handle_err(err, "close json");

    ////////////////////////////////////////////////////////////
    // USING TEXT END POINT
    ////////////////////////////////////////////////////////////
    err = network_open(ipify_text, 4, 0);
    handle_err(err, "open text");

    strcpy(result, "123456789012345678901234567890");

    network_read(ipify_text, (uint8_t *)result, 15); // aaa.bbb.ccc.ddd is longest string

    // it isn't the library's responsibility to set nuls, or return "strings", so application has to take care of that.
    if (fn_bytes_read < 15) {
        result[fn_bytes_read] = 0x00;
    }

    handle_err(err, "read");
    printf("  IP (text): %s\n", result);
    printf("  read count: %d\n", fn_bytes_read);
    network_close(ipify_text);
    handle_err(err, "close text");

    exit(0);
}

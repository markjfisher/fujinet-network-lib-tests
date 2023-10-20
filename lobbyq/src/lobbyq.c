#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fujinet-network.h"

#ifdef BUILD_APPLE2
#include "fujinet-network-apple2.h"
#endif

#include "debug.h"
#include "lobbyq.h"

char *lobby = "N:http://fujinet.online:8080/view?platform=atari";
char url_buffer[128];
char result[4096];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

char *version = "v1.0.0";

void main(void) {
    setup();

    printf("lobby query %s\n", version);
    printf("Base URL: %s\n", lobby);

    do_query("");

    exit(0);
}

void setup() {
    bzero(url_buffer, 128);
    bzero(result, 1024);
    gotox(0);
}

void do_query(char *path) {
    url = lobby;
    err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");
    err = network_json_parse(url);
    handle_err("parse");

    err = network_json_query(url, path, result);
    handle_err("query");

    printf("/get >%s<, r: >%s<\n", path, result);
    err = network_close(url);
    handle_err("close json");
}

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        exit(1);
    }
}

// This demonstrates why library should have specialised functions, instead of consumers having to use ifdef
uint8_t set_http_channel_mode(char *devicespec, uint8_t mode) {
    #ifdef BUILD_APPLE2
    // uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, int16_t use_aux, void *buffer, uint16_t len);
    return network_ioctl('M', 0, mode, devicespec, 1, 0, 2);
    #endif

    #ifdef BUILD_ATARI
    // uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, [uint8_t dstats, uint16_t dbuf, uint16_t dbyt]);
    return network_ioctl('M', 0, mode, devicespec, 0, 0, 0);
    #endif
}

void add_header(char *devicespec, char *header) {
    err = set_http_channel_mode(devicespec, HTTP_CHAN_MODE_SET_HEADERS);
    handle_err("header chan mode");
    err = network_write(devicespec, (uint8_t *) header, strlen(header));
    handle_err("write header");
}

void body(char *devicespec, char *r, uint16_t len) {
    strcpy(r, "NO DATA");
    err = set_http_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
    handle_err("body chan mode");
    err = network_read(devicespec, (uint8_t *) r, len);
    handle_err("body read");
}

void set_json(char *devicespec) {
    add_header(devicespec, "Accept: application/json");
    add_header(devicespec, "Content-Type: application/json");
}

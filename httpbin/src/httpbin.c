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

char *httpbin = "N:HTTP://httpbin.org/";
char url_buffer[128];
char result[1024];
uint8_t err = 0;

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        exit(1);
    }
}

char *create_url(char *method) {
    sprintf(url_buffer, "%s%s", httpbin, method);
    return (char *) url_buffer;
}

uint8_t set_http_channel_mode(char *devicespec, uint8_t mode) {
// This demonstrates why library should have specialised functions, instead of consumers having to use ifdef
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

void post(char *devicespec, char *data) {
    err = set_http_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
    handle_err("post chan mode");
    err = network_write(devicespec, (uint8_t *) data, strlen(data));
    handle_err("post: write");
    err = set_http_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
    handle_err("body chan mode");
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

void main(void) {
    char *url;
    bzero(url_buffer, 128);
    bzero(result, 1024);

    gotox(0);
    printf("httpbin v1.0.6\n\n");

    // POST JSON, get result
    url = create_url("post");
    printf("Post to: [%s]\n", url);
    err = network_open(url, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
    handle_err("post:open");

    set_json(url);
    post(url, "{\"name\":\"fenrock\"}");
    err = network_json_parse(url);
    handle_err("post:json parse");
    network_json_query(url, "N:/json/name", result);
    handle_err("post:json query");
    printf("POST: name = %s\n", result);
    network_close(url);
    handle_err("post:close");

    exit(0);
}

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

void handle_err(uint8_t err, char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        exit(1);
    }
}

char *create_url(char *method) {
    sprintf(url_buffer, "%s%s", httpbin, method);
    return (char *) url_buffer;
}

void set_http_channel_mode(char *devicespec, uint8_t mode) {
// This demonstrates why library should have specialised functions, instead of consumers having to use ifdef
#ifdef BUILD_APPLE2
    // uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, [void *buffer]);
    network_ioctl('M', 0, mode, devicespec, 0);
#endif

#ifdef BUILD_ATARI
    // uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, [uint8_t dstats, uint16_t dbuf, uint16_t dbyt]);
    network_ioctl('M', 0, mode, devicespec, 0, 0, 0);
#endif
}

void add_header(char *devicespec, char *header) {
    set_http_channel_mode(devicespec, HTTP_CHAN_MODE_SET_HEADERS);
    network_write(devicespec, (uint8_t *) header, strlen(header));
}


void post(char *devicespec, char *data) {
    set_http_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
    network_write(devicespec, (uint8_t *) data, strlen(data));
    set_http_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}

void body(char *devicespec, char *r, uint16_t len) {
    strcpy(r, "NO DATA");
    set_http_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
    network_read(devicespec, (uint8_t *) r, len);
}

void set_json(char *devicespec) {
    add_header(devicespec, "Accept: application/json");
    add_header(devicespec, "Content-Type: application/json");
}

void main(void) {
    uint8_t err = 0;
    char *url;

    gotox(0);
    printf("httpbin v1.0.3\n\n");

    // POST JSON, get result
    url = create_url("post");
    err = network_open(url, OPEN_MODE_HTTP_POST, OPEN_TRANS_CR);
    set_json(url);
    post(url, "{\"name\":\"fenrock\"}");
    network_json_parse(url);
    network_json_query(url, "N:/json/name", result);
    printf("POST: name = %s\n", result);
    network_close(url);

    exit(0);
}

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
#include "httpbin.h"

char *httpbin = "N:HTTPS://httpbin.org/";
char *del_path = "N:HTTP://192.168.1.85:5987/delete";
char url_buffer[128];
char result[1024];
uint8_t err = 0;
char *url;
uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

char *version = "v1.2.0";

void main(void) {
    setup();

    printf("httpbin %s\n", version);
    printf("Base URL: %s\n", httpbin);

    test_post();
    test_put();
    test_delete();

    exit(0);
}

void setup() {
    bzero(url_buffer, 128);
    bzero(result, 1024);
    gotox(0);
}

// -------------------------------------------------------------------------------
// POST JSON, get result
void test_post() {
    url = create_url("post");
    err = network_open(url, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
    handle_err("post:open");

    post(url, "{\"name\":\"fenrock\"}");
    err = network_json_parse(url);
    handle_err("post:json parse");
    network_json_query(url, "N:/json/name", result);
    handle_err("post:json query");
    printf("/post   :  name=%s\n", result);
    network_close(url);
    handle_err("post:close");
}

// -------------------------------------------------------------------------------
// PUT JSON, get result. PUT is just a slightly different POST
void test_put() {
    url = create_url("put");
    err = network_open(url, OPEN_MODE_HTTP_PUT_H, OPEN_TRANS_NONE);
    handle_err("put:open");

    post(url, "{\"level\":11}");
    err = network_json_parse(url);
    handle_err("put:json parse");
    network_json_query(url, "N:/json/level", result);
    handle_err("put:json query");
    printf("/put    : level=%s\n", result);
    network_close(url);
    handle_err("put:close");
}

// -------------------------------------------------------------------------------
// DELETE - no data to send, but response will have data in it
void test_delete() {
    // url = del_path;
    url = create_url("delete");
    err = network_open(url, OPEN_MODE_HTTP_DELETE, OPEN_TRANS_NONE);
    handle_err("del:open");
    err = network_json_parse(url);
    handle_err("delete:json parse");
    network_json_query(url, "N:/headers/host", result);
    handle_err("delete:json query");
    printf("/delete :  host=%s\n", result);
    network_close(url);
    handle_err("delete:close");
}

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

void post(char *devicespec, char *data) {
    set_json(url);
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

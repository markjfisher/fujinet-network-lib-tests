#ifndef HTTPBIN_H
#define HTTPBIN_H

#include <stdint.h>

void add_header(char *devicespec, char *header);
void handle_err(char *reason);
uint8_t set_http_channel_mode(char *devicespec, uint8_t mode);
void set_json(char *devicespec);
void setup();

void body(char *devicespec, char *r, uint16_t len);

void do_query();

#endif
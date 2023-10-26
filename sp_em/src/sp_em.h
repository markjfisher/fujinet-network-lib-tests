#ifndef HTTPBIN_H
#define HTTPBIN_H

#include <stdint.h>

void handle_err(char *reason);
void hex_dump(uint8_t *p, uint16_t len);

#endif
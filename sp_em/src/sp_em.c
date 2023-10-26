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
#include "sp_em.h"

char *version = "v1.0.1";
uint8_t err;
uint8_t sp_id;
uint8_t* c000 = (uint8_t *) 0xC000;
uint8_t* c500 = (uint8_t *) 0xc500;

bool found_sp = false;

void main(void) {
    char c;
    int id;

    printf("sp emulator %s\n", version);

    sp_init();
    id = sp_find_network();
    printf("id: %d", id);

    // err = network_open("N:HTTP://foo.bar:8080/", 4, 0);
    // handle_err("open");
    // hex_dump(c500, 16);
    c = cgetc();
    printf("Exiting\n");

    exit(0);
}

void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        exit(1);
    }
}

void hex_dump(uint8_t *p, uint16_t len) {
    uint16_t i;
    uint16_t j;
    for (i = 0; i < len; i += 8) {
        printf("%04x : ", (unsigned int)(p + i));

        // Print up to 8 bytes per line
        for (j = 0; j < 8; j++) {
            if (i + j < len) {
                printf("%02x ", *(p + i + j));
                if (j == 3) {
                    printf(" ");  // Extra space after 4th byte
                }
            }
        }

        printf("\n");
    }
}
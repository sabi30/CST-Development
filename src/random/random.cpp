#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "rand.h"

i32 cst::get_random_linux(void) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        fd = open("/dev/random", O_RDONLY);
    }

    if (fd < 0) {
#ifdef CST_USE_LIBC
        perror("Failed to open /dev/urandom or /dev/random");
#endif
        return -1;
    }

    int rand_value = 0;
    ssize_t bytes_read = read(fd, &rand_value, sizeof(rand_value));
    close(fd);

    if (bytes_read != sizeof(rand_value)) {
        return -1;
    }

    return rand_value;
}

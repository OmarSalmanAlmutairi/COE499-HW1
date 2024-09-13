#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd;
    char *buffer;
    size_t size;

    // Get the system's physical memory size
    size = sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES);

    // Allocate a buffer
    buffer = malloc(size);
    if (!buffer) {
        perror("malloc");
        return 1;
    }

    // Open /dev/mem
    fd = open("/dev/mem", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Read from /dev/mem and write to the buffer
    if (read(fd, buffer, size) == -1) {
        perror("read");
        return 1;
    }

    // Write the buffer to a file
    if (write(1, buffer, size) == -1) {
        perror("write");
        return 1;
    }

    free(buffer);
    close(fd);

    return 0;
}

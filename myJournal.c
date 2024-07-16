#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int createDirectory(const char *path) {
    if (mkdir(path, 0777) == 0 || errno == EEXIST) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int main() {
    // Get current date time to get file name
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Create Journal folder
    char path[250];
    const char *homeDir = getenv("HOME");
    snprintf(path, sizeof(path), "%s/Journal", homeDir);
    createDirectory(path);

    // temp var for parsing strings
    char temp[12];

    // Create Year folder
    snprintf(temp, sizeof(path), "/%04d", tm.tm_year + 1900);
    strcat(path, temp);
    createDirectory(path);
    temp[0] = '\0';

    // Create Day folder
    snprintf(temp, sizeof(path), "/%02d", tm.tm_mon + 1);
    strcat(path, temp);
    createDirectory(path);
    temp[0] = '\0';

    // Create file
    snprintf(temp, sizeof(path), "/%02d%s", tm.tm_mday, ".txt");
    strcat(path, temp);

    // Open the file in append mode, which creates the file if it does not exist
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        perror("Error opening file");
        return errno;
    }

    // Close the file
    fclose(file);

    char command[256];
    snprintf(command, sizeof(command), "nvim %s", path);

    // Execute the command
    int status = system(command);

    if (status != 0) {
        fprintf(stderr, "Failed to open file in nvim\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
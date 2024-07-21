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
    char plannerPath[250];
    const char *homeDir = getenv("HOME");
    snprintf(path, sizeof(path), "%s/Journal", homeDir);
    snprintf(plannerPath, sizeof(plannerPath), "%s/Journal", homeDir);
    createDirectory(path);

    // temp var for parsing strings
    char temp[20];

    // Create Year folder
    snprintf(temp, sizeof(path), "/%04d", tm.tm_year + 1900);
    strcat(path, temp);
    strcat(plannerPath, temp);
    createDirectory(path);
    temp[0] = '\0';

    // Create Day folder
    snprintf(temp, sizeof(path), "/%02d", tm.tm_mon + 1);
    strcat(path, temp);
    strcat(plannerPath, temp);
    createDirectory(path);
    temp[0] = '\0';

    // Create journal file
    snprintf(temp, sizeof(temp), "/%02d%s", tm.tm_mday, ".txt");
    strcat(path, temp);
    temp[0] = '\0';

    FILE *journalFile = fopen(path, "a");
    if (journalFile == NULL) {
        perror("Error opening file");
        return errno;
    }
    fclose(journalFile);

    // Create planner file
    snprintf(temp, sizeof(temp), "/%02dPlanner%s", tm.tm_mday, ".txt");
    strcat(plannerPath, temp);
    temp[0] = '\0';

    FILE *plannerFile = fopen(plannerPath, "a+");
    if (plannerFile == NULL) {
        perror("Error opening file");
        return errno;
    }
    fseek(plannerFile, 0, SEEK_END); // Move the file pointer to the end of the file
    long fileSize = ftell(plannerFile); // Get the current file pointer position, which represents the file size
    if (fileSize == 0) {
        // Write sequence of numbers starting at 6 with two empty lines between each
        for (int i = 6; i <= 22; ++i) { // You can change the end value as needed
            fprintf(plannerFile, "%d\n\n\n", i);
        }
    }
    fclose(plannerFile);

    char command[800];
    snprintf(command, sizeof(command), "nvim -O %s %s", path, plannerPath);

    // Execute the command
    int status = system(command);

    if (status != 0) {
        fprintf(stderr, "Failed to open file in nvim\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>


int FILE_COUNTER = 0;

struct File {
        char path [500];
        char name [100];
        int size;
        };

int sort_by_size(const void *a, const void *b) {
        struct File *fileA = (struct File *)a;
        struct File *fileB = (struct File *)b;
        return (fileA->size - fileB->size);
}

int sort_by_name(const void *a, const void *b) {
        struct File *fileA = (struct File *)a;
        struct File *fileB = (struct File *)b;
        return(strcmp(fileA->name, fileB->name));
}

void sort_files(struct File *files, int sort_int) {
        if (sort_int == 1) {
                qsort (files, FILE_COUNTER, sizeof(struct File), sort_by_name);
        } else if (sort_int == 2) {
                qsort (files, FILE_COUNTER, sizeof(struct File), sort_by_size);
        }
}

int copy_files(struct File *files, char *path) {
        DIR *dir;
        FILE *file;
        FILE *output;
        char c;

        if((dir=opendir(path))==NULL) {
                printf("Error with open dir\n");
                return -1;
        }
        
        for (int i=0; i < FILE_COUNTER; i++) {
                char destination[strlen(path) + strlen(files[i].name) + 1];
                sprintf(destination, "%s%s%s", path, "/", files[i].name);

                if (((file=fopen(files[i].path, "r"))==NULL) ||
                        ((output=fopen(destination, "w"))==NULL)){
                        printf("Errorwith open file\n");
                        return -1;
                }
                c = fgetc(file);
                while (c != EOF) {
                        fputc(c, output);
                        c = fgetc(file);
                }
                if (fclose(file)!=0 || fclose (output) !=0){
                        printf ("Error with close file\n");
                        return -1;
                }
                printf ("%s - %s - %d\n", files[i].path, files[i].name, files[i].size);
        }
        return 0;
}

int read_directory(struct File *files, char *path) {
        DIR *dir;
        struct stat buffer;
        struct dirent *next_item;
        int result = 0;

        if((dir=opendir(path))==NULL) {
                printf("Error with open dir/n");
                return -1;
        }
        next_item = readdir(dir);

        while(next_item != NULL) {
                char path_to_item[strlen(path) + strlen(next_item->d_name) + 1];
                sprintf(path_to_item, "%s%s%s", path, "/", next_item->d_name);
                lstat (path_to_item, &buffer);

                if(S_ISDIR(buffer.st_mode)){
                        if((strcmp(next_item->d_name, "..")!=0) &&
                                (strcmp(next_item->d_name, ".")!=0)){
                                result = read_directory(files, path_to_item);
                        }
                }
                else if(S_ISREG(buffer.st_mode)) {
                        struct File file;
                        strcpy(file.name, next_item->d_name);
                        strcpy(file.path, path_to_item);
                        file.size = (int)buffer.st_size;
                        files[FILE_COUNTER] = file;
                        FILE_COUNTER++;
                }
                next_item = readdir(dir);
        }
        if((closedir(dir))!=0) {
                printf("Error with close dir/n");
                return -1;
        }
        return result;
}

int main(int argc, char *argv[]) {
        struct File files[100];
        int result;
        int sort_int = atoi(argv[2]);

        char from_dir[100];
        realpath(argv[1], from_dir);

        char to_dir[100];
        realpath(argv[3], to_dir);

        if ((sort_int != 1) && (sort_int != 2)) {
                printf("Please enter correct sort number\n");
                return -1;
        }
        result = read_directory(files, from_dir);
        if(result != 0) {
                return result;
        }
        sort_files(files, sort_int);
        result = copy_files(files, to_dir);
        return result;
}

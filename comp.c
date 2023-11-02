#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <pthread.h>

// TODO:
// do next:
// - add colored output text and remove old output debug info
//   - while compiling: yellow, when compiled: green, when error: red
//   - when running make output green if correct, else make output red and correct output green 
//      (also maybe turn around order: exptected, got)
// - make exercises

struct Exercise {
    char* file;
    char* output;
    char* hint;
    char* solution;
};

struct Exercise exercises[] = {
    {
        .file = "001_hello_world.c",
        .output = "hello world",
        .hint = "call the printf function",
        .solution = "call the print function by replacing ??? with printf",
    },
    {
        .file = "002_variables.c",
        .output = "number 1 + number 2 = 150",
        .hint = "1. enter the correct data types for number1 and number2 \n"
                "2. use the right variable in the print statement",
        .solution = "both number1 and number2 are integer types. replace ??? with number2 in the print statement",
    }
};

char* get_compiler() {
    char* compiler = malloc(256 * sizeof(char));
    FILE* fptr = fopen(".compiler", "r");
    if (fptr == NULL) {
        return NULL;
    }

    fgets(compiler, 256, fptr);
    return compiler;
}
int set_compiler(char* compiler) {
    if (access(compiler, X_OK) == 0) {
        return -1; // COMPILER DOES NOT EXIST
    }
    FILE* fptr = fopen(".compiler", "w");
    if (fptr == NULL) {
        return -2; // WRITE ERROR
    }
    fprintf(fptr, "%s", compiler);
    return 0; // everything fine 
}

// TODO: make directory dependent on where you are // maybe dependent on comp file itself or some other .config file (or .compiler)
int compile_curr(const struct Exercise* file, const char* compiler) {
    // remove .c suffix
    char* name_source = (char*) file->file;
    char* name_exe = (char*) malloc(strlen(name_source) + 1);

    strcpy(name_exe, name_source); // copy name_source
    char *dot = strrchr(name_exe, '.'); // find last occurance of .
    if (dot != NULL) {
        *dot = '\0'; // remove .c
    }

    char* command = (char*) malloc(1024 * sizeof(char));
    snprintf(command, 1024, "%s ./exercises/%s -o ./build/%s", compiler, name_source, name_exe);
    int ret = system(command);

    free(command);
    free(name_exe);

    if (ret != 0) {
        printf("compiled with errors\n");
        return 1; 
    }

    printf("compiled successfully\n");
    return 0;
}

int run_curr(const struct Exercise* file) {
    char* name_exe = (char*) malloc(strlen(file->file) + 1);

    strcpy(name_exe, file->file); // copy name_source
    char *dot = strrchr(name_exe, '.'); // find last occurance of .
    if (dot != NULL) {
        *dot = '\0'; // remove .c
    }

    char* command = (char*) malloc(1024 * sizeof(char));
    snprintf(command, 1024, "./build/%s > ./out/%s.output", name_exe, name_exe);
    system(command);

    char* path_out = (char*) malloc(1024 * sizeof(char));
    snprintf(path_out, 1024, "./out/%s.output", name_exe);
    unsigned int out_size = 1 * 1024;
    char output[out_size];

    FILE* fptr;
    fptr = fopen(path_out, "r");
    // ERROR HANDLING
    fgets(output, out_size, fptr);
    fclose(fptr);

    if (strcmp(file->output, output) == 0) {
        printf("CORRECT\n");
        printf("%s", output);
        printf("\n");
        return 0;
    } else {
        printf("INCORRECT\n");
        printf("expected output:\n%s\n", file->output);
        printf("your output:\n%s", output);
        printf("\n");
        return 1;
    }
}

void* monitor_directory(void* arg) {
    const char* directory = ((const char**)arg)[0];
    const char* compiler = ((const char**)arg)[1];

    struct stat current_stat, previous_stat;

    // Initial directory status
    if (stat(directory, &previous_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    
    int initial_comp = -1;
    for (int i = 0; i < sizeof(exercises) / sizeof(struct Exercise);) {
        // Get the current directory status
        if (stat(directory, &current_stat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // Check for changes in the directory
        if (current_stat.st_mtime != previous_stat.st_mtime || initial_comp != i) {
            initial_comp = i;

            printf("compiling: ");
            printf("%s\n", exercises[i].file);
            fflush(stdout);

            // if it compiled and ran successfully go to next task
            // THIS HANDLES COMPILING
            if (compile_curr(&exercises[i], compiler) == 0 ) {
                // THIS HANDLES RUNNING THE FILE AND COMPARING OUTPUT WITH SOLUTION
                if (run_curr(&exercises[i]) == 0) {
                    i++;
                }
            }
        }
        previous_stat = current_stat;

        // Sleep for a specified interval (e.g., 1 second)
        sleep(1);
    }

    pthread_exit(NULL);
}

int watch(char* compiler) {
    pthread_t thread;

    char* directory = "./exercises/";

    // spawn watch thread and watch the files, meanwhile wait for user input (scanf)
    void* args[] = {directory, compiler};
    if (pthread_create(&thread, NULL, monitor_directory, args) != 0 ) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }


    if (pthread_join(thread, NULL) != 0) {
        perror("watch_error");
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    char* compiler = get_compiler();

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--set-compiler") == 0) {
            if( argc < i+1 ) {
                printf("Missing arguments to %s \n"
                        "expected path to compiler try adding a path to an executable \n"
                        "example for arguments to %s: \n"
                        "    path/to/compiler (usr/bin/gcc) \n"
                        "    compile_name (gcc) \n", 
                        argv[i], argv[i]);
            } else {
                int sce = set_compiler(argv[i+1]);
                switch (sce) {
                    case -1:
                        printf("compiler executable not found - compiler not set");
                        return -1;
                    case -2:
                        printf("could not save compiler to .compiler - compiler not set");
                        return -1;
                }
            }
        }

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("c-lings help menu \n\n"
                   "    -c, --set-compiler [path] - set the path to your c compiler\n"
                   "                                (example: /usr/bin/gcc, gcc)\n"
                   "    watch                     - wait for code changes and compile\n"
                   "                                and check for errors\n"
                   "    -h , --help               - display this help menu \n\n"
                   "watch help menu:\n\n"
                   "    hint                      - display hints\n"
                   "    solution                  - give the solution to the problem\n");
        } else if (strcmp(argv[i], "watch") == 0) {
            int err = watch(compiler);
            if ( err != 0) {
                return -1; 
            }
        }
    }
    
    free(compiler);
    return 0;
}

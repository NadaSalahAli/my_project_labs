#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_VARS 100

char* environment_vars[MAX_VARS];
int env_count = 0;
char log_filename[] = "shell.log";

void parse_input(char* input, char** args, int* arg_count, int* background);
void execute_command(char** args, int arg_count, int background);
void execute_builtin(char** args, int arg_count);
void change_directory(char* path);
void echo_command(char** args, int arg_count);
void export_command(char** args, int arg_count);
void write_to_log(const char* message);
void expand_variables(char** args, int* arg_count);
char* get_variable_value(char* name);

void write_to_log(const char* message) {
    FILE* log_file = fopen(log_filename, "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s", message);
        fclose(log_file);
    }
}

void parse_input(char* input, char** args, int* arg_count, int* background) {
    *background = 0;
    *arg_count = 0;
    input[strcspn(input, "\n")] = '\0';

    char* token = strtok(input, " \t");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1;
        } else {
            args[(*arg_count)++] = token;
        }
        token = strtok(NULL, " \t");
    }
    args[*arg_count] = NULL;
}

void expand_variables(char** args, int* arg_count) {
    for (int i = 0; i < *arg_count; i++) {
        char* arg = args[i];
        if (arg[0] == '$') {
            char* var_name = arg + 1;
            char* var_value = get_variable_value(var_name);
            if (var_value != NULL) {
                args[i] = var_value;
            }
        }
    }
}

char* get_variable_value(char* name) {
    char* env_value = getenv(name);
    if (env_value != NULL) return env_value;

    for (int i = 0; i < env_count; i++) {
        char* var = environment_vars[i];
        char* eq = strchr(var, '=');
        if (eq != NULL) {
            *eq = '\0';
            if (strcmp(var, name) == 0) {
                *eq = '=';
                return eq + 1;
            }
            *eq = '=';
        }
    }
    return NULL;
}

void execute_builtin(char** args, int arg_count) {
    if (strcmp(args[0], "cd") == 0) {
        change_directory(arg_count > 1 ? args[1] : getenv("USERPROFILE"));
    } else if (strcmp(args[0], "echo") == 0) {
        echo_command(args, arg_count);
    } else if (strcmp(args[0], "export") == 0) {
        export_command(args, arg_count);
    }
}

void change_directory(char* path) {
    if (_chdir(path) != 0) {
        perror("cd");
    }
}

void echo_command(char** args, int arg_count) {
    for (int i = 1; i < arg_count; i++) {
        char* arg = args[i];
        size_t len = strlen(arg);
        if (len > 1 && arg[0] == '"' && arg[len - 1] == '"') {
            arg[len - 1] = '\0';
            arg++;
        }
        printf("%s ", arg);
    }
    printf("\n");
}

void export_command(char** args, int arg_count) {
    if (arg_count < 2) return;

    char* var = args[1];
    char* eq = strchr(var, '=');

    if (eq != NULL) {
        *eq = '\0';
        char* name = var;
        char* value = eq + 1;

        if (value[0] == '"' && value[strlen(value) - 1] == '"') {
            value[strlen(value) - 1] = '\0';
            value++;
        }

        _putenv_s(name, value);

        if (env_count < MAX_VARS) {
            char* new_var = malloc(strlen(name) + strlen(value) + 2);
            sprintf(new_var, "%s=%s", name, value);
            environment_vars[env_count++] = new_var;
        }
    }
}

void execute_command(char** args, int arg_count, int background) {
    char command_line[MAX_INPUT] = "";
    for (int i = 0; i < arg_count; i++) {
        strcat(command_line, args[i]);
        if (i < arg_count - 1) strcat(command_line, " ");
    }

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed: %lu\n", GetLastError());
        return;
    }

    if (!background) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    } else {
        printf("[%lu]\n", pi.dwProcessId);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    write_to_log("Shell started\n");

    char input[MAX_INPUT];
    char* args[MAX_ARGS];
    int arg_count, background;

    while (1) {
        printf("myshell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        if (strcmp(input, "\n") == 0) continue;

        parse_input(input, args, &arg_count, &background);
        if (arg_count == 0) continue;

        if (strcmp(args[0], "exit") == 0) {
            write_to_log("Shell exited\n");
            break;
        }

        expand_variables(args, &arg_count);

        if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "echo") == 0 || strcmp(args[0], "export") == 0) {
            execute_builtin(args, arg_count);
        } else {
            execute_command(args, arg_count, background);
        }
    }

    for (int i = 0; i < env_count; i++) {
        free(environment_vars[i]);
    }

    return 0;
}

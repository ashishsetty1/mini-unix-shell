#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_HISTORY 100

char history[MAX_HISTORY][MAX_INPUT];
int history_count = 0;

void add_history(char *input) {
    if (history_count < MAX_HISTORY) {
        strcpy(history[history_count], input);
        history_count++;
    }
}

void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s", i + 1, history[i]);
    }
}

void parse_command(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int check_background(char **args) {
    int i = 0;
    while (args[i] != NULL) i++;

    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        args[i - 1] = NULL;
        return 1;
    }
    return 0;
}

int has_pipe(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) return i;
    }
    return -1;
}

void execute_pipe(char **args, int pipe_index) {
    args[pipe_index] = NULL;

    char **left = args;
    char **right = &args[pipe_index + 1];

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        execvp(left[0], left);
        perror("exec");
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        execvp(right[0], right);
        perror("exec");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;
        if (strcmp(input, "\n") == 0) continue;

        add_history(input);
        parse_command(input, args);

        if (!args[0]) continue;

        if (strcmp(args[0], "exit") == 0) break;

        if (strcmp(args[0], "history") == 0) {
            show_history();
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1]) chdir(args[1]);
            else printf("cd: missing directory\n");
            continue;
        }

        int pipe_index = has_pipe(args);
        if (pipe_index != -1) {
            execute_pipe(args, pipe_index);
            continue;
        }

        int background = check_background(args);

        pid_t pid = fork();

        if (pid == 0) {
            execvp(args[0], args);
            perror("exec");
            exit(1);
        } else {
            if (!background) wait(NULL);
            else printf("[background pid %d]\n", pid);
        }
    }

    return 0;
}
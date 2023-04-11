#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 5

int main(void) {
    int keep_going = 1;
    char *args[MAX_LINE/2 + 1];
    char history_stack[MAX_HISTORY][MAX_LINE + 1] = {"",};
    while (keep_going) {
        printf("-osh>>");
        fflush(stdout);

        char input_char;
        char user_input[MAX_LINE + 1] = {0};
        int char_count = 0;

		while ((input_char = getchar()) != '\n' && input_char != EOF)
		{
			if(char_count < MAX_LINE) 
                user_input[char_count] = input_char;
			else if(char_count == MAX_LINE) 
                printf("[錯誤] 命令過長(最多 %d字元)\n", MAX_LINE);
			char_count++;
		}
		if(char_count >= MAX_LINE || char_count == 0) 
            continue; // input check
        user_input[MAX_LINE] = 0; // end of char[] '\0'

        if (strcmp(user_input, "exit") == 0){
            return 0;
        }

        if (strcmp(user_input, "history") == 0){
            for (int stack_i = 0; stack_i < MAX_HISTORY; stack_i++) {
                printf("%d %s\n", stack_i, history_stack[stack_i]);
            }
            continue;
        } else {
            for (int stack_i = MAX_HISTORY - 1; stack_i > 0; stack_i--) {
                strcpy(history_stack[stack_i], history_stack[stack_i - 1]);
            }
            strcpy(history_stack[0], user_input);
        }

        pid_t pid = fork();
        if (pid < 0){
            perror("fork()");
            return 1;
        } else if (pid == 0) {
            char_count = 0;
            char *argv[MAX_LINE/2 - 1];
            char *p;
            p = strtok(user_input, " ");
            argv[char_count++] = p;
            while( p != NULL ) {
                p = strtok(NULL, " ");
                argv[char_count++] = p;
            }
            execvp(argv[0], argv);
            exit(0);
		}
		else wait(NULL);
    }
    return 0;
}
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASH_TOK_BUFSIZE 64
#define ASH_TOK_DELIM " \t\r\n\a"

int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&ash_cd,
	&ash_help,
	&ash_exit
};

int lsh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

int ash_cd(char **args)
{
	if (args[1] == NULL) {
		fprintf(stderr, "ash: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("lsh");
		}
	}
	return 1;
}

int ash_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		// An empty command was entered.
		return 1;
	}

	for (i = 0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return ash_launch(args);
}


int ash_help(char **args)
{
	int i;
	printf("Adam Idris's ASH\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < lsh_num_builtins(); i++) {
		printf("  %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int ash_exit(char **args)
{
	return 0;
}

int ash_launch(char **args){
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0){
		if(execvp(args[0], args) == -1){
			perror("ash");
		}
		exit(EXIT_FAILURE);
	}else if (pid < 0){
		perror("lsh");
	}else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;

}


char **ash_split_line(char *line){
	int bufsize = ASH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, ASH_TOK_DELIM);

	while(token !=  NULL){
		tokens[position] = token;
		position++;

		if(position >= bufsize){
			bufsize += ASH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if(!tokens){
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);

			}
		}

		token = strtok(NULL, ASH_TOK_DELIM);
	}

	tokens[position] = NULL;
	return tokens;

}



#define ASH_RL_BUFFER 1024
char *ash_read_line(void){
	int bufsize = ASH_RL_BUFFER;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);

	int c;

	if(!buffer){
		fprintf(stderr, "ash: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		c = getchar();

		if (c == EOF || c  == '\n'){
			buffer[position] = '\0';
			return buffer;

		}else{
			buffer[position] = c;
		}
		position ++;

		if (position >= bufsize){
			bufsize += ASH_RL_BUFFER;
			buffer = realloc(buffer, bufsize);

			if(!buffer){
				fprintf(stderr, "ash: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return buffer;
}


void ash_loop(void){
	char *line;
	char **args;
	int status;


	do{
		printf("> ");
		line = ash_read_line();
		args = ash_split_line(line);
		status = ash_execute(args);

		free(line);
		free(args);
	}while(status);

}


int main(int argc, char **argv){

	ash_loop();

	return EXIT_SUCCESS;
}

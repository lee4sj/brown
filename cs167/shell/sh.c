#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_READ_BUFFER 1024

char *PATH = "/bin:/usr/local/bin";
/**
 * returns 1 if the command is built-in command, otherwise return 0.
 * stores arguments in arg_array param.
 */
int parse_command(char *read_buffer, char ***arg_array)
{
	int returncode = 0;

	//get the total number of words
	int arg_count = 0;
	char *temp = read_buffer;
	while ((temp = strpbrk(temp, " \t\n\0")) != NULL) {
		arg_count++;
		temp += strspn(temp, " \t\n\0");
	}

	*arg_array = (char **)malloc(sizeof(char *) *
				     ((unsigned int)arg_count + 1));
	(*arg_array)[arg_count] = NULL;

	temp = read_buffer;
	size_t size = 0;
	for (int i = 0; i < arg_count; i++)
	{
		size = strcspn(temp, " \t\n\0") + 1;
		if (size == 0) {
			write(STDOUT_FILENO, "2: strcspn failed!\n", 19);
			return -1;
		}

		(*arg_array)[i] = (char *)malloc(size * sizeof(char));
		if (snprintf((*arg_array)[i], size, "%s", temp) < 0) {
			write(STDOUT_FILENO, "error in snprintf\n", 19);
			return -1;
		}

		temp += size + strspn(temp + size, " \t\n\0");
	}

	//check for built-in command
	if (!strncmp((*arg_array)[0], "cd", 2) ||
	    !strncmp((*arg_array)[0], "ln", 2) ||
	    !strncmp((*arg_array)[0], "rm", 2) ||
	    !strncmp((*arg_array)[0], "exit", 4))
		returncode = 1;

	return returncode;
}

int do_redirection_stdin(char *command, char **file)
{
	char *cur_pos = strpbrk(command, "<");
	if (cur_pos) {
		cur_pos[0] = ' ';
		cur_pos += strspn(cur_pos, " \t");

		//error checking
		size_t size = strcspn(cur_pos, "<>\n\0");
		if (size == 0){
			write(STDOUT_FILENO,
			      "expected filename after IO redirection\n", 39);
			return -1;
		}

		size = strcspn(cur_pos, " \t<>\n\0");
		*file = (char *) malloc(sizeof(char) * (size + 1));
		strncpy(*file, cur_pos, size);
		(*file)[size] = '\0';
		memset(cur_pos, ' ', size);

		if (strpbrk(cur_pos, "<") != NULL) {
			free(*file);
			*file = NULL;
			do_redirection_stdin(command, file);
		}

		return 1;
	}

	return 0;
}

int do_redirection_stdout(char *command, char **file, int *stdout_flag)
{
	char *cur_pos = strpbrk(command, ">");
	if (cur_pos) {
		if (strncmp(&cur_pos[1], ">", 1))
			*stdout_flag = O_WRONLY | O_TRUNC;
		else {
			*stdout_flag = O_RDWR | O_APPEND;
			cur_pos[0] = ' ';
			cur_pos++;
		}
		*stdout_flag |= O_CREAT;

		cur_pos[0] = ' ';
		cur_pos += strspn(cur_pos, " \t");

		//error checking
		size_t size = strcspn(cur_pos, "<>\n\0");
		if (size == 0){
			write(STDOUT_FILENO,
			      "expected filename after IO redirection\n", 39);
			return -1;
		}

		size = strcspn(cur_pos, " \t<>\n\0");
		*file = (char *) malloc(sizeof(char) * (size + 1));
		strncpy(*file, cur_pos, size);
		(*file)[size] = '\0';
		memset(cur_pos, ' ', size);

		if (strpbrk(cur_pos, ">") != NULL) {
			free(*file);
			*file = NULL;
			do_redirection_stdout(command, file, stdout_flag);
		}

		return 1;
	}

	return 0;
}

void recursive_remove(char *path)
{
	DIR *dirp;
	struct dirent *dir = NULL;

	if ((dirp = opendir(path)) == NULL) {
		if (errno == ENOTDIR)
                        if (unlink(path) < 0)
				perror("failed to rm");
		return;
	}

	char dir_path[MAX_READ_BUFFER + 1];
	strcpy(dir_path, path);
	if (dir_path[strlen(dir_path)] != '/') {
		dir_path[strlen(dir_path)] = '/';
	}

	do {
		if ((dir = readdir(dirp)) == NULL)
			continue;

		if (!strncmp(dir->d_name, "..", 2) ||
		    !strncmp(dir->d_name, ".", 1))
			continue;

		char file_path[MAX_READ_BUFFER + 1];
		snprintf(file_path, MAX_READ_BUFFER,
			 "%s%s", dir_path, dir->d_name);

	        if (unlink(file_path)) {
			if (errno == EISDIR) {
				recursive_remove(file_path);
			}

			else {
				perror("failed to rm");
			}
		}
	} while (dir != NULL);

	if (rmdir(path) == -1)
		perror("failed to remove dir");
}

/**
 * returns 1 if found, 0 if not found, -1 if error
 */
int look_up_dir(char *path, char *filename) {
	DIR *dirp;
	struct dirent *dir = NULL;

	
	if ((dirp = opendir(path)) == NULL) {
		return -1;
	}

	char dir_path[MAX_READ_BUFFER + 1];
	strcpy(dir_path, path);
	if (dir_path[strlen(dir_path)] != '/') {
		dir_path[strlen(dir_path)] = '/';
	}

	do {
		if ((dir = readdir(dirp)) == NULL)
			continue;

		if (!strcmp(dir->d_name, filename))
			return 1;

	} while (dir != NULL);

	return 0;
}

int path_resolution(char *command, char *result) {
	if (command[0] == '/' ||
	    !strncmp(command, "./", 2) ||
	    !strncmp(command, "../", 2)) {
		strcpy(result, command);
		return 1;
	}

	if (!strcmp(PATH, ""))
		return 0;

	char *cur_pos = PATH - 1;
	do {
		cur_pos++;
		size_t size = strcspn(cur_pos, ":\n\0");
		char path[size + 1];
		strncpy(path, cur_pos, size);
		path[size] = '\0';
		if (look_up_dir(path, command)) {
			snprintf(result, MAX_READ_BUFFER,
				 "%s/%s", path, command);
			return 1;
		}
	} while ((cur_pos = strpbrk(cur_pos, ":")) != NULL);

	return 0;
}

void do_builtin_command(char *argv[])
{
	if (!strncmp(argv[0], "cd", 2)) {
		if (chdir(argv[1]) < 0)
			perror("failed to cd ");
	}

	else if (!strncmp(argv[0], "ln", 2)) {
		if (link(argv[1], argv[2]) < 0)
			perror("failed to ln ");
	}

	else if (!strncmp(argv[0], "rm", 2)) {
		char *path = argv[1];
		int index = 1;
		while (path != NULL) {
			if (!strncmp(path, "-r", 2)) {
				index++;
				path = argv[index];
				recursive_remove(path);
			}

			else if (unlink(path) < 0)
				perror("failed to rm ");

			index++;
			path = argv[index];
		}
	}

	else if (!strncmp(argv[0], "exit", 4))
		exit(0);
}

int main()
{
	char *envp[] = { NULL };
	for(;;)
	{
		write(STDOUT_FILENO, "$ ", 2);
		char read_buffer[MAX_READ_BUFFER + 1];
		ssize_t bytes_read = 0;
		if ((bytes_read =
		     read(STDIN_FILENO, read_buffer, MAX_READ_BUFFER)) == -1) {
			perror("failed to read the command");
		}
		read_buffer[bytes_read] = '\0';

		char **arg_array = NULL;

		//redirection
		char *stdin_file = NULL;
		char *stdout_file = NULL;
		int flags = 0;
		do_redirection_stdin(read_buffer, &stdin_file);
		do_redirection_stdout(read_buffer, &stdout_file, &flags);

		int builtin = parse_command(read_buffer, &arg_array);

		pid_t pid;
		//if built-in command
		if (builtin)
			do_builtin_command(arg_array);			

		//if child
		if (!builtin && (pid = fork()) == 0) {
			if (stdin_file) {
				close(STDIN_FILENO);
				if (open(stdin_file, O_RDONLY) == -1) {
					perror("could not open!");
					exit(1);
				}
			}

			if (stdout_file) {
				mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;
				close(STDOUT_FILENO);
				if (open(stdout_file, flags, mode) == -1) {
					perror("could not open!");
					exit(1);
				}
			}

			char result[MAX_READ_BUFFER + 1];
			path_resolution(arg_array[0], result);

			execve(result, arg_array, envp);
			perror("failed to execve");
			exit(1);
		}

		int status;
		pid_t temp_pid;
		while (!builtin && pid != (temp_pid = wait(&status))) {
			if (temp_pid == -1) {
				perror("failed to wait");
				exit(1);
			}
		}

		//free variables.
		for (int i = 0; arg_array[i] != NULL; i++) {
			free(arg_array[i]);
		}
		free(arg_array);

		if (stdin_file)
			free(stdin_file);
		if (stdout_file)
			free(stdout_file);

		
	}

	return 0;
}

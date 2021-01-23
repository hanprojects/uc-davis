#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMDLINE_MAX 512
#define ARG_NUM_MAX 16
#define TOKEN_CHAR_MAX 32
#define VAR_SIZE 26
#define PIPE_MAX_SIZE 4

// Global table to store value of a-z variables from `set function
char VALUE_TABLE[VAR_SIZE][VAR_SIZE] = {"", "", "", "", "" , "" , "" , "", "", "", 
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

// Global struct to store arguments for multiple commands to pipe
typedef struct Pipes
{
        char cmd[CMDLINE_MAX];
        char *args[CMDLINE_MAX];
        int exit_status;
} Pipes;

// ------------------------------------------------------------------------------- //
// ----------------------------- UTILITY FUNCTIONS ------------------------------- //
// ------------------------------------------------------------------------------- //

// FUNCTION TO SCAN COMMAND
// Source : https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c
char *get_arguments(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX])
{
        if (sizeof(*cmd) != 0) // make sure it is not null
        {
                int i = 0;

                // Split tokens by whitespace
                char *token = strtok(cmd, " \t");
                
                while (token != NULL)
                {
                        arguments[i] = token;
                        token = strtok(NULL, " \t");
                        i++;
                }
        }

        return *arguments;
}

// FUNCTION TO REMOVE LEADING WHITESPACES
// Source: https://www.geeksforgeeks.org/c-program-to-trim-leading-white-spaces-from-string/
char* remove_leading_whitespaces(char *cmd)
{
        static char cmd_copy[CMDLINE_MAX];
        int i = 0;
        int j, k;

        while (cmd[i] == ' ' || cmd[i] == '\t')
        {
                i++;
        }

        for (j = i, k = 0; cmd[j] != '\0'; j++, k++)
        {
                cmd_copy[k] = cmd[j];
        }

        cmd_copy[k] = '\0';

        return cmd_copy;
}

// FUNCTION TO REMOVE TRAILING WHITESPACES
// Source: https://codeforwin.org/2016/04/c-program-to-trim-trailing-white-space-characters-in-string.html
char* remove_trailing_space(char *cmd)
{
        int index, i;

        // Set default index
        index = -1;

        // Find last index of non-white space character
        i = 0;
        while(cmd[i] != '\0')
        {
                if(cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
                {
                        index = i;
                }

                i++;
        }

        // Mark next character to last non-white space character as NULL
        cmd[index + 1] = '\0';

        return cmd;
}

// FUNCTION TO FIND NUMBER OF ARGUMENTS IN COMMAND
int count_argument_number(char *arguments[CMDLINE_MAX])
{
        int arg_count = 0;

        while (arguments[arg_count])
        {
                arg_count++;
        }

        return arg_count;
}

// FUNCTION TO FIND LENGTH IN COMMAND
int count_command_length(char cmd[CMDLINE_MAX])
{
        int count = 0;

        while (cmd[count])
        {
                count++;
        }

        return count;
}

// FUNCTION TO GET ARGUMENTS FOR OUTPUT REDIRECTION
char *get_output_redir_args(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX])
{
        if (sizeof(*cmd) != 0) // make sure it is not null
        {
                int i = 0;
                
                // Split tokens by whitespace and ">"
                char *token = strtok(cmd, " >\t");

                while (token != NULL)
                {
                        arguments[i] = token;
                        token = strtok(NULL, " >\t");
                        i++;
                }
        }

        return *arguments;
}

// FUNCTION TO DETECT OUTPUT REDIRECTION
int return_output_redirection_status(char cmd[CMDLINE_MAX])
{
        int redir_status = 0;
        int i = 0;

        // Detect if there is ">" in the command line
        while (cmd[i] != '\0')
        {
                if (cmd[i] == '>')
                {
                        redir_status = 1;
                        break;
                }

                i++;
        }

        return redir_status;
}

// FUNCTION TO GET NUMBER OF PIPES
int count_pipe_number(char cmd[CMDLINE_MAX])
{
        int pipe_count = 0;
        int i = 0;

        // Detect if there is "|" in the command line then increment the pipe count
        while (cmd[i] != '\0')
        {
                if (cmd[i] == '|')
                {
                        pipe_count++;
                }

                i++;
        }

        return pipe_count;
}

// --------------------------------------------------------------------------------------- //
// ---------------------------- COMPLETED MESSAGE FUNCTIONS ------------------------------ //
// --------------------------------------------------------------------------------------- //

// FUNCTION TO PRINT "+ completed ..." MESSAGE (success case)
void print_complete_success_message(char cmd[CMDLINE_MAX], int retval)
{
        fprintf(stderr, "+ completed '%s' [%d]\n", cmd, WEXITSTATUS(retval));
}

// FUNCTION TO PRINT "+ completed ..." MESSAGE (error case)
void print_complete_error_message(char cmd[CMDLINE_MAX])
{
        fprintf(stderr, "+ completed '%s' [%d]\n", cmd, 1);
}

// --------------------------------------------------------------------------------------- //
// ---------------------------- ERROR MANAGEMENT FUNCTIONS ------------------------------- //
// --------------------------------------------------------------------------------------- //

// FUNCTION TO DETECT INVALID VARIABLE WITH `$` (e.g. `echo $a $?`)
int detect_dollar_sign_error(char *arguments[CMDLINE_MAX])
{
        int i = 1;
        int error = 0;

        while (arguments[i])
        {
                char initial[1];
                memcpy(initial, arguments[i], 1);

                if (!strcmp(initial, "$"))
                {
                        if (!strcmp(arguments[i], "$a") || !strcmp(arguments[i], "$b") || !strcmp(arguments[i], "$c") || !strcmp(arguments[i], "$d")
                                || !strcmp(arguments[i], "$e") || !strcmp(arguments[i], "$f") || !strcmp(arguments[i], "$g") || !strcmp(arguments[i], "$h")
                                || !strcmp(arguments[i], "$i") || !strcmp(arguments[i], "$j") || !strcmp(arguments[i], "$k") || !strcmp(arguments[i], "$l")
                                || !strcmp(arguments[i], "$m") || !strcmp(arguments[i], "$n") || !strcmp(arguments[i], "$o") || !strcmp(arguments[i], "$p")
                                || !strcmp(arguments[i], "$q") || !strcmp(arguments[i], "$r") || !strcmp(arguments[i], "$s") || !strcmp(arguments[i], "$t")
                                || !strcmp(arguments[i], "$u") || !strcmp(arguments[i], "$v") || !strcmp(arguments[i], "$w") || !strcmp(arguments[i], "$x")
                                || !strcmp(arguments[i], "$y") || !strcmp(arguments[i], "$z"))
                        {
                                error = 0;
                        }

                        else
                        {
                                error = 1;
                                break;
                        }
                }

                i++;
        }

        return error;
}

// FUNCTION TO DETECT VARIOUS ERRORS
int detect_command_errors(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX], int arg_count)
{
        int i = 0;
        int j = 0;
        int k = 0;
        int m = 0;

        int error = 0;
        int cmd_length = count_command_length(cmd);

        int is_special_char_initial = 0;
        int is_pipe_output_order = 0;
        int is_output_pipe_order = 0;
        int is_pipe_pipe_order = 0;

        int output_count = 0;
        int pipe_count = 0;

        int first_output_pos = 0;
        int first_pipe_pos = 0;

        int does_exist_dollar_arg = 0;
        int is_invalid_variable = detect_dollar_sign_error(arguments);

        // Detect first argument when arg_count == 1

        if (cmd[0] == '|' || cmd[0] == '>' || cmd[0] == '<' || cmd[0] == '&')
        {
                is_special_char_initial = 1;
        }

        // Detect combo "| > " and "> |" and "| |"
        while (cmd[i])
        {
                if (cmd[i] == '|')
                {
                        pipe_count++;

                        if (cmd[i+1] == '>')
                        {
                                is_pipe_output_order = 1;
                                break;
                        }

                        else if (cmd[i+1] == '|')
                        {
                                is_pipe_pipe_order = 1;
                        }
                }

                else if (cmd[i] == '>')
                {
                        output_count++;

                        if (cmd[i+1] == '|')
                        {
                                is_output_pipe_order = 1;
                                break;
                        }
                }

                i++;
        }

        // Get position of the first ">" char
        while (cmd[j])
        {
                if (cmd[j] == '>')
                {
                        first_output_pos = j;
                        break;
                }

                j++;
        }

        // Get position of the first "|" char
        while (cmd[k])
        {
                if (cmd[k] == '|')
                {
                        first_pipe_pos = k;
                        break;
                }

                k++;
        }

        // Detect first `$` sign
        while (arguments[m])
        {
                if (!strcmp(arguments[m], "$"))
                {
                        does_exist_dollar_arg = 1;
                        break;
                }

                m++;
        }

        // Exceeding 16 arguments
        if (arg_count > ARG_NUM_MAX)
        {
                error = 1;
        }

        // Conditions for `Error: mislocated output redirection`
        else if (arg_count > 2 && first_output_pos < first_pipe_pos && output_count == 1 && cmd[first_pipe_pos-1] != '>')
        {
                error = 2;
        }

        // Conditions for `Error: missing command`
        else if (is_special_char_initial == 1 || is_pipe_output_order == 1 || is_pipe_pipe_order == 1 || (cmd[cmd_length-1] == '|' && output_count == 0))
        {
                error = 3;
        }

        // Conditions to handle `$` sign --> `Error: invalid variable name`
        else if (does_exist_dollar_arg == 1 || !strcmp(arguments[0], "$") || is_invalid_variable == 1)
        {
                if ((!strcmp(arguments[0], "$a") || !strcmp(arguments[0], "$b") || !strcmp(arguments[0], "$c") || !strcmp(arguments[0], "$d")
                                || !strcmp(arguments[0], "$e") || !strcmp(arguments[0], "$f") || !strcmp(arguments[0], "$g") || !strcmp(arguments[0], "$h")
                                || !strcmp(arguments[0], "$i") || !strcmp(arguments[0], "$j") || !strcmp(arguments[0], "$k") || !strcmp(arguments[0], "$l")
                                || !strcmp(arguments[0], "$m") || !strcmp(arguments[0], "$n") || !strcmp(arguments[0], "$o") || !strcmp(arguments[0], "$p")
                                || !strcmp(arguments[0], "$q") || !strcmp(arguments[0], "$r") || !strcmp(arguments[0], "$s") || !strcmp(arguments[0], "$t")
                                || !strcmp(arguments[0], "$u") || !strcmp(arguments[0], "$v") || !strcmp(arguments[0], "$w") || !strcmp(arguments[0], "$x")
                                || !strcmp(arguments[0], "$y") || !strcmp(arguments[0], "$z")))
                {
                        error = 0;
                }

                else if ((does_exist_dollar_arg == 1 && m == 0) || (!strcmp(arguments[0], "$") && arg_count == 1))
                {
                        error = 4; // print both errors (invalid variable name + missing command)
                }

                else
                {
                        error = 5; // print only invalid variable name
                }
        }

        // Conditions for `Error: no output file`
        else if (is_output_pipe_order > 0 || (output_count >= 2 && is_pipe_output_order == 0) || (output_count >= 2 && pipe_count == 0) || cmd[cmd_length-1] == '>')
        {
                error = 6;
        }

        return error;
}

// FUNCTION TO MANAGE COMMAND ERRORS
int handle_command_errors(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX], int arg_count)
{
        int error = detect_command_errors(cmd, arguments, arg_count);

        // Exceeding 16 arguments
        if (error == 1)
        {
                fprintf(stderr, "Error: too many process arguments\n");
        }

        // Mislocated output redirection
        else if (error == 2)
        {
                fprintf(stderr, "Error: mislocated output redirection\n");
        }

        // Missing command errors
        else if (error == 3)
        {
                fprintf(stderr, "Error: missing command\n");
        }

        // Invalid variables + Missing command errors
        else if (error == 4)
        {
                fprintf(stderr, "Error: invalid variable name\nError: missing command\n");
        }

        // Only invalid variables errors
        else if (error == 5)
        {
                fprintf(stderr, "Error: invalid variable name\n");
        }

        // No output file
        else if (error == 6)
        {
                fprintf(stderr, "Error: no output file\n");
        }

        return error;
}

// ------------------------------------------------------------------------------ //
// ------------------------- REGULAR COMMAND EXECUTION -------------------------- //
// ------------------------------------------------------------------------------ //

// Source: Lecture 03.syscalls (page 20/43)
void execute_regular_commands(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX])
{
        pid_t pid;
        pid = fork();

        if (pid == 0) // Child process
        {
                execvp(arguments[0], arguments);
                fprintf(stderr, "Error: command not found\n");
                exit(EXIT_FAILURE);
        }

        else if (pid > 0) // Parent process
        {
                int status;
                waitpid(pid, &status, 0);
                print_complete_success_message(cmd, status);
        }

        else
        {
                fprintf(stderr, "Error: process cannot be forked\n");
                exit(EXIT_FAILURE);
        }
}

// ------------------------------------------------------------------------------ //
// ---------------------------- CD & PWD FUNCTIONS ------------------------------ //
// ------------------------------------------------------------------------------ //

// FUNCTION TO EXECUTE CD COMMAND
// Source: https://stackoverflow.com/questions/16094814/implementing-cd-system-call-using-c-if-condition
void execute_cd_command(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX], char curr_dir[CMDLINE_MAX], int retval)
{
        char initial[1];
        memcpy(initial, arguments[1], 1); // Get first character of the directory path

        // If first character is "/", chdir() the whole path in arguments[1]
        if (!strcmp(initial, "/"))
        {
                strcpy(curr_dir, arguments[1]);

                if (chdir(curr_dir) != 0)
                {
                        fprintf(stderr, "Error: cannot cd into directory\n");
                        print_complete_error_message(cmd);
                }

                else
                {
                        print_complete_success_message(cmd, retval);
                }
        }

        // Else, concatenate the current directory with "/" and path, and chdir()
        else
        {
                getcwd(curr_dir, CMDLINE_MAX);
                strcat(curr_dir, "/");
                strcat(curr_dir, arguments[1]);
   
                if (chdir(curr_dir) != 0)
                {
                        fprintf(stderr, "Error: cannot cd into directory\n");
                        print_complete_error_message(cmd);
                }

                else
                {
                        print_complete_success_message(cmd, retval);
                }
        }
}

// FUNCTION TO EXECUTE PWD COMMAND
// Source: Lecture 03.syscalls (page 33/43)
void execute_pwd_command(char cmd[CMDLINE_MAX], char curr_dir[CMDLINE_MAX], int retval)
{
        getcwd(curr_dir, CMDLINE_MAX);
        printf("%s\n", curr_dir);
        print_complete_success_message(cmd, retval);
}

// ------------------------------------------------------------------------------ //
// ------------------------ OUTPUT REDIRECTION FUNCTION ------------------------- //
// ------------------------------------------------------------------------------ //

// Source: 03.syscalls (page 31/43) + http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html (redirect.c)
void execute_output_redirection(char cmd[CMDLINE_MAX], char *output_redir_args[CMDLINE_MAX])
{
        char *null_args[CMDLINE_MAX];
        char cmd_copy[CMDLINE_MAX];
        strcpy(cmd_copy, cmd); // Avoid modifying `cmd`

        *output_redir_args = get_output_redir_args(cmd_copy, output_redir_args);
        memcpy(null_args, output_redir_args, CMDLINE_MAX);

        // Get list of arguments before "<" and add a NULL value
        int count = count_argument_number(output_redir_args);
        int null_args_count = count_argument_number(null_args);
        null_args[null_args_count-1] = NULL;

        // Open file descriptor
        int fd = open(output_redir_args[count-1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fd == -1)
        {
                fprintf(stderr, "Error: cannot open output file\n");
                return;
        }

        // Execute output redirection
        pid_t pid;
        pid = fork();

        if (pid != 0) // Parent process
        {
                int status;
                waitpid(pid, &status, 0);
        }

        else // Child process
        {
                dup2(fd, STDOUT_FILENO);
                close(fd);

                execvp(null_args[0], null_args);
                perror("execvp");
                exit(EXIT_FAILURE);
        }
}

// ------------------------------------------------------------------------------ //
// ------------------------- PIPING COMMANDS FUNCTIONS -------------------------- //
// ------------------------------------------------------------------------------ //

// Source: https://stackoverflow.com/questions/8082932/connecting-n-commands-with-pipes-in-a-shell
// TODO: Find a way to handle the exit status correctly
void execute_pipe(char cmd[CMDLINE_MAX], int pipe_count, int redir_status)
{
        Pipes *pipes = malloc(sizeof(Pipes));

        // Get the commands in pipeline
        int i = 0;

        for (int j = 0; j <= pipe_count; j++)
        {
                int index = 0;
                
                while (cmd[i] != '|')
                {
                        pipes[j].cmd[index] = cmd[i];
                        index++;
                        i++;
                }

                i++;
        }
        
        // Get arguments for each command
        *pipes[0].args = get_arguments(pipes[0].cmd, pipes[0].args);

        for (int n = 1; n <= pipe_count; n++)
        {
                if (redir_status != 0)
                {
                        if (n == pipe_count)
                        {
                                *pipes[n].args = get_output_redir_args(pipes[n].cmd, pipes[n].args);
                        }

                        else
                        {
                                *pipes[n].args = get_arguments(pipes[n].cmd, pipes[n].args);
                        }
                }

                else
                {
                        *pipes[n].args = get_arguments(pipes[n].cmd, pipes[n].args);
                }
        }

        // Add NULL value to the arguments
        for (int k = 0; k <= pipe_count; k++)
        {
                int arg_count = 0;
                arg_count = count_argument_number(pipes[k].args);
                pipes[k].args[arg_count+1] = NULL;
        }

        // Pipe commands and execute pipe
        int fd[2];
        int prev_fdin = 0;

        for (int p = 0; p < pipe_count; p++)
        {
                pipe(fd);
                pid_t pid;
                pid = fork();

                if (pid == 0)
                {
                        if (prev_fdin != 0)
                        {
                                dup2(prev_fdin, STDIN_FILENO);
                                close(prev_fdin);
                        }

                        if (fd[1] != 1)
                        {
                                dup2(fd[1], STDOUT_FILENO);
                                close(fd[1]);
                        }

                        execvp(pipes[p].args[0], pipes[p].args);
                        fprintf(stderr, "Error: command not found\n");
                        exit(EXIT_FAILURE);
                }

                else
                {
                        // Add the waitpid() here and retrieve the exit status for the current pipe command
                        int status;
                        waitpid(pid, &status, 0);
                        pipes[p].exit_status = WEXITSTATUS(status);
                        close(fd[1]);
                        prev_fdin = fd[0];
                }
        }

        // Execute the last pipe command
        if (redir_status > 0)
        {
                // If there is output redirection, execute the last command as redirection
                execute_output_redirection(pipes[pipe_count].cmd, pipes[pipe_count].args);
        }

        else
        {
                pid_t pid;
                pid = fork();

                if (pid == 0)
                {
                        if (prev_fdin != 0)
                        {
                                dup2(prev_fdin, 0);
                        }

                        execvp(pipes[pipe_count].args[0], pipes[pipe_count].args);
                        fprintf(stderr, "Error: command not found\n");
                        exit(EXIT_FAILURE);
                }

                else // Add this to avoid premature exit
                {
                        int status;
                        waitpid(pid, &status, 0);
                        pipes[pipe_count].exit_status = WEXITSTATUS(status);
                }
        }

        // Add complete messages
        fprintf(stderr, "+ complete %s [%d]", cmd, pipes[0].exit_status);
        for (int q = 1; q <= pipe_count; q++)
        {
                fprintf(stderr, "[%d]", pipes[q].exit_status);
        }
        fprintf(stderr, "\n");
}

// ------------------------------------------------------------------------------ //
// -------------------------- EXTRA FEATURE FUNCTION ---------------------------- //
// ------------------------------------------------------------------------------ //

void execute_set_command(char cmd[CMDLINE_MAX], char *arguments[CMDLINE_MAX], int arg_count, int retval)
{
        if (arg_count == 1 || !(!strcmp(arguments[1], "a") || !strcmp(arguments[1], "b") || !strcmp(arguments[1], "c") || !strcmp(arguments[1], "d")
                || !strcmp(arguments[1], "e") || !strcmp(arguments[1], "f") || !strcmp(arguments[1], "g") || !strcmp(arguments[1], "h")
                || !strcmp(arguments[1], "i") || !strcmp(arguments[1], "j") || !strcmp(arguments[1], "k") || !strcmp(arguments[1], "l")
                || !strcmp(arguments[1], "m") || !strcmp(arguments[1], "n") || !strcmp(arguments[1], "o") || !strcmp(arguments[1], "p")
                || !strcmp(arguments[1], "q") || !strcmp(arguments[1], "r") || !strcmp(arguments[1], "s") || !strcmp(arguments[1], "t")
                || !strcmp(arguments[1], "u") || !strcmp(arguments[1], "v") || !strcmp(arguments[1], "w") || !strcmp(arguments[1], "x")
                || !strcmp(arguments[1], "y") || !strcmp(arguments[1], "z")))
        {
                fprintf(stderr, "Error: invalid variable name\n");
                print_complete_error_message(cmd);
                return;
        }

        else if (arg_count == 2)
        {
                print_complete_success_message(cmd, retval);
                return;
        }

        else
        {
                // Source: https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
                int pos = *arguments[1] - 'a';
                strcpy(&VALUE_TABLE[pos][pos], arguments[2]);
                print_complete_success_message(cmd, retval);
        }
}

// --------------------------------------------------------------------------- //
// ----------------------------- MAIN FUNCTION ------------------------------- //
// --------------------------------------------------------------------------- //

int main(void)
{
        char cmd[CMDLINE_MAX];

        while (1)
        {
                char *nl;
                int retval = 0;

                // Define necessary variables here:
                char *arguments[CMDLINE_MAX];

                char cmd_copy[CMDLINE_MAX];
                char cmd_copy_2[CMDLINE_MAX];
                char *cmd_ptr;
                char *cmd_ptr_2;

                char curr_dir[CMDLINE_MAX];

                char *output_redir_args[CMDLINE_MAX];
                int redir_status = 0;

                int pipe_count = 0;

                // Print prompt
                printf("sshell@ucd$ ");
                fflush(stdout);

                // Get command line
                fgets(cmd, CMDLINE_MAX, stdin);

                // Print command line if stdin is not provided by terminal
                if (!isatty(STDIN_FILENO))
                {
                        printf("%s", cmd);
                        fflush(stdout);
                }

                // Remove trailing newline from command line
                nl = strchr(cmd, '\n');
                if (nl)
                        *nl = '\0';

                // Builtin command
                if (!strcmp(cmd, "exit"))
                {
                        fprintf(stderr, "Bye...\n");
                        print_complete_success_message(cmd, retval);
                        break;
                }

        // MODIFYING COMMAND LINE TO MAKE SURE IT HAS NO LEADING WHITESPACES / NULL COMMAND / WHITESPACE-ONLY
                memset(arguments, 0, sizeof(arguments));
                strcpy(cmd_copy, cmd);

                // Remove leading whitespaces of the cmd_copy
                cmd_ptr = remove_leading_whitespaces(cmd_copy);

                // Null command case (after removing leading whitespaces)
                if (!strcmp(cmd_ptr, "\0"))
                {
                        continue;
                }

                strcpy(cmd_copy, cmd_ptr);

                // Remove trailing whitespaces of the cmd_copy
                strcpy(cmd_copy_2, cmd_copy);
                cmd_ptr_2 = remove_trailing_space(cmd_copy_2);
                strcpy(cmd_copy_2, cmd_ptr_2);

        // SCAN THE COMMAND LINE TO ACQUIRE ARGUMENTS
                *arguments = get_arguments(cmd_copy, arguments);

                // Find number of items in *arguments[] here:
                int arg_count = count_argument_number(arguments);

        // EXTRA EXIT CASES ("exit ", "exit h", etc.):
                if (!strcmp(arguments[0], "exit"))
                {
                        fprintf(stderr, "Bye...\n");
                        print_complete_success_message(cmd, retval);
                        break;
                }

        // ERROR MANAGEMENT & HANDLING
                int error = handle_command_errors(cmd_copy_2, arguments, arg_count);

                if (error != 0)
                {
                        continue;
                }

        // EXECUTE `CD` COMMAND:
                if (!strcmp(arguments[0], "cd"))
                {
                        if (arg_count == 1)
                        {
                                fprintf(stderr, "Error: cannot cd into directory\n");
                                print_complete_error_message(cmd);
                                continue;
                        }

                        else
                        {
                                execute_cd_command(cmd, arguments, curr_dir, retval);
                        }
                }

        // EXECUTE `PWD` COMMAND:
                else if (!strcmp(arguments[0], "pwd"))
                {
                        execute_pwd_command(cmd, curr_dir, retval);
                }

        // EXECUTE `SET` COMMAND:
                else if (!strcmp(arguments[0], "set"))
                {
                        execute_set_command(cmd, arguments, arg_count, retval);
                }

        // EXECUTE REGULAR AND OTHER COMMANDS
                else
                {
                        // Get status of output redirection
                        redir_status = return_output_redirection_status(cmd);

                        // Get number of pipes
                        pipe_count = count_pipe_number(cmd);

                // Output Redirection: If there is an output redirection, execute it
                        if (redir_status == 1 && pipe_count == 0)
                        {
                                execute_output_redirection(cmd, output_redir_args);
                                print_complete_success_message(cmd, retval);
                        }

                // Piping: If there are pipes, execute piping commands
                        else if (pipe_count > 0)
                        {
                                execute_pipe(cmd, pipe_count, redir_status);
                        }

                // Regular command (with case to check variables $a $b...)
                        else
                        {
                                int i = 0;
                                while (i < arg_count)
                                {
                                        if (!strcmp(arguments[i], "$a"))
                                                arguments[i] = &VALUE_TABLE[0][0];
                                        else if (!strcmp(arguments[i], "$b"))
                                                arguments[i] = &VALUE_TABLE[1][1];
                                        else if (!strcmp(arguments[i], "$c"))
                                                arguments[i] = &VALUE_TABLE[2][2];
                                        else if (!strcmp(arguments[i], "$d"))
                                                arguments[i] = &VALUE_TABLE[3][3];
                                        else if (!strcmp(arguments[i], "$e"))
                                                arguments[i] = &VALUE_TABLE[4][4];
                                        else if (!strcmp(arguments[i], "$f"))
                                                arguments[i] = &VALUE_TABLE[5][5];
                                        else if (!strcmp(arguments[i], "$g"))
                                                arguments[i] = &VALUE_TABLE[6][6];
                                        else if (!strcmp(arguments[i], "$h"))
                                                arguments[i] = &VALUE_TABLE[7][7];
                                        else if (!strcmp(arguments[i], "$i"))
                                                arguments[i] = &VALUE_TABLE[8][8];
                                        else if (!strcmp(arguments[i], "$j"))
                                                arguments[i] = &VALUE_TABLE[9][9];
                                        else if (!strcmp(arguments[i], "$k"))
                                                arguments[i] = &VALUE_TABLE[10][10];
                                        else if (!strcmp(arguments[i], "$l"))
                                                arguments[i] = &VALUE_TABLE[11][11];
                                        else if (!strcmp(arguments[i], "$m"))
                                                arguments[i] = &VALUE_TABLE[12][12];
                                        else if (!strcmp(arguments[i], "$n"))
                                                arguments[i] = &VALUE_TABLE[13][13];
                                        else if (!strcmp(arguments[i], "$o"))
                                                arguments[i] = &VALUE_TABLE[14][14];
                                        else if (!strcmp(arguments[i], "$p"))
                                                arguments[i] = &VALUE_TABLE[15][15];
                                        else if (!strcmp(arguments[i], "$q"))
                                                arguments[i] = &VALUE_TABLE[16][16];
                                        else if (!strcmp(arguments[i], "$r"))
                                                arguments[i] = &VALUE_TABLE[17][17];
                                        else if (!strcmp(arguments[i], "$s"))
                                                arguments[i] = &VALUE_TABLE[18][18];
                                        else if (!strcmp(arguments[i], "$t"))
                                                arguments[i] = &VALUE_TABLE[19][19];
                                        else if (!strcmp(arguments[i], "$u"))
                                                arguments[i] = &VALUE_TABLE[20][20];
                                        else if (!strcmp(arguments[i], "$v"))
                                                arguments[i] = &VALUE_TABLE[21][21];
                                        else if (!strcmp(arguments[i], "$w"))
                                                arguments[i] = &VALUE_TABLE[22][22];
                                        else if (!strcmp(arguments[i], "$x"))
                                                arguments[i] = &VALUE_TABLE[23][23];
                                        else if (!strcmp(arguments[i], "$y"))
                                                arguments[i] = &VALUE_TABLE[24][24];
                                        else if (!strcmp(arguments[i], "$z"))
                                                arguments[i] = &VALUE_TABLE[25][25];
                                                
                                        i++;
                                }

                                execute_regular_commands(cmd, arguments);
                        }
                }
        }

        return EXIT_SUCCESS;
}
# ECS 150 - Project 1 Report

Student Names:
- Xuan Huang: xwhuang@ucdavis.edu, 915840915
- Han Nguyen: hnknguyen@ucdavis.edu, 917278789

Our communication is conducted mainly via WeChat and Discord. We work on the  
project on a daily basis and distribute an equal amount of work to each  
other. The project takes 50 hours to finish, with the help from Piazza and  
code websites.

## Phase 0: Preliminary Work
In this phase, we have a careful look at the skeleton code of the professor,  
and understand the requirements of the project. With the provision of the  
reference executable program `sshell_ref`, we can try different cases to  
understand how the shell should work and also base on it to make sure our  
code will work exactly the same. We learn that `exit()` has been provided and  
`system()` should be removed and replaced by the `fork() + wait() + exec()`  
combination.

## Phase 1 & 2: Running Command Lines with Arguments
We mutually agree that there should a pointer array  
`char *arguments[ARG_NUM_MAX]` that carries tokens split from the  
whitespaces. Once we obtain the array, we put it in the process of  
`fork() + wait() + exec()` under then functions `execute_regular_commands()`  
to create parent and child processes. We decide to use `execvp()` because it  
automatically looks for the path of the commands in `/bin` of the operating  
system. We also have a function `print_complete_success_message()` if the  
execution is successful, or `exit(EXIT_FAILURE)` if error occurs.

## Phase 3: Built-in Commands
We set up `char curr_dir[CMDLINE_MAX]` to store the name of the current  
directory. The `pwd` execution, under the function `execute_pwd_command()`  
requires only `getcwd()` on `curr_dir` and display. However, the execution  
of `cd` command is trickier. We detect that there should be two cases of  
changing directory:

(1) changing directory inside the current directory, which just needs  
the name of the directory that needs to be changed to.

(2) changing directory based on the directory path, which can goes to any  
directory outside the current one.

For the first case, we use `getcwd()` to get the name of the current  
directory, and the concatenate the name with `/` and `arguments[1]` (the  
directory to be changed) by using `strcat` to make a full path of directory  
to do`chdir()`. For the second case, we detect the first character of the  
`arguments[1]` to see if it is the `/` character. If yes, then we copy  
`arguments[1]` to `curr_dir` (so that later `pwd` will not detect old  
directory), and do `chdir()`.

## Phase 4: Output Redirection
We have a function `return_redir_status` to detect if there is a redirection  
in the command line. If yes, we implement output redirection under  
`execute_output_redirection()` function.

We use `get_output_redir_args()` function to create a list `*output_redir_args`  
of tokens from removal of `>` and whitespaces. We create another list  
`*null_args` to store the tokens of the command before the `>` and add a  
`NULL` value. We open the file (stored in the last token of  
`*output_redir_args` under `O_WRONLY | O_CREAT, 0644)` in `fd`. We also check  
that if the file cannot be opend, we will display  
`Error: cannot open output file` error message and exit the function back to  
the shell. With a simple `dup2(fd, STDOUT_FILENO)`, we transfer the input of  
the first command to get the output, and using`fork() + wait() + execvp()` to  
write the command into the file. 
 
## Phase 5: Pipeline Commands
We create a `struct Pipes` with 4 stacks to store commands, list of argument  
tokens and exit status of pipes. We detect the number of pipes with  
`count_pipe_number()` and put it under `pipe_count`. If `pipe_count > 0`, we  
detect there is a pipeline of commands, and will execute the piped commands  
with `execute_pipe()` function.

As required that the last piped command be output redirected, we pass the  
`redir_status` in the function. With a `for` loop, we get the arguments for  
each pipe command in the stack by determine if it is not the last pipe, then  
use `*get_arguments()` function. If it is the last pipe and  
`redir_status != 0`, we will use `*get_output_redir_args()`, otherwise just  
simply `*get_arguments()`. Once all the pipe commands have their own  
arguments, we will add a `NULL` value and get ready to be executed in the  
pipeline.

We create a file descriptor `fd[2]` with `fd[0]` is the value of input to  
read and `fd[1]` is the output to write. We use a `for` loop to `pipe()` the  
commands and move from each stack of pipe to pipe (except for the last  
command) to execute. We reason that in a pipeline of commands, the input of  
previous command will be the output of the next command. Hence, we have a  
variable `prev_fdin` to store the value of the input of the previous command.  
Under child process, if `prev_fdin != 0`, it means that it is not the first  
process, hence we use `dup2(prev_fdin, STDIN_FILENO)` to read in the input  
from the previous command. If the previous command has not yielded the ouput  
as `fd[1] != 1`, we will use `dup2(fd[1], STDOUT_FILENO)` get the output.  
Then we execute the command. After the `for` loop execution, if the process  
returns to its parent, then we `waitpid()`, retrieve the exit status of the  
current pipe command, copy the input of the command, close the file  
descriptor and escape the loop for the last command. 

In the last command, we will check if it is output redirected. If yes, we  
will execute the command by using `execute_output_redirection()` function.  
If not, we will take the input from the previous command and use a simple  
`fork() + wait() + execvp()` to execute the last command. 

## Phase 6: Extra Feature(s)
We write the `set` feature under `execute_set_command()` function. We have a  
global 2D array `char VALUE_TABLE` that will store the value of the set  
variables and will not be impacted after every `while (1)` loop. We determine  
that if there are only `set` argument in the command or the second argument  
does not fall in `a-z`, we will detect invalid variable name and return back  
to the loop. If not, then by applying the ASCII subtraction, we store the  
value of `arguments[2]` in the table by order as `a = 0`, `b = 1`, and so on.

Inside the `while(1) loop`, before executing regular commands, we will have a  
`while` loop to check if there are any arguments of `$a`, `$b`, etc. If yes,  
then we replace that argument with the value stored in the global table by  
the same order.

After replacement, we will execute the command. 

## Launching Errors
### Error Commands
We have 6 cases of error, described as below: 

(1) Exceeding 16 arguments: when `arg_count > 16`, we display  
`Error: too many process arguments`.

(2) Mislocated output redirection: if the redirected command happens before  
the pipe (`>` before `|`), we display `Error: mislocated output redirection`.

(3) Missing command: when any stand-alone special characters or in wrong  
position of output redirection or piping, we display `Error: missing command`.

(4) & (5) Invalid variable name: when any stand-alone `$` or wrong format of  
variable (not `$a` to `$z`), we display `Error: invalid variable name` or  
both with `Error: missing command` in certain cases.

(6) No output file: if the `>` is in the final position of the command, then  
we display `Error: no output file`.

If any of the 6 cases happens, we will return the error status as 1-6, and go  
back to the loop with `continue`. 

### Changing Directory
In both cases mentioned in Phase 3, if the `chdir()` is unsuccessful, then we  
say display error message `Error: cannot cd into directory` and complete  
message with status 1 under `print_complete_error_message()` function.

### Extra case of `exit`
We detect that when typing `exit hhh` (for example), the reference program  
still exits, while ours returns error message. We decide that after  
retrieving the tokens for `arguments[]`, if `arguments[0]` equals to `exit`,  
then our program still exits. 

## Limitations
Our program does not support multiple redirections (e.g  
`echo Hello > file1 > file2 > ...`). 

## Sources & Citations
### For primary functions
- Lecture 03.syscalls (for `pwd`, output redirection & regular commands)
- https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c  
(for getting arguments)
- https://stackoverflow.com/questions/16094814/implementing-cd-system-call  
-using-c-if-condition (for `cd` command)
- http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html  
(for output redirection)
- https://stackoverflow.com/questions/8082932/connecting-n-commands-with-  
pipes-in-a-shell (for piping)
- https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html (for  
character subtraction - used for `set` command)

### For helper and utility functions
- https://www.geeksforgeeks.org/c-program-to-trim-leading-white-spaces-  
from-string/ (removing leading whitespaces)
- https://codeforwin.org/2016/04/c-program-to-trim-trailing-white-space-  
characters-in-string.html (removing trailing whitespaces)

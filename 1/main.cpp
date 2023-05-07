#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    struct user_regs_struct regs;
    long *x;

    // Create a new process to trace
    if ((pid = fork()) == 0)
    {
        // Child process
        printf("Enter value for child process:\n");
        while (true)
        {
            scanf("%ld", &x);
        }
        return 0;
    }
    else
    {
        int status;
        // Parent process
        printf("Parent process: child pid = %d\n", pid);

        // Attach to the child process and trace it
        if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1)
        {
            perror("ptrace attach");
            return 1;
        }

        while (1)
        {

            waitpid(pid, &status, 0);
            ptrace(PTRACE_SYSCALL,
                   pid, NULL, NULL);
            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                printf("Child process exited with status %d\n", WEXITSTATUS(status));
                return 0;
            }

            // // Read the value of the ebx register in the child process
            if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1)
            {
                perror("ptrace getregs");
                return 1;
            }

            // printf("orig_rax: %llu, rax: %llu, status: %d\n", regs.orig_rax, regs.rax, status);

            if (regs.orig_rax == 0)
            {
                // Read the value of x from the child's memory
                long tmp = ptrace(PTRACE_PEEKDATA, pid, &x, NULL);
                printf("Parent process: x = %ld\n", tmp);
            }

            ptrace(PTRACE_SYSCALL,
                   pid, NULL, NULL);
        }

        // Detach from the child process and continue its execution
        if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1)
        {
            perror("ptrace detach");
            return 1;
        }
    }

    return 0;
}

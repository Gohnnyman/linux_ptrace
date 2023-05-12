#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    pid_t pid = std::stoi(argv[1]);

    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1)
    {
        perror("ptrace_attach");
        return 1;
    }

    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return 1;
    }

    while (true)
    {
        if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1)
        {
            perror("PTRACE_SYSCALL");
            break;
        }

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            break;
        }

        if (WIFEXITED(status))
        {
            std::cout << "process ended" << std::endl;
            break;
        }

        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1)
        {
            perror("PTRACE_GETREGS");
            break;
        }

        if (regs.orig_rax == 0)
        {
            char buffer[1024] = {0};
            for (size_t i = 0; i < 1024; i++)
            {
                long data = ptrace(PTRACE_PEEKDATA, pid, regs.rsi + i * sizeof(char), NULL);
                buffer[i] = (char)(data);
                if (data == 0)
                {
                    break;
                }
            }

            std::cout << "Data from another PID: " << buffer << std::endl;
        }
    }

    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1)
    {
        perror("PTRACE_DETACH");
        return 1;
    }

    return 0;
}

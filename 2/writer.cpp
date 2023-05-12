#include <iostream>
#include <string>
#include <unistd.h>

int main()
{
    // Отримання та виведення PID поточного процесу
    pid_t current_pid = getpid();
    std::cout << "PID поточного процесу: " << current_pid << std::endl;

    // Запит на введення тексту
    std::cout << "Введіть текст: ";
    while (true)
    {

        std::string input_text;
        std::getline(std::cin, input_text);
    }

    return 0;
}

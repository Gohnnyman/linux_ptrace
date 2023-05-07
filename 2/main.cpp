#include <iostream>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/input/event3", O_RDONLY);

    if (fd < 0)
    {
        std::cerr << "Error opening input device" << std::endl;
        return -1;
    }

    while (true)
    {
        input_event ev;
        read(fd, &ev, sizeof(ev));

        if (ev.type == EV_KEY && ev.value == 1)
        {
            char key = ev.code + 96; // Convert key code to ASCII
            std::cout << "Key pressed: " << key << std::endl;
        }
    }

    close(fd);

    return 0;
}

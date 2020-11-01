#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>

#include <string>


class Controller
{
public:

Controller(const std::string &device) : _device(device) 
{}

bool open();
void triggerRead();
void normalRead();

private:

std::string _device;
int _fd{-1};
fd_set _rfds;
struct timeval _tv;
uint8_t _buf[512];
uint32_t _buf_offset{0};


};
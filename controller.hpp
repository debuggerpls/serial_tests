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
struct timeval _tv;


};
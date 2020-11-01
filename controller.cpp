


#include "controller.hpp"

bool Controller::open()
{
  int fd;
  struct termios options;

  /* open the port */
  _fd = ::open(_device.c_str(), O_RDWR | O_SYNC | O_NONBLOCK);
  if (_fd < 0)
    return false;

  /* raw mode, like Version 7 terminal driver */
  cfmakeraw(&options);
  options.c_cflag |= (CLOCAL | CREAD);

  /* set the options */
  cfsetspeed(&options, B19200);
  tcsetattr(_fd, TCSANOW, &options);
  tcflush(_fd, TCIOFLUSH);

  return true;
}

void Controller::triggerRead()
{
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(_fd, &rfds);
  _tv.tv_sec = 1;
  _tv.tv_usec = 0;
  int retval;

  retval = select(_fd + 1, &rfds, NULL, NULL, &_tv);
  /* Don't rely on the value of tv now! */

  uint8_t buf[512];

  printf("Retval: %d", retval);

  if (retval == -1)
  {
    perror("select()");
  }
  else if (retval)
  {
    printf("Data is available now.\n");

    auto cnt = read(_fd, buf, 512);
    if (cnt > 0)
    {
      for (int i = 0; i < cnt; ++i)
      {
        printf("%x ", buf[i]);
      }
      printf("\n");
    }
  }
  else
  {
    printf("No data within 1 seconds.\n");
  }
}

void Controller::normalRead()
{

  uint8_t buf[512];

  auto cnt = read(_fd, buf, 512);
  if (cnt > 0)
  {
    printf("Data available:\n");
    for (int i = 0; i < cnt; ++i)
    {
      printf("%x ", buf[i]);
    }
    printf("\n");
  }
  else
  {
    printf("No data available!\n");
  }
  
}
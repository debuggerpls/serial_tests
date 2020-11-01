
#include <boost/crc.hpp>

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
  FD_ZERO(&_rfds);
  FD_SET(_fd, &_rfds);
  _tv.tv_sec = 1;
  _tv.tv_usec = 0;
  int retval;

  retval = select(_fd + 1, &_rfds, NULL, NULL, &_tv);
  /* Don't rely on the value of tv now! */

  

  printf("Retval: %d", retval);

  if (retval == -1)
  {
    perror("select()");
  }
  else if (retval)
  {
    printf("Data is available now.\n");

    auto cnt = read(_fd, &_buf[_buf_offset], 512 - _buf_offset);
    if (cnt > 0)
    {
      for (int i = 0; i < cnt + _buf_offset; ++i)
      {
        printf("%x ", _buf[i]);
      }
      printf("\n");

      bool done = false;
      int i;
      for (i = 0; !done && i < cnt + _buf_offset; ++i)
      {
        if (_buf[i] != 0xfe)
        {
          printf("Skipping byte: %x\n", _buf[i]);
          continue;
        }
        
        if (i + 5 >= cnt + _buf_offset)
        {
          // not full msg, set new offset and quit
          printf("Not full msg\n");
          _buf_offset = cnt;
          break;
        }

        uint8_t length = _buf[i+2];

        if (length > 3)
        {
          printf("Length too long\n");
          continue;
        }

        if (i + length + 4 >= cnt + _buf_offset)
        {
          // not full msg
          printf("Not full msg\n");
          _buf_offset = cnt;
          break;
        }

        boost::crc_ccitt_type ccitt;
        ccitt.process_bytes(&_buf[i], 3 + length);
        auto crc16 = ccitt.checksum();
        // calculate crc
        printf("Calculated CRC: %x %x\n", ((crc16 >> 8) & 0xff), (crc16 & 0xff) );

        exit(EXIT_SUCCESS);
      }

      // ended loop with nothing found
      if (i >= cnt + _buf_offset)
      {
        printf("No correct cmds found\n");
        _buf_offset = 0;
      }

      printf("_buf_offset = %d\n", _buf_offset);



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
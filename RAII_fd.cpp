
#include "timer.h"

using namespace std;

RAII_fd::RAII_fd(int fd, const string &name)
{
  this->fd = fd;
  this->name = name;
  if (this->fd == -1)
  {
    string tmps = strerror(errno);
    Exception err(name, tmps, (int)errno);
    throw err;
  }
}

RAII_fd::~RAII_fd()
{
  close(fd);
}

int RAII_fd::get_fd()
{
  return this->fd;
}
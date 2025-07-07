#include "timer.h"

using namespace std;

Data::Data()
{
}

Data::~Data()
{
}

Data::Data(const string &name)
{
  this->name = name;
}

struct _Data &Data::get_data()
{
  return this->_data;
}

const struct _Data &Data::get_data_read() const
{
  return this->_data;
}

const string &Data::get_name_read() const
{
  return this->name;
}

const bool Data::get_run() const
{
  return this->run;
}

void Data::set_run(bool flag)
{
  this->run = flag;
  return;
}
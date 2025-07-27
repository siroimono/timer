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

void Data::set_data_time(time_t t)
{
  this->_data.total_time = t;
  return;
}

void Data::set_data_s(const string &s)
{
  this->_data.time_s = s;
  return;
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

const int Data::set_run(bool flag)
{
  this->run = flag;
  return 0;
}
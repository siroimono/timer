#include "timer.h"

using namespace std;

Data::Data(const string &name)
{
  this->name = name;
}
Data::~Data()
{
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
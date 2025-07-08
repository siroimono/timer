#include "timer.h"

using namespace std;

Exception::Exception(string &err_type, string &err_name, int err_no)
{
  this->err_type = err_type;
  this->err_name = err_name;
  this->err_no = err_no;
}

Exception::~Exception()
{
}

const string &Exception::get_err_type() const
{
  return this->err_type;
}
const string &Exception::get_err_name() const
{
  return this->err_name;
}
const int &Exception::get_err_no() const
{
  return this->err_no;
}
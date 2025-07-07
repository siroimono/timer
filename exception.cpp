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
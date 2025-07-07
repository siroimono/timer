#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <vector>
//====================C++===================

#include <fcntl.h> // open()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mode_t
#include <time.h>
#include <unistd.h> // write(), close()
//=====================C=====================

using namespace std;

class Control;

struct _Data
{
  string total_time;
};

class Data
{
private:
  string name;
  bool run;
  struct _Data _data;

public:
  Data();
  ~Data();
  Data(const string &name);

  struct _Data &get_data();
  const struct _Data &get_data_read() const;

  const string &get_name_read() const;

  const bool get_run() const;
  void set_run(bool flag);
};

class Control
{
private:
  map<string, Data> db;

public:
  Control();
  ~Control();

  const string &get_l_time();
  const map<string, Data> &get_db_read() const;

  bool add_data(string &name);
};

class UI
{
private:
  Control &ctl;

public:
  UI(Control &ctl);
  ~UI();

  void run();
  void add_data_UI();
};

class Exception
{
private:
  string err_type;
  string err_name;
  int err_no;

public:
  Exception(string &err_type, string &err_name, int err_no);
  ~Exception();
};

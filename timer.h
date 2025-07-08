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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mode_t
#include <sys/time.h>
#include <time.h>
#include <unistd.h> // write(), close()
//=====================C=====================

using namespace std;

class Control;

struct _Data
{
  time_t total_time;
  string time_s;
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
  void set_data_time(time_t t);
  void set_data_s(const string &s);

  const struct _Data &get_data_read() const;

  const string &get_name_read() const;

  const bool get_run() const;
  const int set_run(bool flag);
};

class Control
{
private:
  static map<string, Data> db;

public:
  Control();
  ~Control();

  int get_l_time(); // 현재시간 값 확인 후 대입

  map<string, Data> &get_db();
  const map<string, Data> &get_db_read() const;

  bool add_data(string &name);
  bool del_data(string &name);

  string convert(time_t t);
  void convert_2(string &s);
  string convert_l(const time_t &t);

  int alarm();    // 60초 간격 SIGALRM
  int sig_alrm(); // SIGALRM signal 대응 함수
  static void sig_alrm_handler(int sig, siginfo_t *sig_info, void *vvv);
  // SIGALRM signal 대응 함수2

  int sig_int();
  static void sig_int_handler(int sig, siginfo_t *sig_info, void *vvv);

  // bool find_data(const string &name);
  const int set_run_Ctl(const string &name, bool set);
};

class UI
{
private:
  Control &ctl;

public:
  static bool sig_int_flag;

  UI(Control &ctl);
  ~UI();

  void menu();

  void add_data_UI();

  void del_data_UI();

  void timer_set();

  void timer_run();

  void stat();

  void run_stat();

  void check_UI(const int, const string &name);
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

  const string &get_err_type() const;
  const string &get_err_name() const;
  const int &get_err_no() const;
};

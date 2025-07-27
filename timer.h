#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <sys/types.h>
#include <vector>
//====================C++===================

#include <ctype.h>
#include <fcntl.h> // open()
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mode_t
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
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

struct io_Data
{

  char name_local[12];
  time_t local; // 년 월 일 까지만 저장

  char name_1[20];
  time_t time_1;

  char name_2[20];
  time_t time_2;

  char name_3[20];
  time_t time_3;
};


struct medical
{
  char date[20];
  int num;
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
  map<int,string> medical;

public:
  Control();
  ~Control();

  static int back_up();

  const map<string, Data> read_ctl();

  int get_l_time(); // 현재시간 값 확인 후 대입

  map<string, Data> &get_db();
  const map<string, Data> &get_db_read() const;

  void change_db(const map<string, Data> load_data);

  bool add_data(string &name);
  bool del_data(string &name);

  string convert(time_t t);
  time_t convert_time(const string &s_time);
  void convert_2(string &s);
  string convert_l(const time_t &t);
  // const load_Data convert_io(io_Data &io_data) const;

  int alarm();    // 60초 간격 SIGALRM
  int sig_alrm(); // SIGALRM signal 대응 함수
  static void sig_alrm_handler(int sig, siginfo_t *sig_info, void *vvv);
  // SIGALRM signal 대응 함수2

  int sig_int();
  static void sig_int_handler(int sig, siginfo_t *sig_info, void *vvv);

  int sig_hup();
  static void sig_hup_handler(int sig, siginfo_t *sig_ingo, void *vvv);

  // bool find_data(const string &name);
  const int set_run_Ctl(const string &name, const string &flag);
  const int set_Data_time_ctl(const string &s_time, const string &name);

  int exit_save_ctl();

  const vector<map<string, Data>> log_read_ctl();

  int log_dell_ctl();

  int medical_ctl();

  int set_medical();
};

class UI
{
private:
  Control &ctl;

public:
  static bool sig_int_flag;
  static void *th_func(void *vp);
  static bool hup_print();

  UI(Control &ctl);
  ~UI();

  void menu(); //   void menu(this* 0);

  void add_data_UI();

  void del_data_UI();

  void timer_set();

  int timer_run();

  void stat();

  void run_stat();

  void save_UI();

  void read_UI();

  void exit_save_UI();

  void log_UI();

  void log_print_UI();

  void log_dell_UI();

  void read_print_UI();

  bool read_UI_empty(char *c_p, time_t t);

  void check_UI(const int, const string &name);

  void medical_UI();
};

class Exception
{
private:
  string err_type;
  string err_name;
  int err_no;

public:
  Exception(const string &err_type, string &err_name, int err_no);
  ~Exception();

  const string &get_err_type() const;
  const string &get_err_name() const;
  const int &get_err_no() const;
};

class RAII_fd
{
  private:
    int fd;
    string name;

  public:
    RAII_fd(int fd, const string &name);
    ~RAII_fd();

    int get_fd();
};

class check_err
{
  private:
  public:
    check_err () {}
    ~check_err () {}

    template<typename T>
      static int check(const char* name, T ret, T f_value)
      {
        if(ret == f_value)
        {
          string s_name = name;
          string err_name = strerror(errno);
          Exception err (s_name, err_name, errno);
          throw err;
        }
        //printf("success %s\n", name);
        return 0;
      }

    template<typename T>
      static int check_ENOENT(const char* name, T ret, T f_value)
      {
        if(ret == f_value && errno != ENOENT)
        {
          string s_name = name;
          string err_name = strerror(errno);
          Exception err (s_name, err_name, errno);
          throw err;
        }
        //printf("success %s\n", name);
        return 0;
      }

    template<typename T>
      static int check(const string name, T ret, T f_value)
      {
        if(ret == f_value)
        {
          string s_name = name;
          string err_name = strerror(errno);
          Exception err (s_name, err_name, errno);
          throw err;
        }
        //printf("success %s\n", name.c_str());
        return 0;
      }
};
/*
   class Sig_Guard
   {
   private:
   sigset_t sig_set_block;
   sigset_t sig_set_old;

   public:
   Sig_Guard();
   ~Sig_Guard();
   int sig_block_run(vector<int> &vi);
   };
   */

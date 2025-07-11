#include "timer.h"
#include <csignal>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

Control::Control()
{
}

Control::~Control()
{
}

map<string, Data> Control::db; // static

int Control::back_up() // static
{
  io_Data back_up_data = {};
  vector<pair<string, time_t>> stk;

  for (auto it = Control::db.begin(); it != Control::db.end(); it++)
  {
    if (it->first == "local_time")
    {
      strcpy(back_up_data.name_local, "local_time");

      // struct tm st_tm = {};
      time_t t = it->second.get_data_read().total_time;
      /*
      auto flag_localtime = localtime_r(&t, &st_tm);
      if (flag_localtime == NULL)
      {
        string tmp1("void Control::back_up() ->  localtime_r");
        string tmp2((strerror(errno)));
        Exception err(tmp1, tmp2, (int)errno);
        throw err;
      }*/

      back_up_data.local = t;
    }
    else
    {
      stk.push_back({it->first, it->second.get_data().total_time});
    }
  }

  if (stk.size() > 0)
  {
    strcpy(back_up_data.name_1, stk[0].first.c_str());
    back_up_data.time_1 = stk[0].second;
  }

  if (stk.size() > 1)
  {
    strcpy(back_up_data.name_2, stk[1].first.c_str());
    back_up_data.time_2 = stk[1].second;
  }

  if (stk.size() > 2)
  {
    strcpy(back_up_data.name_3, stk[2].first.c_str());
    back_up_data.time_3 = stk[2].second;
  }

  int fd = open("back_up.txt", O_RDWR | O_APPEND | O_CREAT, 0755);
  RAII_fd a_fd(fd, "back_up() open");

  int flag_write = write(a_fd.get_fd(), &back_up_data, sizeof(io_Data));

  if (flag_write == -1)
  {
    string tmp1("void Control::back_up() -> write()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  return 0;
}

const map<string, Data> Control::read_ctl()
{
  int fd = open("back_up.txt", O_RDONLY | O_CREAT, 0755);
  RAII_fd a_fd(fd, "read -> open");

  off_t end_offset = lseek(a_fd.get_fd(), 0, SEEK_END);
  if (end_offset == -1)
  {
    string tmp1("int Control::read_ctl() -> lseek()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  if (end_offset < (off_t)sizeof(io_Data))
  {
    int flag_tmp = lseek(a_fd.get_fd(), 0, SEEK_SET);
    if (flag_tmp == -1)
    {
      string tmp1("int Control::read_ctl() -> lseek()");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }
  }
  else
  {
    int flag_tmp = lseek(a_fd.get_fd(), -sizeof(io_Data), SEEK_END);
    if (flag_tmp == -1)
    {
      string tmp1("int Control::read_ctl() -> lseek()");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }
  }

  struct io_Data buf_read = {};
  int flag_read = read(a_fd.get_fd(), &buf_read, sizeof(io_Data));
  if (flag_read == -1)
  {
    string tmp1("int Control::read_ctl() -> read()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }
  else if (flag_read == 0)
  {
    static map<string, Data> empty_db;
    return empty_db;
  }

  map<string, Data> l_data;
  if (buf_read.name_local[0] != '\0')
  {
    l_data[buf_read.name_local].get_data().total_time = buf_read.local;
    l_data[buf_read.name_local].set_data_s(this->convert_l(buf_read.local));
  }
  if (buf_read.name_1[0] != '\0')
  {
    l_data[buf_read.name_1].get_data().total_time = buf_read.time_1;
    l_data[buf_read.name_1].set_run(false);
  }
  if (buf_read.name_2[0] != '\0')
  {
    l_data[buf_read.name_2].get_data().total_time = buf_read.time_2;
    l_data[buf_read.name_2].set_run(false);
  }
  if (buf_read.name_3[0] != '\0')
  {
    l_data[buf_read.name_3].get_data().total_time = buf_read.time_3;
    l_data[buf_read.name_3].set_run(false);
  }
  return l_data;
}

void Control::change_db(const map<string, Data> load_data)
{
  Control::db = load_data;
  return;
}

int Control::get_l_time()
{
  // Data local_time("local_time");
  string tmps("local_time");
  this->add_data(tmps);

  time_t t_sec = 0;
  int flag_t = time(&t_sec);
  if (flag_t == -1)
  {
    string tmp1("void Control::get_l_time -> time");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  this->db[tmps].set_data_time(t_sec);
  this->db[tmps].set_data_s(
      move(this->convert_l(this->db[tmps].get_data_read().total_time)));
  return 0;
}

void Control::convert_2(string &s)
{
  if (s.empty())
  {
    s = "00";
  }
  else if (s.size() == 1)
  {
    string tmps_2 = "0";
    s = tmps_2 + s;
  }
}

string Control::convert(time_t t)
{
  string hours = to_string(t / 3600);
  string min = to_string((t % 3600) / 60);
  string sec = to_string(t % 60);
  this->convert_2(hours);
  this->convert_2(min);
  this->convert_2(sec);

  return (hours + ":" + min + ":" + sec);
}

time_t Control::convert_time(const string &s_time)
{
  string hours = s_time.substr(0, 2);
  string min = s_time.substr(3, 2);
  string sec = s_time.substr(6);

  time_t hours_t = (time_t)atoi(hours.c_str()) * 3600;
  time_t min_t = (time_t)atoi(min.c_str()) * 60;
  time_t sec_t = (time_t)atoi(sec.c_str());

  return hours_t + min_t + sec_t;
}

string Control::convert_l(const time_t &t)
{
  struct tm local_t = {};
  // memset(&local_t, 0, sizeof(struct tm));
  auto flag_local = localtime_r(&t, &local_t);
  if (flag_local == NULL)
  {
    string tmp1("string Control::convert(time_t t) -> localtime\n");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  string year = to_string(local_t.tm_year + 1900);
  string month = to_string(local_t.tm_mon + 1);
  string day = to_string(local_t.tm_mday);

  string wday;
  switch (local_t.tm_wday)
  {
  case 1:
  {
    wday = "[Mon]";
    break;
  }
  case 2:
  {
    wday = "[Tue]";
    break;
  }
  case 3:
  {
    wday = "[Wed]";
    break;
  }
  case 4:
  {
    wday = "[Thur]";
    break;
  }
  case 5:
  {
    wday = "[Fri]";
    break;
  }
  case 6:
  {
    wday = "[Sat]";
    break;
  }
  case 0:
  {
    wday = "[Sun]";
    break;
  }
  }

  string hours = to_string(local_t.tm_hour);
  string min = to_string(local_t.tm_min);
  string sec = to_string(local_t.tm_sec);

  convert_2(hours);
  convert_2(min);
  convert_2(sec);

  return (year + "." + month + "." + day + " " + wday + "   [" + hours + ":" +
          min + ":" + sec + "]\n");
}

map<string, Data> &Control::get_db()
{
  return this->db;
}

const map<string, Data> &Control::get_db_read() const
{
  return this->db;
}

bool Control::add_data(string &name)
{
  auto it = this->db.find(name);
  if (it != db.end())
  {
    return false;
  }
  if (db.size() > 4)
  {
    return false;
  }

  db[name].get_data().total_time = 0;
  db[name].set_run(false);
  return true;
}

bool Control::del_data(string &name)
{
  auto it = this->db.find(name);
  if (it != db.end())
  {
    db.erase(it);
    return true;
  }
  return false;
}

int Control::alarm()
{
  int tmpi = 0;
  for (auto &v : Control::db)
  {
    if (v.second.get_run() == true)
    {
      tmpi++;
    }
  }

  if (tmpi <= 0)
  {
    return -1;
  }

  struct itimerval st_itime = {};
  st_itime.it_interval.tv_sec = 60;
  st_itime.it_value.tv_sec = 60;

  int flag = setitimer(ITIMER_REAL, &st_itime, NULL);
  if (flag == -1)
  {
    string tmp1("void Control::alarm() -> setitimer");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  return 0;
}

int Control::sig_alrm()
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = this->sig_alrm_handler;

  int flag = sigaction(SIGALRM, &sa, NULL);
  if (flag == -1)
  {
    string tmp1("int Control::sig_alrm()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  return 0;
}

void Control::sig_alrm_handler(int sig, siginfo_t *sig_info, void *vvv)
{ // static
  for (auto it = db.begin(); it != db.end(); it++)
  {
    if (it->second.get_run() == true)
    {
      it->second.get_data().total_time += 60;
    }
  }
}

int Control::sig_int()
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = sig_int_handler;
  sigemptyset(&sa.sa_mask);

  int flag = sigaction(SIGINT, &sa, NULL);
  if (flag == -1)
  {
    string tmp1("int Control::sig_int()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }
  return 0;
}

void Control::sig_int_handler(int sig, siginfo_t *sig_info, void *vvv)
{ // static
  UI::sig_int_flag = true;
  for (auto it = Control::db.begin(); it != Control::db.end(); it++)
  {
    it->second.set_run(false);
  }
}

int Control::sig_hup()
{
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = sig_hup_handler;
  sigemptyset(&sa.sa_mask);

  int flag = sigaction(SIGHUP, &sa, NULL);
  if (flag == -1)
  {
    string tmp1("int Control::sig_hup()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }
  return 0;
}

void Control::sig_hup_handler(int sig, siginfo_t *sig_ingo, void *vvv)
{ // static
  bool flag = UI::hup_print();
  if (flag == true)
  {
    Control::back_up();
  }
  union sigval s_val = {};
  pid_t pid = getpid();
  sigqueue(pid, 1, s_val);
  return;
}

/*
bool Control::find_data(const string &name)
{
  auto it = this->db.find(name);
  if (it != db.end())
  {
    if (it->second.get_run() == false)
      return true;
  }
  return false;
}*/

const int Control::set_run_Ctl(const string &name, const string &flag)
{
  bool set_bool = false;

  if (flag == "1")
  {
    set_bool = true;
  }
  else if (flag == "2")
  {
    set_bool = false;
  }
  else
  {
    return -1;
  }

  auto it = this->db.find(name);

  if (it != db.end())
  {
    if (set_bool == true && it->second.get_run() == false)
    {
      if (it->second.set_run(set_bool) != 0)
      {
        return -1;
      }
      return 0;
    }
    else if (set_bool == false && it->second.get_run() == true)
    {
      if (it->second.set_run(set_bool) != 0)
      {
        return -1;
      }
      return 0;
    }
  }
  return -1;
}

const int Control::set_Data_time_ctl(const string &s_time, const string &name)
{
  auto it = this->db.find(name);

  if (it != db.end())
  {
    time_t t = convert_time(s_time);
    this->db[name].set_data_time(t);
    return 0;
  }
  return -1;
}

int Control::exit_save_ctl()
{
  Control::back_up();
  return 0;
}

const vector<map<string, Data>> Control::log_read_ctl()
{
  int fd = open("back_up.txt", O_RDONLY | O_CREAT, 0755);
  RAII_fd a_fd(fd, "read -> open");

  vector<map<string, Data>> ret;

  off_t empty_check = lseek(a_fd.get_fd(), 0, SEEK_END);
  if (empty_check == -1)
  {
    string tmp1(
        "const vector<map<string, Data>> Control::log_read_ctl() -> lseek()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  if (empty_check == 0)
  {
    static vector<map<string, Data>> empty_db;
    return empty_db;
  }

  off_t recovery_offset = lseek(a_fd.get_fd(), 0, SEEK_SET);
  if (recovery_offset == -1)
  {
    string tmp1(
        "const vector<map<string, Data>> Control::log_read_ctl() -> lseek()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  int flag_read = 0;
  do
  {
    struct io_Data buf_read = {};
    flag_read = read(a_fd.get_fd(), &buf_read, sizeof(io_Data));
    if (flag_read == -1)
    {
      string tmp1("int Control::read_ctl() -> read()");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }
    else if (flag_read == 0)
    {
      break;
    }

    map<string, Data> l_data;
    if (buf_read.name_local[0] != '\0')
    {
      l_data[buf_read.name_local].get_data().total_time = buf_read.local;
      l_data[buf_read.name_local].set_data_s(this->convert_l(buf_read.local));
    }
    if (buf_read.name_1[0] != '\0')
    {
      l_data[buf_read.name_1].get_data().total_time = buf_read.time_1;
      l_data[buf_read.name_1].set_run(false);
    }
    if (buf_read.name_2[0] != '\0')
    {
      l_data[buf_read.name_2].get_data().total_time = buf_read.time_2;
      l_data[buf_read.name_2].set_run(false);
    }
    if (buf_read.name_3[0] != '\0')
    {
      l_data[buf_read.name_3].get_data().total_time = buf_read.time_3;
      l_data[buf_read.name_3].set_run(false);
    }

    ret.push_back(l_data);
  } while (flag_read != -1 && flag_read != 0);

  return ret;
}

int Control::log_dell_ctl()
{
  pid_t pid_child = fork();
  if (pid_child == -1)
  {
    string tmp1("void Control::log_dell_ctl() -> fork()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  if (pid_child > 0)
  {
    int child_pid_status = 0;
    int flag_waitpid = waitpid(pid_child, &child_pid_status, 0);
    if (flag_waitpid == -1)
    {
      string tmp1("void Control::log_dell_ctl() -> waitpid()");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }

    if (WIFEXITED(child_pid_status))
    {
      return 0;
    }

    return -1;
  }

  char *argv[] = {(char *)"rm", (char *)"back_up.txt", NULL};
  int flag_execvp = execvp("rm", argv);
  if (flag_execvp == -1)
  {
    string tmp1("void Control::log_dell_ctl() -> execvp()");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }
  return 0;
}
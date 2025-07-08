#include "timer.h"

using namespace std;

Control::Control()
{
}

Control::~Control()
{
}

map<string, Data> Control::db; // static

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
  string min = to_string(t / 60);
  string sec = to_string(t % 60);
  this->convert_2(hours);
  this->convert_2(min);
  this->convert_2(sec);

  return (hours + ":" + min + ":" + sec);
}

string Control::convert_l(const time_t &t)
{
  struct tm local_t;
  memset(&local_t, 0, sizeof(struct tm));
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

  string wday = to_string(local_t.tm_wday);

  string hours = to_string(local_t.tm_hour);
  string min = to_string(local_t.tm_min);
  string sec = to_string(local_t.tm_sec);

  convert_2(hours);
  convert_2(min);
  convert_2(sec);

  return (year + "." + month + "." + day + "   [" + hours + ":" + min + ":" +
          sec + "]\n");
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

const int Control::set_run_Ctl(const string &name, bool set)
{
  auto it = this->db.find(name);
  if (it != db.end())
  {
    if (set == true && it->second.get_run() == false)
    {
      if (it->second.set_run(set) != 0)
      {
        printf("ERR int Control::set_run_Ctl(const string &name)\n");
        return -1;
      }
      return 0;
    }
    else if (set == false && it->second.get_run() == true)
    {
      if (it->second.set_run(set) != 0)
      {
        printf("ERR int Control::set_run_Ctl(const string &name)\n");
        return -1;
      }
      return 0;
    }
  }
  return -1;
}

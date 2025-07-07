#include "timer.h"

using namespace std;

Control::Control()
{
}

Control::~Control()
{
}

const string &Control::get_l_time()
{
  Data local_time("local_time");

  time_t t_sec = 0;
  int flag_t = time(&t_sec);
  if (flag_t == -1)
  {
    string tmp1("void Control::get_l_time -> time");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  struct tm local_t;
  memset(&local_t, 0, sizeof(struct tm));
  auto flag_local = localtime_r(&t_sec, &local_t);
  if (flag_local == NULL)
  {
    string tmp1("void Control::get_l_time -> localtime_r");
    string tmp2((strerror(errno)));
    Exception err(tmp1, tmp2, (int)errno);
    throw err;
  }

  string tmps = "";
  tmps += to_string(local_t.tm_year + 1900) + "." +
          to_string(local_t.tm_mon + 1) + "." + to_string(local_t.tm_mday) +
          "   " + to_string(local_t.tm_hour) + ":" + to_string(local_t.tm_min) +
          ":" + to_string(local_t.tm_sec) + "\n";

  local_time.get_data().total_time = tmps;
  this->db[local_time.get_name_read()] = local_time;
  return this->db["local_time"].get_data_read().total_time;
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
  db[name].get_data().total_time = "0";
  db[name].set_run(false);
  return true;
}
#include "timer.h"

using namespace std;

Control::Control()
{
}

Control::~Control()
{
}

void Control::init()
{
  Data local_data("local_data");

  while (1)
  {
    time_t t_sec = 0;
    int flag_t = time(&t_sec);
    if (flag_t == -1)
    {
      string tmp1("void Control::init() -> time");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }

    struct tm local_t;
    memset(&local_t, 0, sizeof(struct tm));
    auto flag_local = localtime_r(&t_sec, &local_t);
    if (flag_local == NULL)
    {
      string tmp1("void Control::init() -> localtime_r");
      string tmp2((strerror(errno)));
      Exception err(tmp1, tmp2, (int)errno);
      throw err;
    }

    string tmps;
    tmps += to_string(local_t.tm_year) + " year \t" +
            to_string(local_t.tm_mon + 1) + " month \t" +
            to_string(local_t.tm_mday) + " day\n" + to_string(local_t.tm_hour) +
            " hour \t" + to_string(local_t.tm_min) + " min \t" +
            to_string(local_t.tm_sec) + " sec \n";

    local_data.get_data().total_time = tmps;
    this->db[local_data.get_name_read()] = local_data;
    sleep(300);
  }
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
  db[name];
  return true;
}
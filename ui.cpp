#include "timer.h"
#include <unistd.h>

using namespace std;

UI::UI(Control &ctl) : ctl(ctl)
{
}
UI::~UI()
{
}

void UI::run()
{
  printf("\n====================== Timer Run ======================\n");
  const map<string, Data> &db = this->ctl.get_db_read();
  if (db.size() == 0)
  {
    printf("empty ...\n");
    while (1)
    {
      printf("want add ? \t [Y] / [n]\n");
      string buf;
      getline(cin, buf);
      if (buf.empty() || buf == "y" || buf == "y")
      {
        this->add_data_UI();
        if (db.size() > 0)
        {
          break;
        }
      }
      else if (buf == "N" || buf == "n")
      {
        printf("exit(0)...\n");
        exit(0);
      }
      else
      {
        printf("select plz [Y] / [n]\n");
      }
    }
  }
  this->ctl.get_l_time();
  for (auto it = db.cbegin(); it != db.cend(); it++)
  {
    if (it->first == "local_time")
    {
      printf("%s -> %s\n", it->first.c_str(),
             it->second.get_data_read().total_time.c_str());
    }
    else
    {
      bool tmp_flag = it->second.get_run();
      string tmps = "";
      if (tmp_flag)
      {
        tmps = "run";
      }
      else
      {
        tmps = "stop";
      }
      printf("%s -> %s   stat -> %s\n", it->first.c_str(),
             it->second.get_data_read().total_time.c_str(), tmps.c_str());
    }
  }
}

void UI::add_data_UI()
{
  printf("input data name\n");
  string buf;
  getline(cin, buf);
  bool flag = this->ctl.add_data(buf);
  if (flag == true)
  {
    printf("success add data %s\n\n", buf.c_str());
  }
  else
  {
    printf("before using name.. %s\n\n", buf.c_str());
  }
  return;
}
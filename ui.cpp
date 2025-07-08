#include "timer.h"
#include <csignal>

using namespace std;

UI::UI(Control &ctl) : ctl(ctl)
{
}
UI::~UI()
{
}

bool UI::sig_int_flag = false; // static

void UI::menu()
{
  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);

  string buf_s = "";
  while (buf_s != "0")
  {
    printf("\n====================== Timer Menu ======================\n");
    printf("[1] stat   [2] add   [3] del   [4] set   [5] run\n");

    sigprocmask(SIG_BLOCK, &st, NULL);
    getline(cin, buf_s);
    sigprocmask(SIG_UNBLOCK, &st, NULL);

    if (buf_s == "1")
    {
      this->stat();
    }
    else if (buf_s == "2")
    {
      this->add_data_UI();
    }
    else if (buf_s == "3")
    {
      this->del_data_UI();
    }
    else if (buf_s == "4")
    {
      this->timer_set();
    }
    else if (buf_s == "5")
    {
      this->timer_run();
      this->run_stat();
    }
    else
    {
      printf("plz input 1~5 \n");
    }
  }
}

void UI::timer_set()
{
  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  sigprocmask(SIG_BLOCK, &st, NULL);
  printf("\n====================== Timer Set ======================\n");
  printf("plz input set name\n");
  string buf_org;
  getline(cin, buf_org);

  printf("[1] run   [2] stop\n");
  string buf_s;
  getline(cin, buf_s);

  int ret0 = this->ctl.set_run_Ctl(buf_org, buf_s);
  this->check_UI(ret0, "set_run_Ctl");

  this->stat();
  sigprocmask(SIG_UNBLOCK, &st, NULL);
}

void UI::timer_run()
{
  int ret1 = this->ctl.alarm();
  this->check_UI(ret1, "alarm");

  int ret2 = this->ctl.sig_alrm();
  this->check_UI(ret2, "sig_alrm");

  int ret3 = this->ctl.sig_int();
  this->check_UI(ret3, "sig_int");

  printf("timer_run");
  return;
}

void UI::check_UI(const int ret, const string &name)
{
  if (ret != 0)
  {
    printf("failed %s\n", name.c_str());
  }
  else
  {
    printf("success %s\n", name.c_str());
  }
  return;
}
// 1분 주기 누적값 출력
// 시그널 받으면 세팅으로 넘어감

void UI::add_data_UI()
{
  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  sigprocmask(SIG_BLOCK, &st, NULL);
  printf("input data name\n");
  string buf;
  getline(cin, buf);
  bool flag = this->ctl.add_data(buf);
  if (flag == true)
  {
    printf("success add data %s\n\n", buf.c_str());
    this->stat();
  }
  else
  {
    printf("before using name.. %s\n\n", buf.c_str());
  }
  sigprocmask(SIG_UNBLOCK, &st, NULL);

  return;
}

void UI::del_data_UI()
{
  printf("input data name\n");
  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  string buf;
  sigprocmask(SIG_BLOCK, &st, NULL);
  getline(cin, buf);
  sigprocmask(SIG_UNBLOCK, &st, NULL);
  bool flag = this->ctl.del_data(buf);
  if (flag == true)
  {
    printf("success del data %s\n\n", buf.c_str());
    this->stat();
  }
  else
  {
    printf("before using name.. %s\n\n", buf.c_str());
  }
  return;
}

void UI::stat()
{
  printf("\n====================== Timer Stat ======================\n");
  const map<string, Data> &db = this->ctl.get_db_read();

  this->ctl.get_l_time();
  for (auto it = db.cbegin(); it != db.cend(); it++)
  {
    if (it->first == "local_time")
    {
      printf("%s -> %s\n", it->first.c_str(),
             it->second.get_data_read().time_s.c_str());
    }
    else
    {
      bool tmp_flag = it->second.get_run();
      auto &tmp_read = it->second.get_data_read();
      string total_time = this->ctl.convert(tmp_read.total_time);

      string tmps = "";
      if (tmp_flag)
      {
        tmps = "run";
      }
      else
      {
        tmps = "stop";
      }
      printf("%s -> %s   stat -> %s\n\n", it->first.c_str(), total_time.c_str(),
             tmps.c_str());
    }
  }
}

void UI::run_stat()
{
  while (1)
  {
    this->stat();
    sleep(5);
    if (UI::sig_int_flag == true)
    {
      break;
    }
  }
  UI::sig_int_flag = false;
  printf("\t breck run_stat() \n");
  return;
}
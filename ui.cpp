#include "timer.h"
#include <csignal>
#include <pthread.h>

using namespace std;

UI::UI(Control &ctl) : ctl(ctl)
{
}
UI::~UI()
{
}

// ==================== static ====================
bool UI::sig_int_flag = false;

void *UI::th_func(void *vp)
{
  UI *p_this = (UI *)vp;
  p_this->run_stat();
  return NULL;
}

bool UI::hup_print()
{
  printf("want cur data write -> [Y]\n");

  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  sigprocmask(SIG_BLOCK, &st, NULL);
  string buf;
  getline(cin, buf);
  sigprocmask(SIG_UNBLOCK, &st, NULL);
  if (buf.empty() || buf == "Y" || buf == "y")
  {
    return true;
  }
  return false;
}

// ==================== static ====================

void UI::menu()
{

  this->read_UI();

  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);

  string buf_s = "";
  while (buf_s != "0")
  {
    printf("\n====================== Timer Menu ======================\n");
    printf("[1] stat   [2] add   [3] del   [4] set   [5] run\n"
           "[6] save   [7] read  [8] log   [0] eixt\n");

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
      pthread_t tid;

      int flag_pthread_create =
          pthread_create(&tid, NULL, th_func, (void *)this);
      if (flag_pthread_create != 0)
      {
        string tmp1("pthread_create(&tid, NULL, th_func, (void *)this)\n ");
        string tmp2((strerror(errno)));
        Exception err(tmp1, tmp2, (int)errno);
        throw err;
      }

      sigset_t st;
      sigemptyset(&st);
      sigfillset(&st);
      sigprocmask(SIG_BLOCK, &st, NULL);
      int flag_pthread_join = pthread_join(tid, NULL);
      if (flag_pthread_join != 0)
      {
        string tmp1("pthread_join(tid, NULL)\n");
        string tmp2((strerror(errno)));
        Exception err(tmp1, tmp2, (int)errno);
        throw err;
      }
      sigprocmask(SIG_BLOCK, &st, NULL);
    }
    else if (buf_s == "6")
    {
      this->save_UI();
    }
    else if (buf_s == "7")
    {
      this->read_UI();
    }
    else if (buf_s == "8")
    {
      this->log_UI();
    }
    else if (buf_s == "0")
    {
      this->exit_save_UI();
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

  printf("[1] run   [2] stop   [3] edit\n");
  string buf_s;
  getline(cin, buf_s);

  if (buf_s == "1" || buf_s == "2")
  {
    int ret0 = this->ctl.set_run_Ctl(buf_org, buf_s);
    this->check_UI(ret0, "set_run_Ctl");
  }
  else if (buf_s == "3")
  {
    printf(">>>   **:**:**   <<< \n");
    string set_t;
    getline(cin, set_t);
    if (set_t.size() == 8)
    {
      string check_1 =
          set_t.substr(0, 2) + set_t.substr(3, 2) + set_t.substr(6);
      string check_2 = set_t.substr(2, 1) + set_t.substr(5, 1);

      bool flag_1 = all_of(check_1.begin(), check_1.end(), [](char x)
                           { return isdigit(x) > 0 ? true : false; });

      bool flag_2 = all_of(check_2.begin(), check_2.end(),
                           [](char x) { return x == ':' ? true : false; });

      if (flag_1 && flag_2)
      {
        int ret1 = this->ctl.set_Data_time_ctl(set_t, buf_org);
        this->check_UI(ret1, "set_Data_time_ctl");
      }
      else
      {
        printf("not available value\n");
      }
    }
    else
    {
      printf("not available value\n");
    }
  }
  else
  {
    printf("not available value\n");
    return;
  }

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
  printf("input data name   ! ~19byte\n");
  string buf;
  getline(cin, buf);
  if (buf.size() > 19)
  {
    printf("buffer over flow\n");
    return;
  }
  bool flag = this->ctl.add_data(buf);
  if (flag == true)
  {
    printf("success add data %s\n\n", buf.c_str());
    this->stat();
  }
  else
  {
    printf("before using name.. or size up to 4 %s\n\n", buf.c_str());
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
  auto it = db.find("local_time");

  printf("%s -> %s\n", it->first.c_str(),
         it->second.get_data_read().time_s.c_str());

  for (auto it = db.cbegin(); it != db.cend(); it++)
  {
    if (it->first != "local_time")
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
  UI::sig_int_flag = false;
  while (1)
  {
    this->stat();
    sleep(60);
    if (UI::sig_int_flag == true)
    {
      break;
    }
  }
  UI::sig_int_flag = false;
  printf("\t breck run_stat() \n");
  return;
}

void UI::save_UI()
{
  int flag = Control::back_up();
  if (flag == 0)
  {
    printf("success Control::back_up()\n");
  }
  else
  {
    printf("failed Control::back_up()\n");
  }
  return;
}

void UI::read_UI()
{
  auto read_data = this->ctl.read_ctl();

  printf("\n====================== read data ======================\n");

  auto it = read_data.find("local_time");
  if (it == read_data.cend())
  {
    printf("back_up.txt -> empty\n");
    return;
  }

  printf("%s -> %s\n", it->first.c_str(),
         it->second.get_data_read().time_s.c_str());

  for (auto it = read_data.cbegin(); it != read_data.cend(); it++)
  {
    if (it->first != "local_time")
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
  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);

  sigprocmask(SIG_BLOCK, &st, NULL);
  printf("want load? [Y]\n");
  string buf;
  getline(cin, buf);
  sigprocmask(SIG_UNBLOCK, &st, NULL);

  if (buf.empty() || buf == "Y" || buf == "y")
  {
    this->ctl.change_db(read_data);
    printf("success load\n");
  }
  else
  {
    printf("not load\n");
    return;
  }

  return;
}

void UI::exit_save_UI()
{

  this->stat();

  printf("want cur data save ?   [Y]\n");

  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  sigprocmask(SIG_BLOCK, &st, NULL);
  string buf;
  getline(cin, buf);
  sigprocmask(SIG_UNBLOCK, &st, NULL);
  if (buf.empty() || buf == "Y" || buf == "y")
  {
    int flag_exit = this->ctl.exit_save_ctl();
    if (flag_exit == 0)
    {
      printf("success exit_save()\n");
    }
  }
  return;
}

void UI::log_UI()
{
  printf("[1] log print   [2] log dell\n");

  sigset_t st;
  sigemptyset(&st);
  sigfillset(&st);
  sigprocmask(SIG_BLOCK, &st, NULL);
  string buf;
  getline(cin, buf);
  sigprocmask(SIG_UNBLOCK, &st, NULL);

  if (buf == "1")
  {
    this->log_print_UI();
  }
  else if (buf == "2")
  {
    this->log_dell_UI();
  }
  else
  {
    printf("unavailable value\n");
  }

  return;
}

void UI::log_print_UI()
{
  printf("\n====================== Total log ======================\n");
  const vector<map<string, Data>> log = this->ctl.log_read_ctl();

  if (log.empty())
  {
    printf("log empty\n");
    return;
  }

  for (auto it_1 = log.cbegin(); it_1 != log.cend(); it_1++)
  {
    auto it = it_1->find("local_time");

    printf("\n====================== ///////// ======================\n");
    printf("%s -> %s\n", it->first.c_str(),
           it->second.get_data_read().time_s.c_str());

    for (auto it = it_1->cbegin(); it != it_1->cend(); it++)
    {
      if (it->first != "local_time")
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
        printf("%s -> %s   stat -> %s\n\n", it->first.c_str(),
               total_time.c_str(), tmps.c_str());
      }
    }
  }
  return;
}

void UI::log_dell_UI()
{
  int ret_log_dell_ctl = this->ctl.log_dell_ctl();
  if (ret_log_dell_ctl == 0)
  {
    printf("success remove log\n");
  }
  else
  {
    printf("failed remove log\n");
  }
  return;
}
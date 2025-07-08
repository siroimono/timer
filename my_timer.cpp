#include "timer.h"

using namespace std;

int main(int argc, char **argv)
{
  try
  {
    Control ctl;
    UI ui(ctl);
    ui.menu();
  }
  catch (Exception err)
  {
    printf("err type == %s\n"
           "err name == %s\n"
           "err no ==%d\n",
           err.get_err_type().c_str(), err.get_err_name().c_str(),
           err.get_err_no());
  }
  return 0;
}

/*
타이머를 제작하자...
++ 어제의 데이터를 기록해서 파일로 저장해둠
-> UI에 해당 데이터 cat으로 읽을 수 있도록
-> fork() -> execv 구현

타이머의 필요 기능
1. 현재 시간을 출력
2. 각각의 타겟에 대한 누적 시간 출력
ex) A에 대한 누적 시간 1시간 30분...
    B에 대한 누적 시간 3시간 ....


대충 구성도
타이머 실행 시킴
init() 실행 < 멀티 쓰레드로 돌려보자
-> 현재 시간을 받아와서 출력 대기값으로 저장함
-> 분마다 대기 값 갱신시킴
-> 얘는 이 루프 계속 돌려둠

2번 시그널 보냄 -> 뭐 할건지? UI 실행댐
원하는 거 실행시킴
UI에는 타겟 추가 / 타겟 시간 누적 시작 / 정지 / 삭제
이정도만 넣어두면 될 듯?

MVC로 나누면
UI / DATA / C / main

*/

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
1. 누적 시간 수동으로 수정가능하게
2. 요일 출력 추가

3. 멀티스레드
4. stat() 출력 정렬

5. 누적 시간 백업
  1. 파일에 적는 형식을 만듦 + add할 때 20바이트 미만으로 제어 + 갯수 3 이하
  2. 오프셋 끝에서 해당 형식 바이트 만큼 뒤로와서 읽음
  3. 해당 값이 금일 시간이 존재하면 그 시간 로드할거냐고 물어봄
  4. 해당 값 받아와서 이어감

*/

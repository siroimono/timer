# 컴파일러와 옵션
CC = g++
CFLAGS = -Wall -g

# 최종 실행파일 이름
TARGET = my_timer

# 개별 오브젝트 파일들
OBJS = my_timer.o ui.o exception.o data.o control.o RAII_fd.o

# 1. 최종 타겟: my_timer 실행파일
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# 2. 개별 오브젝트 생성 규칙
my_timer.o: my_timer.cpp timer.h
	$(CC) $(CFLAGS) -c my_timer.cpp -o my_timer.o

ui.o: ui.cpp timer.h
	$(CC) $(CFLAGS) -c ui.cpp -o ui.o

exception.o: exception.cpp timer.h
	$(CC) $(CFLAGS) -c exception.cpp -o exception.o

data.o: data.cpp timer.h
	$(CC) $(CFLAGS) -c data.cpp -o data.o

control.o: control.cpp timer.h
	$(CC) $(CFLAGS) -c control.cpp -o control.o
	
RAII_fd.o: RAII_fd.cpp timer.h
	$(CC) $(CFLAGS) -c RAII_fd.cpp -o  RAII_fd.o

# 3. 청소용
clean:
	rm -f *.o $(TARGET)


/* Программа управления COM-портом. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define BUFFER_SIZE 256

int open_port(void);

void main (void) {
  int fd;
  int n; /* */ int d = 0;
  char buff[BUFFER_SIZE]; /* буфер ввода */
  char *bufptr;           /*  */

//  memset(buf, sizeof(buf), 0);

  printf("Test!\n");
  fd =  open_port();

  /* отправка тестовой посылки в COM */
//  n = write(fd, "TEST!\nCOM-port is work!\n", 24);
//  if (n < 0) {
//    fputs("write() of 5 bytes failed!\n", stderr);
//  }

  /* чтение данных из COM */
  while ( d != 'q') {
    sleep(3);
    n = read(fd, buff, BUFFER_SIZE);
//    buff[n] = 0;
    fputs(buff, stdout);
    printf("n = %d\n", n);
    printf("\nДля выхода нажать \"q\"\n");
    memset(buff, 0, BUFFER_SIZE);
    d = getchar();
//    bufptr = strstr(buff, "St");
//    printf("string is %d\n", bufptr);
//  if (d == 'q') break;
  } 
  /* закрытие COM-порта */
  close(fd);
}

int open_port(void) {
  int fd; /* Файловый дескриптор порта */
  struct termios options;

  /* открытие порта */
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    perror("open_port: Unable to open /dev/ttyUSB0 - ");
    return fd;
  }
  else {
    fcntl(fd, F_SETFL, 0);

    /* получение текущих опций */
    tcgetattr(fd, &options);

    /* установка скорости передачи */
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    cfmakeraw(&options);

    options.c_cflag &= ~(CBAUD | CSTOPB);
    options.c_cflag |= (B115200 | CLOCAL | CREAD);

    /* разрешение приёмника и установка локального режима */
//    options.c_cflag |= (CLOCAL | CREAD);

    /* установка контроля чётности и размеров символов */
//    options.c_cflag &= ~PARENB;
//    options.c_cflag &= ~CSTOPB;
//    options.c_cflag &= ~CSIZE;
//    options.c_cflag |= CS8;

    /* деактивирование аппаратного управления потоком */
//    options.c_cflag &= ~CNEW_RTSCTS;

    /* выбор неканонического (raw) ввода */
//    options.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* выбор необработанного (raw) вывода */
//    options.c_cflag &= ~OPOST;

    /* установка новыз опций для порта */
    tcsetattr(fd, TCSANOW, &options);
  }
  return fd;
}

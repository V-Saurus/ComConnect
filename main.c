/* Программа управления COM-портом. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define BUFFER_SIZE 64

int open_port(void);

void main (void) {
  int fd;
  int n; /* */ int d = 0;
  char buff[BUFFER_SIZE + 1]; /* буфер ввода */
  char *bufptr;           /*  */

//  memset(buf, sizeof(buf), 0);

  printf("Test!\n");
  fd =  open_port();

  /* отправка тестовой посылки в COM */
  n = write(fd, "TEST!\nCOM-port is work!\n", 24);
  if (n < 0) {
    fputs("write() of 5 bytes failed!\n", stderr);
  }

  /* чтение данных из COM */
  while ((n = read(fd, buff, BUFFER_SIZE)) > 0) {
    buff[n] = 0;
    fputs(buff, stdout);
   d = getchar();
//    bufptr = strstr(buff, "St");
//    printf("string is %d\n", bufptr);
    if (d == 'q') break;
  }
  fputs(buff, stdout);
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

    /* разрешение приёмника и установка локального режима */
    options.c_cflag |= (CLOCAL | CREAD);

    /* установка новыз опций для порта */
    tcsetattr(fd, TCSANOW, &options);

    /* установка контроля чётности и размеров символов */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    /* деактивирование аппаратного управления потоком */
//    options.c_cflag &= ~CNEW_RTSCTS;

    /* выбор неканонического (raw) ввода */
    options.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* выбор необработанного (raw) вывода */
    options.c_cflag &= ~OPOST;
  }
  return fd;
}

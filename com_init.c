/* Файл инициализации COM-порта */


#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

int open_port(void) {
  int fd; /* Файловый дескриптор порта */
  struct termios options;

  /* открытие порта */
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    perror("open_port: Unable to open /dev/ttyUSB0 ");
    return fd;
  }
  else {
    fcntl(fd, F_SETFL, FNDELAY);

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


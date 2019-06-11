/* Программа управления COM-портом. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "com_init.h"
#include "send_get.h"

#define BUFFER_SIZE 256

void main (void) {
  int fd;
  int n; /* */ int d = 0;
  char buff[BUFFER_SIZE]; /* буфер ввода */
  char *bufptr;           /*  */

//  memset(buf, sizeof(buf), 0);

  printf("Test!\n");
  memset(buff, 0, BUFFER_SIZE);
  fd =  open_port();

  /* отправка тестовой посылки в COM */
  send_comand(fd, U);
  getchar();

  /* чтение данных из COM */
  get_test(fd);

  /* закрытие COM-порта */
  close(fd);
}


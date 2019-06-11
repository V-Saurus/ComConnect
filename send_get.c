/* Модуль отправки команд и файла в COM-порт и приём и расшифровка ответных сообщений */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "send_get.h"

#define BUFFER_SIZE 256

/* получение ответа без КС из порта */
int get_unswer(int comand) {
  
}

/* получение ответа с КС из порта */
int get_ks_unswer() {
  
}

/* отправка команды в порт */
int send_comand(int fd, int command) {
  int cerr;

  switch (command) {
  case U:
    cerr = write(fd, "u\n", 2);
    if (cerr < 0) {
      fputs("send \"u\" failed!\n", stderr);
    }
    return cerr;

  case LI:
    cerr = write(fd, "li\n", 3);
    if (cerr < 0) {
      fputs("send \"li\" failed!\n", stderr);
    }
    return cerr;

  case B:
    cerr = write(fd, "b\n", 2);
    if (cerr < 0) {
      fputs("send \"b\" failed!\n", stderr);
    }
    return cerr;

  case C:
    cerr = write(fd, "c\n", 2);
    if (cerr < 0) {
      fputs("send \"c\" failed!\n", stderr);
    }
    return cerr;
  }
}

/* отправка файла в порт */
int send_file() {
  
}


/* тестовая функция приёма данных из порта */
void get_test(int fd) {
  int n; /* */
  int d = 0;
  char buff[BUFFER_SIZE]; /* буфер ввода */

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
}


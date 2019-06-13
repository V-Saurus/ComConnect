/* Модуль отправки команд и файла в COM-порт и приём и расшифровка ответных сообщений */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "send_get.h"

#define BUFFER_SIZE 256
#define STR_SIZE 20

/* получение ответа без КС из порта */
int get_unswer(int fd, int comand) {
  int n;                           /* количество считанных символов */
  int d = 0;
  int err_counter = 0;             /* счётчик ошибок проверок маски */
  int ok_flag = 0;
  unsigned char buff[BUFFER_SIZE]; /* буфер ввода */
  char str[STR_SIZE];                     /* строка для поиска */
  char *istr;

  memset(str, 0, STR_SIZE);

  /* выбор подстроки */
  switch (comand) {
  case H:
    str[0] = 60; str[1] = 72; str[3] = 62; str[4] = 32; str[5] = 32;
    str[6] = 45; str[7] = 32; str[8] = 241; str[9] = 239; str[10] = 240;
    str[11] = 224; str[12] = 226; str[13] = 234; str[14] = 224; str[15] = 13;
    str[16] = 13; str[17] = 10; str[18] = 46;
    break;
  case U:
    break;
  case LI:
    break;
  case B:
    break;
  case C:
    break;
  case R:
    break;
  default:
    break;
  }
  /* чтение данных из COM */
  while ((n != -1) && (err_counter < 5)) {
//    sleep(3);
    n = read(fd, buff, BUFFER_SIZE);
//    buff[n] = 0;
    fputs(buff, stdout);
    printf("n = %d\n", n);
    printf("err = %d\n", err_counter);
//    printf("\nДля выхода нажать \"q\"\n");
    istr = strstr(buff, str);
    if (istr == NULL) {
      printf("String do not detected!");
      err_counter++;
      d = getchar();
    }
    else {
      printf("String detected!");
      d = getchar();
      ok_flag = 1;
    }
//    bufptr = strstr(buff, "St");
//    printf("string is %d\n", bufptr);
//  if (d == 'q') break;
    memset(buff, 0, BUFFER_SIZE);
  }
  printf("ok_flag = %d\n", ok_flag);
  return ok_flag;
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
  case H:
    cerr = write(fd, "h\n", 2);
    if (cerr < 0) {
      fputs("send \"h\" failed!\n", stderr);
    }
    return cerr;
  case R:
    cerr = write(fd, "r\n", 2);
    if (cerr < 0) {
      fputs("send \"r\" failed!\n", stderr);
    }
    return cerr;
  default:
    fputs("sending failed!\n", stderr);
    return -1;
  }
}

/* отправка файла в порт */
int send_file() {
  
}


/* тестовая функция приёма данных из порта */
void get_test(int fd) {
  int n; /* */
  int d = 0;
  unsigned char buff[BUFFER_SIZE]; /* буфер ввода */
  char str[4] = "COM";
  char *istr;

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
/*    { unsigned int i;
    for (i = 0; i < n; i++) {
      printf("%d\n", buff[i]);
    }
    }*/
    printf("n = %d\n", n);
    printf("\nДля выхода нажать \"q\"\n");
    istr = strstr(buff, str);
    if (istr == NULL) 
      printf("String do not detected!");
    else
      printf("String detected!");
    d = getchar();
//    bufptr = strstr(buff, "St");
//    printf("string is %d\n", bufptr);
//  if (d == 'q') break;
    memset(buff, 0, BUFFER_SIZE);
  }
}



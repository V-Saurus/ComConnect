/* Модуль отправки команд и файла в COM-порт и приём и расшифровка ответных сообщений */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "send_get.h"
#include "strindex.h"

#define BUFFER_SIZE 256
#define STR_SIZE 20
#define LENGTH_KSUM 14

/* получение ответа без КС из порта */
int get_answer(int fd, int comand) {
  int n = 0;                       /* количество считанных символов */
  int d = 0; //
  int err_counter = 0;             /* счётчик ошибок проверок маски */
  int ok_flag = 0;                 /* флаг наличия ожидаемой подстроки */
  unsigned char buff[BUFFER_SIZE]; /* буфер ввода */
  char str[STR_SIZE];              /* строка для поиска */
  char *istr;

  memset(str, 0, STR_SIZE);

  /* выбор подстроки */
  switch (comand) {
  case H:   /**/
    str[0] = 60; str[1] = 72; str[2] = 62; str[3] = 32; str[4] = 32;
    str[5] = 45; str[6] = 32; str[7] = 241; str[8] = 239; str[9] = 240;
    str[10] = 224; str[11] = 226; str[12] = 234; str[13] = 224; str[14] = 13;
    str[15] = 13; str[16] = 10; str[17] = 46;
    break;
  case U:   /*"U 'CR' 'LF' Стирание..."*/
    str[0] = 85; str[1] = 13; str[2] = 10; str[3] = 209; str[4] = 242;
    str[5] = 232; str[6] = 240; str[7] = 224; str[8] = 237; str[9] = 232;
    str[10] = 229; str[11] = 46; str[12] = 46; str[13] = 46;
    break;
  case U2:  /*"'CR' 'CR' 'LF' ."*/
    str[0] = 13; str[1] = 13; str[2] = 10; str[3] = 46;
//    str[0] = 13; str[1] = 10; str[2] = 46;
    break;
  case LI:  /*"LI 'CR' 'LF' Загрузка образа."*/
    str[0] = 76; str[1] = 73; str[2] = 13; str[3] = 10; str[4] = 199;
    str[5] = 224; str[6] = 227; str[7] = 240; str[8] = 243; str[9] = 231;
    str[10] = 234; str[11] = 224; str[12] = 32; str[13] = 238; str[14] = 225;
    str[15] = 240; str[16] = 224; str[17] = 231; str[18] = 224; str[19] = 46;
    break;
  case B:   /*"B 'CR' 'LF' Запись ... 'CR' 'LF' Адрес "*/
    str[0] = 66; str[1] = 13; str[2] = 10; str[3] = 199; str[4] = 224;
    str[5] = 239; str[6] = 232; str[7] = 241; str[8] = 252; str[9] = 46;
    str[10] = 46; str[11] = 46; str[12] = 13; str[13] = 10; str[14] = 192;
    str[15] = 228; str[16] = 240; str[17] = 229; str[18] = 241; str[19] = 32;
    break;
  case C:   /*"C 'CR' 'LF' Сравнение... 'CR' 'LF' Адр"*/
    str[0] = 67; str[1] = 13; str[2] = 10; str[3] = 209; str[4] = 240;
    str[5] = 224; str[6] = 226; str[7] = 237; str[8] = 229; str[9] = 237;
    str[10] = 232; str[11] = 229; str[12] = 46; str[13] = 46; str[14] = 46;
    str[15] = 13; str[16] = 10; str[17] = 192; str[18] = 228; str[19] = 240;
 break;
  case R:   /*!!! ВНИМАНИЕ !!! Массив информации от ВИНС-М при старте превышает 256 символов, при этом
              КС выпадает на переход. Для корректной работы необходимо увеличить массив принятых символов.
              "Контрольная сумма:  "*/
    str[0] = 202; str[1] = 238; str[2] = 237; str[3] = 242; str[4] = 240;
    str[5] = 238; str[6] = 235; str[7] = 252; str[8] = 237; str[9] = 224;
    str[10] = 255; str[11] = 32; str[12] = 241; str[13] = 243; str[14] = 236;
    str[15] = 236; str[16] = 224; str[17] = 58; str[18] = 32; str[19] = 32;
    break;
  default:
    break;
  }
  /* чтение данных из COM */
  while ((n != -1) && (err_counter < 5)) {
    n = read(fd, buff, BUFFER_SIZE);
//    fputs(buff, stdout);
    istr = strstr(buff, str);
    if (istr == NULL) {
      err_counter++;
    }
    else {
      ok_flag = 1;
    }
    memset(buff, 0, BUFFER_SIZE);
  }
//  printf("\nok_flag = %d\nn = %d\n", ok_flag, n);
  return ok_flag;
}


/* получение ответа с КС из порта */
int get_ks_answer(int fd, int comand, char out_ksum[]) {
  int n = 0;                       /* количество считанных символов */
  int poz = 0;                     /* позиция позиция подстроки */
  int err_counter = 0;             /* счётчик ошибок проверок маски */
  int ok_flag = 0;                 /* флаг наличия ожидаемой подстроки */
  unsigned char buff[BUFFER_SIZE]; /* буфер ввода */
  char str[STR_SIZE];              /* строка для поиска */
  char *istr;                      /* указатель на первый элемент подстроки */

  char ksum[LENGTH_KSUM]; /* "Контр. сумма  " */
  ksum[0] = 202; ksum[1] = 238; ksum[2] = 237; ksum[3] = 242; ksum[4] = 240; ksum[5] = 46; ksum[6] = 32;
  ksum[7] = 241; ksum[8] = 243; ksum[9] = 236; ksum[10] = 236; ksum[11] = 224; ksum[12] = 32; ksum[13] = 32;//TODO

  memset(str, 0, STR_SIZE);

  /* выбор подстроки */
  switch (comand) {
  case B:   /*"B 'CR' 'LF' Запись ... 'CR' 'LF' Адрес "*/
    str[0] = 66; str[1] = 13; str[2] = 10; str[3] = 199; str[4] = 224;
    str[5] = 239; str[6] = 232; str[7] = 241; str[8] = 252; str[9] = 46;
    str[10] = 46; str[11] = 46; str[12] = 13; str[13] = 10; str[14] = 192;
    str[15] = 228; str[16] = 240; str[17] = 229; str[18] = 241; str[19] = 32;
    break;
  case C:   /*"C 'CR' 'LF' Сравнение... 'CR' 'LF' Адр"*/
    str[0] = 67; str[1] = 13; str[2] = 10; str[3] = 209; str[4] = 240;
    str[5] = 224; str[6] = 226; str[7] = 237; str[8] = 229; str[9] = 237;
    str[10] = 232; str[11] = 229; str[12] = 46; str[13] = 46; str[14] = 46;
    str[15] = 13; str[16] = 10; str[17] = 192; str[18] = 228; str[19] = 240;
 break;
  default:
    break;
  }
  /* чтение данных из COM */
  while ((n != -1) && (err_counter < 5)) {
    n = read(fd, buff, BUFFER_SIZE);
//    fputs(buff, stdout);
    istr = strstr(buff, str);
    if (istr == NULL) {
      err_counter++;
    }
    else {
      ok_flag = 1;
    }
  }
  if (ok_flag) {
    poz = strindex(buff, ksum);
//    printf("%s\n%s\nd = %d\n", buff, ksum, poz);
    if (poz != -1) {
      int i;
      for (i = 0; i < 8; i++) {
//      printf("%c", buff[poz + LENGTH_KSUM + i]);
        out_ksum[i] = buff[poz + LENGTH_KSUM + i];
      }
//      printf("%s\n", out_ksum);
      return 1;
    }
  }
  return 0;
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
int send_file(int fdi, int fdc) {
  char buff[BUFFER_SIZE];
  char ch = 0;
  int n = 0;

  while ((n = read (fdi, buff, BUFFER_SIZE)) > 0) {
    write(fdc, buff, n);
    usleep(25000);
//    write(1, buff, n);
  }
  return 1;
}

/* очистка буфера*/
int flush_data(int fd) {
  int n = 0;                       /* количество считанных символов */
  unsigned char buff[BUFFER_SIZE]; /* буфер ввода */

  while (n != -1) {
    n = read(fd, buff, BUFFER_SIZE);
  }
 return n;
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

/*//// вывод текста
    fputs(buff, stdout);
/*/

///// вывод кодов символов
    { int i;
    for (i = 0; i < n; i++) {
      printf("%d\n", buff[i]);
    }
    }
//

///// вывод количества принятых символов
    printf("n = %d\n", n);
//
    printf("\nДля выхода нажать \"q\"\n");

/*//// Поиск строки
    istr = strstr(buff, str);
    if (istr == NULL) 
      printf("String do not detected!");
    else
      printf("String detected!");
/*/

    d = getchar();
    memset(buff, 0, BUFFER_SIZE);
  }
}




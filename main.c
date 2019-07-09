/* Программа управления COM-портом. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "com_init.h"
#include "send_get.h"

#define BUFFER_SIZE 256
#define KSUM_LENGTH 8

int main (int argc, char ** argv) {
  int fd, fdi;                        /* файловые дескрипторы */
  char buff[BUFFER_SIZE];             /* буфер ввода */
  char *check;                        /* проверка расширения файла */
  char load_ksum[KSUM_LENGTH] = {0};  /* контрольная сумма при загрузке (от целевого устройства) */
  char check_ksum[KSUM_LENGTH] = {0}; /* контрольная сумма при проверке (от целевого устройства) */

  memset(buff, 0, BUFFER_SIZE);

  /* проверка наличия одного входного файла */
  if (argc < 2) {
    printf("No file selected!\n");
    return -1;
  }
  if (argc > 2) {
    printf("Selected a lot of files!\n");
    return -1;
  }

  /* проверка расширения входного файла */
  check = strstr(argv[1], ".i10");
  if ((check == NULL) || (strlen(argv[1]) != (check - argv[1] + 4))) {
    printf("Type of file is not '.i10'\n");
    return -1;
  }

  /* открытие входного файла */
  fdi = open(argv[1], O_RDONLY);
  if (fdi == -1) {
    perror("open_file: Unable to open file ");
    return -1;
  }

  /* открытие и настройка COM-порта */
  fd =  open_port();
  if (fd == -1) {
    return -1;
  }

  /* посылка команд и обработка ответов */
  printf("Connecting...\n");
  send_comand(fd, H);
  sleep(1);
  if (!get_answer(fd, H)) {
    printf("Device not detected!\n");
    return -2;
  }

  printf("Errasing...");
  send_comand(fd, U);
  sleep(1);
  if (!get_answer(fd, U)) {
    printf("No answer from device. Errasing does not start.\n");
    return -2;
  }

  printf(" (~30 s)\n");
  sleep(30);
  if (!get_answer(fd, U2)) {
    printf("No answer from device. Errasing fault.\n");
    return -2;
  }

  printf("Errasing OK.\n");
  send_comand(fd, LI);
  sleep(3);
  if (!get_answer(fd, LI)) {
    printf("No answer from device. Loading image will not start.\n");
    return -2;
  }

  printf("Loading file... (~2 min)\n");
  send_file(fdi, fd);
  sleep(3);
  close(fdi);
  sleep(3);
  flush_data(fd);

  printf("ROM recording...\n");
  send_comand(fd, B);
  sleep(3);
  if(!get_ks_answer(fd, B, load_ksum)) {
    printf("No answer from device. ROM record fault.\n");
    return -2;
  }
  printf("Loading checksum: 0x%s\n", load_ksum);

  printf("Checking...\n");
  send_comand(fd, C);
  sleep(3);
  if(!get_ks_answer(fd, C, check_ksum)) {
    printf("No answer from device. Checking fault.\n");
    return -2;
  }
  printf("Checking checksum: 0x%s\n", check_ksum);

  /* сравнение контрольных сумм */
  if (memcmp(load_ksum, check_ksum, KSUM_LENGTH) == 0) {
    printf("Checksums ara equal.\n");
  }
  else {
    printf("Checksums do not match!.\n");
    return -3;
  }

  printf("Run loaded program.\n");
  send_comand(fd, R);
  sleep(3);
  get_answer(fd, R);

  /* закрытие COM-порта */
  close(fd);
}

/* Программа управления COM-портом. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "com_init.h"
#include "send_get.h"

#define BUFFER_SIZE 256

int main (int argc, char ** argv) {
  int fd, fdi;
  int n; /* */ int d = 0;
  char buff[BUFFER_SIZE]; /* буфер ввода */
  char *check;           /*  */

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

  /* открытие входного файла */
  fdi = open(argv[1], O_RDONLY);
  if (fdi == -1) {
    perror("open_file: Unable to open file ");
    return -1;
  }

  /* проверка расширения входного файла */
  check = strstr(argv[1], ".i10");
  if ((check == NULL) || (strlen(argv[1]) != (check - argv[1] + 4))) {
    printf("Type of file is not '.i10'\n");
    return -1;
  }

  /* открытие и настройка COM-порта */
  fd =  open_port();
  if (fd == -1) {
    return -1;
  }

  /* посылка команд и обработка ответов */
  printf("SEND H\n");
  send_comand(fd, H);
  sleep(1);
  if (!get_answer(fd, H)) {
    printf("Device not detected!\n");
    return -2;
  }

  printf("SEND U\n");
  send_comand(fd, U);
  sleep(1);
  if (!get_answer(fd, U)) {
    printf("No answer from device: (Errase).\n");
    return -2;
  }

  printf("Errasing...\n");
  sleep(30);
  if (!get_answer(fd, U2)) {
    printf("No answer from device: (Errasing complite).\n");
    return -2;
  }

  printf("SEND LI\n");
  send_comand(fd, LI);
  sleep(3);
  if (!get_answer(fd, LI)) {
    printf("No answer from device: (Load image).\n");
    return -2;
  }

  printf("Loading file...\n");
  send_file(fdi, fd);
  sleep(3);
  close(fdi);
  sleep(3);
//  printf("Стирание буфера\n");
  flush_data(fd);

  printf("SEND B\n");
  send_comand(fd, B);
  sleep(3);
  if(!get_answer(fd, B)) {
    printf("No answer from device: (ROM record).\n");
    return -2;
  }

  printf("SEND C\n");
  send_comand(fd, C);
  sleep(3);
  if(!get_answer(fd, C)) {
    printf("No answer from device: (Check).\n");
    return -2;
  }

  printf("SEND R\n");
  send_comand(fd, R);
  sleep(3);
  get_answer(fd, R);

  /* закрытие COM-порта */
  close(fd);
}

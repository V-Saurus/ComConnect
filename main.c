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
  char *bufptr;           /*  */

  if (argc < 2) {
    printf("No file selected!\n");
    return -1;
  }
  if (argc > 2) {
    printf("Selected a lot of files!\n");
    return -1;
  }

//  memset(buf, sizeof(buf), 0);
  fdi = open(argv[1], O_RDONLY);

  printf("Test!\n");
  memset(buff, 0, BUFFER_SIZE);
  fd =  open_port();

  printf("SEND H\n");
  send_comand(fd, H);
  usleep(300000);
  get_unswer(fd, H);

  printf("SEND U\n");
//  getchar();
  send_comand(fd, U);
  sleep(1);
  get_unswer(fd, U);

  printf("SLEEP 30\n");
  sleep(30);
//  getchar();
  get_unswer(fd, U2);

  printf("SEND LI\n");
//  getchar();
  send_comand(fd, LI);
//  getchar();
  sleep(3);
  get_unswer(fd, LI);

  printf("Loading file...\n");
  send_file(fdi, fd);
  sleep(3);
  close(fdi);
//  getchar();
  sleep(3);
  printf("Стирание буфера\n");
  flush_data(fd);

  printf("SEND B");
//  getchar();
  send_comand(fd, B);
 sleep(3);
//  getchar();
  get_unswer(fd, B);

  printf("SEND C\n");
//  getchar();
  send_comand(fd, C);
  sleep(3);
//  getchar();
  get_unswer(fd, C);


//  getchar();


  /* закрытие COM-порта */
  close(fd);
}

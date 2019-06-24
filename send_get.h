/* Заголовочный файл модуля send_get.c */

#define U 1
#define LI 2
#define B 3
#define C 4
#define R 5
#define H 6
#define U2 7

/* получение ответа без КС из порта */
int get_unswer(int fd, int comand);

/* получение ответа с КС из порта */
int get_ks_unswer();

/* отправка команды в порт */
int send_comand(int fd, int command);

/* отправка файла в порт */
int send_file(int fdi, int fdc);

/* очистка буфера приёмника */
int flush_data();

/* тестовая функция приёма данных из порта */
void get_test(int fd);

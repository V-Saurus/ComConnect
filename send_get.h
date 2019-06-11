/* Заголовочный файл модуля send_get.c */

#define U 1
#define LI 2
#define B 3
#define C 4

/* получение ответа без КС из порта */
int send_comand(int fd, int command);

/* получение ответа с КС из порта */
int get_ks_unswer();

/* отправка команды в порт */
int send_comand();

/* отправка файла в порт */
int send_file();

/* тестовая функция приёма данных из порта */
void get_test(int fd);
Описание к проекту ComConnect.

Программа разработана для прошивки целевого устройства по COM-порту.

Программа принимает путь к файлу прошивки, проверяет его наличие и расширение файла прошивки.
Целевое устройство поддерживает определённый перечень команд для прошивки и запуска прошитой программы.
Программа подготавливает целевое устройство с помощью передачи команд для приёма файла прошивки,
передаёт файл прошивки, обрабатывает ответы от целевого устройства,
при отсутствии ожидаемой информации выдаёт сообщения о соответствующей ошибке.

Установка:
  cd ComConnect
  make clean  
  make

Использование:
  cd ComConnect
  ./comcon [FILE(*.i10)]

Файлы проекта:
  com_init.c - содержит процедуры настройки COM-порта
  main.c - содержит вызовы функций посылки команд, обработки ответов, передачи файла прошивки и обработку ошибок.
  send_get.c - содержит процедуры посылки команд, обработки ответов.
  strindex.c - содержит процедуру поиска подстроки в строке (взята из книги "Язык программирования C, Керниган, Ритчи"
  *.h - заголовочные файлы


# TCP server/client
## Сборка проекта

```bash
cmake -B build ./
cmake --build build
```

## Запуск сервера

```bash
./build/server
```

Чтобы изменить порт сервера, измените значение port в server.h
## Запуск клиента

```bash
./build/client 127.0.0.1 888
```

## Возможности клиента

connections - посмотреть количество подключений. \
любой текст - подсчитать кол-во букв в слове. \
exit - отключиться.
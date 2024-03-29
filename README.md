# NetPlay

NetPlay - универсальный игровой движок, позволяющий легко создавать многопользовательские сетевые игры.


##  Сборка

### Необходимые компоненты

- CMake >=3.16
- SDL 2 (только для клиента)
- SDL 2 Image (только для клиента)

### Сервер

Перейдите в директорию `Server` и выполните команды

```sh
cmake -B build
cd build
```

Далее выполните команду для сборки, соответствующую используемой в вашей системе программы
для сборки, например `make` или `ninja`.

Для запуска тестовой игры потребуется создать в директории `build` файл `level_1.map` со следующим текстом

```
[DOOR]
520 0
40 200

[KEY]
565 330
65 35

[END]
```

### Клиент

Перейдите в директорию `Client` и выполните команду

```sh
cmake -B build
```

Если команда завершилась с ошибкой, необходимо редактировать файл `CMakeLists.txt` так, чтобы
библиотеки `SDL 2` и `SDL 2 Image` были найдены. Они устанавливаются по-разному в
разных системах. 

После успешной генерации содержимого директории `build`, в неё необходимо перейти и создать
символическую ссылку на директорию `sprites`

```sh
cd build
ln -s ../sprites
```

После этого выполните команду для сборки, соответствующую используемой в вашей системе программы
для сборки, например `make` или `ninja`.


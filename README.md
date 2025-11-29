# Course-Work
"РАЗРАБОТКА АВТОМАТИЗИРОВАННОЙ СИСТЕМЫ ДЛЯ ОРГАНИЗАЦИИ РАБОТЫ ПО КУПЛЕ - ПРОДАЖЕ МЕДИАФАЙЛОВ"

## О проекте / About the Project

Консольное приложение на C++ для автоматизированной системы купли-продажи медиафайлов.
C++ console application for an automated media file buying/selling system.

## Функции / Features

- **Авторизация** - регистрация и вход пользователей с разделением ролей (покупатель, продавец, администратор)
- **Шифрование паролей** - хранение паролей в зашифрованном виде (Caesar cipher)
- **CRUD операции** - просмотр, добавление, редактирование и удаление медиафайлов
- **Поиск и фильтрация** - поиск по названию/автору, фильтрация по типу и цене
- **Сортировка** - по цене, названию, размеру
- **Отчёты** - генерация отчётов о покупках и каталоге
- **Сохранение данных** - хранение в текстовых файлах

## Реализованные требования / Implemented Requirements

- ✅ Пользовательское меню с разделением ролей
- ✅ OOP принципы (инкапсуляция, наследование, полиморфизм)
- ✅ Абстрактные классы (MediaFile)
- ✅ Пространства имён (MediaShopNS)
- ✅ Шаблоны классов и методов (SortHelper, filterFiles)
- ✅ Статические методы и поля
- ✅ Перегрузка операторов (<<, ==, <)
- ✅ Дружественные функции
- ✅ Виртуальные функции
- ✅ Умные указатели (shared_ptr)
- ✅ Обработка ошибок (try-catch)
- ✅ Потоки C++ с контролем ошибок

## Сборка и запуск / Build & Run

### Требования / Requirements
- C++17 compiler (g++, clang++, or MSVC)
- GNU Make (optional)

### Сборка / Build

```bash
# Using Makefile
make

# Or manually
g++ -std=c++17 -o mediashop main.cpp Auth.cpp User.cpp MediaFile.cpp MediaShop.cpp Menu.cpp
```

### Запуск / Run

```bash
./mediashop
```

### Очистка / Clean

```bash
make clean
```

## Структура проекта / Project Structure

```
├── main.cpp        - Точка входа / Entry point
├── Auth.h/cpp      - Авторизация / Authentication
├── User.h/cpp      - Классы пользователей / User classes
├── MediaFile.h/cpp - Классы медиафайлов / Media file classes
├── MediaShop.h/cpp - Магазин / Shop class
├── Menu.cpp        - Меню / Menu functions
├── Makefile        - Сборка / Build configuration
└── README.md       - Документация / Documentation
```

## Типы медиафайлов / Media File Types

- **AudioFile** - аудио файлы (mp3, wav, etc.)
- **VideoFile** - видео файлы (mp4, avi, etc.)
- **ImageFile** - изображения (jpg, png, etc.)

## Авторы / Authors

БГУИР Course Work Project

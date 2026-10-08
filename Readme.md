# Лабораторная работа №2

Курс: Операционные системы и архитектура компьютера
Тема: Управление процессами и взаимодействие через каналы (pipe)
Вариант: Группа 1, Вариант 2

## Задание

Пользователь вводит числа вида «число число число». Числа передаются от родительского процесса в дочерний. Дочерний процесс считает их сумму и выводит её в файл. Числа имеют тип float, количество чисел произвольно.

## Структура проекта

    lab2/
    ├── inc/
    │   ├── common.h
    │   └── platform.h
    ├── src/
    │   ├── parent.c
    │   ├── child.c
    │   ├── platform_win.c
    │   ├── platform_linux.c
    │   └── common.c
    ├── Makefile
    └── README.md

## Сборка и запуск

Windows (MinGW):

    mingw32-make
    .\parent.exe

Linux:

    make
    ./parent

## Пример работы

    Enter filename: result.txt
    Enter numbers (space-separated): 1 5 4 2 4 2.3
    Parent received sum: 18.30
    Child finished.

Файл result.txt:

    Sum: 18.30

## Системные вызовы

Windows              Linux            Назначение
CreatePipe           pipe             Создание канала
CreateProcess        fork + exec      Запуск дочернего процесса
SetHandleInformation dup2             Настройка наследования / перенаправление потоков
WriteFile / ReadFile write / read     Обмен данными через канал
WaitForSingleObject  waitpid          Ожидание завершения потомка
CloseHandle          close            Освобождение ресурсов

## Демонстрация работы

Windows: Process Monitor (ProcMon). Ключевые события — CreateFile (каналы), Process Create (child.exe), WriteFile / ReadFile (обмен данными). Полный лог - в приложении Б отчета.

## Примечания

- Код кроссплатформенный: логика в parent.c и child.c не зависит от ОС.
- Все системные вызовы проверяются на код возврата.
- После завершения работы все ресурсы ОС освобождаются.
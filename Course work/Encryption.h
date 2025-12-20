#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#define BLOCK_SIZE 64
#define BLOCK_PART 56
#define HASH_SIZE 8

#include <string>
#include <cstdint>
#include <iostream>
#include <cstring>

//Определение переменной для хранения длины блока
extern uint32_t block_length;

//Определение переменной для хранения размера получаемых данных
extern uint64_t bit_length;

//Определение округлённых констант
extern const uint32_t keys[BLOCK_SIZE];

//Определение значений хеша
extern uint32_t hash[HASH_SIZE];

//Функция для перевода строки в массив восьмибитовых чисел
uint8_t* stringTransform(std::string&);

//Функция для разделение на блоки (по 512 бит) входного сообщения
uint8_t* blockDivision(const uint8_t*, size_t);

//Функция для дополнения до блока (до 512 бит)
void blockFulfil(const uint8_t*);

//Функция для изменения значений хеша
void blockTransform(uint8_t*);

//Функции для работы с битами
uint32_t sigma0(uint32_t);
uint32_t sigma1(uint32_t);
uint32_t rightRotate(uint32_t, uint32_t);

//Функции для работы с битами значений хеша
uint32_t majority(uint32_t, uint32_t, uint32_t);
uint32_t choose(uint32_t, uint32_t, uint32_t);

//Функция для получения выходного хеш-значения
uint8_t* digest(uint8_t*);

//Функция для получения порядка big-endian
void reverse(uint8_t*);

//Переводим значение хеша в строку
std::string toString(const uint8_t*);

//Функция для глобальных переменных
void nulify();

//Получение хеша строки
void toHash(std::string&);
#endif

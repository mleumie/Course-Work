#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>

#include "Encryption.h"

//Инициализация переменной для хранения длины блока
uint32_t block_length = 0;

//Инициализация переменной для хранения размера получаемых данных
uint64_t bit_length = 0;

//Инициализация округлённых констант

/*Каждое значение - это первые 32 бита дробных частей
кубических корней первых 64 простых чисел (от 2 до 311)*/
const uint32_t keys[BLOCK_SIZE]{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

//Инициализация значений хеша

/*Каждое значение - первые 32 бита дробных частей
квадратных корней первых 8 простых чисел (от 2 до 19)*/
uint32_t hash[HASH_SIZE]{
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

/*Функция для перевода строки в массив восьмибитовых чисел
reinterpret_cast <type> (variable) используется для представления
переменной в другом типе данных*/
uint8_t* stringTransform(std::string& input_string) {
	uint8_t* temp = new uint8_t[BLOCK_SIZE];

	temp = blockDivision(reinterpret_cast <const uint8_t*> (input_string.c_str()), input_string.size());
	return temp;
}

/*Функция для разделение на блоки (по 512 бит) входной строки
Она производит рассчёт количества битов получаемой строки,
осуществляет вызов функции изменения значения хеша при заполнении
очередного блока, сохраняет последний блок строки*/
uint8_t* blockDivision(const uint8_t* bin_string, size_t length) {
	uint8_t* block = new uint8_t[BLOCK_SIZE];

	for (size_t i = 0; i < length; i++) {
		block[block_length++] = bin_string[i];

		if (block_length == BLOCK_SIZE) {
			blockTransform(block);

			bit_length += 512;
			block_length = 0;
		}
	}

	return block;
}

/*Функция для дополнения до блока
Она заполняет последний блок (если он содержит менее 56 байт) одним
единичным битом и нулевыми битами вплоть до 448 бита, а последние 8
байт заполняются числом битов в получаемой строке.
Если в блоке более 56 байт, то изменяются значения хеша, первые 56
байт блока обнуляются, а последние 8 байт заполняются числом битов
в получаемой строке.*/
void blockFulfil(uint8_t* block) {

	uint64_t i = block_length;
	uint16_t end = block_length < BLOCK_PART ? BLOCK_PART : BLOCK_SIZE;

	block[i++] = 0x80; // Добавляем единичный бит + 7 нулевых бит
	while (i < end) {
		block[i++] = 0x00; // Заполнение нулевыми байтами
	}

	/*Если в блоке заполнено более 56 байт, то изменяем значение
	хеша и заполняем первые 56 байт блока нулевыми байтами*/
	if (block_length >= BLOCK_PART) {
		blockTransform(block);
		memset(block, 0, BLOCK_PART);
	}

	//Дополняем до полного блока
	bit_length += block_length * 8;
	block[63] = bit_length;
	block[62] = bit_length >> 8;
	block[61] = bit_length >> 16;
	block[60] = bit_length >> 24;
	block[59] = bit_length >> 32;
	block[58] = bit_length >> 40;
	block[57] = bit_length >> 48;
	block[56] = bit_length >> 56;
	blockTransform(block);
}

//Функция для изменения значений хеша
/*Вызывается при заполнении очередного блока*/
void blockTransform(uint8_t* block) {
	uint32_t maj, xorA, ch, xorE, sum, newA, newE, new_block[BLOCK_SIZE];
	uint32_t new_hash[HASH_SIZE];

	/*Разбиваем массив 8-битных данных на части по 32 бита
	и переписываем в новый массив*/
	for (uint8_t i = 0, j = 0; i < 16; i++, j += 4) {
		new_block[i] = (block[j] << 24) | (block[j + 1] << 16) | (block[j + 2] << 8) | (block[j + 3]);
	}

	for (uint8_t k = 16; k < 64; k++) { //Для оставшихся 48 частей по 32 бита
		new_block[k] = sigma1(new_block[k - 2]) + new_block[k - 7] + sigma0(new_block[k - 15]) + new_block[k - 16];
	}

	for (uint8_t i = 0; i < 8; i++) {
		new_hash[i] = hash[i];
	}

	for (uint8_t i = 0; i < 64; i++) {
		maj = majority(new_hash[0], new_hash[1], new_hash[2]);
		xorA = rightRotate(new_hash[0], 2) ^ rightRotate(new_hash[0], 13) ^ rightRotate(new_hash[0], 22);

		ch = choose(new_hash[4], new_hash[5], new_hash[6]);

		xorE = rightRotate(new_hash[4], 6) ^ rightRotate(new_hash[4], 11) ^ rightRotate(new_hash[4], 25);

		sum = new_block[i] + keys[i] + new_hash[7] + ch + xorE;
		newA = xorA + maj + sum;
		newE = new_hash[3] + sum;

		new_hash[7] = new_hash[6];
		new_hash[6] = new_hash[5];
		new_hash[5] = new_hash[4];
		new_hash[4] = newE;
		new_hash[3] = new_hash[2];
		new_hash[2] = new_hash[1];
		new_hash[1] = new_hash[0];
		new_hash[0] = newA;
	}

	for (uint8_t i = 0; i < 8; i++) {
		hash[i] += new_hash[i];
	}
}

//Функция сигма нулевое
uint32_t sigma0(uint32_t x) {
	return rightRotate(x, 7) ^ rightRotate(x, 18) ^ (x >> 3);
}

//Функция сигма первое
uint32_t sigma1(uint32_t x) {
	return rightRotate(x, 17) ^ rightRotate(x, 19) ^ (x >> 10);
}

//Функция для циклического сдвига вправо
uint32_t rightRotate(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}

/*Специальная функция для работы с битами значений хеша
от нулевого до второго*/
uint32_t majority(uint32_t a, uint32_t b, uint32_t c) {
	return (a & (b | c)) | (b & c);
}

/*Специальная функция для работы с битами значений хеша
от четвёртого до шестого*/
uint32_t choose(uint32_t e, uint32_t f, uint32_t g) {
	return (e & f) ^ (~e & g);
}

//Функция для получения выходного хеш-значения
uint8_t* digest(uint8_t* block) {
	uint8_t* hash = new uint8_t[32];

	blockFulfil(block);
	reverse(hash);

	nulify();
	return hash;
}

/* SHA использует порядок байтов big-endian, поэтому
"разворачиваем" байты*/
void reverse(uint8_t* hashes) {
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			hashes[i + (j * 4)] = (hash[j] >> (24 - i * 8)) & 0x000000ff;
		}
	}
}

/*Переводим значение хеша в строку с помощью
строкового потока*/
std::string toString(const uint8_t* digest) {
	std::stringstream stream;
	stream << std::setfill('0') << std::hex;

	for (uint8_t i = 0; i < 32; i++) {
		stream << std::setw(2) << (unsigned int)digest[i];
	}

	return stream.str();
}

/*Функция для возвращения начальных значений
глобальных переменных*/
void nulify() {
	bit_length = 0;
	block_length = 0;
	hash[0] = 0x6a09e667;
	hash[1] = 0xbb67ae85;
	hash[2] = 0x3c6ef372;
	hash[3] = 0xa54ff53a;
	hash[4] = 0x510e527f;
	hash[5] = 0x9b05688c;
	hash[6] = 0x1f83d9ab;
	hash[7] = 0x5be0cd19;
}

void toHash(std::string& password) {
	uint8_t* digest_string;

	digest_string = stringTransform(password);
	uint8_t* dig = digest(digest_string);

	password = toString(dig);

	delete[] dig;
	delete[] digest_string;
}
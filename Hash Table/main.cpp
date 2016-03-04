#include <iostream>
#include <string>
#include "hash_table.h"

class simple_hash
{
public:
	unsigned long long operator()(long long elem, long long mod) {return elem % mod;}
};

int main()
{
	typedef hash_table<unsigned int, std::string, simple_hash> htable;
	htable example;

	// вставляем несколько пар в хэш-таблицу.
	// хэш-таблица после вставок содержит уже 6 пар с одинаковым значением хэш-функции (75)
	// но т.к. этому значению соответствует один и тот же ключ (876), таблица не перестраивается:
	// неважно, как мы перестроим таблицу, у неё всё равно останется те же 6 пар с одинаковым значением хэш-функции
    example.insert(1957, "FORTRAN");
    example.insert(876, "ALGOL");
    example.insert(0, "LISP");
    example.insert(88, "COBOL");
    example.insert(876, "APL");
    example.insert(876, "SIMULA");
    example.insert(876, "BASIC");
    example.insert(876, "PL/I");
    example.insert(876, "ISWIM");
	std::cout << "First variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;

	// добавляется ещё 4 пары со значением хэш-функции 75, но уже с различными ключами. итого получится 5 различных пар с одинаковым значением хэш-функции
	// на 6-ой паре таблица перестроится с новым верхнем значением для хэш-функции
    example.insert(75, "Prolog");
    example.insert(164, "C");
    example.insert(253, "Pascal");
    example.insert(342, "Scheme");
	std::cout << "Second variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;

	// добавляем 6-ую пару
    example.insert(431, "OPS5");
	std::cout << "Third variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;

	// добавление двух пар с ключом, хэш-функция которого равна 45
    example.insert(45, "CSP");
    example.insert(284, "FP");
	std::cout << "Fourth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;


	// поиск пары с ключом, хэш-функция которого встречается в таблице 1 раз
	for(htable::searcher s(example, 75); !s.end(); ++s)
		std::cout << "Found: " << 75 << ' ' << *s << std::endl;
	// поиск пары с ключом, хэш-функция которого встречается в таблице несколько раз, но сам ключ встречается единожды
	for(htable::searcher s(example, 45); !s.end(); ++s)
		std::cout << "Found: " << 45 << ' ' << *s << std::endl;
	// поиск пар с ключом, встречающимся в таблице несколько раз
	for(htable::searcher s(example, 876); !s.end(); ++s)
		std::cout << "Found: " << 876 << ' ' << *s << std::endl;
	// поиск пары с ключом, хэш-функция которого встречается в таблице (45), но сам ключ в таблице не встречается (не выводится ничего)
	for(htable::searcher s(example, 523); !s.end(); ++s)
		std::cout << "Found: " << 523 << ' ' << *s << std::endl;
	// поиск пары с ключом, который не встречается в таблице (не выводится ничего)
	for(htable::searcher s(example, 89); !s.end(); ++s)
		std::cout << "Found: " << 89 << ' ' << *s << std::endl;
	std::cout << std::endl;


	// удаление пары с ключом, хэш-функция которого встречается в таблице 1 раз
	example.remove(75);
	std::cout << "Fifth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// удаление пары с ключом, хэш-функция которого встречается в таблице 1 раз c помощью remove_all
	example.remove_all(431);
	std::cout << "Sixth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// удаление пары с ключом, хэш-функция которого встречается в таблице несколько раз, но сам ключ встречается единожды
	example.remove(284);
	std::cout << "Seventh variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// удаление первой пары с ключом, встречающимся в таблице несколько раз
	example.remove(876);
	std::cout << "Eighth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// удаление всех пар с ключом, встречающимся в таблице несколько раз
	example.remove_all(876);
	std::cout << "Ninth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// поиск пары с ключом, который не встречается в таблице (не удаляется ничего)
	example.remove(89);
	std::cout << "Tenth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;


	std::system("pause");
	return 0;
}
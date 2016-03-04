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

	// ��������� ��������� ��� � ���-�������.
	// ���-������� ����� ������� �������� ��� 6 ��� � ���������� ��������� ���-������� (75)
	// �� �.�. ����� �������� ������������� ���� � ��� �� ���� (876), ������� �� ���������������:
	// �������, ��� �� ���������� �������, � �� �� ����� ��������� �� �� 6 ��� � ���������� ��������� ���-�������
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

	// ����������� ��� 4 ���� �� ��������� ���-������� 75, �� ��� � ���������� �������. ����� ��������� 5 ��������� ��� � ���������� ��������� ���-�������
	// �� 6-�� ���� ������� ������������ � ����� ������� ��������� ��� ���-�������
    example.insert(75, "Prolog");
    example.insert(164, "C");
    example.insert(253, "Pascal");
    example.insert(342, "Scheme");
	std::cout << "Second variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;

	// ��������� 6-�� ����
    example.insert(431, "OPS5");
	std::cout << "Third variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;

	// ���������� ���� ��� � ������, ���-������� �������� ����� 45
    example.insert(45, "CSP");
    example.insert(284, "FP");
	std::cout << "Fourth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;


	// ����� ���� � ������, ���-������� �������� ����������� � ������� 1 ���
	for(htable::searcher s(example, 75); !s.end(); ++s)
		std::cout << "Found: " << 75 << ' ' << *s << std::endl;
	// ����� ���� � ������, ���-������� �������� ����������� � ������� ��������� ���, �� ��� ���� ����������� ��������
	for(htable::searcher s(example, 45); !s.end(); ++s)
		std::cout << "Found: " << 45 << ' ' << *s << std::endl;
	// ����� ��� � ������, ������������� � ������� ��������� ���
	for(htable::searcher s(example, 876); !s.end(); ++s)
		std::cout << "Found: " << 876 << ' ' << *s << std::endl;
	// ����� ���� � ������, ���-������� �������� ����������� � ������� (45), �� ��� ���� � ������� �� ����������� (�� ��������� ������)
	for(htable::searcher s(example, 523); !s.end(); ++s)
		std::cout << "Found: " << 523 << ' ' << *s << std::endl;
	// ����� ���� � ������, ������� �� ����������� � ������� (�� ��������� ������)
	for(htable::searcher s(example, 89); !s.end(); ++s)
		std::cout << "Found: " << 89 << ' ' << *s << std::endl;
	std::cout << std::endl;


	// �������� ���� � ������, ���-������� �������� ����������� � ������� 1 ���
	example.remove(75);
	std::cout << "Fifth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// �������� ���� � ������, ���-������� �������� ����������� � ������� 1 ��� c ������� remove_all
	example.remove_all(431);
	std::cout << "Sixth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// �������� ���� � ������, ���-������� �������� ����������� � ������� ��������� ���, �� ��� ���� ����������� ��������
	example.remove(284);
	std::cout << "Seventh variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// �������� ������ ���� � ������, ������������� � ������� ��������� ���
	example.remove(876);
	std::cout << "Eighth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// �������� ���� ��� � ������, ������������� � ������� ��������� ���
	example.remove_all(876);
	std::cout << "Ninth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;
	// ����� ���� � ������, ������� �� ����������� � ������� (�� ��������� ������)
	example.remove(89);
	std::cout << "Tenth variant of table:" << std::endl << std::endl;
	example.print_table();
	std::cout << std::endl;


	std::system("pause");
	return 0;
}
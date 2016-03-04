#ifndef __HUFFMAN_CODE__
#define __HUFFMAN_CODE__
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#define STATISTICS 1 // программа периодически сообщает, что она делает

// алгоритм: http://habrahabr.ru/post/144200/
// оттуда же вз€т алгоритм записи дерева ’аффмана в файл (п€тый комментарий сверху)

class base // базовый узел дерева ’аффмана, который содержит только переменную sum, €вл€ющуюс€ суммой всех вхождений в исходный файл собственного байта и байта всех потомков
{
public:
	unsigned long long sum;
	base(): sum(0) {}
	base(unsigned long long s): sum(s){}
	virtual ~base() = 0; // просто дл€ того, чтобы сделать этот класс абстрактным
};

inline base::~base() {}

class leaf: public base // лист, содержащий байт, встречающийс€ в файле
{
public:
	unsigned char byte;
	leaf(): byte(0) {}
	leaf(unsigned char c, unsigned long long s): base(s), byte(c) {}
};

class node: public base // узел, у которого есть два потомка, но нет байта
{
public:
	base* left;
	base* right;
	node(): left(NULL), right(NULL) {}
	node(base* l, base* r): base(l -> sum + r -> sum), left(l), right(r) {}
	~node() {if(left) delete left; if(right) delete right;}
};

class bigger_node // третий параметр дл€ очереди с приоритетом
{
public:
	bool operator()(base* x, base* y) {return x -> sum > y ->sum;}
};

class vector_of_bits // вектор из unsigned char, содержащий специальные функции дл€ работы с побитовым представлением
{
	std::vector<unsigned char> vect;
	int count; // количество значимых бит в последнем unsigned char, если count == 0, то это показатель, что вектор битов не инициализирован
public:
	vector_of_bits(): count(0) { vect.push_back(0); }
	~vector_of_bits() {vect.clear();}
	void push_back(bool b) // добавить бит в конец
	{
		if(count == 8)
		{
			if(b)
				vect.push_back(128);
			else
				vect.push_back(0);
			count = 1;
		}
		else
		{
			if(b)
				vect[vect.size() - 1] |= (1 << (7 - count));
			++count;
		}
	}
	void push_back(unsigned char c) // добавить байт в конец
	{
		if(count == 0)
		{
			vect.clear();
			vect.push_back(c);
			count = 8;
		}
		else if(count == 8)
			vect.push_back(c);
		else
		{
			vect[vect.size() - 1] |= (c >> count);
			vect.push_back( c << (8 - count) );
		}
	}
	void push_back(const vector_of_bits& v) // добавить другой vector_of_bits в конец
	{
		for(unsigned int i = 0; i < v.vect.size() - 1; ++i)
			push_back(v.vect[i]);
		for(int i = 1; i <= v.count; ++i)
			push_back( bool(v.vect[v.vect.size() - 1] & (1 << (8 - i)) ) );
	}
	bool is_ready_to_print()const {return count == 8;}
	bool empty()const {return count == 0;}
	void clear() {vect.clear(); count = 0; vect.push_back(0);}
	int bits()const {return count + (vect.size() - 1) * 8;}
	char* to_string()const
	{
		char* result = new char[bits() + 1];
		result[bits()] = 0;
		unsigned int i;
		for(i = 0; i < (vect.size() - 1); ++i)
			for(int j = 0; j < 8; ++j)
				result[i * 8 + j] = ( vect[i] & (1 << (7 - j) ) )? '1': '0';
		for(int j = 0; j < count; ++j)
			result[i * 8 + j] = ( vect[i] & (1 << (7 - j) ) )? '1': '0';
		return result;
	}
	void shift_on_bit() // сдвинуть влево на один бит
	{
		vect[0] <<= 1;
		for(unsigned int i = 1; i < vect.size(); ++i)
		{
			if(vect[i] & 128)
				vect[i - 1] |= 1;
			vect[i] <<= 1;
		}
		if(count == 1)
		{
			if(vect.size() == 1)
				count = 0;
			else
			{
				vect.pop_back();
				count = 8;
			}
		}
		else
			--count;
	}
	void shift_on_byte() // сдвинуть влево на один байт
	{
		if(count != 0)
		{
			if(vect.size() > 1)
				vect.erase(vect.begin());
			else
			{
				vect[0] = 0;
				count = 0;
			}
		}
	}
	vector_of_bits& operator--() // стереть последний бит
	{
		if(count == 0)
			throw "Vector of bits error";
		if(count > 1)
		{
			vect[vect.size() - 1] = (vect[vect.size() - 1] >> (9 - count)) << (9 - count);
			--count;
		}
		else
		{
			vect.pop_back();
			count = 8;
		}
		return *this;
	}
	bool extract_first_bit() {bool result = vect[0] & 128; shift_on_bit(); return result;} // извлечь первый бит и сдвинуть вектор влево на этот бит
	unsigned char extract_first_byte() {unsigned char result = vect[0]; shift_on_byte(); return result;} // извлечь первый байт и сдвинуть вектор влево на этот байт
	friend std::ostream& operator<<(std::ostream& out, const vector_of_bits& v);
};

unsigned long long* frequency(std::ifstream& infile); // как часто входит каждый символ в файл

void building_table(base* b, vector_of_bits& c, vector_of_bits* table); // построение таблицы кодов дл€ каждого символа по дереву ’аффмана

void print_tree(std::ofstream& outfile, base* tree, vector_of_bits& buf); // запись дерева в файл

void Huffman_encode(const char* in, const char* out);

void Huffman_decode(const char* in, const char* out);

#endif
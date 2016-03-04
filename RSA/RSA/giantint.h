#ifndef __GIANTINT_H__
#define __GIANTINT_H__
#include <iostream>
#include <math.h>
#include "List.h"
#include <string>
#include <climits>
#include <deque>

void StringDivision(const char* dividend, unsigned long long divider, char* result, unsigned int& remainder); // деление числовой строки на число с возвращением частного и остатка, функция используется в конструкторе gint от строки
unsigned long long StringToInt(const char* str, unsigned int first, unsigned int last); // преобразование части строки от first до last строки в unsigned long long
unsigned long long StringToInt(const char* str); // преобразование всей строки в unsigned long long
char* IntToString(unsigned long long X); // преобразование целого числа в строку
std::string StringMultiplication(std::string& first, std::string& second); // умножение числовой строки на числовую строку, класс string был выбран из-за необходимости периодически добавлять символы в строку, функция используется для преобразования в десятичный вид при выводе на экран
std::string StringToDigitMultiplication(std::string& s, int x); //  умножение числовой строки на число от 0 до 9
std::string StringAddition(std::string& first, std::string& second); // сложение числовых строк, функция используется для преобразования в десятичный вид при выводе на экран
unsigned int uint_rand(); // случайное unsigned int число, используется для метода big_rand()

const unsigned long long base = (unsigned long long)UINT_MAX + 1; // константы, равные числу всех возможных значений, представимых в unsigned int
const char* const sbase = IntToString(base);
	

class gint // число хранится в виде списка из unsigned int в обратном порядке, каждый элемент списка - цифра в системе счисления base
{
	List<unsigned int> number; // список в качестве контейнера был выбран из-за необходимости периодически добавлять новые элементы
	int sign; // 1 - если плюс, -1 - если минус, ноль считается положительным числом
public:
	gint():sign(1) {number.push_back(0);}
	gint(int X): sign( (X >= 0)? 1 : -1) { number.push_back(abs(X));}
	gint(unsigned long X): sign(1) { number.push_back(X);}
	gint(long long X): sign( (X >= 0)? 1 : -1 )
	{
		if(abs(X) < base)
			number.push_back(abs(X));
		else
		{
			number.push_back(abs(X) % base);
			number.push_back(abs(X) / base);
		}
	}
	gint(unsigned long long X): sign(1)
	{
		if(X < base)
			number.push_back(X);
		else
		{
			number.push_back(X % base);
			number.push_back(X / base);
		}
	}
	gint(const char* X);
	gint(const gint& X) {sign = X.sign; number = X.number; }
	gint(const char* buf, unsigned long long len); // перевод buf в gint, где buf - это число с системой счисления 256 (один char - одна цифра) в прямом порядке (первый char - старший разряд), а len - длина buf
	~gint() {number.clear();}

	gint& operator=(int X)
	{
		number.clear();
		sign = (X > 0)? 1 : -1;
		number.push_back(abs(X));
		return *this;
	}
	gint& operator=(unsigned long X)
	{
		number.clear();
		sign = 1;
		number.push_back(X);
		return *this;
	}
	gint& operator=(long long X)
	{
		number.clear();
		sign = (X > 0)? 1 : -1;
		if(abs(X) < base)
			number.push_back(abs(X));
		else
		{
			number.push_back(abs(X) % base);
			number.push_back(abs(X) / base);
		}
		return *this;
	}
	gint& operator=(unsigned long long X)
	{
		number.clear();
		sign = 1;
		if(X < base)
			number.push_back(X);
		else
		{
			number.push_back(X % base);
			number.push_back(X / base);
		}
		return *this;
	}
	gint& operator=(const char* X)
	{
		(*this) = gint(X);
		return *this;
	}
	gint& operator=(const gint& X)
	{
		if(this != &X)
		{
			sign = X.sign;
			number = X.number;
		}
		return *this;
	}
	bool operator<(int X)const
	{
		if(number.size() != 1)
			return sign == -1;
		if(sign != ((X >= 0)? 1 : -1))
			return sign == -1;
		List<unsigned int>::iterator pos = number.begin();
		if(sign == 1)
			return (*pos) < X;
		else
			return (*pos) > abs(X);
	}
	bool operator<(unsigned long X)const
	{
		if(number.size() != 1)
			return sign == -1;
		if(sign == -1)
			return true;
		List<unsigned int>::iterator pos = number.begin();
		return (*pos) < X;
	}
	bool operator<(long long X)const
	{
		if(number.size() > 2)
			return sign == -1;
		if(sign != ((X >= 0)? 1 : -1))
			return sign == -1;
		List<unsigned int>::iterator pos = number.begin();
		unsigned long long tmp = *pos;
		++pos;
		if(pos != number.end())
			tmp += (unsigned long long)(*pos) * base;
		if(sign == 1)
			return tmp < X;
		else
			return tmp > abs(X);
	}
	bool operator<(unsigned long long X)const
	{
		if(number.size() > 2)
			return sign == -1;
		if(sign == -1)
			return true;
		List<unsigned int>::iterator pos = number.begin();
		unsigned long long tmp = *pos;
		++pos;
		if(pos != number.end())
			tmp += (unsigned long long)(*pos) * base;
		return tmp < X;
	}
	bool operator<(const gint& X)const
	{
		if(sign != X.sign)
			return X.sign == 1;
		if(number.size() != X.number.size())
			if(number.size() < X.number.size())
				return sign == 1;
			else
				return sign == -1;
		List<unsigned int>::reverse_iterator this_pos = number.rbegin();
		List<unsigned int>::reverse_iterator X_pos = X.number.rbegin();
		for(; this_pos != number.rend(); ++this_pos, ++X_pos)
		{
			if( (*this_pos) != (*X_pos) )
			{
				if(sign == 1)
					return ( (*this_pos) < (*X_pos) );
				else
					return ( (*this_pos) > (*X_pos) );
			}
		}
		return false;
	}
	bool operator<(const char* X)const
	{
		gint Y(X);
		return (*this < Y);
	}
	bool operator<=(int X)const
	{
		if(number.size() != 1)
			return sign == -1;
		if(sign != ((X >= 0)? 1 : -1))
			return sign == -1;
		List<unsigned int>::iterator pos = number.begin();
		if(sign == 1)
			return (*pos) <= X;
		else
			return (*pos) >= abs(X);
	}
	bool operator<=(unsigned long X)const
	{
		if(number.size() != 1)
			return sign == -1;
		if(sign == -1)
			return true;
		List<unsigned int>::iterator pos = number.begin();
		return (*pos) <= X;
	}
	bool operator<=(long long X)const
	{
		if(number.size() > 2)
			return sign == -1;
		if(sign != ((X >= 0)? 1 : -1))
			return sign == -1;
		List<unsigned int>::iterator pos = number.begin();
		unsigned long long tmp = *pos;
		++pos;
		if(pos != number.end())
			tmp += (unsigned long long)(*pos) * base;
		if(sign == 1)
			return tmp <= X;
		else
			return tmp >= abs(X);
	}
	bool operator<=(unsigned long long X)const
	{
		if(number.size() > 2)
			return sign == -1;
		if(sign == -1)
			return true;
		List<unsigned int>::iterator pos = number.begin();
		unsigned long long tmp = *pos;
		++pos;
		if(pos != number.end())
			tmp += (unsigned long long)(*pos) * base;
		return tmp <= X;
	}
	bool operator<=(const gint& X)const
	{
		if(sign != X.sign)
			return X.sign == 1;
		if(number.size() != X.number.size())
			if(number.size() < X.number.size())
				return sign == 1;
			else
				return sign == -1;
		List<unsigned int>::reverse_iterator this_pos = number.rbegin();
		List<unsigned int>::reverse_iterator X_pos = X.number.rbegin();
		for(; this_pos != number.rend(); ++this_pos, ++X_pos)
		{
			if( (*this_pos) != (*X_pos) )
			{
				if(sign == 1)
					return ( (*this_pos) < (*X_pos) );
				else
					return ( (*this_pos) > (*X_pos) );
			}
		}
		return true;
	}
	bool operator<=(const char* X)const
	{
		gint Y(X);
		return (*this <= Y);
	}
	bool operator>(int X)const
	{
		return !(*this <= X);
	}
	bool operator>(unsigned long X)const
	{
		return !(*this <= X);
	}
	bool operator>(long long X)const
	{
		return !(*this <= X);
	}
	bool operator>(unsigned long long X)const
	{
		return !(*this <= X);
	}
	bool operator>(const gint& X)const
	{
		return !(*this <= X);
	}
	bool operator>(const char* X)const
	{
		return !(*this <= X);
	}
	bool operator>=(int X)const
	{
		return !(*this < X);
	}
	bool operator>=(unsigned long X)const
	{
		return !(*this < X);
	}
	bool operator>=(long long X)const
	{
		return !(*this < X);
	}
	bool operator>=(unsigned long long X)const
	{
		return !(*this < X);
	}
	bool operator>=(const gint& X)const
	{
		return !(*this < X);
	}
	bool operator>=(const char* X)const
	{
		return !(*this < X);
	}
	bool operator==(int X)const
	{
		if( sign != ( (X >= 0)? 1 : - 1) )
			return false;
		if(number.size() != 1)
			return false;
		List<unsigned int>::iterator pos = number.begin();
		return sign * (*pos) == X;
	}
	bool operator==(unsigned long X)const
	{
		if(sign != 1)
			return false;
		if(number.size() != 1)
			return false;
		List<unsigned int>::iterator pos = number.begin();
		return (*pos) == X;
	}
	bool operator==(long long X)const
	{
		if( sign != ( (X >= 0)? 1 : - 1) )
			return false;
		if(number.size() > 2)
			return false;
		List<unsigned int>::iterator pos = number.begin();
		if(number.size() == 1)
			return (*pos) == X;
		unsigned long long tmp = (*pos);
		++pos;
		tmp += (unsigned long long)(*pos) * base;
		return sign * tmp == X;
	}
	bool operator==(unsigned long long X)const
	{
		if(sign != 1)
			return false;
		if(number.size() > 2)
			return false;
		List<unsigned int>::iterator pos = number.begin();
		if(number.size() == 1)
			return (*pos) == X;
		unsigned long long tmp = (*pos);
		++pos;
		tmp += (unsigned long long)(*pos) * base;
		return tmp == X;
	}
	bool operator==(const gint& X)const
	{
		if(sign != X.sign)
			return false;
		if(number.size() != X.number.size())
			return false;
		List<unsigned int>::iterator this_pos = number.begin();
		List<unsigned int>::iterator X_pos = X.number.begin();
		for(; this_pos != number.end(); ++this_pos, ++ X_pos)
			if( (*this_pos) != (*X_pos) )
				return false;
		return true;
	}
	bool operator==(const char* X)const
	{
		gint Y(X);
		return (*this) == Y;
	}
	bool operator!=(int X)const
	{
		return !( (*this) == X );
	}
	bool operator!=(unsigned long X)const
	{
		return !( (*this) == X );
	}
	bool operator!=(long long X)const
	{
		return !( (*this) == X );
	}
	bool operator!=(unsigned long long X)const
	{
		return !( (*this) == X );
	}
	bool operator!=(const gint& X)const
	{
		return !( (*this) == X );
	}
	bool operator!=(const char* X)const
	{
		return !( (*this) == X );
	}

	gint operator-()const { gint X(*this); if(X != 0) X.sign = -X.sign; return X; }
	gint operator+(const gint& X)const;
	gint operator-(const gint& X)const;
	gint operator*(const gint& X)const;
	gint operator*(const unsigned long X)const;
	gint operator/(const gint& X)const;
	gint operator%(const gint& X)const;
	gint& operator++()
	{
		List<unsigned int>::iterator pos = number.begin();
		bool transfer = ( (*pos) == UINT_MAX );
		if(transfer)
			(*pos) = 0;
		else
			++(*pos);
		++pos;
		while(transfer && (pos != number.end()))
		{
			transfer = ( (*pos) == UINT_MAX );
			if(transfer)
				(*pos) = 0;
			else
				++(*pos);
			++pos;
		}
		if(transfer)
			number.push_back(1);
		return *this;
	}
	bool odd()const { List<unsigned int>::iterator pos = number.begin(); return (*pos) & 1;  } // нечетное ли число
	friend std::ostream& operator<<(std::ostream& out, const gint& X);
	friend std::istream& operator>>(std::istream& out, gint& X);
	gint ShiftingWords(unsigned int n); // сдвиг числа на n машинных слов (n * 32 бит), используется для деления
	gint ShiftingBytes(unsigned int n); // сдвиг числа на n байт (n принимает значения от 1 до 3), используется для деления
	void big_rand(unsigned int low_lim, unsigned int upp_lim); // случайное положительное число с количеством цифр (в с.с. 2^32) от low_lim до upp_lim включительно
	void big_rand(const gint& X); // случайное число от 0 до X
	unsigned long long size()const {return number.size();}
	unsigned long long bits()const; // количество бит
	gint PowAndRem(const gint& d, const gint& n)const; // ((*this)^d) % n 
	void binwrite(std::ostream& outfile, unsigned long long bytes); // запись числа в таком виде, в каком он представляется в компьютере (если бы в качестве контейнера был выбран массив, а не список), если число байтов, которые занимает число, меньше bytes, то при выводе оно дополняется слева нулевыми байтами
};


gint pow(const gint& X, const gint& Y); // вычисление степени
gint abs(const gint &X);
// операторы сравнения, где слева от оператора стоит число или строка
bool operator<(int X, gint& Y);
bool operator<(unsigned long X, gint& Y);
bool operator<(long long X, gint& Y);
bool operator<(unsigned long long X, gint& Y);
bool operator<(const char* X, gint& Y);
bool operator<=(int X, gint& Y);
bool operator<=(unsigned long X, gint& Y);
bool operator<=(long long X, gint& Y);
bool operator<=(unsigned long long X, gint& Y);
bool operator<=(const char* X, gint& Y);
bool operator>(int X, gint& Y);
bool operator>(unsigned long X, gint& Y);
bool operator>(long long X, gint& Y);
bool operator>(unsigned long long X, gint& Y);
bool operator>(const char* X, gint& Y);
bool operator>=(int X, gint& Y);
bool operator>=(unsigned long X, gint& Y);
bool operator>=(long long X, gint& Y);
bool operator>=(unsigned long long X, gint& Y);
bool operator>=(const char* X, gint& Y);
bool operator==(int X, gint& Y);
bool operator==(unsigned long X, gint& Y);
bool operator==(long long X, gint& Y);
bool operator==(unsigned long long X, gint& Y);
bool operator==(const char* X, gint& Y);
bool operator!=(int X, gint& Y);
bool operator!=(unsigned long X, gint& Y);
bool operator!=(long long X, gint& Y);
bool operator!=(unsigned long long X, gint& Y);
bool operator!=(const char* X, gint& Y);

#endif
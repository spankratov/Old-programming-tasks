#include "giantint.h"
unsigned long long StringToInt(const char* str, unsigned int first, unsigned int last)
{
	unsigned long long x = 0;
	for(int i = first; i <= last; ++i)
	{
		x *= 10;
		x += (str[i] - '0');
	}
	return x;
}

unsigned long long StringToInt(const char* str)
{
	unsigned long long x = 0;
	unsigned long long last = strlen(str);
	for(int i = 0; i < last; ++i)
	{
		x *= 10;
		x += (str[i] - '0');
	}
	return x;
}

char* IntToString(unsigned long long X)
{
	int len = log10((long double)X) + 1;
	char* result = new char[len + 1];
	for(int i = len - 1; i >= 0; --i)
	{
		result[i] = (X % 10) + '0';
		X /= 10;
	}
	result[len] = 0;
	return result;
}

void StringDivision(const char* dividend, unsigned long long divider, char* result, unsigned int& remainder)
{
	// ������� �������������� �� ��������� ������� ���������
	unsigned int divider_length = log10((double)divider) + 1;
	unsigned int dividend_length = strlen(dividend);
	if(divider_length > dividend_length)
	{
		strcpy(result, "0");
		remainder = StringToInt(dividend);
		return;
	}
	unsigned long long sub_dividend = StringToInt(dividend, 0, divider_length - 1);
	unsigned int d_index = divider_length;
	if(sub_dividend < divider)
	{
		if(divider_length == dividend_length)
		{
			strcpy(result, "0");
			remainder = StringToInt(dividend);
			return;
		}
		else
		{
			sub_dividend *= 10;
			sub_dividend += (dividend[divider_length] - '0');
			++d_index;
		}
	}
	result[0] = sub_dividend / divider + '0';
	sub_dividend -= divider * (result[0] - '0');
	unsigned int r_index = 1;
	while(d_index < dividend_length)
	{
		sub_dividend *= 10;
		sub_dividend += (dividend[d_index] - '0');
		if(sub_dividend < divider)
		{
			result[r_index] = '0';
			++r_index; ++d_index;
		}
		else
		{
			result[r_index] = sub_dividend / divider + '0';
			sub_dividend -= divider * (result[r_index] - '0');
			++r_index; ++d_index;
		}
	}
	remainder = sub_dividend;
	result[r_index] = 0;
}

gint::gint(const char* X)
	{
		// �������������� �������� ���������� ������ � ����� � ���������� base
		if( (X[0] == '+') || (X[0] == '-') )
		{
			sign = (X[0] == '+')? 1 : -1;
			++X;
		}
		else
			sign = 1;
		if( (strlen(X) < 10) || ( (strlen(X) == 10) && (strcmp(X, sbase) < 0) ) ) // ���� �����, ������������ � ���� ������ X, ������, ��� base
		{
			unsigned int x = StringToInt(X);
			number.push_back(x);
			return;
		}
		unsigned int len = strlen(X);
		char* result = new char[len];
		unsigned int remainder = 0;
		StringDivision(X, base, result, remainder);
		number.push_back(remainder);
		while( (strlen(result) > 10) || ( (strlen(result) == 10) && (strcmp(result, sbase) > 0) ) ) // ���� �����, ������������ � ���� ������ X, ������, ��� base
		{
			char* tmp = new char[len];
			strcpy(tmp, result);
			StringDivision(tmp, base, result, remainder);
			number.push_back(remainder);
			delete[] tmp;
		}
		number.push_back(StringToInt(result));
		delete[] result;
	}

gint::gint(const char* buf, unsigned long long len)
{
	sign = 1;
	unsigned long long i;
	for(i = len - 1; i > 3; i -= 4)
		number.push_back((unsigned int)buf[i] + buf[i - 1] * 256 + buf[i - 2] * 256 * 256 + buf[i - 3] * 256 * 256 * 256);
	if(i == 3)
		number.push_back((unsigned int)buf[i] + buf[i - 1] * 256 + buf[i - 2] * 256 * 256 + buf[i - 3] * 256 * 256 * 256);
	else if(i == 2)
		number.push_back((unsigned int)buf[i] + buf[i - 1] * 256 + buf[i - 2] * 256 * 256);
	else if(i == 1)
		number.push_back((unsigned int)buf[i] + buf[i - 1] * 256);
	else
		number.push_back((unsigned int)buf[i]);
}

std::ostream& operator<<(std::ostream& out, const gint& X)
{
	if(X.sign == -1)
		out << '-';
	List<unsigned int>::iterator pos = X.number.begin();
	if(X.number.size() == 1)
	{
		out << (*pos);
		return out;
	}
	std::string result = std::to_string((unsigned long long)(*pos));
	++pos;
	for(unsigned long long i = 1; i < X.number.size(); ++i)
	{
		std::string power(sbase);
		// ���������� base^i
		unsigned long long j = 1;
		for(; (j * 2) <= i; j *= 2)
			power = StringMultiplication(power, power);
		for(; j < i; ++j)
			power = StringMultiplication(power, std::string(sbase));
		result = StringAddition( result, StringMultiplication(std::to_string((unsigned long long)(*pos)), power) );
		++pos;
	}
	out << result;
	return out;
}

std::istream& operator>>(std::istream& out, gint& X)
{
	char* s = new char[1000];
	out >> s;
	X = gint(s);
	return out;
}

std::string StringToDigitMultiplication(std::string& s, int x)
{
	if(x == 0)
		return std::string("0");
	std::string result;
	result.reserve(s.length() + 1); // ��������� �� ����� ����������� ����� ��� �� ���� �����
	unsigned long long i = s.length() - 1;
	int tmp = (s[i] - '0') * x;
	int transfer = tmp / 10; // ������� � ������� ������
	result.insert(0, 1, (tmp % 10) + '0');
	--i;
	for(; i != ULLONG_MAX; --i) // "���� i >= 0" (���� i == 0, � �� ��������� �������� --i, �� i ������ ����� ULLONG_MAX - ����������� ���������� � unsigned long long
	{
		tmp = (s[i] - '0') * x + transfer;
		transfer = tmp / 10;
		result.insert(0, 1, (tmp % 10) + '0');
	}
	if(transfer)
		result.insert(0, 1, transfer + '0');
	return result;
}

std::string StringMultiplication(std::string& first, std::string& second)
{
	// ��������� ���������� �� ��������� ��������� � �������
	std::string* big; // �����������, ����� ������ �������. ���� �� �� ������ �������� ������ �� ������, ��, ��������, ��� ��������� ������� �� ����� ������� �����, ������� ���������� �� ������������
	std::string* small;
	if(first.length() > second.length())
	{
		big = &first;
		small = &second;
	}
	else
	{
		big = &second;
		small = &first;
	}
	unsigned long long i = (*small).length() - 1;
	std::string result = StringToDigitMultiplication( (*big), (*small)[i] - '0');
	--i;
	unsigned long long shift = 1; // �� ������� �������� ���������� �����, ����� big ���������� �� ���� �� ���� small
	for(; i != ULLONG_MAX; --i, ++shift)
	{
		std::string tmp = StringToDigitMultiplication( (*big), (*small)[i] - '0');
		tmp.insert(tmp.length(), shift, '0');
		result = StringAddition(result, tmp);
	}
	return result;
}

std::string StringAddition(std::string& first, std::string& second)
{
	std::string result;
	unsigned long long r_index, p_index;
	std::string* p;
	if(first.length() > second.length())
	{
		result = first;
		p = &second;
		r_index = first.length() - 1;
		p_index = second.length() - 1;
	}
	else
	{
		result = second;
		p = &first;
		r_index = second.length() - 1;
		p_index = first.length() - 1;
	}
	result[r_index] += ((*p)[p_index] - '0');
	int transfer;
	if(result[r_index] > '9')
	{
		result[r_index] -= 10;
		transfer = 1;
	}
	else
		transfer = 0; 
	--r_index; -- p_index;
	for(; p_index != ULLONG_MAX; --r_index, --p_index)
	{
		result[r_index] += ((*p)[p_index] - '0' + transfer);
		if(result[r_index] > '9')
		{
			result[r_index] -= 10;
			transfer = 1;
		}
		else
			transfer = 0; 
	}
	while( transfer && (r_index != ULLONG_MAX) )
	{
		if(result[r_index] < '9')
		{
			++result[r_index];
			transfer = 0;
		}
		else
		{
			result[r_index] = '0';
			transfer = 1;
		}
		--r_index;
	}
	if(transfer)
		result.insert(0, 1, '1');
	return result;
}

unsigned int uint_rand()
{
	unsigned int x = 0;
	for(int i = 1; i <= sizeof(x) * 8; ++i)
		x |= (rand() % 2) << (i - 1);
	return x;
}

gint gint::ShiftingWords(unsigned int n)
{
	gint result = *this;
	for(unsigned int i = 1; i <= n; ++i)
		result.number.push_front(0);
	return result;
}

gint gint::ShiftingBytes(unsigned int n)
{
	gint result = *this;
	List<unsigned int>::reverse_iterator this_pos = result.number.rbegin();
	List<unsigned int>::reverse_iterator next_pos = result.number.rbegin(); ++next_pos;
	if((*this_pos) >> (4 - n) * 8)
		result.number.push_back((*this_pos) >> (4 - n) * 8);
	(*this_pos) <<= n * 8;
	while(next_pos != result.number.rend())
	{
		(*this_pos) |= (*next_pos) >> (4 - n) * 8;
		(*next_pos) <<= n * 8;
		++this_pos;
		++next_pos;
	}
	return result;
}

void gint::big_rand(unsigned int low_lim, unsigned int upp_lim)
{
	number.clear();
	sign = 1;
	unsigned int digits = low_lim + rand() % (upp_lim - low_lim + 1);
	unsigned int x = uint_rand();
	while(x == 0)
		x = uint_rand();
	number.push_back(x);
	for(unsigned int i = 2; i <= digits; ++i)
		number.push_back(uint_rand());
}

void gint::big_rand(const gint& X)
{
	number.clear();
	sign = 1;
	unsigned int amount = 1 + rand() % X.number.size();
	if(amount < X.number.size())
	{
		unsigned int x = uint_rand();
		while(x == 0)
			x = uint_rand();
		number.push_back(x);
		for(unsigned int i = 2; i <= amount; ++i)
			number.push_back(uint_rand());
	}
	else
	{
		List<unsigned int>::reverse_iterator pos = X.number.rbegin();
		unsigned int num = uint_rand() % ( (*pos) + 1 );
		while(num == 0)
			num = uint_rand() % ( (*pos) + 1 );
		number.push_back(num);
		unsigned int i = 2;
		while( (num == (*pos)) && (i <= amount) )
		{
			++pos;
			num = uint_rand() % ( (*pos) + 1 );
			number.push_back(num);
			++i;
		}
		for(; i <= amount; ++i)
			number.push_back(uint_rand());
	}
}

unsigned long long gint::bits()const
{
	List<unsigned int>::reverse_iterator pos = number.rbegin();
	unsigned int C = ((unsigned long long)UINT_MAX + 1) / 2;
	int i = sizeof(unsigned int) * 8;
	while(!(C & (*pos)))
	{
		--i;
		C >>= 1;
		if(i == 0)
			break;
	}
	if(i == 0)
		return 1; // ������, ����� ����� ����� ����
	return i + (number.size() - 1) * sizeof(unsigned int) * 8;
}

gint gint::PowAndRem(const gint& d, const gint& n)const
{
	if(n == 0)
		throw "Division by zero";
	if(n == 1)
		return 0;
	if(d == 0)
		return 1;
	if(d == 1)
		return (*this) % n;
	unsigned long long r = d.bits() - 1;
	List<unsigned int>::reverse_iterator pos = d.number.rbegin();
	unsigned int C; // ������� �� ����� d_1
	if( (r % (sizeof(unsigned int) * 8)) != 0 ) // ���� ������� ������ number - �� ������� (����� d_0 ����� ���������� �� ������ ����� �������� �������, � ��� d_1 ����� ���������� �� ��������� ����� �������, ���������� ����� ��������)
		C = 1 << ( (r - (d.size() - 1) * sizeof(unsigned int) * 8 ) - 1);
	else
	{
		C = 1 << (sizeof(unsigned int) * 8 - 1);
		++pos;
	}
	gint a_prev = (*this);
	gint a_next;
	for(unsigned long long i = 1; i <= r; ++i)
	{
		a_next = a_prev * a_prev;
		if(C & (*pos))
			a_next = a_next * (*this);
		a_next = a_next % n;
		if(C != 1)
			C >>= 1;
		else
		{
			C = 1 << (sizeof(unsigned int) * 8 - 1);
			++pos;
		}
		a_prev = a_next;
	}
	return a_next;
}

void gint::binwrite(std::ostream& outfile, unsigned long long bytes)
{
	unsigned long long real_bytes = bits() / 8 + ( (bits() % 8 != 0)? 1 : 0 ); // ������� ���� � ����� �� ����� ����
	for(unsigned long long i = 1; i <= bytes - real_bytes; ++i) // ���������� �������� �������
		outfile.put(0);
	List<unsigned int>::reverse_iterator pos = number.rbegin();
	if(real_bytes % sizeof(unsigned int) != 0) // ���� ������� ������ ����� �������� �� sizeof(unsigned int) ���� (�� ���� ������ ���� �� ���� ������� ������)
	{
		unsigned int x = *pos;
		int i = 1;
		while(x >> ( (sizeof(unsigned int) - 1) * 8 ) == 0) // ������������ �� ����� ������, ���� ��� ����� ����
		{
			x <<= 8;
			++i;
		}
		for(; i <= sizeof(unsigned int); ++i) // ���������� ��������� �����
		{
			outfile.put( (char)(x >> ( (sizeof(unsigned int) - 1) * 8 )) );
			x <<= 8;
		}
		++pos;
	}
	for(; pos != number.rend(); ++pos) // ���������� ���������� �����
	{
		unsigned int x = *pos;
		for(int i = 1; i <= sizeof(unsigned int); ++i)
		{
			outfile.put( (char)(x >> ( (sizeof(unsigned int) - 1) * 8 )) );
			x <<= 8;
		}
	}
}

bool operator<(int X, gint& Y)
{
	return Y > X;
}
bool operator<(unsigned long X, gint& Y)
{
	return Y > X;
}
bool operator<(long long X, gint& Y)
{
	return Y > X;
}
bool operator<(unsigned long long X, gint& Y)
{
	return Y > X;
}
bool operator<(const char* X, gint& Y)
{
	return Y > X;
}
bool operator<=(int X, gint& Y)
{
	return Y >= X;
}
bool operator<=(unsigned long X, gint& Y)
{
	return Y >= X;
}
bool operator<=(long long X, gint& Y)
{
	return Y >= X;
}
bool operator<=(unsigned long long X, gint& Y)
{
	return Y >= X;
}
bool operator<=(const char* X, gint& Y)
{
	return Y >= X;
}
bool operator>(int X, gint& Y)
{
	return Y < X;
}
bool operator>(unsigned long X, gint& Y)
{
	return Y < X;
}
bool operator>(long long X, gint& Y)
{
	return Y < X;
}
bool operator>(unsigned long long X, gint& Y)
{
	return Y < X;
}
bool operator>(const char* X, gint& Y)
{
	return Y < X;
}
bool operator>=(int X, gint& Y)
{
	return Y <= X;
}
bool operator>=(unsigned long X, gint& Y)
{
	return Y <= X;
}
bool operator>=(long long X, gint& Y)
{
	return Y <= X;
}
bool operator>=(unsigned long long X, gint& Y)
{
	return Y <= X;
}
bool operator>=(const char* X, gint& Y)
{
	return Y <= X;
}
bool operator==(int X, gint& Y)
{
	return Y == X;
}
bool operator==(unsigned long X, gint& Y)
{
	return Y == X;
}
bool operator==(long long X, gint& Y)
{
	return Y == X;
}
bool operator==(unsigned long long X, gint& Y)
{
	return Y == X;
}
bool operator==(const char* X, gint& Y)
{
	return Y == X;
}
bool operator!=(int X, gint& Y)
{
	return Y != X;
}
bool operator!=(unsigned long X, gint& Y)
{
	return Y != X;
}
bool operator!=(long long X, gint& Y)
{
	return Y != X;
}
bool operator!=(unsigned long long X, gint& Y)
{
	return Y != X;
}
bool operator!=(const char* X, gint& Y)
{
	return Y != X;
}

gint gint::operator+(const gint& X)const
{
	gint result;
	if( (*this).sign != X.sign)
	{
		if( (*this).sign == -1)
			return X - (-(*this));
		else
			return (*this) - (-X);
	}
	List<unsigned int>::iterator this_pos = number.begin();
	List<unsigned int>::iterator X_pos = X.number.begin();
	int transfer;
	if( (base - (*this_pos)) <= (*X_pos) )
	{
		result = (*X_pos) - (base - (*this_pos));
		transfer = 1;
	}
	else
	{
		result = (unsigned long long)(*X_pos) + (*this_pos);
		transfer = 0;
	}
	++this_pos;
	++X_pos;
	result.sign = X.sign;
	while( (this_pos != number.end()) && (X_pos != X.number.end()) )
	{
		if( (base - (*this_pos) - transfer) <= (*X_pos) )
		{
			result.number.push_back( (*X_pos) - (base - (*this_pos) - transfer) );
			transfer = 1;
		}
		else
		{
			result.number.push_back( (*X_pos) + (*this_pos) + transfer );
			transfer = 0;
		}
		++this_pos;
		++X_pos;
	}
	if(this_pos != number.end())
	{
		if(transfer)
		{
			while(transfer && (this_pos != number.end()))
			{
				if( (*this_pos) == (base - 1) )
				{
					result.number.push_back(0);
					transfer = 1;
				}
				else
				{
					result.number.push_back( (*this_pos) + 1 );
					transfer = 0;
				}
				++this_pos;
			}
			if(transfer)
				result.number.push_back(1);
			while(this_pos != number.end())
			{
				result.number.push_back( (*this_pos) );
				++this_pos;
			}
		}
		else
		{
			while(this_pos != number.end())
			{
				result.number.push_back( (*this_pos) );
				++this_pos;
			}
		}
	}
	else
	{
		if(transfer)
		{
			while(transfer && (X_pos != X.number.end()))
			{
				if( (*X_pos) == (base - 1) )
				{
					result.number.push_back(0);
					transfer = 1;
				}
				else
				{
					result.number.push_back( (*X_pos) + 1);
					transfer = 0;
				}
				++X_pos;
			}
			if(transfer)
				result.number.push_back(1);
			while(X_pos != X.number.end())
			{
				result.number.push_back( (*X_pos) );
				++X_pos;
			}
		}
		else
		{
			while(X_pos != X.number.end())
			{
				result.number.push_back( (*X_pos) );
				++X_pos;
			}
		}
	}
	return result;
}

gint gint::operator-(const gint& X)const
{
	if((*this).sign != X.sign)
		return (*this) + (-X);
	else
	{
		if((*this).sign == 1)
		{
			if(X > (*this))
				return -(X - (*this));
		}
		else
		{
			return (-X) - (-(*this));
		}
	}
	gint result = (*this);
	List<unsigned int>::iterator this_pos = result.number.begin();
	List<unsigned int>::iterator X_pos = X.number.begin();
	int transfer = 0;
	while(X_pos != X.number.end())
	{
		if( (long long)(*this_pos) - transfer - (*X_pos) < 0 )
		{
			(*this_pos) = (long long)base + ( (long long)(*this_pos) - transfer - (*X_pos) );
			transfer = 1;
		}
		else
		{
			(*this_pos) = (long long)(*this_pos) - transfer - (*X_pos);
			transfer = 0;
		}
		++this_pos;
		++X_pos;
	}
	while(transfer)
	{
		if( (*this_pos) == 0 )
		{
			(*this_pos) = (unsigned int)(base - 1);
			transfer = 1;
		}
		else
		{
			--(*this_pos);
			transfer = 0;
		}
		++this_pos;
	}
	// ������� ���������� ����
	List<unsigned int>::reverse_iterator r_pos = result.number.rbegin();
	for(unsigned long long i = result.number.size(); i > 1; --i)
	{
		if( (*r_pos) == 0 )
			result.number.remove(r_pos);
		else
			break;
	}
	return result;
}

gint gint::operator*(const unsigned long X)const
{
	if(X == 0)
		return gint(0);
	if(X == 1)
		return *this;
	if(X == -1)
		return -(*this);
	List<unsigned int>::iterator pos = number.begin();
	unsigned long long tmp = (unsigned long long)(*pos) * X;
	unsigned int transfer = tmp / base; // ������� � ������� ������
	gint result = tmp % base;
	result.sign = sign;
	++pos;
	for(; pos != number.end(); ++pos)
	{
		tmp = (unsigned long long)(*pos) * X + transfer;
		transfer = tmp / base;
		result.number.push_back(tmp % base);
	}
	if(transfer)
		result.number.push_back(transfer);
	return result;

}

gint gint::operator*(const gint& X)const
{
	if(X == 0)
		return gint(0);
	if(X == 1)
		return *this;
	if(X == -1)
		return -(*this);
	// ��������� ���������� �� ��������� ��������� � �������
	const gint* big; // �����������, ����� ������ �������. ���� �� �� ������ �������� ������ �� ������, ��, ��������, ��� ��������� ������� �� ����� ������� �����, ������� ���������� �� ������������
	const gint* small;
	if(number.size() > X.number.size())
	{
		big = this;
		small = &X;
	}
	else
	{
		big = &X;
		small = this;
	}
	List<unsigned int>::iterator pos = (*small).number.begin();
	gint result = (*big) * (*pos);
	++pos;
	unsigned long long shift = 1; // �� ������� �������� ���������� �����, ������ big ���������� �� ���� �� ���� small
	for(; pos != (*small).number.end(); ++pos, ++shift)
	{
		gint tmp = (*big) * (*pos);
		for(int i = 1; i <= shift; ++i)
			tmp.number.push_front(0);
		result = result + tmp;
	}
	result.sign = (*big).sign * (*small).sign;
	return result;
}

gint pow(const gint& X, const gint& Y)
{
	if(Y == 0)
		return gint(1);
	gint result = X;
	gint i = 1;
	for(; i * gint(2) <= Y; i = i * gint(2))
		result = result * result;
	for(; i < Y; ++i)
		result = result * X;
	return result;
}

gint abs(const gint &X)
{
	if(X >= 0) return X; else return -X;
}

gint gint::operator/(const gint& X)const
{
	/*
	������� ���������� �� ����������������� ��������� ����������. �� ���������� tmp, � ������ ������ ��������, ���������� �������� ��������. �� ������ ���� ���������� result ������������� �� �� ����������, ������� ��� �� ����� ��������. ���� ���������, ����� tmp ���������� ������ ��������. ������� � ����� ���������� result.
	�� ������ ���� ����������� ������� � ���������� �������� d � ��������� �������� �������� ������� tmp � �������� ������� ��������, ������������ �� ������� (���� �� �� �� ��������� ������� ������ �������� �� �������, ��, ��������, ��� tmp = 5,0,0,... � ��������, ������ 2,9,9,9,... �� tmp �������� ������� �� ��� ���� (5/2 = 2), � tmp ����� �� �������������, ��� ������� �� � ������������� ����������).
	���� d == 0 � m > 0, �� �� tmp m ��� ���������� ��������. � result ������������ m.
	���� d == 0 � m == 0, �� ���� ����� � tmp � ������� ���� � �� �� ���������� ����, � ����� � ��� ���������� ������� ������ (� ������������ ���������� ��������� ������ �������) �� �� tmp ���� ��� ���������� ��������. Result ������������� �� �������.
	���� d > 0 � m > 0, �� �������� ����� ������� base^d ���. ��������� �������� �� base^d ������������ ��� ������ ����� �� d ��������. ����� ����������� �������� ShiftingWords(). ������������� �������� ���������� �� m. � result ������������ m * base^d.
	���� �� d > 0 � m == 0 (������� ������ tmp ������ ��� ����� �������� ������� ��������), �� �� ��� ������� ����� �������� �������� �� base^(d-1). ����� ����, ������������� �����������, ����� �� �� �������� ������������ �������� �� 256^n, ��� n ��������� �������� �� 1 �� 3. ��������� �� 256 � ������� n ������������ ������ �������� ������������� ����� �� n ������, ��� ����������� �������� ShiftingBytes(). � ��������� ������, ����� ���� ��������� �������� �� 256^1 ��������� (tmp ������ ����� �����), �� ����� ��������� ��������� �� ����� 255 "�������������" ��������� (tmp = tmp - X).
	*/
	if( abs((*this)) < abs(X)) return gint(0);
	if( (*this) == X) return gint(1);
	if( X == 0 ) throw "Division by zero";
	if( X == 1 ) return *this;
	if( X == 2 )
	{
		gint result = (*this);
		List<unsigned int>::iterator pos = result.number.begin();
		List<unsigned int>::iterator next_pos = result.number.begin(); ++next_pos;
		(*pos) >>= 1;
		for(; next_pos != result.number.end(); ++pos, ++next_pos)
		{
			(*pos) |= (*next_pos) << 31;
			(*next_pos) >>= 1;
		}
		if(!(*pos))
			result.number.remove(pos);
		return result;
	}
	gint result(0);
	gint tmp = (*this);
	tmp.sign = 1;
	gint aX = abs(X);
	while(tmp >= aX)
	{
		unsigned long d = tmp.number.size() - aX.number.size();
		List<unsigned int>::reverse_iterator tmp_pos = tmp.number.rbegin();
		List<unsigned int>::reverse_iterator aX_pos = aX.number.rbegin();
		unsigned long m = (*tmp_pos) / ((*aX_pos) + 1);
		if( (d == 0) && (m >= 1) )
		{
			tmp = tmp - aX * gint(m);
			result = result + gint(m);
		}
		else if( (d == 0) && (m == 0) )
		{
			tmp = tmp - aX;
			++result;
		}
		else if( (*tmp_pos) > (*aX_pos) )
		{
			tmp = tmp - aX.ShiftingWords(d) * gint(m);
			result = result + pow(gint(base), gint(d)) * gint(m);
		}
		else
		{
			if( (*tmp_pos) > ((*aX_pos) >> 8) )
			{
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(3);
				result = result + gint(256 * 256 * 256) * pow(gint(base), gint(d - 1));
			}
			else if( (*tmp_pos) > ((*aX_pos) >> 16) )
			{
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(2);
				result = result + gint(256 * 256) * pow(gint(base), gint(d - 1));
			}
			else if( (*tmp_pos) > ((*aX_pos) >> 24) )
			{
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(1);
				result = result + gint(256) * pow(gint(base), gint(d - 1));
			}
			else
			{
				tmp = tmp - aX.ShiftingWords(d - 1);
				if(d == 1)
					++result;
				else
					result = result + pow(gint(base), gint(d-1));
			}
		}
	}
	result.sign = sign * X.sign;
	return result;
}

gint gint::operator%(const gint& X)const
{
	// �������� ���������� ��������� �������, �� ����������� ����, ��� �� ����������� result, � � ����� ������� ���������� tmp
	if( abs((*this)) < abs(X)) return *this;
	if( (*this) == X) return gint(0);
	if( X == 0 ) throw "Division by zero";
	if( X == 1 ) return gint(0);
	if( X == 2 )
	{
		if( (*this).odd() )
			return gint(1);
		else
			return gint(0);
	}
	gint tmp = (*this);
	tmp.sign = 1;
	gint aX = abs(X);
	while(tmp >= aX)
	{
		unsigned long d = tmp.number.size() - aX.number.size();
		List<unsigned int>::reverse_iterator tmp_pos = tmp.number.rbegin();
		List<unsigned int>::reverse_iterator aX_pos = aX.number.rbegin();
		unsigned long m = (*tmp_pos) / ((*aX_pos) + 1);
		if( (d == 0) && (m > 0) ) 
			tmp = tmp - aX * gint(m);
		else if( (d == 0) && (m == 0) )
			tmp = tmp - aX;
		else if( (d > 0) && (m > 0) )
			tmp = tmp - aX.ShiftingWords(d) * gint(m);
		else
		{
			if( (*tmp_pos) > ((*aX_pos) >> 8) )
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(3);
			else if( (*tmp_pos) > ((*aX_pos) >> 16) )
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(2);
			else if( (*tmp_pos) > ((*aX_pos) >> 24) )
				tmp = tmp - (aX.ShiftingWords(d - 1)).ShiftingBytes(1);
			else
				tmp = tmp - aX.ShiftingWords(d - 1);
		}
	}
	return tmp;
}
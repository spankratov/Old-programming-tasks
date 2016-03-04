#ifndef __RSA_H__
#define __RSA_H__
#include "giantint.h"
#include "Matrix.h"
#include <ctime>
#include <fstream>
#define STATISTICS 1 // вывод статистики во время создания ключей, шифровки и дешифровки, даёт понять, что программа не зависла

class RSA
{
	gint p, q, e, d, n;
public:
	RSA(): p(0), q(0), e(0), d(0), n(0) {}
	void SettingP(gint a) {p = a;}
	void SettingQ(gint a) {q = a;}
	void SettingE(gint a) {e = a;}
	void SettingD(gint a) {d = a;}
	void SettingN(gint a) {n = a;}
	void MakingKeys();
	void Encode(const char* s1, const char* s2);
	void Decode(const char* s1, const char* s2);
};

bool isPrime(const gint& m)
{
	if(m == 1)
		return false;
	if(m == 0)
		return false;
	if(!m.odd())
		return false;
	unsigned long long r = m.size() * sizeof(unsigned int) * 8;
	gint s = 0, t = m - 1;
	while(!t.odd())
	{
		t = t / 2;
		++s;
	}
	for(unsigned long long i = 1; i <= r; ++i)
	{
		gint tmp;
		tmp.big_rand(m - 4);
		gint a = tmp + 2;
		gint x = a.PowAndRem(t, m);
		if( (x == 1) || (x == m - 1) )
			continue;
		gint j;
		for(j = 1; j < s; ++j)
		{
			x = x.PowAndRem(2, m);
			if(x == 1)
				return false;
			if(x == (m - 1))
				break;
		}
		if(j == s)
			return false;
	}
	return true;
}

gint GCD(const gint& X, const gint& Y)
{
	if(X == Y)
		return X;
	if( (X == 0) || (Y == 0) )
		return 0;
	gint first, second;
	if(X > Y)
	{
		first = abs(X);
		second = abs(Y);
	}
	else
	{
		first = abs(Y);
		second = abs(X);
	}
	while(second > 0)
	{
		gint tmp = first % second;
		first = second;
		second = tmp;
	}
	return first;
}

gint d_e_mod_phi(const gint& e, const gint& Phi)
{
	gint a = Phi, b = e, y, q, r;
	gint** E;
	CreateMatrix(E, 2, 2);
	E[0][0] = 1; E[0][1] = 0; E[1][0] = 0; E[1][1] = 1;
	while(y == 0)
	{
		q = a / b;
		r = a - b * q;
		if(r == 0)
			y = E[1][1];
		else
		{
			gint** TMP;
			CreateMatrix(TMP, 2, 2);
			TMP[0][0] = 0; TMP[0][1] = 1; TMP[1][0] = 1; TMP[1][1] = -q;
			E = MultMatrix(E, TMP, 2, 2, 2);
			DeleteMatrix(TMP, 2);
			a = b;
			b = r;
		}
	}
	DeleteMatrix(E, 2);
	if(y < 0)
		return y + a;
	else
		return y;
}

void RSA::MakingKeys()
{
	if( (p == 0) && (n == 0) )
	{
		#ifdef STATISTICS
		std::cout << "Making p." << std::endl << std::endl;
		#endif
		p.big_rand(3, 3); // число в 3 unsigned int'а выбрано для ускорения вычислений. Проверка числа такой длины на простоту занимает от 10 секунд (в случае, если уже на первом шаге цикла становится ясно, что число не простое) до 17 минут (если число простое). Число больше 500 цифр - это как минимум 53 unsigned int'а
		#ifdef STATISTICS
		std::cout << "Checking " << p << " on primarity" << std::endl;
		#endif
		while(!isPrime(p))
		{
			#ifdef STATISTICS
			std::cout << p << " is not prime" << std::endl << std::endl;
			#endif
			p.big_rand(3, 3);
			#ifdef STATISTICS
			std::cout << "Checking " << p << " on primarity" << std::endl;
			#endif
		}
		#ifdef STATISTICS
		std::cout << p << " is prime" << std::endl << std::endl;
		#endif
	}
	#ifdef STATISTICS
	std::cout << "p = " << p << std::endl << std::endl;
	#endif
	if( (q == 0) && (n == 0) )
	{
		#ifdef STATISTICS
		std::cout << "Making q." << std::endl << std::endl;
		#endif
		q.big_rand(3, 3);
		#ifdef STATISTICS
		std::cout << "Checking " << q << " on primarity" << std::endl;
		#endif
		while(!isPrime(q))
		{
			#ifdef STATISTICS
			std::cout << q << " is not prime" << std::endl << std::endl;
			#endif
			q.big_rand(3, 3);
			#ifdef STATISTICS
			std::cout << "Checking " << q << " on primarity" << std::endl;
			#endif
		}
		#ifdef STATISTICS
		std::cout << q << " is prime" << std::endl << std::endl;
		#endif
	}
	#ifdef STATISTICS
	std::cout << "q = " << q << std::endl << std::endl;
	#endif
	if(n == 0)
	{
		#ifdef STATISTICS
		std::cout << "Making n." << std::endl << std::endl;
		#endif
		n = p * q;
	}
	#ifdef STATISTICS
	std::cout << "n = " << n << std::endl << std::endl;
	#endif
	gint euler_func = (p - 1) * (q - 1);
	if(e == 0)
	{
		#ifdef STATISTICS
		std::cout << "Making e." << std::endl << std::endl;
		#endif
		if(euler_func.size() == 1)
			e = 3;
		else
			e = 4294900033;
		while(GCD(euler_func, e) > 1)
			e = e + 2;
	}
	#ifdef STATISTICS
	std::cout << "e = " << e << std::endl << std::endl;
	#endif
	if(d == 0)
	{
		#ifdef STATISTICS
		std::cout << "Making d." << std::endl << std::endl;
		#endif
		d = d_e_mod_phi(e, euler_func);
	}
	#ifdef STATISTICS
	std::cout << "d = " << d << std::endl << std::endl;
	#endif
}

void RSA::Encode(const char* s1, const char* s2)
{
	std::ifstream infile(s1, std::ios_base::binary);
	std::ofstream outfile(s2);
	infile.seekg(0, std::ios_base::end);
	unsigned long long size = infile.tellg(); // размер входного файла в байтах
	infile.seekg(0, std::ios_base::beg);
	unsigned long long bytes = n.bits() / 8 - 1; // максимальное число байт, которых мы можем представить в виде целого числа от 0 до n-1
	while(size % bytes != 0) // определяем такое bytes, чтобы количество всех байтов из infile делилось нацело на bytes
		--bytes;
	#ifdef STATISTICS
	std::cout << "Encoding file \"" << s1 << '\"' << std::endl;
	unsigned long long count = 1;
	#endif
	outfile << bytes << ' '; // иначе при декодировке bytes не узнать
	while(infile.peek() != EOF)
	{
		char* buf = new char[bytes];
		for(unsigned long long i = 0; i < bytes; ++i)
			buf[i] = infile.get();
		gint M(buf, bytes);
		gint C = M.PowAndRem(e, n);
		outfile << C << ' ';
		#ifdef STATISTICS
		std::cout << count++ << " block has been encoded." << std::endl;
		#endif
	}
	#ifdef STATISTICS
	std::cout << "Encoding is completed." << std::endl << std::endl;
	#endif
	infile.close();
	outfile.close();
}

void RSA::Decode(const char* s1, const char* s2)
{
	std::ifstream infile(s1);
	std::ofstream outfile(s2, std::ios_base::binary);
	unsigned long long bytes;
	infile >> bytes;
	#ifdef STATISTICS
	std::cout << "Decoding file \"" << s1 << '\"' << std::endl;
	unsigned long long count = 1;
	#endif
	while(infile.peek() != EOF)
	{
		char* S = new char[1024];
		infile >> S;
		if(strcmp(S, "") == 0)
			break;
		gint C(S);
		gint M = C.PowAndRem(d, n);
		M.binwrite(outfile, bytes);
		#ifdef STATISTICS
		std::cout << count++ << " block has been decoded." << std::endl;
		#endif
	}
	#ifdef STATISTICS
	std::cout << "Decoding is completed." << std::endl << std::endl;
	#endif
	infile.close();
	outfile.close();
}

#endif
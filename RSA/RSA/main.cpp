#include <iostream>
#include "RSA.h"

int main()
{
	srand((unsigned int)time(NULL));
	RSA solution;
	solution.SettingP("1988502688910633868977902567");
	solution.SettingQ("23186126111202333752765850497"); // p и q вводятся вручную из-за высокого времени поиска p и q (проверка на простоту простого числа, представленного в виде 3 разрядов в системе счисления 2^32 занимает 17 минут). Без этих двух строчек программа выберет p и q самостоятельно
	solution.MakingKeys();
	solution.Encode("7.txt", "8.txt");
	solution.Decode("8.txt", "9.txt");
	std::system("pause");
	return 0;
}
#include <iostream>
#include "RSA.h"

int main()
{
	srand((unsigned int)time(NULL));
	RSA solution;
	solution.SettingP("1988502688910633868977902567");
	solution.SettingQ("23186126111202333752765850497"); // p � q �������� ������� ��-�� �������� ������� ������ p � q (�������� �� �������� �������� �����, ��������������� � ���� 3 �������� � ������� ��������� 2^32 �������� 17 �����). ��� ���� ���� ������� ��������� ������� p � q ��������������
	solution.MakingKeys();
	solution.Encode("7.txt", "8.txt");
	solution.Decode("8.txt", "9.txt");
	std::system("pause");
	return 0;
}
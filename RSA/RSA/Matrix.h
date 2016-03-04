#include <iostream>

template <typename TIP>
void CreateMatrix(TIP** &M, int rows, int cols)
{
	M = new TIP*[rows];
	for(int i = 0; i < rows; ++i)
		M[i] = new TIP[cols];
}

template <typename TIP>
void DeleteMatrix(TIP**M, int rows)
{
	for(int i = 0; i < rows; ++i)
		delete[] M[i];
	delete[] M;
}

template <typename TIP>
void PrintMatrix(TIP** M, int rows, int cols, const char *str = "")
{
	std::cout << str << "(" << std::endl;
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
			std::cout << M[i][j] << '\t';
		std::cout << std::endl;
	}
	std::cout << ")\n";
}

template <typename TIP>
void RandomMatrix(TIP** M, int rows, int cols, TIP min = 0, TIP max = 100)
{
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			M[i][j] = (TIP)(min + (max - min)*(double)rand()/RAND_MAX);
}

template <typename TIP>
void InputMatrix(TIP** M, int rows, int cols)
{
	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
		{
			std::cout << "M[" << i+1 << "][" << j+1 << "]=";
			std::cin >> M[i][j];
		}
}

template <typename TIP>
TIP** MultMatrix(TIP** A, TIP **B, int rA, int cA, int cB)
{
	TIP** C;
	CreateMatrix(C, rA, cB);
	for(int i = 0; i < rA; ++i)
		for(int j = 0; j < cB; ++j)
		{
			C[i][j] = 0;
			for(int k = 0; k < cA; ++k)
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
		}
	return C;
}

template <typename TIP>
TIP& min(TIP** M, int rows, int cols)
{
	int imin = 0, jmin = 0;
	for(int i =0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			if (M[i][j] < M[imin][jmin])
			{
				imin = i;
				jmin = j;
			}
	return M[imin][jmin];
}

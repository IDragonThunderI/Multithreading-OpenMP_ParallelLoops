#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

#define matrixType std::vector<std::vector<int>>
//#define DEBUG

// Заполнение матрицы рандомными числами
void fillMatrix(matrixType& matrix, std::mt19937& randomGenerator) {
	for (auto& row : matrix) {
		for (auto& column : row) {
			std::uniform_int_distribution<int> distribution(-10, 10);
			column = distribution(randomGenerator);
		}
	}
}

// Вывод матрицы на экран
void printMatrix(const matrixType& matrix) {
	for (auto& row : matrix) {
		for (auto& column : row) {
			std::cout << column << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Перемножение матриц
matrixType matrixMultiplication(const matrixType& matrixA, const matrixType& matrixB, const bool& parallelMode = false) {
	int matrixSize = matrixA.size();
	matrixType result(matrixSize, std::vector<int>(matrixSize, 0));

	// Представление матрицы
	/*												{matrix}
					 [row]				             [row]							 [row]
		{[(column), (column), (column)], [(column), (column), (column)], [(column), (column), (column)]}

	*/

	#pragma omp parallel if (parallelMode)
	{
		#ifdef DEBUG
		#pragma omp for ordered
		#endif // DEBUG

		#ifndef DEBUG
		#pragma omp for
		#endif // !DEBUG

		for (int row = 0; row < matrixSize; row++) {
			#ifdef DEBUG
			#pragma omp ordered
			#endif // DEBUG

			for (int column = 0; column < matrixSize; column++) {
				for (int position = 0; position < matrixSize; position++) {
					result.at(row).at(column) += matrixA.at(row).at(position) * matrixB.at(position).at(column);
					#ifdef DEBUG
					std::cout << "Поток " << omp_get_thread_num() << " перемножил элементы A[" << row << "][" << position << "]" <<
						" и " << "B[" << position << "][" << column << "] и прибавил к C[" << row << "][" << column << "]" <<
						" (" << matrixA.at(row).at(position) << " * " << matrixB.at(position).at(column) << " + " <<
						result.at(row).at(column) - matrixA.at(row).at(position) * matrixB.at(position).at(column) << " = " <<
						result.at(row).at(column) << ")" << std::endl;
					#endif // DEBUG
				}
			}
		}
	}
	return result;
}

int main() {
	system("chcp 1251 > nul");
	std::mt19937 randomGenerator;
	std::random_device device;
	randomGenerator.seed(device());

	size_t matrixSize;
	std::cout << "Введите размерность матрицы: ";
	std::cin >> matrixSize;
	matrixType matrixA(matrixSize, std::vector<int>(matrixSize));
	matrixType matrixB(matrixSize, std::vector<int>(matrixSize));
	matrixType matrixC(matrixSize, std::vector<int>(matrixSize));

	fillMatrix(matrixA, randomGenerator);
	fillMatrix(matrixB, randomGenerator);

	if (matrixSize <= 5) {
		std::cout << "matrixA: " << std::endl;
		printMatrix(matrixA);
		std::cout << "matrixB: " << std::endl;
		printMatrix(matrixB);
	}

	// При умножении квадратных матриц размера nxn последовательным способом количество выполненных операций имеет порядок O(n^3)
	std::cout << "Поэлементное перемножение:" << std::endl;
	double start = omp_get_wtime();
	matrixC = matrixMultiplication(matrixA, matrixB);
	double finish = omp_get_wtime();
	std::cout << "Время выполнения: " << finish - start << std::endl;

	std::cout << std::endl;

	// При умножении квадратных матриц размера nxn ленточным способом по строкам количество выполненных операций имеет порядок O(n)
	std::cout << "Ленточное перемножение по строкам:" << std::endl;
	start = omp_get_wtime();
	matrixC = matrixMultiplication(matrixA, matrixB, true);
	finish = omp_get_wtime();
	std::cout << "Время выполнения: " << finish - start << std::endl;

	if (matrixSize <= 5) {
		std::cout << "matrixC = matrixA * matrixB: " << std::endl;
		printMatrix(matrixC);
	}

	return 0;
}
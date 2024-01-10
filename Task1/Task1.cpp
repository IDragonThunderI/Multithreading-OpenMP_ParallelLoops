#include <iostream>
#include <unordered_map>
#include <omp.h>

int main() {
	system("chcp 1251 > nul");

#ifndef _OPENMP
    std::cout << "############################################" << std::endl;
    std::cout << "OpenMP не поддерживается вашим компилятором!" << std::endl;
    std::cout << "############################################" << std::endl;
#endif // !_OPENMP

#ifdef _OPENMP
    std::unordered_map<unsigned, std::string> map {
        {199810,"1.0"},
        {200203,"2.0"},
        {200505,"2.5"},
        {200805,"3.0"},
        {201107,"3.1"},
        {201307,"4.0"},
        {201511,"4.5"},
        {201811,"5.0"},
        {202011,"5.1"},
        {202111,"5.2"}
    };
    std::cout << "#########################" << std::endl;
    std::cout << "Текущая версия OpenMP " << map.at(_OPENMP) << std::endl;
    std::cout << "#########################" << std::endl << std::endl;

    std::cout << "Количество ядер процессора: " << omp_get_num_procs() / 2 << std::endl;
#endif // _OPENMP
    return 0;
}
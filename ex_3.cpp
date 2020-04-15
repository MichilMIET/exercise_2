#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <chrono>

#define SIZE_0_MASS 10000

using namespace std;
using namespace chrono;

long mass_size = SIZE_0_MASS;
int* mass = new int[mass_size];

void ReducerMaxTest(int* mass_pointer, const long size)
{
	cilk::reducer<cilk::op_max_index<long, int>> maximum;
	cilk_for(long i = 0; i < size; ++i)
	{
		maximum->calc_max(i, mass_pointer[i]);
	}
	printf("Maximal element = %d has index = %d\n\n",
		maximum->get_reference(), maximum->get_index_reference());
}

void ReducerMinTest(int* mass_pointer, const long size) // Функция поиска минимума
{
	cilk::reducer<cilk::op_min_index<long, int>> minimum;
	cilk_for(long i = 0; i < size; ++i)
	{
		minimum->calc_min(i, mass_pointer[i]);
	}
	printf("Minimum element = %d has index = %d\n\n",
		minimum->get_reference(), minimum->get_index_reference());
}

void ParallelSort(int* begin, int* end)
{
	if (begin != end)
	{
		--end;
		int* middle = partition(begin, end, bind2nd(std::less<int>(), *end));
		swap(*end, *middle);
		cilk_spawn ParallelSort(begin, middle);
		ParallelSort(++middle, ++end);
		cilk_sync;
	}
}
long create_mass(int mass_divide) {
	long mass_new_size = SIZE_0_MASS * mass_divide;
	int* mass_new = new int[mass_new_size];

	for (long i = 0; i < mass_new_size; ++i)
	{
		mass_new[i] = (rand() % 25000) + 1;
	}
	mass = mass_new;
	return mass_new_size;
}


int main()
{
	srand((unsigned)time(0));

	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");


	int* mass_begin, * mass_end;
	int divide_index[3] = { 10, 50, 100 };

	for (int j = 0; j < 3; j++) {
		delete[]mass;
		mass_size = create_mass(divide_index[j]);
		cout << "Mass size: "<< mass_size << endl;
		mass_begin = mass;
		mass_end = mass_begin + mass_size;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		ParallelSort(mass_begin, mass_end);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);
		cout << "Duration is: " << duration.count() << " seconds" << endl; // считаем время
	}
	delete[]mass;
	system("pause");
}
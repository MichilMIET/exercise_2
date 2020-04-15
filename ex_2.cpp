#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

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

void ReducerMinTest(int* mass_pointer, const long size) // ������� ������ ��������
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


int main()
{
	srand((unsigned)time(0));

	// ������������� ���������� ���������� ������� = 4
	__cilkrts_set_param("nworkers", "4");

	long i;
	const long mass_size = 10000;
	int* mass_begin, * mass_end;
	int* mass = new int[mass_size];

	for (i = 0; i < mass_size; ++i)
	{
		mass[i] = (rand() % 25000) + 1;
	}
	mass_begin = mass;
	mass_end = mass_begin + mass_size;
	cout << "	--Before sort--" << endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);
	ParallelSort(mass_begin, mass_end);
	cout << "	--After sort--" << endl;
	ReducerMaxTest(mass, mass_size);
	ReducerMinTest(mass, mass_size);
	delete[]mass;
	system("pause");
}
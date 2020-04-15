#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <iostream>
#include <vector>
#include <chrono>


using namespace std;
using namespace chrono;


void CompareForAndCilk_For(size_t sz) {
	vector<int> v;
	size_t i;
	cilk::reducer<cilk::op_vector<int>>red_vec;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (i = 0; i < sz; i++) {
		v.push_back(rand() % 20000 + 1);
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> duration = (t2 - t1);
	cout << "Duration for normal FOR is: " << duration.count() << " seconds" << endl; // считаем время
	t1 = high_resolution_clock::now();
	 cilk_for(i = 0; i < sz; i++) {
		red_vec->push_back(rand() % 20000 + 1);
	}
	t2 = high_resolution_clock::now();
	duration = (t2 - t1);
	cout << "Duration for cilk_for FOR is: " << duration.count() << " seconds" << endl; // считаем время
}

int main()
{
	srand((unsigned)time(0));

	// устанавливаем количество работающих потоков = 4
	__cilkrts_set_param("nworkers", "4");


	int iput_arg[8] = { 1000000, 100000, 10000, 1000, 500, 100, 50, 10 };

	for (int j = 0; j < 8; j++) {
		cout << "Size mass: " << iput_arg[j] << endl;
		CompareForAndCilk_For(iput_arg[j]);
		cout << endl;
	}
	system("pause");
}
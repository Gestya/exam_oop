#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <utility>
#include <cmath>
#include <iomanip>


/* The structure of data we will work and sort with.
 */
struct Part
{
	uint32_t id;    // id - unsigned ID of part.
	int count;      // count - amount of parts in a warehouse.

	friend bool operator == (const Part&, const Part&);
	friend bool operator != (const Part&, const Part&);
};


/* Operator 'Equal' for pair of parts. Check left and right element
  input: a - left opernd, b - right opernd.
  return: true if elements are equal
 */
bool operator == (const Part& a, const Part& b)
{
	return a.id == b.id && a.count == b.count;
}



/* Operator 'Equal' for pair of parts. Check left and right element
  input: a - left opernd, b - right opernd.
  return: true if elements  aren`t equal
 */
bool operator != (const Part& a, const Part& b)
{
	return a.id != b.id || a.count != b.count;
}


/* Algorithm 'Quick sort'. It is implemented as a template function and make quit sort.
   input:  &a - pointer to an array.
		   low - index of a lower element of range which should be sorted.
	   	   high - index of a higher element of range which should be sorted.
		   cmp_fun - comparison functor.

 */
template<class T>
void quick_sort(T* a, size_t low, size_t high,
	const std::function<bool(const T&, const T&)>& cmp_fun)
{
	if (low >= high)
		return;

	// Pivot value is a value of the middle element.
	T pivot = a[(low + high) / 2];

	size_t i = low;
	size_t j = high;

	while (true)
	{
		while (cmp_fun(a[i], pivot))
			i++;

		while (cmp_fun(pivot, a[j]))
			j--;

		if (i >= j)
			break;

		std::swap(a[i], a[j]);
		i++;
		j--;
	}

	quick_sort(a, low, j, cmp_fun);
	quick_sort(a, j + 1, high, cmp_fun);
}


/* Algorithm 'Selecting sort'. It is implemented as a template function and make sort.
   input: &a - pointer to an array.
		  low - index of a lower element of range which should be sorted.
		  high - index of a higher element of range which should be sorted.
          cmp_fun - comparison functor.
 */
template<class T>
void selecting_sort(T* a, size_t low, size_t high,
	const std::function<bool(const T&, const T&)>& cmp_fun)
{
	while (low < high)
	{
		T min = a[low];
		size_t pos = low;

		size_t i = low + 1;
		while (i <= high)
		{
			if (cmp_fun(a[i], min))
			{
				min = a[i];
				pos = i;
			}
			i++;
		}

		if (low != pos)
			std::swap(a[low], a[pos]);

		low++;
	}
}


/* Function creates an array of Parts which is randomly filled.
 	input: cnt - count of elements in a result array.
	return: vector of a random element with struct Part
 */
std::vector<Part> get_random_array(size_t cnt)
{
	std::vector<Part> result;
	for (size_t i = 0; i < cnt; ++i)
	{
		uint32_t id = rand();
		int count = (rand() % 5) + 1;
		Part part{ id, count };
		result.emplace_back(part);
	}
	return result;
}


/* The Main function.
 */
int main()
{
	
	// Comparison functor is lambda-function here.
	// This one compares by 'count' field.
	auto cmp_by_count = [](const Part& p1, const Part& p2) -> bool {
		return p1.count < p2.count;
	};

	// Initialization (randomized filling)
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout << "Initial array\n";
	auto parts = get_random_array(10);
	for (const auto& p : parts)
		std::cout << std::setw(10) << p.id << std::setw(15) << p.count << std::endl;

	// Run sorting algorithms in two different threads.
	size_t last_ndx = parts.size() - 1;
	std::vector<Part> qs{ parts };
	std::thread quick_sort_thread(quick_sort<Part>, qs.data(), 0, last_ndx, cmp_by_count);
	std::vector<Part> ss{ parts };
	std::thread selecting_sort_thread(selecting_sort<Part>, ss.data(), 0, last_ndx, cmp_by_count);


	// Wait for both threads complete.
	quick_sort_thread.join();
	selecting_sort_thread.join();


	// Output results of sorting.
	std::cout << "Quick sort (by count of parts)\n";
	for (const auto& p : qs)
		std::cout << std::setw(10) << p.id << std::setw(15) << p.count << std::endl;

	std::cout << "Selecting sort (by count of parts)\n";
	for (const auto& p : ss)
		std::cout << std::setw(10) << p.id << std::setw(15) << p.count << std::endl;

	
	// Search of pairwise differences.
	std::vector<std::pair<Part, Part>> diff;
	for (size_t i = 0; i <= last_ndx; ++i)
	{
		if (qs[i] != ss[i])
			diff.push_back({ qs[i], ss[i] });
	}

	// ------------------------------------------------------------------
	// Output of pairwise differences.
	std::cout << "A list of pairwise differences\n";
	for (const auto& d : diff)
		std::cout << std::setw(10) << d.first.id << std::setw(15) << d.first.count
		<< std::setw(10) << d.second.id << std::setw(15) << d.second.count << std::endl;
}
// TODO learn std::condition_variale in real project, wait for

// TODO learn std::future

// TODO learn std::mutex

// TODO learn std::lock_guard

// TODO learn std::async

// TODO learn timeout
#include <thread>
#include <iostream>
#include "testTiming.h"
#include <future>

namespace TestCreatingThreadsBasic {
	using TestScopeTiming::Timer;
	constexpr auto start = 0;
	constexpr auto end = 19000000;
	size_t evenSum = 0;
	size_t oddSum = 0;

	void findEven(const size_t start, const size_t end) {
		for (size_t i = 0; i < end; ++i) {
			if ((i & 1) == 0) {
				evenSum += i;
			}
		}
	}

	void findOdd(const size_t start, const size_t end) {
		for (size_t i = 0; i < end; ++i) {
			if ((i & 1) == 1) {
				oddSum += i;
			}
		}
	}


	void test() {
		Timer timer;
		std::thread worker1(findEven, start, end);
		std::thread worker2(findOdd, start, end);

		worker1.join();
		worker2.join();
		std::cout << "Sume of even: " << evenSum << "\n";
		std::cout << "Sume of odd: " << oddSum << "\n";
	}
}

namespace TestAsyncFutureBasic {
	using TestScopeTiming::Timer;
	constexpr auto start = 0;
	constexpr auto end = 19000000;

	size_t findEven(const size_t start, const size_t end) {
		size_t evenSum = 0;
		for (size_t i = 0; i < end; ++i) {
			if ((i & 1) == 0) {
				evenSum += i;
			}
		}
		return evenSum;
	}

	size_t findOdd(const size_t start, const size_t end) {
		size_t oddSum = 0;
		for (size_t i = 0; i < end; ++i) {
			if ((i & 1) == 1) {
				oddSum += i;
			}
		}
		return oddSum;
	}

	void test() {
		Timer timer;
		// Asynchronous start. std::async must immediately create a new thread and start the execution of the task in the new thread
		std::future<size_t> evenSumFuture = std::async(std::launch::async, findEven, start, end);

		//  Deferred execution. When using this strategy, tasks are delayed until the caller accesses the get() or wait() method of the std::future object associated with std::async. 
		// If these methods are never called, the task may never be executed.
		std::future<size_t> oddSumFuture = std::async(std::launch::async, findOdd, start, end);

		// Start asynchronous tasks and let the system decide whether to delay or execute them immediately
		//auto f = std::async(std::launch::async | std::launch::deferred, task);

		//  retrieve the result from the std::future object. This call will block until the future is ready, 
		// i.e., until the thread completes its task.
		size_t evenSum = evenSumFuture.get();
		size_t oddSum = oddSumFuture.get();
		
		std::cout << "Sume of even: " << evenSum << "\n";
		std::cout << "Sume of odd: " << oddSum << "\n";
	}
}

void test() {
	TestAsyncFutureBasic::test();

}
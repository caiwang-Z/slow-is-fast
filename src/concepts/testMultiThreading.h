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
#include <chrono>

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

namespace TestJoinAndDetach {
	/*
	// TOPIC: Use of join(), detach() and joinable() In Thread In C++ (C++11)
	// JOIN NOTES
	// 0. Once a thread is started we wait for this thread to finish by calling join() function on thread object.
	// 1. Double join will result into the whole program termination (always check if joinable before calling join).
	// 2. If needed we should check thread is joinable before joining. ( using joinable() function)
	
	// DETACH NOTES
	// 0. This is used to detach newly created thread from the parent thread. It means the parent thread or main thread would not wait for its finish. 
	// 1. Always check before detaching a thread that it is joinable otherwise we may end up double detaching and
	// double detach() will result into the wohle program termination.
	// 2. If we have detached thread and main function is returning then the detached thread execution is suspended (Means it still runs background, not terminated). But in parent thread we would not care it anymore.
	
	// NOTES:
	// Either join() or detach() should be called on thred object, otherwise during thread object's destructor it will
	// terminate the program. Because inside destructor it checks if thread is still joinable? if yes then it terminates the program.
	*/
	void run(int count) {
		while (count > 0) {
			std::cout << "Wokring, count " << count << std::endl;
			count--;
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));

	}

	void testDoubleJoin() {
		std::thread t1(run, 30);
		std::cout << "main...\n";
		t1.join();
		// t1.join();  // double join will cause in termination of whole program
		if (t1.joinable()) {
			t1.join(); // call join second time safely
		}
	}

	void testDoubleDetach() {
		std::thread worker(run, 10);
		std::cout << "main....\n";
		worker.detach();
		std::cout << "Main after...\n";
		// worker.detach();  // double detach will cause in termination of whole program
		if (worker.joinable()) {
			worker.join();
		}
	}

}

void test() {
	//TestAsyncFutureBasic::test();
	TestJoinAndDetach::testDoubleDetach();

}
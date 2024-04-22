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
#include <mutex>
#include <format>

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

namespace TestMutexBasic {
	/*
	// TOPIC: Mutex In C++ Threading | Why Use Mutex | What Is Race Condition And How To Solve It? | What Is Critical Section
	// Mutex: Mutual Exclusion
	// RACE CONDITION:
	// 0. Race condition is a situation where two or more threads/process happend to change a common data at the same time.
	// 1. If there is a race condition then we have to protect it and the protected setion is
	it and the protected setion is called critical section/region.
	// MUTEX:
	// 0. Mutex is used to avoid race condition.
	// 1. We use lock(), unlock() on mutex to avoid race condition.
	*/
	size_t amount = 0;
	void addAmount() {
		++amount;
	}

	void test() {
		std::cout << "amout: " << amount << std::endl;  // 0

		std::thread worker1(addAmount);
		std::thread worker2(addAmount);
		if (worker1.joinable()) {
			worker1.join();
		}

		if (worker2.joinable()) {
			worker2.join();
		}
		std::cout << "amout: " << amount << std::endl;  // sometimes 2, sometimes 1
	}

	std::mutex mtx_amount;
	void addAmountWithMutex() {
		mtx_amount.lock();
		++amount;
		mtx_amount.unlock();
	}

	void testWithMutex() {
		std::cout << "amout: " << amount << std::endl;  // 0

		std::thread worker1(addAmountWithMutex);
		std::thread worker2(addAmountWithMutex);
		if (worker1.joinable()) {
			worker1.join();
		}

		if (worker2.joinable()) {
			worker2.join();
		}
		std::cout << "amout: " << amount << std::endl;  // sometimes 2, sometimes 1
	}

}

namespace TestLockGuard {
	/*
	// TOPIC: lock_guard In C++ (std::lock_guard<mutex> lock(m1))
	// NOTES:
	// 0. It is very light weight wrapper for owning mutex on scoped basis.
	// 1. It aquires mutex lock the moment you create the object of lock_guard.
	// 2. It automatically removes the lock while goes out of scope.
	// 3. You can not explicitly unlock the lock_guard.
	// 4. You can not copy lock_guard.
	// 5. You can use this when you want to give a explict to the viewer of your code that after owning this mutex
	//    I do not want to release this mutex until the scope is finished.
	*/
	size_t buffer = 0;
	std::mutex mtx_buffer;

	void task(const char* threadNumber, int loopFor) {
		mtx_buffer.lock();
		for (size_t i = 0; i < loopFor; ++i) {
			++buffer;
			std::cout << "Thread number: " << threadNumber << " , buffer: " << buffer << std::endl;
		}

		mtx_buffer.unlock();
	}

	void taskWithMutex(const char* threadNumber, int loopFor) {
		std::lock_guard lg(mtx_buffer);  // acquire ownership of the mutex and lock
		for (size_t i = 0; i < loopFor; ++i) {
			++buffer;
			std::cout << "Thread number: " << threadNumber << " , buffer: " << buffer << std::endl;
		}
	}

	void test() {
		std::thread t1(taskWithMutex, "T1", 10);
		std::thread t2(taskWithMutex, "T2", 10);

		if (t1.joinable()) {
			t1.join();
		}

		if (t2.joinable()) {
			t2.join();
		}
	}
}

namespace TestUniqueLock {
	/*
	// TOPIC: unique_lock In C++ (std::unique_lock<mutex> lock(m1))
	// NOTES:
	// 1. The class unique_lock is a mutex ownership wrapper.
	// 2. It Allows:
			a. Can Have Different Locking Strategies
			b. time-constrained attempts at locking (try_lock_for, try_lock_until)
			c. recursive locking
			d. transfer of lock ownership (move not copy)
			e. condition variables. (See this in coming videos)
	// Locking Strategies
	// TYPE						EFFECTS (S)
	// 1. defer lock			do not acquire ownership of the mutex.
	// 2. try_to_lock			try to acquire ownership of the mutex without blocking.
	// 3. adopt lock			assume the calling thread already has ownership of the mutex.
	*/
	size_t buffer = 0;
	std::mutex mtx_buffer;

	void task(const char* threadNumber, int loopFor) {
		std::unique_lock<std::mutex> lock(mtx_buffer); // acquire ownership of the mutex and lock
		for (size_t i = 0; i < loopFor; ++i) {
			++buffer;
			std::cout << "Thread number: " << threadNumber << " , buffer: " << buffer << std::endl;
		}
	}

	void taskWithDeferLock(const char* threadNumber, int loopFor) {
		std::unique_lock<std::mutex> lock(mtx_buffer, std::defer_lock); // do not acquire ownership of the mutex yet, because used defer_lock
		
		// here can have some code or business logic
		lock.lock(); // here we will have to explicitly tell to lock when ever we want to lock mutex mtx_buffer
		
		for (size_t i = 0; i < loopFor; ++i) {
			++buffer;
			std::cout << "Thread number: " << threadNumber << " , buffer: " << buffer << std::endl;
		}

		// lock.unlock(); // is not needed as it will be unlocked in destructor of unique_lock
	}

	// TODO 
	void taskWithTryToLock(const char* threadNumber, int loopFor) {};

	// TODO
	void taskWithAdoptLock(const char* threadNumber, int loopFor) {};
}

namespace TestConditionVariableBasic{
	/*
	// TOPIC: Condition Variable In C++ Threading
	//IMPORTANT POINT: CV are used for two purpose
	// A. Notify other threads
	// B. Waiting for some conditions

	// 1. Condition Variable allows running threads to wait on some conditions and once those conditions are met the waiting thread
	//	  is notified using :
			a. notify_one();
			b. notify_all();
	// 2. You need mutex to use condition variable.
	// 3. If some thread want to wait on some condition then it has to do these things:
			a. Acquire the mutex lock using std::unique_lock<std::mutex> lock(m);.
			b. Execute wait, wait_for, or wait until. The wait operations atomically release the mutex
			   and suspend the execution of the thread.
			c. When the condition variable is notified, the thread is awakened, and the mutex is atomically reacquired.
			   The thread should then check the condition and resume waiting if the wake up was spurious.

	// NOTE:
	// 1. Condition variables are used to synchronize two or more threads.
	// 2. Best use case of condition variable is Producer/Consumer problem.
	*/

	std::condition_variable cv_money;
	size_t balance = 0;
	std::mutex mtx_balance;

	void addMoney(int numMoney) {
		std::unique_lock<std::mutex> lk(mtx_balance);
		balance += numMoney;
		std::cout << "Added money: " << numMoney << ", balance: " << balance << "\n";
		cv_money.notify_one();
	}

	void withdrawMoneyWithWait(int numMoney) {
		std::unique_lock<std::mutex> lk(mtx_balance);
		cv_money.wait(lk, [numMoney]() {  // return value: void
			bool res = (balance > numMoney);
			if (!res) {
				std::cout << "Money withdraw failed. Balance: " << balance << ", withdraw: " << numMoney << std::endl;
			}
			return res;
			});
		balance -= numMoney;
		std::cout << "You withdrew " << numMoney << " , current balance: " << balance << std::endl;
	}

	void test() {
		std::cout << "Main starts." << std::endl;
		std::thread t1(withdrawMoneyWithWait, 400);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::thread t2(addMoney, 500);

		if (t1.joinable()) {
			t1.join();
		}

		if (t2.joinable()) {
			t2.join();
		}

		std::cout << "Main ends." << std::endl;
	}

	void fakeAddMoney(int numMoney) {
		std::unique_lock<std::mutex> lk(mtx_balance);
		//balance += numMoney;
		std::cout << "Tried to add money: " << numMoney << ", but somehow failed. Balance: " << balance << "\n";
		cv_money.notify_one();
	}

	void withdrawMoneyWithWaitFor(int numMoney) {
		std::unique_lock<std::mutex> lk(mtx_balance);
		if (cv_money.wait_for(lk, std::chrono::seconds(8), [numMoney]() {
			// In the case of a wait_for timeout, the condition is indeed checked one last time. This is to confirm that the condition is satisfied at the moment of timeout, 
			// and to correctly reflect the state of the condition even if it is woken up at exactly the moment of timeout.
			bool res = (balance > numMoney);
			if (!res) {
				std::cout << "Money withdraw failed. Balance: " << balance << ", withdraw: " << numMoney << std::endl;
			}
			return res;
			})) {
			// condition is met before timeout
			balance -= numMoney;
			std::cout << "You withdrew " << numMoney << " , current balance: " << balance << std::endl;
		}
		else {
			// timeout, condition has not been met yet.
			std::cout << "Timeout, Withdraw money: " << numMoney << " failed!\n";

		}
	

	}

	void testWaitFor() {
		std::cout << "Main starts." << std::endl;
		std::thread t1(withdrawMoneyWithWaitFor, 400);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::thread t2(fakeAddMoney, 500);

		if (t1.joinable()) {
			t1.join();
		}

		if (t2.joinable()) {
			t2.join();
		}

		std::cout << "Main ends." << std::endl;
	}

	void withdrawMoneyWithWaitUntil(int numMoney) {
		std::unique_lock<std::mutex> lk(mtx_balance);
		const auto deadline = std::chrono::system_clock::now() + std::chrono::microseconds(4000);
		if (cv_money.wait_until(lk, deadline, [numMoney]() {
			bool res = (balance > numMoney);
			if (!res) {
				std::cout << "Money withdraw failed. Balance: " << balance << ", withdraw: " << numMoney << std::endl;
			}
			return res;
			})) {
			// Condition met by deadline
			balance -= numMoney;
			std::cout << "You withdrew " << numMoney << " , current balance: " << balance << std::endl;
		}
		else {
			// Deadline reached, conditions still not met
			std::cout << "Timeout, Reached the deadline!" << " Withdraw money : " << numMoney << " failed!\n";
		}
	}

	void testWitUntil() {
		std::cout << "Main starts." << std::endl;
		std::thread t1(withdrawMoneyWithWaitUntil, 400);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::thread t2(fakeAddMoney, 500);

		if (t1.joinable()) {
			t1.join();
		}

		if (t2.joinable()) {
			t2.join();
		}

		std::cout << "Main ends." << std::endl;
	}
}

void test() {
	//TestAsyncFutureBasic::test();
	//TestJoinAndDetach::testDoubleDetach();
	//TestMutexBasic::test();
	//TestMutexBasic::testWithMutex();
	//TestLockGuard::test();
	//TestConditionVariableBasic::test();
	//TestConditionVariableBasic::testWaitFor();
	TestConditionVariableBasic::testWitUntil();
}
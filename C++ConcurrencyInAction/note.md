# World of concurrency of C++
- 1.1 what is concurrency?
    - 1.1.1 concurrency in computer systems
    - 1.1.2 approaches to concurrency
        - concurrency with multiple processes
        - concurrency with multiple threads
            - threads in a process share the same address space
    - 1.1.3 concurrency vs parallelism
- 1.2 why use concurrency?
    - 1.2.1 using concurrency for separation of concerns
    - 1.2.2 using concurrency for performance: task and data parallelism
    - 1.2.3 when not to use concurrency
        - each thread requires a separate stack space.
- 1.3 concurrency and multithreading in C++
    - 1.3.1 history of multithreading in C++
    - 1.3.2 concurrency support in the C++11 standard
    - 1.3.3 more support for concurrency and parallelism in C++14 and C++17
    - 1.3.4 efficiency in C++ thread library
    - 1.3.5 platform-specific facilities
- 1.4 getting started
    - 1.4.1 Hello, Concurrency World
- Summary

# Managing threads
- 2.1 Basic thread management
    - 2.1.1 Launching a thread
        - if you don't decide before the std::thread object is destroyed, then your whole program is terminated(the std::thread destructor calls std::terminate()).
        - It's therefore imperative that you ensure that the thread is correctly joined or detached, even in the presence of exceptions.
    - 2.1.2 Waiting for a thread to complete
    - 2.1.3 Waiting in exceptional circumstances
        -  This means that the call to join() is liable to be skipped if an exception is thrown after the thread has been started but before the call to join().
        - RAII
        - the local objects are destroyed in reverse order of construction. 
        - detaching breaks the association of the thread with the std::thread object and ensures that std::terminated() won't be called when the std::thread object is destroyed.
    - 2.1.4 Running threads in the background
        - Detached threads truly run in the background; 
        ownership and control are passed over to the C++ Runtime Library, which ensures that resources associated with the thread are correctly reclaimed when the thread exits
- 2.2 Passing arguments to a thread function
    - by default, the arguments are copied into internal storage where they can be assessed by the newly created thread of execution.
    By default, the std::thread object stores the copies of the arguments passed in
    - std::constructors is oblivious(do not know) to the types of the arguments expected by the function and blindly copies the supplied values.
    but the internal code passes copied arguments as rvalues.
    std::ref

- 2.3 Transferring ownership of a thread
    - you can't just drop a thread by assigning a new value to the std::thread object that manages it.
    - The move support in std::thread also allows for containers of std::thread objects if those containers are move-aware.

- 2.4 Choosing the number of threads at runtime
    - std::thread::hardware_concurrency();
- 2.5 Identifying threads
    - get_id()
    - std::thread::id
    - std::this_thread::get_id()
    - std::thread::id can be used as keys in associative containers, unordered associative containers
- Summary

# Sharing data between threads
- 3.1 Problems with sharing data between threads
    - if all shared data is read-only, there is no problem.
    - invariants
    - race condition
    - 3.1.1 Race conditions
        - Race condition is anything where the outcome depends on the relative ordering of execution of operations
    - 3.1.2 Avoiding problematic race conditions
        - lock-free programming
        - transaction
            - software transactional memory(STM)
        - mutex
- 3.2 Protecting shared data with mutexes
    - mutually exclusive
    - 3.2.1 Using mutexes in C++
        - std::mutex
            - lock()
            - unlock()
        - std::lock_guard
        - template argument deduction
        - std::scoped_lock
            - like std::lock_guard, but manage multiple mutexes
    - 3.2.2 Structuring code for protecting shared data
        - Don't pass pointers and references to protected data outside the scope of the lock
        whether by returning them from a function, storing them in externally visible memory,
        or passing them as arguments to user-supplied functions
    - 3.2.3 Spotting race conditions inherent in interfaces
        - image a stack<vector<int>>
        - Option 1: Pass IN A REFERENCE
        - Option 2: REQUIRE A NO_THROW COPY CONSTRUCTOR OR MOVE CONSTRUCTOR
            - std::is_nothrow_copy_constructor
            - std::is_nothrow_move_constructor
        - Option 3: RETURN A POINTER TO THE POPPED ITEM
            - std::shared_ptr
        - Option 4: PROVIDE BOTH OPTION 1 AND EITHER OPTION 2 OR 3
        - EXAMPLE DEFINITION OF A THREAD-SAFE STACK
        - find-grained locking schemes
        - deadlock
    - 3.2.4 Deadlock: the problem and solution
        - lock mutexes in the same order does not make sense in some cases
        - std::lock
            - Listing 3.6 Using std::lock() and std::lock_guard in a swap operation
                - std::lock locks tow mutexes at one time which avoids deadlock
                - std::adopt_lock indicates to std::lock_guard that std::mutex is already locked.
            - std::lock provide all-or-nothing semantics
        - std::scoped_lock<>
            - equivalent to std::lock_guard<>
            - variadic template
            - using the same algorithm as std::lock
    - 3.2.5 Further guidelines for avoiding deadlock
        - Don't wait for another thread if there's a chance it's waiting for you.
        - AVOID NESTED LOCKS
            - Don't acquire a lock if you already hold one.
            - Do it with std::lock() or std::scoped_lock<>
        - AVOID CALLING USER-SUPPLIED CODE WHILE HOLDING A LOCK
        - ACQUIRE LOCKS IN A FIXED ORDER
            - define a order of traverse
            - disallowing reverse traversal
        - USE A LOCK HIERARCHY
            - hierarchical_mutex
            - thread-local variable
            - Listing 3.8 A simple hierarchical mutex
        - EXTENDING THESE GUIDELINES BEYOND LOCKS
    - 3.2.6 Flexible locking with std::unique_lock
        - std::unique_lock
            - std::adopt_lock
            - std::defer_lock indicates that the mutex should remain unlocked on construction
            - owns_lock()
            - deferred locking
    - 3.2.7 Transferring mutex ownership between scopes
        - std::unique_lock instances don't have to own their associated mutexes
    - 3.2.8 Locking at an appropriate granularity
        - Try to do any processing of the data outside the lock
        - Don't do any time-consuming activities like file I/O while holding a lock.
- 3.3 Alternative facilities for protecting shared data
    - The shared data needs protection only from concurrent access while it's being initialized
    - 3.3.1 Protecting shared data during initialization
        - Lazy initialization
        - Listing 3.11 Thread-safe lazy initialization using a mutex
        - 
            ```c++
            Singleton *Singleton::getInstance() {
                if (m_instance == nullptr) { // 1
                    // in line 4, for optimization, the compiler may allocate memory and assign it to m_instance, and then execute the constructor
                    // Suppose a scenario like this
                    // Thread A calls getInstance() and run to 4, and then m_instance is assigned, but constructor is not called yet.
                    // And at this moment, Thread B calls getInstance() and because m_instance is null, so it get instance successfully.
                    // However the state of this instance is not correct, if Thread B can not do things correctly with it.
                    Lock lock; // 2
                    if (m_instance == nullptr) { // 3
                        m_instance = new Singleton(); // 4 
                    }
                }
                return m_instance;
            }
            ```
        - infamous double-checked locking pattern
        - std::once_flag
            - each instance of std::once_flag corresponds to a different initialization
            - can't be copied or moved
        - std::call_once
            - std::call_once works with any function and callable object
        - static local variable
    - 3.3.2 Protecting rarely updated data structures
        - reader-writer mutex: exclusive access by a single "writer" thread, and concurrent access by multiple "reader" threads
        - std::shared_mutex
            - C++17
            - performance benefit
        - std::shared_timed_mutex
            - C++14
            - supports additional operations
        - updater threads std::lock_guard<std::shared_mutex> or std::unique_lock<std::shared_mutex>
        - reader threads std::shared_lock<std::shared_mutex>
    - 3.3.3 Recursive locking
        - std::recursive_mutex
        - not recommended
- Summary
    - std::mutex
    - std::lock
    - std::call_once()
    - std::shared_mutex

# Synchronizing concurrent operations
- synchronize actions on separate threads
- condition variables and futures
- latches and barriers
- 4.1 Waiting for an event or other condition
    - 4.1.1 Waiting for a condition with condition variables
        - std::condition_variable
            - limited to working with std::mutex
        - std::condition_variable_any
            - can work with anything that meetings the minimal criteria for being mutex-like
            - potential for additional costs in terms of size, performance, or OS resources
        - Listing 4.1 Waiting for data to process with std::condition_variable
            - data_cond.wait(lk, []{ return !data_queue.empty();})
            - lambda function is the condition
                - if condition is not satisfied, put thread in a blocked or waiting state
                - when being notified, thread wakes from its slumber, reacquires the lock on mutex, and checks the condition again, returning from wait() with mutex still locked if the condition has been satisfied.
                If the condition hasn't been satisfied, the thread unlocks the mutex and resumes waiting
        - spurious wake
        - std::condition_variable is an optimization over a busy-wait
    - 4.1.2 Building a thread-safe queue with condition variables
        - three groups of operations:
            - query the state of the whole queue(empty() and size())
            - query the elements of the queue(front() and back())
            - modify the queue(push(), pop() and emplace())
        - No guarantee of which thread will be notified or even if there is a thread waiting to be notified(all the processing threads might still be processing data)
    - 4.2 Waiting for on-off events with future
        - std::future<>
            - unique futures
            - one and only one instance that refers to its associated event
            - std::future<void>
        - std::shared_future<>
            - shared futures
            - multiple instances of std::shared_future may refer to the same event
            - std::future<void>
        - 4.2.1 Returning values from background tasks
            - std::async
                - asynchronous task
                - return a std::future object
                - get() of std::future object blocks until it is ready
            - Listing 4.6 Using std::future to get the return value of an asynchronous task
            - std::launch
                - std::launch::deferred
                    - run in wait() or get()
                - std::launch::async
                    - run in new thread
        - 4.2.2 Associating a task with a future
            - std::packaged_task<>
                - ties a future to a function or callable object
                - when std::packaged_task<> object is invoked, it call function or...
            - Listing 4.8 Partial class definition for a specialization of std::packed_task<>
            - PASSING TASKS BETWEEN THREADS
                - LISTING 4.9 Running code one a GUI thread using std::packed_task
        - 4.2.3 Making (std::)promises
            - std::promise<T>
                - get_future()
                - set_value()
                - std::promise<bool>/std::future<bool>
                - <span style="color:red;">The underlying implementation of  std::promise<>/std::future is atomic operation and condition variables</span>
        - 4.2.4 Saving an exception for the future
            - The future becomes ready, and a call to get() rethrows that stored exception
            - set_exception()
            - set_value()
            - std::current_exception()
            - std::make_exception_ptr()
            - std::future_error and std::future_error::broken_promise
        - 4.2.5 Waiting for multiple threads
            - std::future
                - only moveable
            - std::share_future
                - protect accesses with a lock
                - pass a copy of std::shared_future object to each thread
                - share() member function of std::future

    - 4.3 Waiting with a time limit
        - a duration-based timeout
        - an absolute timeout
        - 4.3.1 Clocks
            - std::chrono::system_clock::now()
            - std::ratio<1, 25>
            - The tick period of the clock
            - <span style="color:red;">use std::chrono::steady_clock instead of std::chrono::system_clock</span>
            - std:chrono::high_resolution_clock
        - 4.3.2 Duration
            - std::chrono::duration<> class template
            - A number of minutes stored in a short is std::chrono::duration<short, std::ratio<60, 1>>
            - std::chrono_literals, auto one_day=24h
            - count()
                - std::chrono::milliseconds(1234).count() is 1234
            - returned status from wait_for and wait_until
                - std::future_status::timeout
                - std::future_status::ready
                - std::future_status::deferred, if std::future is deferred until get()
        - 4.3.3 Time points
            - std::chrono::time_point<> class template
            - time_since_epoch()
            - add durations and subtract durations from instance of std::chrono::time_point<> to produce new time points
            - Listing 4.11 Waiting for a condition variable with a timeout
        - 4.3.4 Functions that accept timeouts
            - std::this_thread::sleep_for()
            - std::this_thread::sleep_until()
    - 4.4 Using synchronization of operations to simplify code
        - 4.4.1 Functional programming with futures
            - functional programming (FP)
            - FP-STYLE QUICKSORT
            - Divide and Conquer
            - Listing 4.13 Parallel Quicksort using features
            - CSP(Communicating Sequential Process)
            
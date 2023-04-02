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
        - if you don't decide before the std::thread object is destroyed, then you program is terminated(the std::thread destructor calls std::terminate()).
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
    - std::constructors is oblivious to the types of the arguments expected by the function and blindly copies the supplied values.
    but the internal code passes copied arguments as rvalues.

- 2.3 Transferring ownership of a thread
    - you can't just drop a thread by assigning a new value to the std::thread object that manages it.
    - The move support in std::thread also allows for containers of std::thread objects if those containers are move-aware.

- 2.4 Choosing the number of threads at runtime
    - std::cout << "sum: " << sum << std::endl;
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
    - 3.2.2 Structuring code for protecting shared data
        - Don't pass pointers and references to protected data outside the scope of the lock
        whether by returning them from a function, storing them in externally visible memory,
        or passing them as arguments to user-supplied functions
    - 3.2.3 Spotting race conditions inherent in interfaces
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
            - Don't it with std::lock() or std::scoped_lock<>
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
            - std::defer_lock leave leaves mutexes unlocked
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
    
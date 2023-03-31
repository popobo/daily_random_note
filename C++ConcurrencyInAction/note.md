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

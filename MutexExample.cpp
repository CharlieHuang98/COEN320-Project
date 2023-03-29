//#include <iostream>
//#include <mutex>
//#include <thread>
//
//std::mutex mtx;  // create a mutex object
//
//void shared_function(int id) {
//    // acquire the lock
//    mtx.lock();
//
//    // critical section
//    std::cout << "Thread " << id << " is accessing the shared resource." << std::endl;
//
//    // release the lock
//    mtx.unlock();
//}
//
//int main() {
//    std::thread t1(shared_function, 1);
//    std::thread t2(shared_function, 2);
//
//    t1.join();
//    t2.join();
//
//    return 0;
//}

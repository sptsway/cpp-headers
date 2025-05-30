#include <mutex>
#include <condition_variable>
using namespace std;

class Semaphore {
private:
    int count;
    condition_variable cv;
    mutex mtx;
public:
    Semaphore(int n): count(n) {}

    void V() {
        unique_lock lock(mtx);
        count++;
        cv.notify_one();
    }

    void increment() {V();}

    void P() {
        unique_lock lock(mtx);
        cv.wait(lock, [&] {
           return count > 0;
        });
        count--;
    }

    void decrement() {P();}
};

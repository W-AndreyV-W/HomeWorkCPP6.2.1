#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>


//#define atomic_heart std::memory_order_seq_cst
#define atomic_heart std::memory_order_consume
//#define atomic_heart std::memory_order_acquire
//#define atomic_heart std::memory_order_release
//#define atomic_heart std::memory_order_acq_rel
//#define atomic_heart std::memory_order_relaxed

void client(int max_client, std::atomic<int>& queue) {

    int num = 0;

    do {

         num = queue.fetch_add(1, atomic_heart) + 1;

         std::cout << "Clients + 1: " << num << std::endl;

         std::this_thread::sleep_for(std::chrono::seconds(1));

    } while (max_client > num);
}

void the_operator(std::atomic<int>& queue) {

    int num = 0;
   
    do {

        std::this_thread::sleep_for(std::chrono::seconds(2));

        num = queue.fetch_sub(1, atomic_heart) - 1;
       
        std::cout << "Clients - 1: " << num << std::endl;

    } while (num > 0);
}

int main()
{
    std::atomic<int> queue {0};
    int max_client = 10;

    std::thread flow_1(client, max_client, std::ref(queue));
    std::thread flow_2(the_operator, std::ref(queue));
    flow_1.join();
    flow_2.join();
}
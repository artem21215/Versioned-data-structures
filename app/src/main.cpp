#include <chrono>
#include <iostream>
#include <thread>

#include "QueueThreadController.h"
#include "VersionedQueue.h"

namespace {
    constexpr size_t NEW_THREAD_INDEX = 1;

    void PrintVector(const std::vector<int> &vec) {
        for (const auto &elem : vec) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    void AddElements(VersionedStructures::QueueThreadController<int> &queueTCtrl, const size_t threadIndex) {
        auto queue = queueTCtrl.GetQueue(threadIndex);

        queue.GetHistory().Dump(std::cout);
        queue.Push(12);
        queue.Push(13);
        queue.Push(14);
        std::cout << "Dump other process:" << std::endl;
        queue.GetHistory().Dump(std::cout);
        PrintVector(queue.ConvertToVector());
        std::cout << "End dump other process:" << std::endl;
    }
}

int main() {
    VersionedStructures::QueueThreadController<int> queueTCtrl(12);
    auto mainQueue = queueTCtrl.GetQueue();
    mainQueue.Push(5);
    mainQueue.Push(6);
    mainQueue.Push(1);
    mainQueue.Pop();
    mainQueue.Push(3);
    PrintVector(mainQueue.ConvertToVector());

    std::thread t(AddElements, std::ref(queueTCtrl), NEW_THREAD_INDEX);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    mainQueue.Push(9);
    mainQueue.Pop();
    mainQueue.Push(1);

    // mainQueue.Rebase(queueTCtrl.GetQueueState(NEW_THREAD_INDEX));
    mainQueue.Push(9);

    // mainQueue.GetHistory().Dump(std::cout);
    // PrintVector(mainQueue.ConvertToVector());
    mainQueue.Undo();
    mainQueue.Push(9);
    // mainQueue.Undo();
    PrintVector(mainQueue.ConvertToVector());
    // mainQueue.GetHistory().Dump(std::cout);
    // PrintVector(mainQueue.ConvertToVector());
    mainQueue.Rebase(queueTCtrl.GetQueueState(NEW_THREAD_INDEX));
    PrintVector(mainQueue.ConvertToVector());
    t.join();

    return 0;
}

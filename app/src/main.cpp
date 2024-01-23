#include <chrono>
#include <iostream>
#include <thread>

#include "StackThreadController.h"
#include "VersionedStack.h"

namespace {
    constexpr size_t NEW_THREAD_INDEX = 1;

    void PrintVector(const std::vector<int> &vec) {
        for (const auto &elem : vec) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    void AddElements(VersionedStructures::StackThreadController<int> &stackTCtrl, const size_t threadIndex) {
        auto stack = stackTCtrl.GetStack(threadIndex);

        stack.GetHistory().Dump(std::cout);
        stack.Push(12);
        stack.Push(13);
        stack.Push(14);
        std::cout << "Dump other process:" << std::endl;
        stack.GetHistory().Dump(std::cout);
        PrintVector(stack.ConvertToVector());
        std::cout << "End dump other process:" << std::endl;
    }
}

int main() {
    VersionedStructures::StackThreadController<int> stackTCtrl(12);
    auto mainStack = stackTCtrl.GetStack();
    mainStack.Push(5);
    mainStack.Push(6);
    mainStack.Push(1);
    mainStack.Pop();
    mainStack.Push(3);
    PrintVector(mainStack.ConvertToVector());

    std::thread t(AddElements, std::ref(stackTCtrl), NEW_THREAD_INDEX);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    mainStack.Push(9);
    mainStack.Pop();
    mainStack.Push(1);

    // mainStack.Rebase(stackTCtrl.GetStackState(NEW_THREAD_INDEX));
    mainStack.Push(9);

    // mainStack.GetHistory().Dump(std::cout);
    // PrintVector(mainStack.ConvertToVector());
    mainStack.Undo();
    mainStack.Push(9);
    // mainStack.Undo();
    PrintVector(mainStack.ConvertToVector());
    // mainStack.GetHistory().Dump(std::cout);
    // PrintVector(mainStack.ConvertToVector());
    mainStack.Rebase(stackTCtrl.GetStackState(NEW_THREAD_INDEX));
    PrintVector(mainStack.ConvertToVector());
    t.join();

    return 0;
}

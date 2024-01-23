#include <fstream>
#include <stack>
#include <stdexcept>

#include "HashSet.h"

namespace VersionedStructures {
    std::ofstream out("C:\\Users\\xinull\\projects\\Versioned-data-structures\\build\\CMakeFiles\\tests\\test-"
                      "cases\\temp.txt");
    enum class HistoryAction {
        Insert,
        Erase,
        Merge,
    };

    template <typename T> struct HistoryNode {
        HistoryNode(const HistoryAction &newAction, std::optional<T> &&newValue) : action(newAction), value(newValue) {}
        const HistoryAction action;
        const std::optional<T> value;
    };

    template <typename T> struct VersionState {
        VersionState(size_t threadsCount)
            : deletedSets(threadsCount), threadSets(threadsCount), threadHistory(threadsCount) {}
        HashSet<T> mergedSet;
        std::vector<HashSet<T>> deletedSets;
        std::vector<HashSet<T>> threadSets;
        std::vector<std::stack<HistoryNode<T>>> threadHistory;
    };

    template <typename T> class VersionedSet {
    public:
        explicit VersionedSet(const size_t threadsCount)
            : m_threadsCount(threadsCount), m_threadsStateId(threadsCount) {
            m_states.push_back(std::make_shared<VersionState<T>>(threadsCount));
        }

        void Insert(const size_t thread, const T &item, bool writeToHistory = true) {
            ThreadIdCheck(thread);
            if (GetDeletedSet(thread).Exists(item)) {
                GetDeletedSet(thread).Erase(item);
                if (writeToHistory) {
                    UpdateHistory(thread, HistoryAction::Insert, item);
                }
                return;
            }
            if (!GetMergedSet(thread).Exists(item)) {
                if (!GetThreadSet(thread).Exists(item)) {
                    GetThreadSet(thread).Insert(item);
                    if (writeToHistory) {
                        UpdateHistory(thread, HistoryAction::Insert, item);
                    }
                }
            }
        }

        void Erase(const size_t thread, const T &item, bool writeToHistory = true) {
            ThreadIdCheck(thread);
            if (GetMergedSet(thread).Exists(thread)) {
                GetDeletedSet(thread).Insert(item);
                if (writeToHistory) {
                    UpdateHistory(thread, HistoryAction::Erase, item);
                }
                return;
            }
            if (GetThreadSet(thread).Exists(item)) {
                GetThreadSet(thread).Erase(item);
                if (writeToHistory) {
                    UpdateHistory(thread, HistoryAction::Erase, item);
                }
            }
        }

        void Merge() {
            auto newState = std::make_shared<VersionState<T>>(m_threadsCount);

            for (size_t threadId = 0; threadId < m_threadsCount; threadId++) {
                newState->mergedSet.Merge(GetActualSet(threadId));
            }

            m_states.push_back(newState);

            for (size_t threadId = 0; threadId < m_threadsCount; threadId++) {
                m_threadsStateId[threadId]++;
                UpdateHistory(threadId, HistoryAction::Merge, std::nullopt);
            }
        }

        bool Undo(const size_t thread) {
            if (GetHistory(thread).size() == 0) {
                return false;
            }
            const auto lastStep = PopLastStep(thread);

            switch (lastStep.action) {
            case HistoryAction::Insert:
                if (lastStep.value) {
                    Erase(thread, *lastStep.value, false);
                }
                break;
            case HistoryAction::Erase:
                if (lastStep.value) {
                    Insert(thread, *lastStep.value, false);
                }
                break;
            case HistoryAction::Merge:
                while ((m_threadsStateId[thread] > 0) && (GetHistory(thread).size() == 0)) {
                    m_threadsStateId[thread]--;
                }
                break;
            default:
                throw std::logic_error("Undefined operation");
            };
            return true;
        }

        [[nodiscard]] bool Exists(const size_t thread, const T &item) const {
            ThreadIdCheck(thread);
            if (GetDeletedSet(thread).Exists(item)) {
                return false;
            }

            if (GetMergedSet(thread).Exists(item) || GetThreadSet(thread).Exists(item)) {
                return true;
            }
            return false;
        }

        [[nodiscard]] size_t Size(const size_t thread) const {
            ThreadIdCheck(thread);
            return GetMergedSet(thread).Size() + GetThreadSet(thread).Size() - GetDeletedSet(thread).Size();
        }

        [[nodiscard]] bool IsEmpty(const size_t thread) const { return Size(thread) == 0; }

        [[nodiscard]] std::vector<T> ConvertToVector(const size_t thread) const {
            return GetActualSet(thread).ConvertToVector();
        }

    private:
        HashSet<T> GetActualSet(const size_t thread) const {
            HashSet<T> tempHashSet;
            tempHashSet.Merge(GetMergedSet(thread));
            tempHashSet.Subtract(GetDeletedSet(thread));
            tempHashSet.Merge(GetThreadSet(thread));
            return tempHashSet;
        }

        void UpdateHistory(const size_t thread, const HistoryAction &action, std::optional<T> &&value) {
            GetHistory(thread).push(HistoryNode(action, std::move(value)));
        }

        HistoryNode<T> PopLastStep(const size_t thread) {
            const auto lastStep = GetHistory(thread).top();
            GetHistory(thread).pop();
            return lastStep;
        }

        std::shared_ptr<VersionState<T>> GetState(const size_t thread) const {
            return m_states[m_threadsStateId[thread]];
        }
        HashSet<T> &GetMergedSet(const size_t thread) const { return GetState(thread)->mergedSet; }
        HashSet<T> &GetDeletedSet(const size_t thread) const { return GetState(thread)->deletedSets[thread]; }
        HashSet<T> &GetThreadSet(const size_t thread) const { return GetState(thread)->threadSets[thread]; }
        std::stack<HistoryNode<T>> &GetHistory(const size_t thread) const {
            return GetState(thread)->threadHistory[thread];
        }

        void ThreadIdCheck(const size_t thread) const {
            if (thread >= m_threadsCount) {
                throw std::out_of_range("Thread id is greater than the threads count");
            }
        }

        const size_t m_threadsCount;
        std::vector<size_t> m_threadsStateId;
        std::vector<std::shared_ptr<VersionState<T>>> m_states;
    };
}
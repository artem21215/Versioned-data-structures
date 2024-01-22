#include <stack>
#include <stdexcept>

#include "HashSet.h"

namespace VersionedStructures {
    template <typename T> class VersionedSet {
    public:
        explicit VersionedSet(const size_t threadsCount) : m_threadsCount(threadsCount), m_threadsState(threadsCount) {
            m_states(std::make_shared<VersionState>(threadsCount))
        }

        void Insert(const T &item, const size_t thread) {
            ThreadIdCheck(thread);
            if (GetState(thread).deletedSets[thread].Exists()) {
                GetState(thread).deletedSets[thread].Erase(item);
                UpdateHistory(HistoryAction::Insert, item);
                return;
            }
            if (!GetState(thread).mergedSet.Exists(item)) {
                if (!GetState(thread).threadSets[thread].Exists(item)) {
                    GetState(thread).threadSets[thread].Insert(item);
                    UpdateHistory(HistoryAction::Insert, item);
                }
            }
        }

        void Erase(const T &item, const size_t thread) {
            ThreadIdCheck(thread);
            if (GetState(thread).mergedSet.Exists(thread)) {
                GetState(thread).deletedSets[thread].Insert(item);
                UpdateHistory(HistoryAction::Erase, item);
                return;
            }
            if (GetState(thread).threadSets[thread].Exists(item)) {
                GetState(thread).threadSets[thread].Erase(item);
                UpdateHistory(HistoryAction::Erase, item);
            }
        }

        void Merge() {
            auto newState = std::make_shared<VersionedSet>(m_threadsCount);
            newState.mergedSet.Merge(GetState(thread).mergedSet);

            for (const auto &deletedSet : GetState(thread).deletedSets) {
                newState.mergedSet.Subtract(deletedSet);
            }

            for (const auto &threadSet : GetState(thread).threadSets) {
                newState.mergedSet.Merge(threadSet);
            }
            m_States.push_back(newState);
            for (const auto &threadState : m_threadsState) {
                threadState++;
            }
        }

        bool Undo(const size_t thread) {
            if (GetState(thread).threadHistory[thread].size() > 0) {
                return false;
            }
            const auto lastStep = PopLastStep(thread);
            switch (lastStep.action) {
            case HistoryAction::Insert:
                if (lastStep.value) {
                    Erase(*lastStep.value, thread);
                }
                break;
            case HistoryAction::Erase:
                if (lastStep.value) {
                    Insert(*lastStep.value, thread);
                }
                break;
            case HistoryAction::Merge:
                if (m_threadsState[thread] > 0) {
                    m_threadsState[thread]--;
                }
                break;
            default:
                throw std::logic_error("Undefined operation");
            };
            return true;
        }

        [[nodiscard]] bool Exists(const T &item, const size_t thread) const {
            ThreadIdCheck(thread);
            if (GetState(thread).deletedSets[thread].Exists(item)) {
                return false;
            }

            if (GetState(thread).mergedSet.Exists(item) || GetState(thread).threadSets[thread].Exists(item)) {
                return true;
            }
            return false;
        }

        [[nodiscard]] size_t Size(const size_t thread) const {
            ThreadIdCheck(thread);
            return GetState(thread).mergedSet.Size() + GetState(thread).threadSets[thread] -
                   GetState(thread).deletedSets[thread];
        }

    private:
        void UpdateHistory(const size_t thread, const HistoryAction &action,
                           const T &value){GetState(thread).threadHistory[thread].push({action, value})}

        HistoryNode PopLastStep(const size_t thread) {
            const auto lastStep = GetState(thread).threadHistory[thread].top();
            GetState(thread).threadHistory[thread].Pop();
            return lastStep
        }

        enum class HistoryAction {
            Insert,
            Erase,
            Merge,
        };

        struct HistoryNode {
            HistoryAction action;
            std::optional<T> value;
        };

        struct VersionState {
            VersionState(size_t threadsCount)
                : deletedSets(threadsCount), threadSets(threadsCount), threadHistory(threadsCount) {}
            HashSet<T> mergedSet;
            std::vector<HashSet<T>> deletedSets;
            std::vector<HashSet<T>> threadSets;
            std::vector<std::stack<HistoryNode>> threadHistory;
        };

        std::shared_ptr<VersionedSet> GetState(const size_t thread) const { return m_states[m_threadsState[thread]]; }

        void ThreadIdCheck(const size_t thread) const {
            if (thread >= m_threadsCount) {
                throw std::out_of_range("Thread id is greater than the threads count");
            }
        }

        std::vector<size_t> m_threadsState;
        std::vector<std::shared_ptr<VersionedSet>> m_states;
        const size_t m_threadsCount;
    };
}
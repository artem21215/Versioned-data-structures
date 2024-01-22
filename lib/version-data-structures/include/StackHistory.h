#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "StackState.h"

namespace VersionedStructures {
    enum class HistoryAction { Insert = 0, Erase, Merge };
}

namespace {
    template <class Type> struct HistoryNodeData {
        VersionedStructures::HistoryAction action;
        VersionedStructures::StackState<Type> stackState;
    };
}

namespace VersionedStructures {
    template <class Type> class History {
    public:
        void WriteEvent(const HistoryAction &action, const StackState<Type> &stackState) {
            m_historyData.emplace(action, stackState);
        }

        void Dump(std::ostream &stream) const {
            std::vector<std::string> historyOperations;
            historyOperations.reserve(m_historyData.size());

            auto histDataCopy = m_historyData;
            while (!histDataCopy.empty()) {
                const auto historyNode = histDataCopy.top();
                histDataCopy.pop();
                std::string historyOperation;
                historyOperation += "Operation : ";
                switch (historyNode.action) {
                case HistoryAction::Insert:
                    historyOperation += "Insert";
                    break;
                case HistoryAction::Erase:
                    historyOperation += "Erase";
                    break;
                case HistoryAction::Merge:
                    historyOperation += "Merge";
                    break;
                default:
                    break;
                }
                historyOperation += ";  Current upstream : ";
                const auto node = historyNode.stackState.GetEndState();
                if (!node) {
                    throw std::logic_error("All data in tree must be not deleted!");
                }
                historyOperation += std::to_string(node->GetValue());
                historyOperations.emplace_back(historyOperation);
            }
            std::reverse(historyOperations.begin(), historyOperations.end());
            for (const auto &histOperationInfo : historyOperations) {
                stream << histOperationInfo << std::endl;
            }
        }

        void Pop() { m_historyData.pop(); }

        std::shared_ptr<StackState<Type>> GetLastVersion() const {
            return std::make_shared<StackState<Type>>(m_historyData.top().stackState);
        }

    private:
        std::stack<HistoryNodeData<Type>> m_historyData;
    };
}
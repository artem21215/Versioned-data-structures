#include <functional>
#include <list>
#include <string>
#include <vector>

namespace VersionedStructures {

    template <typename T> class HashSet {
    public:
        explicit HashSet(const size_t bucketSize = 10001) : m_buckets(bucketSize) {}

        void Insert(const T &item) {
            if (!Exists(item)) {
                const size_t hash = GetHash(item);
                m_buckets[hash].push_back(item);
                size++;
            }
        }

        void Erase(const T &item) {
            const size_t hash = GetHash(item);
            const auto iter = Find(item);
            if (iter != m_buckets[hash].end()) {
                m_buckets[hash].erase(iter);
                size--;
            }
        }

        void Merge(const HashSet<T> &rhs) {
            for (const auto &bucket : rhs.m_buckets) {
                for (const auto &item : bucket) {
                    Insert(item);
                }
            }
        }

        void Subtract(const HashSet<T> &rhs) {
            for (const auto &bucket : rhs.m_buckets) {
                for (const auto &item : bucket) {
                    Erase(item);
                }
            }
        }

        [[nodiscard]] bool Exists(const T &item) const {
            const size_t hash = GetHash(item);
            return Find(item) != m_buckets[hash].end();
        }

        [[nodiscard]] size_t Size() const { return size; }

        [[nodiscard]] size_t BucketSize() const { return m_buckets.size(); }

        [[nodiscard]] bool IsEmpty() const { return size == 0; }

        [[nodiscard]] std::vector<T> ConvertToVector() const {
            std::vector<T> vec;
            for (const auto &bucket : m_buckets) {
                for (const auto &item : bucket) {
                    vec.push_back(item);
                }
            }
            return vec;
        }

    private:
        std::list<T>::const_iterator Find(const T &item) const {
            const size_t hash = GetHash(item);
            return std::find(m_buckets[hash].begin(), m_buckets[hash].end(), item);
        }

        size_t GetHash(const T &item) const { return std::hash<T>{}(item) % m_buckets.size(); }

        std::vector<std::list<T>> m_buckets;
        size_t size = 0;
    };
}
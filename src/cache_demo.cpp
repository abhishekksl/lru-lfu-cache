// cache_demo.cpp
// High-performance LRU & LFU cache implementations with a simple micro-benchmark.
// Written in modern C++17. Clean, minimal, and interview-friendly.

#include <bits/stdc++.h>
using namespace std;

/* ============================================================
   LRU Cache
   ------------------------------------------------------------
   Uses:
     - Doubly linked list to track recency
     - Hash map for O(1) access to list nodes
   get()  → O(1)
   put() → O(1)
   ============================================================ */

template <typename K, typename V>
class LRUCache {
public:
    LRUCache(size_t capacity) : capacity_(capacity) {
        lookup_.reserve(capacity * 2);
    }

    optional<V> get(const K& key) {
        auto it = lookup_.find(key);
        if (it == lookup_.end())
            return nullopt;

        // Move the accessed node to the front
        items_.splice(items_.begin(), items_, it->second);
        return it->second->value;
    }

    void put(const K& key, const V& value) {
        auto it = lookup_.find(key);

        // Key exists → update and move to front
        if (it != lookup_.end()) {
            it->second->value = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        // Evict least recent (tail)
        if (items_.size() >= capacity_) {
            const auto& last = items_.back();
            lookup_.erase(last.key);
            items_.pop_back();
        }

        // Insert new node at front
        items_.push_front(Node{key, value});
        lookup_[key] = items_.begin();
    }

private:
    struct Node {
        K key;
        V value;
    };

    size_t capacity_;
    list<Node> items_;
    unordered_map<K, typename list<Node>::iterator> lookup_;
};

/* ============================================================
   LFU Cache
   ------------------------------------------------------------
   Uses:
     - Freq → list of keys
     - Hash map for key → (value, freq)
     - Tracks minimum frequency for quick eviction
   Very interview-friendly implementation.
   ============================================================ */

template <typename K, typename V>
class LFUCache {
public:
    LFUCache(size_t capacity) : capacity_(capacity), minFreq_(0) {
        data_.reserve(capacity * 2);
        iter_.reserve(capacity * 2);
    }

    optional<V> get(const K& key) {
        if (!data_.count(key)) return nullopt;
        increaseFreq(key);
        return data_[key].value;
    }

    void put(const K& key, const V& value) {
        if (capacity_ == 0) return;

        // Update existing
        if (data_.count(key)) {
            data_[key].value = value;
            increaseFreq(key);
            return;
        }

        // Evict if needed
        if (data_.size() >= capacity_) {
            auto& lst = freqList_[minFreq_];
            K evictKey = lst.back();
            lst.pop_back();
            if (lst.empty())
                freqList_.erase(minFreq_);
            data_.erase(evictKey);
            iter_.erase(evictKey);
        }

        // Insert new key with freq = 1
        minFreq_ = 1;
        freqList_[1].push_front(key);
        iter_[key] = freqList_[1].begin();
        data_[key] = Entry{value, 1};
    }

private:
    struct Entry {
        V value;
        int freq;
    };

    size_t capacity_;
    int minFreq_;
    unordered_map<K, Entry> data_;
    unordered_map<int, list<K>> freqList_;
    unordered_map<K, typename list<K>::iterator> iter_;

    void increaseFreq(const K& key) {
        int f = data_[key].freq;
        auto it = iter_[key];

        // Remove from old freq list
        freqList_[f].erase(it);
        if (freqList_[f].empty()) {
            freqList_.erase(f);
            if (minFreq_ == f)
                minFreq_++;
        }

        // Add to new freq list
        int newFreq = f + 1;
        data_[key].freq = newFreq;
        freqList_[newFreq].push_front(key);
        iter_[key] = freqList_[newFreq].begin();
    }
};

/* ============================================================
   Simple benchmark: mixed get/put workload
   This shows approx ops/sec for each cache.
   ============================================================ */

struct Timer {
    chrono::steady_clock::time_point start;
    Timer() { start = chrono::steady_clock::now(); }
    double elapsed() {
        return chrono::duration<double>(chrono::steady_clock::now() - start).count();
    }
};

static void run_benchmark() {
    const size_t ops = 2'000'000;
    const size_t capacity = 50'000;
    const size_t keyRange = 100'000;

    mt19937_64 rng(12345);
    uniform_int_distribution<int> dist(1, keyRange);

    // ---------- LRU ----------
    {
        LRUCache<int,int> cache(capacity);
        Timer t;
        for (size_t i = 0; i < ops; ++i) {
            int k = dist(rng);
            if (i & 1) cache.get(k);
            else cache.put(k, i);
        }
        double s = t.elapsed();
        cout << "LRU ops/sec: " << fixed << setprecision(2)
             << (ops / s) << " (time " << s << "s)\n";
    }

    // ---------- LFU ----------
    {
        LFUCache<int,int> cache(capacity);
        Timer t;
        for (size_t i = 0; i < ops; ++i) {
            int k = dist(rng);
            if (i & 1) cache.get(k);
            else cache.put(k, i);
        }
        double s = t.elapsed();
        cout << "LFU ops/sec: " << fixed << setprecision(2)
             << (ops / s) << " (time " << s << "s)\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Running LRU & LFU benchmark...\n";
    run_benchmark();
    cout << "Done.\n";
    return 0;
}

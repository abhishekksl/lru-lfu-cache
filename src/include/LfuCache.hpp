#pragma once

#include <list>
#include <unordered_map>
#include <optional>
#include <cstddef>

template <typename Key, typename Value>
class LfuCache {
public:
    explicit LfuCache(size_t capacity) : capacity_(capacity), min_freq_(0) {
        key_to_val_.reserve(capacity);
        key_to_iter_.reserve(capacity);
    }

    std::optional<Value> get(const Key& key) {
        if (key_to_val_.find(key) == key_to_val_.end()) return std::nullopt;
        touch(key);
        return key_to_val_[key].value;
    }

    void put(const Key& key, const Value& value) {
        if (capacity_ == 0) return;

        if (key_to_val_.find(key) != key_to_val_.end()) {
            key_to_val_[key].value = value;
            touch(key);
            return;
        }

        if (key_to_val_.size() >= capacity_) evict();

        min_freq_ = 1;
        freq_to_keys_[1].push_front(key);
        key_to_val_[key] = {value, 1};
        key_to_iter_[key] = freq_to_keys_[1].begin();
    }

private:
    struct Node { Value value; int freq; };
    size_t capacity_;
    int min_freq_;
    std::unordered_map<Key, Node> key_to_val_;
    std::unordered_map<int, std::list<Key>> freq_to_keys_;
    std::unordered_map<Key, typename std::list<Key>::iterator> key_to_iter_;

    void touch(const Key& key) {
        int prev = key_to_val_[key].freq;
        int next = prev + 1;
        freq_to_keys_[prev].erase(key_to_iter_[key]);
        
        if (freq_to_keys_[prev].empty()) {
            freq_to_keys_.erase(prev);
            if (min_freq_ == prev) min_freq_++;
        }

        key_to_val_[key].freq = next;
        freq_to_keys_[next].push_front(key);
        key_to_iter_[key] = freq_to_keys_[next].begin();
    }

    void evict() {
        auto& list = freq_to_keys_[min_freq_];
        Key k = list.back();
        list.pop_back();
        if (list.empty()) freq_to_keys_.erase(min_freq_);
        key_to_val_.erase(k);
        key_to_iter_.erase(k);
    }
};

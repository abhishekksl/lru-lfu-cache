#pragma once

#include <list>
#include <unordered_map>
#include <optional>
#include <cstddef>

template <typename Key, typename Value>
class LruCache {
public:
    explicit LruCache(size_t capacity) : capacity_(capacity) {
        cache_map_.reserve(capacity);
    }

    std::optional<Value> get(const Key& key) {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            return std::nullopt;
        }
        // Move accessed node to front (Mark as recently used)
        lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
        return it->second->value;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache_map_.find(key);
        
        // Update existing
        if (it != cache_map_.end()) {
            it->second->value = value;
            lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
            return;
        }

        // Evict if full
        if (lru_list_.size() >= capacity_) {
            auto last = lru_list_.back();
            cache_map_.erase(last.key);
            lru_list_.pop_back();
        }

        // Insert new
        lru_list_.push_front({key, value});
        cache_map_[key] = lru_list_.begin();
    }

private:
    struct Node { Key key; Value value; };
    size_t capacity_;
    std::list<Node> lru_list_;
    std::unordered_map<Key, typename std::list<Node>::iterator> cache_map_;
};

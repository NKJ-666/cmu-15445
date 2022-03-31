//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {
    max_size_ = num_pages;
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
    std::scoped_lock lock {mut_};
    if (lru_list_.empty()) {
        return false;
    }
    * frame_id = lru_list_.back();
    lru_hash_.erase(* frame_id);
    lru_list_.pop_back();
    return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    std::scoped_lock lock {mut_};
    auto value = lru_hash_.find(frame_id);
    if (value == lru_hash_.end()) {
        return;
    }
    lru_list_.erase(value->second);
    lru_hash_.erase(value);
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    std::scoped_lock lock {mut_};
    if (lru_hash_.count(frame_id) != 0) {
        return;
    }
    if (lru_list_.size() >= max_size_) {
        return;
    }
    lru_list_.push_front(frame_id);
    lru_hash_[frame_id] = lru_list_.begin();
}

size_t LRUReplacer::Size() {
    std::scoped_lock lock {mut_};
    return lru_list_.size();
}

}  // namespace bustub

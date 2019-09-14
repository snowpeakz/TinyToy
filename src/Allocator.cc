#include "Allocator.h"

namespace TinyToy {

Allocator::Allocator() {
  size_t bins_num = chunk_capacity.size();
  for (int i = 0; i < bins_num; ++i) {
    Byte* data = new Byte[chunk_capacity[i]];
    Chunk* ch = new Chunk(nullptr, nullptr, chunk_capacity[i], i, false, data);
    Bin* b = new Bin(i, chunk_capacity[i], ch, 1);
    bins_.push_back(b);
  }
}

Allocator::~Allocator() {
  size_t bins_num = chunk_capacity.size();
  for (int i = 0; i < bins_num; ++i) {
    Bin* bin = bins_[i];
    Chunk *it = bin->Chunks_head;
    while (it) {
      auto iter = allocated_.find(it->data);
      if (iter != allocated_.end()) {
        std::cout << "Allocator destructor called before all relased, ptr = "
          << it->data << std::endl;
      }
      if (it->data) {
        delete[] it->data;
        it->data = nullptr;
      }
      if (it ->prev) {
        delete it->prev;
        it->prev = nullptr;
      }
      if (it->next) {
        delete it;
        it = nullptr;
      } else {
        it = it->next;
      }
    }
    delete bin;
  }
}

void* Allocator::allocate(size_t req) {
  if (req == 0) {
    return nullptr;
  }
  size_t bin_id = std::log2(req);
  ++bin_id;
  {
    std::lock_guard<std::mutex> l(bins_[bin_id]->mu);
    Chunk* it = bins_[bin_id]->Chunks_head;
    while (it) {
      if (it->is_allocated == false) {
        mu_.lock();
        allocated_[it->data] = it;
        mu_.unlock();
        it->is_allocated = true;
        return it->data;
      }
      if (!it->next) {
        Byte* d = new Byte[bins_[bin_id]->Chunks_size];
        Chunk* ch = new Chunk(it, nullptr, bins_[bin_id]->Chunks_size, bin_id, false, d);
        it->next = ch;
      }
      it = it->next;
    }
  }
  std::cout << "allocated failed, it is a bug" << std::endl;
  return nullptr;
}

bool Allocator::relase(void* ptr) {
  if (ptr == nullptr) {
    std::cout << "trying to relase a nullptr" << std::endl;
    return true;
  }
  Chunk* ch = nullptr;
  {
    std::lock_guard<std::mutex> l(mu_);
    auto it = allocated_.find(ptr);
    if (it == allocated_.end()) {
      std::cout << "trying to relase a not allocated ptr" << std::endl;
    } else {
      ch = (*it).second;
    }
  }
  // assume user won't relase same Chunk at same time
  {
    std::lock_guard<std::mutex> l(bins_[ch->bin]->mu);
    ch->is_allocated = false;
    auto p = ch->prev, n = ch->next;
    if (p) {
      p->next = n;
    }
    if (n) {
      n->prev = p;
    }
    ch->prev = nullptr;
    ch->next = bins_[ch->bin]->Chunks_head;
    bins_[ch->bin]->Chunks_head->prev = ch;
    bins_[ch->bin]->Chunks_head = ch;
  }
  {
    std::lock_guard<std::mutex> l(mu_);
    allocated_.erase(ptr);
  }
}

} // namespace TinyToy

#ifndef TINYTOY_ALLOCATOR_H
#define TINYTOY_ALLOCATOR_H

#include <vector>
#include <unordered_map>
#include <mutex>
#include<cmath>
#include <iostream>

namespace TinyToy {

using Byte = char;

const std::vector<size_t> chunk_capacity = {2, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};

struct Chunk {
  Chunk* prev;
  Chunk* next;
  size_t size;
  size_t bin;
  bool is_allocated;
  void* data;
  Chunk(Chunk* p, Chunk* n, size_t s, size_t b, bool ia, void* d)
    : prev(p), next(n), size(s), bin(b), is_allocated(ia), data(d) {}
};

struct Bin {
  size_t id;
  size_t Chunks_size;
  Chunk* Chunks_head;
  size_t temperature;
  Bin(size_t i, size_t Cs, Chunk* Ch, size_t t)
    : id(i), Chunks_size(Cs), Chunks_head(Ch), temperature(t) {}
};

class Allocator {
public:
  Allocator();
  ~Allocator();

  void* allocate(size_t req);
  bool relase(void* ptr);
  
private:
//  bool SplitChunk(Chunk* c);
//  bool MergeChunk(Chunk* c1, Chunk* c2);

  std::vector<Bin*> bins_;
  std::unordered_map<void*, Chunk*> allocated_;
  std::vector<std::mutex*> mus_;
  std::mutex mu_;
};

} // namespace TinyToy

#endif //TINYTOY_ALLOCATOR_H
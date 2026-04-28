#ifndef HADAL_RING_BUFFER_HPP
#define HADAL_RING_BUFFER_HPP

#include <atomic>
#include <optional>
#include <vector>

template <typename T, size_t Size>
class RingBuffer {
    static_assert((Size & (Size - 1)) == 0, "Size must be power of 2");
public:
    RingBuffer() : head(0), tail(0) { buffer.resize(Size); }

    bool push(const T& item) {
        size_t h = head.load(std::memory_order_relaxed);
        size_t next_h = (h + 1) & (Size - 1);
        if (next_h == tail.load(std::memory_order_acquire)) return false;
        buffer[h] = item;
        head.store(next_h, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        size_t t = tail.load(std::memory_order_relaxed);
        if (t == head.load(std::memory_order_acquire)) return std::nullopt;
        T item = buffer[t];
        tail.store((t + 1) & (Size - 1), std::memory_order_release);
        return item;
    }

private:
    std::vector<T> buffer;
    std::atomic<size_t> head, tail;
};

#endif

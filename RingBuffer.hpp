#ifndef HADAL_RING_BUFFER_HPP
#define HADAL_RING_BUFFER_HPP

#include <cstdint>
#include <atomic>

/**
 * @brief Hadal Core: Zero-Allocation DMA Ring Buffer
 * Designed for STM32 DMA double-buffering.
 * Copyright © 2026 NIAS.
 */
template <typename T, uint32_t Size>
class RingBuffer {
private:
    T buffer[Size];
    std::atomic<uint32_t> head{0};
    std::atomic<uint32_t> tail{0};

public:
    RingBuffer() = default;

    // Direct access for DMA Controllers
    T* data() { return buffer; }
    uint32_t size() const { return Size; }

    // Lock-free push (Producer: Hardware/DMA)
    bool push(T item) {
        uint32_t next = (head.load(std::memory_order_relaxed) + 1) % Size;
        if (next == tail.load(std::memory_order_acquire)) return false; // Full
        buffer[head.load()] = item;
        head.store(next, std::memory_order_release);
        return true;
    }

    // Lock-free pop (Consumer: CPU/DSP Logic)
    bool pop(T& item) {
        if (head.load(std::memory_order_acquire) == tail.load()) return false; // Empty
        item = buffer[tail.load()];
        tail.store((tail.load() + 1) % Size, std::memory_order_release);
        return true;
    }
};

#endif

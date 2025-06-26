#ifndef FLASHQUERY_CORE_ARENA_HPP
#define FLASHQUERY_CORE_ARENA_HPP

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>
#include <type_traits>
#include <utility>

namespace flashquery {

class Arena {
public:
    explicit Arena(std::size_t sz) : offset(0)
    {
        this->memory = static_cast<char *>(std::malloc(sz));
        if (!this->memory)
            throw std::bad_alloc();
        this->memory_size = sz;
    }

    ~Arena()
    {
        std::free(this->memory);
    }

    template <typename T, std::size_t N = 1, typename... Args> T *alloc(Args &&... args)
    {
        static_assert(std::is_trivially_destructible_v<T>);

        char *unaligned = this->memory + this->offset;
        T *aligned = this->align_ptr_up<T>(reinterpret_cast<T *>(unaligned));
        std::size_t aligned_offset = reinterpret_cast<char *>(aligned) - this->memory;
        std::size_t new_offset = aligned_offset + sizeof(T) * N;

        if (new_offset > this->memory_size) {
            std::size_t new_size = this->memory_size * 2;

            if (new_offset > new_size)
                new_size = new_offset;

            char *tmp = static_cast<char *>(std::realloc(this->memory, new_size));
            if (!tmp)
                throw std::bad_alloc();

            this->memory = tmp;
            this->memory_size = new_size;
       
            unaligned = this->memory + this->offset;
            aligned = this->align_ptr_up<T>(reinterpret_cast<T *>(unaligned));
        }

        this->offset = new_offset;

        for (std::size_t i = 0; i < N; i++)
            new (aligned + i) T(std::forward<Args>(args)...);

        return aligned;
    }

private:
    char *memory;
    std::size_t memory_size;
    std::size_t offset;

    template <typename T> constexpr inline T *align_ptr_up(T *ptr) noexcept
    {
        std::uintptr_t ptr_val = reinterpret_cast<std::uintptr_t>(ptr);
        std::uintptr_t aligned_val = (ptr_val + alignof(T) - 1) & ~(alignof(T) - 1);
        return reinterpret_cast<T *>(aligned_val);
    }
};

} // namespace flashquery

#endif // FLASHQUERY_CORE_ARENA_HPP

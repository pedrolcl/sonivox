#include <stdint.h> // For uint64_t
#include <limits.h> // For USHRT_MAX
#ifdef _MSC_VER
#include <intrin.h> // For _umul128 and _addcarry_u64
#endif

#define MAX_TICK_CONV USHRT_MAX // Use USHRT_MAX for 16-bit unsigned max value

void compute_tick_conv(uint32_t temp, uint32_t ppqn, uint16_t *tickConv) {
    if (ppqn == 0) {
        // Guard against division by zero
        *tickConv = MAX_TICK_CONV;
        return;
    }

    uint64_t temp64;
    int overflow = 0;

#ifdef _MSC_VER
    // MSVC-specific overflow-safe multiplication
    uint64_t high;
    temp64 = _umul128(temp, 1024u, &high);
    if (high != 0) {
        overflow = 1;
    }
#else
    // GCC/Clang-specific overflow-safe multiplication
    if (__builtin_mul_overflow(temp, 1024u, &temp64)) {
        overflow = 1;
    }
#endif

    if (!overflow) {
        temp64 /= ppqn;

#ifdef _MSC_VER
        // MSVC-specific overflow-safe addition
        uint8_t carry = _addcarry_u64(0, temp64, 500, &temp64);
        if (carry || temp64 > MAX_TICK_CONV) {
            overflow = 1;
        }
#else
        // GCC/Clang-specific overflow-safe addition
        if (__builtin_add_overflow(temp64, 500, &temp64) || temp64 > MAX_TICK_CONV) {
            overflow = 1;
        }
#endif
    }

    if (overflow) {
        *tickConv = MAX_TICK_CONV;
    } else {
        *tickConv = (uint16_t)(temp64 / 1000);
    }
}

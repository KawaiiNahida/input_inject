#pragma once

#include <cstdint>
#include <array>

#include "bitset.h"
#include "enums.h"

using status_t = int32_t;
using nsecs_t = int64_t;

/*
 * Maximum number of pointers supported per motion event.
 * Smallest number of pointers is 1.
 * (We want at least 10 but some touch controllers obstensibly configured for 10 pointers
 * will occasionally emit 11.  There is not much harm making this constant bigger.)
 */
static constexpr size_t MAX_POINTERS = 16;
/*
 * Maximum pointer id value supported in a motion event.
 * Smallest pointer id is 0.
 * (This is limited by our use of BitSet32 to track pointer assignments.)
 */
static constexpr size_t MAX_POINTER_ID = 31;


struct PointerCoords {
    enum {
        MAX_AXES = 30
    }; // 30 so that sizeof(PointerCoords) == 136

    // Bitfield of axes that are present in this structure.
    uint64_t bits __attribute__((aligned(8)));

    // Values of axes that are stored in this structure packed in order by axis id
    // for each axis that is present in the structure according to 'bits'.
    std::array<float, MAX_AXES> values;

    // Whether these coordinate data were generated by resampling.
    bool isResampled;

    static_assert(sizeof(bool) == 1); // Ensure padding is correctly sized.
    uint8_t empty[7];

    float getAxisValue(int32_t axis) const;

    status_t setAxisValue(int32_t axis, float value);
};

inline float PointerCoords::getAxisValue(int32_t axis) const {
    return SymCall(hooks::LIBINPUT, "_ZNK7android13PointerCoords12getAxisValueEi", float, const PointerCoords *,
                   int32_t)(this, axis);
}

status_t PointerCoords::setAxisValue(int32_t axis, float value) {
    return SymCall(hooks::LIBINPUT, "_ZN7android13PointerCoords12setAxisValueEif", status_t, PointerCoords *, int32_t,
                   float)(this, axis, value);
}

struct PointerProperties {
    // The id of the pointer.
    int32_t id;
    // The pointer tool type.
    ToolType toolType;

    inline void clear() {
        id = -1;
        toolType = ToolType::UNKNOWN;
    }
};

using PropertiesArray = std::array<PointerProperties, MAX_POINTERS>;
using CoordsArray = std::array<PointerCoords, MAX_POINTERS>;
using IdToIndexArray = std::array<uint32_t, MAX_POINTER_ID + 1>;
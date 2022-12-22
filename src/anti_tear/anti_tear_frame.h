/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#ifndef VCS_FILTER_ANTI_TEAR_FRAME_H
#define VCS_FILTER_ANTI_TEAR_FRAME_H

#include "common/globals.h"
#include "common/assert.h"
#include "common/memory/heap_mem.h"
#include "display/display.h"

struct anti_tear_frame_s
{
    resolution_s resolution;

    heap_mem<u8> pixels;

    anti_tear_frame_s(const resolution_s resolution = {0, 0, 0},
                      u8 *const pixels = nullptr)
    {
        this->resolution = resolution;

        if (pixels)
        {
            this->pixels.point_to(pixels, (resolution.w * resolution.h * (resolution.bpp / 8)));
        }

        return;
    }

    bool is_valid(void)
    {
        return (this->pixels.data() &&
                (this->resolution.w <= MAX_OUTPUT_WIDTH) &&
                (this->resolution.h <= MAX_OUTPUT_HEIGHT) &&
                (this->resolution.bpp <= MAX_OUTPUT_BPP));
    }

    void flip_vertically(void)
    {
        k_assert(this->is_valid(), "Invalid frame.");

        u8 scratch[MAX_OUTPUT_WIDTH * (MAX_OUTPUT_BPP / 8)];

        for (unsigned y = 0; y < (this->resolution.h / 2); y++)
        {
            const unsigned numBytesPerPixel = (this->resolution.bpp / 8);
            const unsigned numBytesPerRow = (this->resolution.w * numBytesPerPixel);
            const unsigned idx1 = (y * this->resolution.w * numBytesPerPixel);
            const unsigned idx2 = ((this->resolution.h - 1 - y) * this->resolution.w * numBytesPerPixel);

            std::memcpy(&scratch,                    (this->pixels.data() + idx1), numBytesPerRow);
            std::memcpy((this->pixels.data() + idx1), (this->pixels.data() + idx2), numBytesPerRow);
            std::memcpy((this->pixels.data() + idx2), &scratch, numBytesPerRow);
        }

        return;
    }
};

#endif

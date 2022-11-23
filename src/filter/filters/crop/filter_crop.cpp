/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#include "filter/filters/crop/filter_crop.h"
#include <opencv2/imgproc/imgproc.hpp>

// Takes a subregion of the frame and either scales it up to fill the whole frame or
// fills its surroundings with black.
void filter_crop_c::apply(u8 *const pixels, const resolution_s &r)
{
    this->assert_input_validity(pixels, r);

    const unsigned x = this->parameter(PARAM_X);
    const unsigned y = this->parameter(PARAM_Y);
    const unsigned w = this->parameter(PARAM_WIDTH);
    const unsigned h = this->parameter(PARAM_HEIGHT);
    const unsigned scalerType = this->parameter(PARAM_SCALER);

    int cvScaler = -1;

    switch (scalerType)
    {
        case SCALE_LINEAR:  cvScaler = cv::INTER_LINEAR; break;
        case SCALE_NEAREST: cvScaler = cv::INTER_NEAREST; break;
        case SCALE_NONE:    cvScaler = -1; break;
        default: k_assert(0, "Unknown scaler type for the crop filter."); break;
    }

    if (((x + w) > r.w) || ((y + h) > r.h))
    {
        /// TODO: Signal a user-facing but non-obtrusive message about the crop
        /// params being invalid.
    }
    else
    {
        cv::Mat output = cv::Mat(r.h, r.w, CV_8UC4, pixels);
        cv::Mat cropped = output(cv::Rect(x, y, w, h)).clone();

        // If the user doesn't want scaling, just append some black borders around the
        // cropping. Otherwise, stretch the cropped region to fill the entire frame.
        if (cvScaler < 0) cv::copyMakeBorder(cropped, output, y, (r.h - (h + y)), x, (r.w - (w + x)), cv::BORDER_CONSTANT, 0);
        else cv::resize(cropped, output, output.size(), 0, 0, cvScaler);
    }

    return;
}

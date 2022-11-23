/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#include "filter/filters/rotate/filter_rotate.h"
#include <opencv2/imgproc/imgproc.hpp>

void filter_rotate_c::apply(u8 *const pixels, const resolution_s &r)
{
    this->assert_input_validity(pixels, r);

    static heap_mem<u8> scratch(MAX_NUM_BYTES_IN_CAPTURED_FRAME, "Rotate filter scratch buffer");

    const double angle = this->parameter(PARAM_ROT);
    const double scale = this->parameter(PARAM_SCALE);
    cv::Mat output = cv::Mat(r.h, r.w, CV_8UC4, pixels);
    cv::Mat temp = cv::Mat(r.h, r.w, CV_8UC4, scratch.data());
    cv::Mat transf = cv::getRotationMatrix2D(cv::Point2d((r.w / 2), (r.h / 2)), -angle, scale);
    
    cv::warpAffine(output, temp, transf, cv::Size(r.w, r.h));
    temp.copyTo(output);

    return;
}

/*
 * 2019 Tarpeeksi Hyvae Soft /
 * VCS
 * 
 * For recording capture output into a video file.
 *
 */

#include "../common/globals.h"
#include "../scaler/scaler.h"
#include "record.h"

#ifdef USE_OPENCV
    #include <opencv2/core/core.hpp>

    // For VideoWriter.
    #if CV_MAJOR_VERSION > 2
        #include <opencv2/videoio/videoio.hpp>
    #else
        #include <opencv2/highgui/highgui.hpp>
    #endif
#endif

static cv::VideoWriter videoWriter;

void krecord_initialize_recording(void)
{
    k_assert(!videoWriter.isOpened(),
             "Attempting to intialize a recording that has already been initialized.");

    #if CV_MAJOR_VERSION > 2
        #define encoder(a,b,c,d) cv::VideoWriter::fourcc((a), (b), (c), (d))
    #else
        #define encoder(a,b,c,d) CV_FOURCC((a), (b), (c), (d))
    #endif

    videoWriter.open("test.avi", encoder('M','J','P','G'), 60, cv::Size(640,480));

    #undef encoder

    k_assert(videoWriter.isOpened(), "Failed to initialize the recording.");

    return;
}

bool krecord_is_recording(void)
{
    return videoWriter.isOpened();
}

void krecord_record_new_frame(void)
{
    k_assert(videoWriter.isOpened(),
             "Attempted to record a video frame before video recording had been initialized.");

    // Get the current output frame.
    const resolution_s r = ks_output_resolution();
    const u8 *const frameData = ks_scaler_output_as_raw_ptr();
    if (frameData == nullptr) return;

    videoWriter << cv::Mat(r.h, r.w, CV_8UC4, (u8*)frameData);

    return;
}

void krecord_finalize_recording(void)
{
    videoWriter.release();

    return;
}

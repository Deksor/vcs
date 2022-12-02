/*
 * 2019 Tarpeeksi Hyvae Soft /
 * VCS
 *
 */

#ifndef VCS_RECORD_RECORD_H
#define VCS_RECORD_RECORD_H

#include <string>
#include "common/globals.h"
#include "common/types.h"
#include "common/propagate/vcs_event.h"

struct image_s;

extern vcs_event_c<void> krecord_evRecordingStarted;
extern vcs_event_c<void> krecord_evRecordingEnded;

bool krecord_start_recording(const char *const filename,
                             const uint width,
                             const uint height,
                             const uint frameRate);

resolution_s krecord_video_resolution(void);

uint krecord_num_frames_recorded(void);

uint krecord_num_frames_dropped(void);

uint krecord_peak_buffer_usage_percent(void);

uint krecord_playback_framerate(void);

i64 krecord_recording_time(void);

double krecord_recording_framerate(void);

std::string krecord_video_filename(void);

bool krecord_is_recording(void);

void krecord_record_frame(const image_s &image);

void krecord_stop_recording(void);

void krecord_initialize(void);

void krecord_release(void);

#endif

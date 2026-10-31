/* Standalone compile check for AudioOnlyMode helpers (no full OBS link). */
#include <cstdint>
#include <cstdio>
#include <cstring>

#define OBS_SOURCE_VIDEO (1u << 0)
#define OBS_SOURCE_AUDIO (1u << 1)
#define OBS_SOURCE_ASYNC (1u << 2)
#define OBS_SOURCE_ASYNC_VIDEO (OBS_SOURCE_ASYNC | OBS_SOURCE_VIDEO)
#define AUDIO_ONLY_MODE_TEST 1

#include "../frontend/utility/AudioOnlyMode.hpp"

static int fails = 0;

static void expect_true(bool v, const char *msg)
{
	if (!v) {
		std::printf("FAIL: %s\n", msg);
		fails++;
	}
}

static void expect_false(bool v, const char *msg)
{
	if (v) {
		std::printf("FAIL: %s\n", msg);
		fails++;
	}
}

int main()
{
	expect_true(kAudioOnlyApp, "kAudioOnlyApp");

	expect_true(AudioOnlySourceTypeAllowed("scene", OBS_SOURCE_VIDEO), "scene");
	expect_true(AudioOnlySourceTypeAllowed("ffmpeg_source", OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO),
		    "ffmpeg_source");
	expect_true(AudioOnlySourceTypeAllowed("coreaudio_input_capture", OBS_SOURCE_AUDIO), "coreaudio input");
	expect_true(AudioOnlySourceTypeAllowed("coreaudio_output_capture", OBS_SOURCE_AUDIO), "coreaudio output");
	expect_true(AudioOnlySourceTypeAllowed("sck_audio_capture", OBS_SOURCE_AUDIO), "sck audio");

	expect_false(AudioOnlySourceTypeAllowed("display_capture", OBS_SOURCE_VIDEO), "display_capture");
	expect_false(AudioOnlySourceTypeAllowed("window_capture", OBS_SOURCE_ASYNC_VIDEO), "window_capture");
	expect_false(AudioOnlySourceTypeAllowed("screen_capture", OBS_SOURCE_VIDEO | OBS_SOURCE_AUDIO),
		     "screen_capture");
	expect_false(AudioOnlySourceTypeAllowed("image_source", OBS_SOURCE_VIDEO), "image_source");
	expect_false(AudioOnlySourceTypeAllowed("av_capture_input", OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO),
		     "av_capture");

	expect_true(AudioOnlyFilterTypeAllowed(OBS_SOURCE_AUDIO), "gain-like filter");
	expect_false(AudioOnlyFilterTypeAllowed(OBS_SOURCE_VIDEO), "color filter");
	expect_false(AudioOnlyFilterTypeAllowed(OBS_SOURCE_ASYNC_VIDEO), "async video filter");

	if (fails) {
		std::printf("%d failure(s)\n", fails);
		return 1;
	}
	std::printf("audio-only-mode-test: ok\n");
	return 0;
}

/******************************************************************************
    Copyright (C) 2026

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#ifdef AUDIO_ONLY_MODE_TEST
/* Test builds supply OBS_SOURCE_* macros before including this header. */
#else
#include <obs.h>
#endif

#include <cstring>

/* Product is an audio processing app: hide video UI, leave libobs video plumbing. */
constexpr bool kAudioOnlyApp = true;

inline bool AudioOnlySourceTypeAllowed(const char *unversioned_id, uint32_t caps)
{
	if (!kAudioOnlyApp) {
		return true;
	}

	if (!unversioned_id) {
		return false;
	}

	/* Scenes remain the container for audio sources. */
	if (strcmp(unversioned_id, "scene") == 0) {
		return true;
	}

	/* Media source is useful for audio files (still carries VIDEO|AUDIO flags). */
	if (strcmp(unversioned_id, "ffmpeg_source") == 0) {
		return true;
	}

	if ((caps & OBS_SOURCE_AUDIO) == 0) {
		return false;
	}

	/* Hide video / A+V capture (display, window, screen, camera, etc.). */
	if ((caps & OBS_SOURCE_VIDEO) != 0) {
		return false;
	}

	return true;
}

inline bool AudioOnlyFilterTypeAllowed(uint32_t filter_caps)
{
	if (!kAudioOnlyApp) {
		return true;
	}

	return (filter_caps & OBS_SOURCE_AUDIO) != 0;
}

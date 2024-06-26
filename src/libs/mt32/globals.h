/* Copyright (C) 2003, 2004, 2005, 2006, 2008, 2009 Dean Beeler, Jerome Fisher
 * Copyright (C) 2011-2022 Dean Beeler, Jerome Fisher, Sergey V. Mikayev
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MT32EMU_GLOBALS_H
#define MT32EMU_GLOBALS_H

#include "config.h"

/* Support for compiling shared library.
 * MT32EMU_SHARED and mt32emu_EXPORTS are defined when building a shared library.
 * MT32EMU_SHARED should also be defined for Windows platforms that provides for a small performance benefit,
 * and it _must_ be defined along with MT32EMU_RUNTIME_VERSION_CHECK when using MSVC.
 */
#ifdef MT32EMU_SHARED
#  if defined _WIN32 || defined __CYGWIN__ || defined __OS2__
#    ifdef _MSC_VER
#      ifdef mt32emu_EXPORTS
#        define MT32EMU_EXPORT_ATTRIBUTE _declspec(dllexport)
#      else /* #ifdef mt32emu_EXPORTS */
#        define MT32EMU_EXPORT_ATTRIBUTE _declspec(dllimport)
#      endif /* #ifdef mt32emu_EXPORTS */
#    else /* #ifdef _MSC_VER */
#      ifdef mt32emu_EXPORTS
#        define MT32EMU_EXPORT_ATTRIBUTE __attribute__ ((dllexport))
#      else /* #ifdef mt32emu_EXPORTS */
#        define MT32EMU_EXPORT_ATTRIBUTE __attribute__ ((dllimport))
#      endif /* #ifdef mt32emu_EXPORTS */
#    endif /* #ifdef _MSC_VER */
#  else /* #if defined _WIN32 || defined __CYGWIN__ || defined __OS2__ */
#    ifdef mt32emu_EXPORTS
#      define MT32EMU_EXPORT_ATTRIBUTE __attribute__ ((visibility("default")))
#    else /* #ifdef mt32emu_EXPORTS */
#      define MT32EMU_EXPORT_ATTRIBUTE
#    endif /* #ifdef mt32emu_EXPORTS */
#  endif /* #if defined _WIN32 || defined __CYGWIN__ || defined __OS2__ */
#else /* #ifdef MT32EMU_SHARED */
#  define MT32EMU_EXPORT_ATTRIBUTE
#endif /* #ifdef MT32EMU_SHARED */

#if MT32EMU_EXPORTS_TYPE == 1 || MT32EMU_EXPORTS_TYPE == 2
#define MT32EMU_EXPORT
#else
#define MT32EMU_EXPORT MT32EMU_EXPORT_ATTRIBUTE
#endif

/* Facilitates easier tracking of the library version when an external symbol was introduced.
 * Particularly useful for shared library builds on POSIX systems that support symbol versioning,
 * so that the version map file can be generated automatically.
 */
#define MT32EMU_EXPORT_V(symbol_version_tag) MT32EMU_EXPORT

/* Helpers for compile-time version checks */

#define MT32EMU_VERSION_MAJOR 2
#define MT32EMU_VERSION_MINOR 7
#define MT32EMU_VERSION_PATCH 0
#define MT32EMU_VERSION "2.7.0"

/* Encodes the given version components to a single integer value to simplify further checks. */
#define MT32EMU_VERSION_INT(major, minor, patch) ((major << 16) | (minor << 8) | patch)

/* The version of this library build, as an integer. */
#define MT32EMU_CURRENT_VERSION_INT MT32EMU_VERSION_INT(MT32EMU_VERSION_MAJOR, MT32EMU_VERSION_MINOR, MT32EMU_VERSION_PATCH)

/* Compares the current library version with the given version components. Intended for feature checks. */
#define MT32EMU_VERSION_ATLEAST(major, minor, patch) (MT32EMU_CURRENT_VERSION_INT >= MT32EMU_VERSION_INT(major, minor, patch))

/* Implements a simple version check that ensures full API compatibility of this library build
 * with the application requirements. The latter can be derived from the versions of used public symbols.
 *
 * Note: This macro is intended for a quick compile-time check. To ensure compatibility of an application
 * linked with a shared library, an automatic version check can be engaged with help of the build option
 * libmt32emu_WITH_VERSION_TAGGING. For a fine-grained feature checking in run-time, see functions
 * mt32emu_get_library_version_int and Synth::getLibraryVersionInt.
 */
#define MT32EMU_IS_COMPATIBLE(major, minor) (MT32EMU_VERSION_MAJOR == major && MT32EMU_VERSION_MINOR >= minor)

/* Useful constants */

/* Sample rate to use in mixing. With the progress of development, we've found way too many thing dependent.
 * In order to achieve further advance in emulation accuracy, sample rate made fixed throughout the emulator,
 * except the emulation of analogue path.
 * The output from the synth is supposed to be resampled externally in order to convert to the desired sample rate.
 */
#define MT32EMU_SAMPLE_RATE 32000

/* The default value for the maximum number of partials playing simultaneously. */
#define MT32EMU_DEFAULT_MAX_PARTIALS 32

/* The higher this number, the more memory will be used, but the more samples can be processed in one run -
 * various parts of sample generation can be processed more efficiently in a single run.
 * A run's maximum length is that given to Synth::render(), so giving a value here higher than render() is ever
 * called with will give no gain (but simply waste the memory).
 * Note that this value does *not* in any way impose limitations on the length given to render(), and has no effect
 * on the generated audio.
 * This value must be >= 1.
 */
#define MT32EMU_MAX_SAMPLES_PER_RUN 4096

/* The default size of the internal MIDI event queue.
 * It holds the incoming MIDI events before the rendering engine actually processes them.
 * The main goal is to fairly emulate the real hardware behaviour which obviously
 * uses an internal MIDI event queue to gather incoming data as well as the delays
 * introduced by transferring data via the MIDI interface.
 * This also facilitates building of an external rendering loop
 * as the queue stores timestamped MIDI events.
 */
#define MT32EMU_DEFAULT_MIDI_EVENT_QUEUE_SIZE 1024

/* Maximum allowed size of MIDI parser input stream buffer.
 * Should suffice for any reasonable bulk dump SysEx, as the h/w units have only 32K of RAM onboard.
 */
#define MT32EMU_MAX_STREAM_BUFFER_SIZE 32768

/* This should correspond to the MIDI buffer size used in real h/w devices.
 * CM-32L control ROM is using 1000 bytes, and MT-32 GEN0 is using only 240 bytes (semi-confirmed by now).
 */
#define MT32EMU_SYSEX_BUFFER_SIZE 1000

#if defined(__cplusplus) && MT32EMU_API_TYPE != 1

namespace MT32Emu
{
const unsigned int SAMPLE_RATE = MT32EMU_SAMPLE_RATE;
#undef MT32EMU_SAMPLE_RATE

const unsigned int DEFAULT_MAX_PARTIALS = MT32EMU_DEFAULT_MAX_PARTIALS;
#undef MT32EMU_DEFAULT_MAX_PARTIALS

const unsigned int MAX_SAMPLES_PER_RUN = MT32EMU_MAX_SAMPLES_PER_RUN;
#undef MT32EMU_MAX_SAMPLES_PER_RUN

const unsigned int DEFAULT_MIDI_EVENT_QUEUE_SIZE = MT32EMU_DEFAULT_MIDI_EVENT_QUEUE_SIZE;
#undef MT32EMU_DEFAULT_MIDI_EVENT_QUEUE_SIZE

const unsigned int MAX_STREAM_BUFFER_SIZE = MT32EMU_MAX_STREAM_BUFFER_SIZE;
#undef MT32EMU_MAX_STREAM_BUFFER_SIZE

const unsigned int SYSEX_BUFFER_SIZE = MT32EMU_SYSEX_BUFFER_SIZE;
#undef MT32EMU_SYSEX_BUFFER_SIZE
}

#endif /* #if defined(__cplusplus) && MT32EMU_API_TYPE != 1 */

#endif /* #ifndef MT32EMU_GLOBALS_H */

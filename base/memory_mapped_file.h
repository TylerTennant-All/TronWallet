// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_MEMORY_MAPPED_FILE_H_
#define BASE_FILES_MEMORY_MAPPED_FILE_H_

#include <stddef.h>
#include <stdint.h>

#include "base/macros.h"
#include "base/build_config.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

namespace base {

class MemoryMappedFile {
 public:
  enum Access {
    // Mapping a file into memory effectively allows for file I/O on any thread.
    // The accessing thread could be paused while data from the file is paged
    // into memory. Worse, a corrupted filesystem could cause a SEGV within the
    // program instead of just an I/O error.
    READ_ONLY,

    // This provides read/write access to a file and must be used with care of
    // the additional subtleties involved in doing so. Though the OS will do
    // the writing of data on its own time, too many dirty pages can cause
    // the OS to pause the thread while it writes them out. The pause can
    // be as much as 1s on some systems.
    READ_WRITE,

    // This provides read/write access but with the ability to write beyond
    // the end of the existing file up to a maximum size specified as the
    // "region". Depending on the OS, the file may or may not be immediately
    // extended to the maximum size though it won't be loaded in RAM until
    // needed. Note, however, that the maximum size will still be reserved
    // in the process address space.
    READ_WRITE_EXTEND,
  };

  // The default constructor sets all members to invalid/null values.
  MemoryMappedFile();
  ~MemoryMappedFile();

  // Used to hold information about a region [offset + size] of a file.
  struct Region {
    static const Region kWholeFile;

    bool operator==(const Region& other) const;
    bool operator!=(const Region& other) const;

    // Start of the region (measured in bytes from the beginning of the file).
    int64_t offset;

    // Length of the region in bytes.
    int64_t size;
  };

  // Opens an existing file and maps it into memory. |access| can be read-only
  // or read/write but not read/write+extend. If this object already points
  // to a valid memory mapped file then this method will fail and return
  // false. If it cannot open the file, the file does not exist, or the
  // memory mapping fails, it will return false.
  bool Initialize(const PathString& file_name, Access access) {
    return Initialize(file_name, Region::kWholeFile, access);
  }
  bool Initialize(const PathString& file_name) {
    return Initialize(file_name, READ_ONLY);
  }

  // As above, but works with a region of an already-opened file. All forms of
  // |access| are allowed. If READ_WRITE_EXTEND is specified then |region|
  // provides the maximum size of the file. If the memory mapping fails, it
  // return false.
  bool Initialize(const PathString& file_name, const Region& region, Access access);
  bool Initialize(const PathString& file_name, const Region& region) {
    return Initialize(file_name, region, READ_ONLY);
  }

  const uint8_t* data() const { return data_; }
  uint8_t* data() { return data_; }
  size_t length() const { return length_; }

  // Is file_ a valid file handle that points to an open, memory mapped file?
  bool IsValid() const;

 private:
  // Given the arbitrarily aligned memory region [start, size], returns the
  // boundaries of the region aligned to the granularity specified by the OS,
  // (a page on Linux, ~32k on Windows) as follows:
  // - |aligned_start| is page aligned and <= |start|.
  // - |aligned_size| is a multiple of the VM granularity and >= |size|.
  // - |offset| is the displacement of |start| w.r.t |aligned_start|.
  static void CalculateVMAlignedBoundaries(int64_t start,
                                           int64_t size,
                                           int64_t* aligned_start,
                                           int64_t* aligned_size,
                                           int32_t* offset);

  // Map the file to memory, set data_ to that memory address. Return true on
  // success, false on any kind of failure. This is a helper for Initialize().
  bool MapFileRegionToMemory(const Region& region, Access access);

  // Closes all open handles.
  void CloseHandles();

  PathString file_path_;
  uint8_t* data_;
  size_t length_;

#if defined(OS_WIN)
  win::ScopedHandle file_;
  win::ScopedHandle file_mapping_;
#elif defined(OS_POSIX)
  int file_;
#endif

  DISALLOW_COPY_AND_ASSIGN(MemoryMappedFile);
};

}  // namespace base

#endif  // BASE_FILES_MEMORY_MAPPED_FILE_H_

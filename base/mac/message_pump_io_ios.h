// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_IO_IOS_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_IO_IOS_H_

#include <memory>

#include "base/location.h"
#include "base/mac/scoped_cffiledescriptorref.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/macros.h"
#include "base/ref_counted.h"
#include "base/mac/message_pump_mac.h"

namespace base {

// This file introduces a class to monitor sockets and issue callbacks when
// sockets are ready for I/O on iOS.
class MessagePumpIOSForIO : public MessagePumpNSRunLoop,
                            public std::enable_shared_from_this<MessagePumpIOSForIO> {
 public:
  // Used with WatchFileDescriptor to asynchronously monitor the I/O readiness
  // of a file descriptor.
  class Watcher {
   public:
    // Called from MessageLoop::Run when an FD can be read from/written to
    // without blocking
    virtual void OnFileCanReadWithoutBlocking(int fd) = 0;
    virtual void OnFileCanWriteWithoutBlocking(int fd) = 0;

   protected:
    virtual ~Watcher() {}
  };

  // Object returned by WatchFileDescriptor to manage further watching.
  class FileDescriptorWatcher {
   public:
    explicit FileDescriptorWatcher(const Location& from_here);
    ~FileDescriptorWatcher();  // Implicitly calls StopWatchingFileDescriptor.

    // NOTE: These methods aren't called StartWatching()/StopWatching() to
    // avoid confusion with the win32 ObjectWatcher class.

    // Stop watching the FD, always safe to call.  No-op if there's nothing
    // to do.
    bool StopWatchingFileDescriptor();

    const Location& created_from_location() { return created_from_location_; }

   private:
    friend class MessagePumpIOSForIO;
    friend class MessagePumpIOSForIOTest;

    // Called by MessagePumpIOSForIO, ownership of |fdref| and |fd_source|
    // is transferred to this object.
    void Init(CFFileDescriptorRef fdref,
              CFOptionFlags callback_types,
              CFRunLoopSourceRef fd_source,
              bool is_persistent);

    void set_pump(std::weak_ptr<MessagePumpIOSForIO> pump) { pump_ = pump; }
    const std::weak_ptr<MessagePumpIOSForIO>& pump() const { return pump_; }

    void set_watcher(Watcher* watcher) { watcher_ = watcher; }

    void OnFileCanReadWithoutBlocking(int fd, MessagePumpIOSForIO* pump);
    void OnFileCanWriteWithoutBlocking(int fd, MessagePumpIOSForIO* pump);

    bool is_persistent_;  // false if this event is one-shot.
    base::mac::ScopedCFFileDescriptorRef fdref_;
    CFOptionFlags callback_types_;
    base::ScopedCFTypeRef<CFRunLoopSourceRef> fd_source_;
    std::weak_ptr<MessagePumpIOSForIO> pump_;
    Watcher* watcher_;

    Location created_from_location_;

    DISALLOW_COPY_AND_ASSIGN(FileDescriptorWatcher);
  };

  enum Mode {
    WATCH_READ = 1 << 0,
    WATCH_WRITE = 1 << 1,
    WATCH_READ_WRITE = WATCH_READ | WATCH_WRITE
  };

  MessagePumpIOSForIO();
  ~MessagePumpIOSForIO() override;

  // Have the current thread's message loop watch for a a situation in which
  // reading/writing to the FD can be performed without blocking.
  // Callers must provide a preallocated FileDescriptorWatcher object which
  // can later be used to manage the lifetime of this event.
  // If a FileDescriptorWatcher is passed in which is already attached to
  // an event, then the effect is cumulative i.e. after the call |controller|
  // will watch both the previous event and the new one.
  // If an error occurs while calling this method in a cumulative fashion, the
  // event previously attached to |controller| is aborted.
  // Returns true on success.
  // Must be called on the same thread the message_pump is running on.
  bool WatchFileDescriptor(int fd,
                           bool persistent,
                           int mode,
                           FileDescriptorWatcher *controller,
                           Watcher *delegate);

  void RemoveRunLoopSource(CFRunLoopSourceRef source);

 private:
  friend class MessagePumpIOSForIOTest;

  static void HandleFdIOEvent(CFFileDescriptorRef fdref,
                              CFOptionFlags callback_types,
                              void* context);

  //ThreadChecker watch_file_descriptor_caller_checker_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpIOSForIO);
};

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_MESSAGE_PUMP_IO_IOS_H_

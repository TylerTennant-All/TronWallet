// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_RUN_LOOP_H_
#define BASE_RUN_LOOP_H_

#include <stack>
#include <vector>
#include <memory>

#include "base/callback_forward.h"
#include "base/logging.h"
//#include "base/containers/stack.h"
#include "base/macros.h"
#include "base/ref_counted.h"
//#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
//#include "base/sequence_checker.h"
//#include "base/threading/thread_checker.h"
#include "base/build_config.h"

namespace base {
#if defined(OS_ANDROID)
class MessagePumpForUI;
#endif

#if defined(OS_IOS)
class MessagePumpUIApplication;
#endif

class SingleThreadTaskRunner;

// Helper class to run the RunLoop::Delegate associated with the current thread.
// A RunLoop::Delegate must have been bound to this thread (ref.
// RunLoop::RegisterDelegateForCurrentThread()) prior to using any of RunLoop's
// member and static methods unless explicitly indicated otherwise (e.g.
// IsRunning/IsNestedOnCurrentThread()). RunLoop::Run can only be called once
// per RunLoop lifetime. Create a RunLoop on the stack and call Run/Quit to run
// a nested RunLoop but please do not use nested loops in production code!
class RunLoop : std::enable_shared_from_this<RunLoop> {
 public:
  // The type of RunLoop: a kDefault RunLoop at the top-level (non-nested) will
  // process system and application tasks assigned to its Delegate. When nested
  // however a kDefault RunLoop will only process system tasks while a
  // kNestableTasksAllowed RunLoop will continue to process application tasks
  // even if nested.
  //
  // This is relevant in the case of recursive RunLoops. Some unwanted run loops
  // may occur when using common controls or printer functions. By default,
  // recursive task processing is disabled.
  //
  // In general, nestable RunLoops are to be avoided. They are dangerous and
  // difficult to get right, so please use with extreme caution. To further
  // protect this: kNestableTasksAllowed RunLoops are only allowed on threads
  // where IsNestingAllowedOnCurrentThread().
  //
  // A specific example where this makes a difference is:
  // - The thread is running a RunLoop.
  // - It receives a task #1 and executes it.
  // - The task #1 implicitly starts a RunLoop, like a MessageBox in the unit
  //   test. This can also be StartDoc or GetSaveFileName.
  // - The thread receives a task #2 before or while in this second RunLoop.
  // - With a kNestableTasksAllowed RunLoop, the task #2 will run right away.
  //   Otherwise, it will get executed right after task #1 completes in the main
  //   RunLoop.
  enum class Type {
    kDefault,
    kNestableTasksAllowed,
  };

  RunLoop(Type type = Type::kDefault);
  ~RunLoop();

  // Run the current RunLoop::Delegate. This blocks until Quit is called. Before
  // calling Run, be sure to grab the QuitClosure in order to stop the
  // RunLoop::Delegate asynchronously.
  void Run();

  // Run the current RunLoop::Delegate until it doesn't find any tasks or
  // messages in its queue (it goes idle). WARNING: This may never return! Only
  // use this when repeating tasks such as animated web pages have been shut
  // down.
  void RunUntilIdle();

  bool running() const {
    //DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
    return running_;
  }

  // Quit() quits an earlier call to Run() immediately. QuitWhenIdle() quits an
  // earlier call to Run() when there aren't any tasks or messages in the queue.
  //
  // These methods are thread-safe but note that Quit() is best-effort when
  // called from another thread (will quit soon but tasks that were already
  // queued on this RunLoop will get to run first).
  //
  // There can be other nested RunLoops servicing the same task queue. Quitting
  // one RunLoop has no bearing on the others. Quit() and QuitWhenIdle() can be
  // called before, during or after Run(). If called before Run(), Run() will
  // return immediately when called. Calling Quit() or QuitWhenIdle() after the
  // RunLoop has already finished running has no effect.
  //
  // WARNING: You must NEVER assume that a call to Quit() or QuitWhenIdle() will
  // terminate the targetted message loop. If a nested RunLoop continues
  // running, the target may NEVER terminate. It is very easy to livelock (run
  // forever) in such a case.
  void Quit();
  void QuitWhenIdle();

  // Convenience methods to get a closure that safely calls Quit() or
  // QuitWhenIdle() (has no effect if the RunLoop instance is gone).
  //
  // The resulting Closure is thread-safe (note however that invoking the
  // QuitClosure() from another thread than this RunLoop's will result in an
  // asynchronous rather than immediate Quit()).
  //
  // Example:
  //   RunLoop run_loop;
  //   PostTask(run_loop.QuitClosure());
  //   run_loop.Run();
  base::Closure QuitClosure();
  base::Closure QuitWhenIdleClosure();

  // Returns true if there is an active RunLoop on this thread.
  // Safe to call before RegisterDelegateForCurrentThread().
  static bool IsRunningOnCurrentThread();

  // Returns true if there is an active RunLoop on this thread and it's nested
  // within another active RunLoop.
  // Safe to call before RegisterDelegateForCurrentThread().
  static bool IsNestedOnCurrentThread();

  // A NestingObserver is notified when a nested RunLoop begins. The observers
  // are notified before the current thread's RunLoop::Delegate::Run() is
  // invoked and nested work begins.
  class NestingObserver {
   public:
    virtual void OnBeginNestedRunLoop() = 0;

   protected:
    virtual ~NestingObserver() = default;
  };

  static void AddNestingObserverOnCurrentThread(NestingObserver* observer);
  static void RemoveNestingObserverOnCurrentThread(NestingObserver* observer);

  // Returns true if nesting is allowed on this thread.
  static bool IsNestingAllowedOnCurrentThread();

  // Disallow nesting. After this is called, running a nested RunLoop or calling
  // Add/RemoveNestingObserverOnCurrentThread() on this thread will crash.
  static void DisallowNestingOnCurrentThread();

  // A RunLoop::Delegate is a generic interface that allows RunLoop to be
  // separate from the uderlying implementation of the message loop for this
  // thread. It holds private state used by RunLoops on its associated thread.
  // One and only one RunLoop::Delegate must be registered on a given thread
  // via RunLoop::RegisterDelegateForCurrentThread() before RunLoop instances
  // and RunLoop static methods can be used on it.
  class Delegate {
   protected:
    Delegate();
    ~Delegate();

    // The client interface provided back to the caller who registers this
    // Delegate via RegisterDelegateForCurrentThread.
    class Client {
     public:
      // Returns true if the Delegate should return from the topmost Run() when
      // it becomes idle. The Delegate is responsible for probing this when it
      // becomes idle.
      bool ShouldQuitWhenIdle() const;

      // Returns true if this |outer_| is currently in nested runs. This is a
      // shortcut for RunLoop::IsNestedOnCurrentThread() for the owner of this
      // interface.
      bool IsNested() const;

      // Returns true if the Delegate is allowed to process application tasks.
      // This typically returns true except in nested RunLoops outside the scope
      // of a ScopedNestableTaskAllowed as, by default, nested RunLoops are only
      // meant to process system events.
      bool ProcessingTasksAllowed() const;

     private:
      // Only a Delegate can instantiate a Delegate::Client.
      friend class Delegate;
      Client(Delegate* outer);

      Delegate* outer_;
    };

   private:
    // While the state is owned by the Delegate subclass, only RunLoop can use
    // it.
    friend class RunLoop;

    // Used by RunLoop to inform its Delegate to Run/Quit. Implementations are
    // expected to keep on running synchronously from the Run() call until the
    // eventual matching Quit() call. Upon receiving a Quit() call it should
    // return from the Run() call as soon as possible without executing
    // remaining tasks/messages. Run() calls can nest in which case each Quit()
    // call should result in the topmost active Run() call returning. The only
    // other trigger for Run() to return is Client::ShouldQuitWhenIdle() which
    // the Delegate should probe before sleeping when it becomes idle. Run()
    // implementations should also check Client::ProcessingTasksAllowed() before
    // processing assigned application tasks (they should only process system
    // tasks otherwise).
    virtual void Run() = 0;
    virtual void Quit() = 0;

    // Invoked right before a RunLoop enters a nested Run() call on this
    // Delegate iff this RunLoop is of type kNestableTasksAllowed. The Delegate
    // should ensure that the upcoming Run() call will result in processing
    // application tasks queued ahead of it without further probing. e.g.
    // message pumps on some platforms, like Mac, need an explicit request to
    // process application tasks when nested, otherwise they'll only wait for
    // system messages.
    virtual void EnsureWorkScheduled() = 0;

    // A vector-based stack is more memory efficient than the default
    // deque-based stack as the active RunLoop stack isn't expected to ever
    // have more than a few entries.
    using RunLoopStack = std::stack<RunLoop*, std::vector<RunLoop*>>;

    bool allow_nesting_ = true;
    RunLoopStack active_run_loops_;
    ObserverList<RunLoop::NestingObserver> nesting_observers_;

#if DCHECK_IS_ON()
    bool allow_running_for_testing_ = true;
#endif

    // True once this Delegate is bound to a thread via
    // RegisterDelegateForCurrentThread().
    bool bound_ = false;

    // Thread-affine per its use of TLS.
    //THREAD_CHECKER(bound_thread_checker_);

    Client client_interface_ = Client(this);

    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  // Registers |delegate| on the current thread. Must be called once and only
  // once per thread before using RunLoop methods on it. |delegate| is from then
  // on forever bound to that thread (including its destruction). The returned
  // Delegate::Client is valid as long as |delegate| is kept alive.
  static Delegate::Client* RegisterDelegateForCurrentThread(Delegate* delegate);

  // Quits the active RunLoop (when idle) -- there must be one. These were
  // introduced as prefered temporary replacements to the long deprecated
  // MessageLoop::Quit(WhenIdle) methods. Callers should properly plumb a
  // reference to the appropriate RunLoop instance (or its QuitClosure) instead
  // of using these in order to link Run()/Quit() to a single RunLoop instance
  // and increase readability.
  static void QuitCurrentDeprecated();
  static void QuitCurrentWhenIdleDeprecated();

  // Run() will DCHECK if called while there's a ScopedDisallowRunningForTesting
  // in scope on its thread. This is useful to add safety to some test
  // constructs which allow multiple task runners to share the main thread in
  // unit tests. While the main thread can be shared by multiple runners to
  // deterministically fake multi threading, there can still only be a single
  // RunLoop::Delegate per thread and RunLoop::Run() should only be invoked from
  // it (or it would result in incorrectly driving TaskRunner A while in
  // TaskRunner B's context).
  class ScopedDisallowRunningForTesting {
   public:
    ScopedDisallowRunningForTesting();
    ~ScopedDisallowRunningForTesting();

   private:
#if DCHECK_IS_ON()
    Delegate* current_delegate_;
    const bool previous_run_allowance_;
#endif  // DCHECK_IS_ON()

    DISALLOW_COPY_AND_ASSIGN(ScopedDisallowRunningForTesting);
  };

 private:
#if defined(OS_ANDROID)
  // Android doesn't support the blocking RunLoop::Run, so it calls
  // BeforeRun and AfterRun directly.
  friend class base::MessagePumpForUI;
#endif

#if defined(OS_IOS)
  // iOS doesn't support the blocking RunLoop::Run, so it calls
  // BeforeRun directly.
  friend class base::MessagePumpUIApplication;
#endif

  // Return false to abort the Run.
  bool BeforeRun();
  void AfterRun();

  // A copy of RunLoop::Delegate for the thread driven by tis RunLoop for quick
  // access without using TLS (also allows access to state from another sequence
  // during Run(), ref. |sequence_checker_| below).
  Delegate* delegate_;

  const Type type_;

#if DCHECK_IS_ON()
  bool run_called_ = false;
#endif

  bool quit_called_ = false;
  bool running_ = false;
  // Used to record that QuitWhenIdle() was called on this RunLoop, meaning that
  // the Delegate should quit Run() once it becomes idle (it's responsible for
  // probing this state via Client::ShouldQuitWhenIdle()). This state is stored
  // here rather than pushed to Delegate via, e.g., Delegate::QuitWhenIdle() to
  // support nested RunLoops.
  bool quit_when_idle_received_ = false;

  // RunLoop is not thread-safe. Its state/methods, unless marked as such, may
  // not be accessed from any other sequence than the thread it was constructed
  // on. Exception: RunLoop can be safely accessed from one other sequence (or
  // single parallel task) during Run() -- e.g. to Quit() without having to
  // plumb ThreatTaskRunnerHandle::Get() throughout a test to repost QuitClosure
  // to origin thread.
  //SEQUENCE_CHECKER(sequence_checker_);

  const scoped_refptr<SingleThreadTaskRunner> origin_task_runner_;

  // WeakPtrFactory for QuitClosure safety.
  //base::WeakPtrFactory<RunLoop> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(RunLoop);
};

}  // namespace base

#endif  // BASE_RUN_LOOP_H_
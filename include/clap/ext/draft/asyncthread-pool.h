#pragma once

#include "../../plugin.h"

/// @page
///
/// This extension lets the plugin use the host's thread pool. It differs
/// from the non-draft extension, thread-pool, by supporting asynchronous
/// servicing outside of the context of the audio thread.  Host may elect
/// to perform tasks on the main thread or in its own worker threads.
//
/// It's possible that a better way to expose this capability is by 
/// versioning ext/thread-pool.h with new entrypoints.
///
/// The plugin must provide @ref clap_plugin_asyncthread_pool, and the host 
/// may provide @ref clap_host_asyncthread_pool. If it doesn't, the plugin 
/// must organize processing its data by its own means.
///
/// Tasks are referred to by their unique void *, owned and managed by plugin.
///
/// When plugin completes a task in another thread it must communicate
/// the task completion state back to its processing state in a threadsafe 
/// fashion.
///
static CLAP_CONSTEXPR const char CLAP_EXT_ASYNCTHREAD_POOL[] = "clap.asyncthread-pool";

#ifdef __cplusplus
extern "C" {
#endif

typedef struct clap_plugin_asyncthread_pool {
   // This will be invoked in a non-audio thread to perform the task uniquely
   // identifeid by void * task. plugin is responsible for safely communicating
   // the results of task to its audio thread.
   // [worker-thread] | [main-thread]
   void(CLAP_ABI *exec)(const clap_plugin_t *plugin, void *task);
} clap_plugin_asyncthread_pool_t;

typedef struct clap_host_asyncthread_pool {
   // Request scheduling of task for asynchronous execution in the
   // host thread pool.  plugin exec entrypoint will be invoked from another thread 
   // at an unspecified time in the future. This interface is an alternative to
   // host->request_callback and clap_host_thread_pool for tasks whose completion 
   // time variability is acceptable. returns true of task is successfully 
   // scheduled.
   // [audio-thread] | [main-thread]
   bool(CLAP_ABI *request_exec)(const clap_host_t *host, void *task);
} clap_host_asyncthread_pool_t;

#ifdef __cplusplus
}
#endif

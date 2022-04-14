/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestSuiteTestsuitesValidation0
 */

/*
 * Copyright (C) 2020 embedded brains GmbH (http://www.embedded-brains.de)
 *               2022 Trinity College Dublin (www.tcd.ie)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This file is part of the RTEMS quality process and was automatically
 * generated.  If you find something that needs to be fixed or
 * worded better please post a report or patch to an RTEMS mailing list
 * or raise a bug report:
 *
 * https://www.rtems.org/bugs.html
 *
 * For information on updating and regenerating please refer to the How-To
 * section in the Software Requirements Engineering chapter of the
 * RTEMS Software Engineering manual.  The manual is provided as a part of
 * a release.  For development sources please refer to the online
 * documentation at:
 *
 * https://docs.rtems.org
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

/**
 * @defgroup RTEMSTestSuiteTestsuitesValidation0 spec:/testsuites/validation-0
 *
 * @ingroup RTEMSTestSuites
 *
 * @brief This general purpose validation test suite provides enough resources
 *   to run basic tests for all specified managers and functions.
 *
 * In SMP configurations, up to three scheduler instances using the SMP EDF
 * scheduler are provided using up to four processors.
 *
 * @{
 */

const char rtems_test_name[] = "BarrierModel0";

#define CONFIGURE_MAXIMUM_PROCESSORS 5

// This is a copy of "ts-default.h" file with a few adjusted parameters,
// such as max number of barriers.

#include <rtems.h>
#include <rtems/bspIo.h>
#include <rtems/chain.h>
#include <rtems/test-info.h>
#include <rtems/testopts.h>

#define MAX_TLS_SIZE RTEMS_ALIGN_UP( 64, RTEMS_TASK_STORAGE_ALIGNMENT )

#define MAX_TASKS 32

#define TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define TASK_STORAGE_SIZE \
  RTEMS_TASK_STORAGE_SIZE( \
    MAX_TLS_SIZE + RTEMS_MINIMUM_STACK_SIZE + \
      CPU_STACK_ALIGNMENT - CPU_HEAP_ALIGNMENT, \
    TASK_ATTRIBUTES \
  )

static char buffer[ 512 ];

static const T_action actions[] = {
  T_report_hash_sha256,
  T_memory_action,
  T_check_task_context,
  T_check_rtems_barriers,
  T_check_rtems_extensions,
  T_check_rtems_message_queues,
  T_check_rtems_partitions,
  T_check_rtems_periods,
  T_check_rtems_semaphores,
  T_check_rtems_tasks,
  T_check_rtems_timers
};

static const T_config test_config = {
  .name = rtems_test_name,
  .buf = buffer,
  .buf_size = sizeof( buffer ),
  .putchar = rtems_put_char,
  .verbosity = RTEMS_TEST_VERBOSITY,
  .now = T_now_clock,
  .allocate = T_memory_allocate,
  .deallocate = T_memory_deallocate,
  .action_count = T_ARRAY_SIZE( actions ),
  .actions = actions
};

static rtems_chain_control free_task_storage =
  RTEMS_CHAIN_INITIALIZER_EMPTY( free_task_storage );

static union {
  RTEMS_ALIGNED( RTEMS_TASK_STORAGE_ALIGNMENT ) char
    storage[ TASK_STORAGE_SIZE ];
  rtems_chain_node node;
} task_storage[ MAX_TASKS ];

static void Init( rtems_task_argument arg )
{
  int exit_code;

  (void) arg;

  rtems_chain_initialize(
    &free_task_storage,
    task_storage,
    RTEMS_ARRAY_SIZE( task_storage ),
    sizeof( task_storage[ 0 ] )
  );

  rtems_test_begin( rtems_test_name, TEST_STATE );
  T_register();
  exit_code = T_main( &test_config );

  if ( exit_code == 0 ) {
    rtems_test_end( rtems_test_name );
  }

  rtems_fatal( RTEMS_FATAL_SOURCE_EXIT, (uint32_t) exit_code );
}

static void *task_stack_allocate( size_t size )
{
  if ( size > sizeof( task_storage[ 0 ] ) ) {
    return NULL;
  }

  return rtems_chain_get_unprotected( &free_task_storage );
}

static void task_stack_deallocate( void *stack )
{
  rtems_chain_append_unprotected(
    &free_task_storage,
    (rtems_chain_node *) stack
  );
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_BARRIERS 1

#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 3

#define CONFIGURE_MAXIMUM_PARTITIONS 3

#define CONFIGURE_MAXIMUM_PERIODS 3

#define CONFIGURE_MAXIMUM_SEMAPHORES 3

#define CONFIGURE_MAXIMUM_TASKS ( 1 + MAX_TASKS )

#define CONFIGURE_MINIMUM_TASKS_WITH_USER_PROVIDED_STORAGE \
  CONFIGURE_MAXIMUM_TASKS

#define CONFIGURE_MAXIMUM_TIMERS 3

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 3

#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 0

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM

#define CONFIGURE_MAXIMUM_THREAD_LOCAL_STORAGE_SIZE MAX_TLS_SIZE

#define CONFIGURE_TASK_STACK_ALLOCATOR_AVOIDS_WORK_SPACE

#define CONFIGURE_TASK_STACK_ALLOCATOR task_stack_allocate

#define CONFIGURE_TASK_STACK_DEALLOCATOR task_stack_deallocate

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_ATTRIBUTES TASK_ATTRIBUTES

#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES

#define CONFIGURE_INIT_TASK_CONSTRUCT_STORAGE_SIZE TASK_STORAGE_SIZE

#if defined( RTEMS_SMP ) && \
  ( CONFIGURE_MAXIMUM_PROCESSORS == 4 || CONFIGURE_MAXIMUM_PROCESSORS == 5 )

#define CONFIGURE_SCHEDULER_EDF_SMP

#include <rtems/scheduler.h>

RTEMS_SCHEDULER_EDF_SMP(a);

RTEMS_SCHEDULER_EDF_SMP(b);

RTEMS_SCHEDULER_EDF_SMP(c);

#define CONFIGURE_SCHEDULER_TABLE_ENTRIES \
  RTEMS_SCHEDULER_TABLE_EDF_SMP(a, rtems_build_name('A', ' ', ' ', ' ')), \
  RTEMS_SCHEDULER_TABLE_EDF_SMP(b, rtems_build_name('B', ' ', ' ', ' ')), \
  RTEMS_SCHEDULER_TABLE_EDF_SMP(c, rtems_build_name('C', ' ', ' ', ' '))

#if CONFIGURE_MAXIMUM_PROCESSORS == 5
#define CONFIGURE_SCHEDULER_ASSIGNMENTS \
  RTEMS_SCHEDULER_ASSIGN(0, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_MANDATORY), \
  RTEMS_SCHEDULER_ASSIGN(1, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL), \
  RTEMS_SCHEDULER_ASSIGN(2, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL), \
  RTEMS_SCHEDULER_ASSIGN(2, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL), \
  RTEMS_SCHEDULER_ASSIGN_NO_SCHEDULER
#else
#define CONFIGURE_SCHEDULER_ASSIGNMENTS \
  RTEMS_SCHEDULER_ASSIGN(0, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_MANDATORY), \
  RTEMS_SCHEDULER_ASSIGN(1, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL), \
  RTEMS_SCHEDULER_ASSIGN(2, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL), \
  RTEMS_SCHEDULER_ASSIGN(2, RTEMS_SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL)
#endif

#endif /* RTEMS_SMP */

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/** @} */

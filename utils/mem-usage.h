/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef MEM_USAGE_H
#define MEM_USAGE_H

#ifdef __linux__
#include <proc/readproc.h>
#include <unistd.h>
// #include <sys/resource.h>
#endif
#ifdef __APPLE__
#include <mach/task.h>
#include <mach/mach_traps.h>
#include <mach/mach.h>
#include <unistd.h>
#include <err.h>
#include <sys/param.h>
#include <mach-o/ldsyms.h>
#endif

/**
 * @brief Utility class to evaluate current usage of RAM
 */
class MemUsage
{
public:
  /**
   * @brief Get memory utilization in bytes
   */
  static inline
  int64_t
  Get ()
  {
#ifdef __linux__
    struct proc_t usage;
    look_up_our_self(&usage);
    return usage.rss * getpagesize ();
    //  struct rusage usage;
    //
    //  int ret = getrusage (RUSAGE_SELF, &usage);
    //  if (ret < 0)
    //    {
    //      os << "NA";
    //      return os;
    //    }
    //
    //  os << (usage.ru_ixrss + usage.ru_idrss + usage.ru_isrss);
    //  return os;
#endif
#ifdef __APPLE__
    task_t task = MACH_PORT_NULL;
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info (mach_task_self (),
                                   TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count))
      {
        return -1; // something is wrong
      }

    return t_info.resident_size;
#endif
    // other systems are not yet supported
    return -1;
  }
};

#endif // MEM_USAGE_H

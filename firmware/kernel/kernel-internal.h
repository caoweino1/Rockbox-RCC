/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by Ulf Ralberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef KERNEL_INTERNAL_H
#define KERNEL_INTERNAL_H

#include "config.h"
#include "debug.h"

/* Make this nonzero to enable more elaborate checks on objects */
#if defined(DEBUG) || defined(SIMULATOR)
#define KERNEL_OBJECT_CHECKS 1 /* Always 1 for DEBUG and sim*/
#else
#define KERNEL_OBJECT_CHECKS 0
#endif

#if KERNEL_OBJECT_CHECKS
#ifdef SIMULATOR
#include <stdlib.h>
#define KERNEL_ASSERT(exp, msg...) \
    ({ if (!({ exp; })) { DEBUGF(msg); exit(-1); } })
#else
#define KERNEL_ASSERT(exp, msg...) \
    ({ if (!({ exp; })) panicf(msg); })
#endif
#else
#define KERNEL_ASSERT(exp, msg...) ({})
#endif


#endif /* KERNEL_INTERNAL_H */

/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * You can obtain a copy of the license at
 * src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL
 * HEADER in each file and include the License file at
 * usr/src/OPENSOLARIS.LICENSE.  If applicable,
 * add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your
 * own identifying information: Portions Copyright [yyyy]
 * [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * log benchmark - should do wider range...
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libmicro.h"

int
benchmark_init()
{
	(void) sprintf(lm_usage, "note: measures log()");
	lm_nsecs_per_op = 75;
	lm_tsdsize = 0;
	return (0);
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int			i;
	/* Added as part of the fix for radar 7508837 */
        double                  t = 0.0;

	for (i = 0; i < lm_optB; i += 10) {
		double value = i + .01;
#if 1 /* Apple added code, see radar 7508837 */
                t += log(value);
                t += log(value + 1.0);
                t += log(value + 2.0);
                t += log(value + 3.0);
                t += log(value + 4.0);
                t += log(value + 5.0);
                t += log(value + 6.0);
                t += log(value + 7.0);
                t += log(value + 8.0);
                t += log(value + 9.0);
        }
        res->re_count = i;

        return ((int)(t - t));
#else
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
		(void) log(value);
	}
	res->re_count = i;

	return (0);
#endif /* end of Apple fix  */
}

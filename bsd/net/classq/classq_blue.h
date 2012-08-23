/*
 * Copyright (c) 2011-2012 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

/*	$NetBSD: altq_blue.h,v 1.5 2006/10/12 19:59:08 peter Exp $	*/
/*	$KAME: altq_blue.h,v 1.7 2002/11/29 04:36:22 kjc Exp $	*/

/*
 * Copyright (C) 1997-2002
 *	Sony Computer Science Laboratories Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY SONY CSL AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL SONY CSL OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NET_CLASSQ_CLASSQ_BLUE_H_
#define	_NET_CLASSQ_CLASSQ_BLUE_H_

#ifdef PRIVATE
#ifdef BSD_KERNEL_PRIVATE
#include <net/classq/if_classq.h>
#endif /* BSD_KERNEL_PRIVATE */

#ifdef __cplusplus
extern "C" {
#endif

struct blue_stats {
	int32_t			q_pmark;
	u_int32_t		_pad;
	u_int64_t		drop_forced;
	u_int64_t		drop_unforced;
	u_int64_t		marked_packets;
};

#ifdef BSD_KERNEL_PRIVATE
/* blue flags */
#define	BLUEF_ECN4	0x01	/* use packet marking for IPv4 packets */
#define	BLUEF_ECN6	0x02	/* use packet marking for IPv6 packets */
#define	BLUEF_ECN	(BLUEF_ECN4 | BLUEF_ECN6)

#define	BLUEF_USERFLAGS							\
	(BLUEF_ECN4 | BLUEF_ECN6)

typedef struct blue {
	u_int32_t blue_flags;	/* blue flags */

	/* blue parameters */
	int32_t	  blue_pmark;	  /* 0-1000 (mark probability*10000) */
	int32_t   blue_max_pmark; /* sets precision of marking probability */
	u_int32_t blue_hold_time; /* hold time in usec */
	struct ifnet *blue_ifp;	  /* back pointer to ifnet */

	/* variables for internal use */
	u_int32_t blue_idle;	  /* queue was empty */
	struct timeval blue_last; /* timestamp when the queue becomes idle */

	/* statistics */
	struct {
		struct pktcntr	xmit_cnt;
		struct pktcntr	drop_cnt;
		u_int64_t	drop_forced;
		u_int64_t	drop_unforced;
		u_int64_t	marked_packets;
	} blue_stats;
} blue_t;

extern void blue_init(void);
extern struct blue *blue_alloc(struct ifnet *, u_int32_t, u_int32_t, u_int32_t);
extern void blue_destroy(struct blue *);
extern int blue_addq(struct blue *, class_queue_t *, struct mbuf *,
    struct pf_mtag *);
extern struct mbuf *blue_getq(struct blue *, class_queue_t *);
extern void blue_purgeq(struct blue *, class_queue_t *, u_int32_t,
    u_int32_t *, u_int32_t *);
extern void blue_getstats(struct blue *, struct blue_stats *);
extern void blue_updateq(struct blue *, cqev_t);
extern int blue_suspendq(struct blue *, class_queue_t *, boolean_t);
#endif /* BSD_KERNEL_PRIVATE */

#ifdef __cplusplus
}
#endif
#endif /* PRIVATE */
#endif /* _NET_CLASSQ_CLASSQ_BLUE_H_ */

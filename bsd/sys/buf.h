/*
 * Copyright (c) 2000-2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_OSREFERENCE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code 
 * as defined in and that are subject to the Apple Public Source License 
 * Version 2.0 (the 'License'). You may not use this file except in 
 * compliance with the License.  The rights granted to you under the 
 * License may not be used to create, or enable the creation or 
 * redistribution of, unlawful or unlicensed copies of an Apple operating 
 * system, or to circumvent, violate, or enable the circumvention or 
 * violation of, any terms of an Apple operating system software license 
 * agreement.
 *
 * Please obtain a copy of the License at 
 * http://www.opensource.apple.com/apsl/ and read it before using this 
 * file.
 *
 * The Original Code and all software distributed under the License are 
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * Please see the License for the specific language governing rights and 
 * limitations under the License.
 *
 * @APPLE_LICENSE_OSREFERENCE_HEADER_END@
 */
/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */
/*
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)buf.h	8.9 (Berkeley) 3/30/95
 */

#ifndef _SYS_BUF_H_
#define	_SYS_BUF_H_

#include <sys/cdefs.h>
#include <sys/kernel_types.h>
#include <mach/memory_object_types.h>


#define	B_WRITE		0x00000000	/* Write buffer (pseudo flag). */
#define	B_READ		0x00000001	/* Read buffer. */
#define	B_ASYNC		0x00000002	/* Start I/O, do not wait. */
#define	B_NOCACHE	0x00000004	/* Do not cache block after use. */
#define	B_DELWRI	0x00000008	/* Delay I/O until buffer reused. */
#define	B_LOCKED	0x00000010	/* Locked in core (not reusable). */
#define	B_PHYS		0x00000020	/* I/O to user memory. */
#define B_CLUSTER	0x00000040	/* UPL based I/O generated by cluster layer */
#define	B_PAGEIO	0x00000080	/* Page in/out */
#define	B_META		0x00000100	/* buffer contains meta-data. */
/*
 * make sure to check when adding flags that
 * that the new flags don't overlap the definitions
 * in buf_internal.h
 */

__BEGIN_DECLS

/*
 * mark the buffer associated with buf_t
 * as AGED with respect to the LRU cache
 */
void	buf_markaged(buf_t);

/*
 * mark the buffer associated with buf_t
 * as invalid... on release, it will go
 * directly to the free list
 */
void	buf_markinvalid(buf_t);

/*
 * mark the buffer assoicated with buf_t
 * as a delayed write... 
 */
void	buf_markdelayed(buf_t);

/*
 * mark the buffer associated with buf_t
 * as having been interrupted... EINTR
 */
void	buf_markeintr(buf_t);

/*
 * returns 1 if the buffer associated with buf_t
 * contains valid data... 0 if it does not
 */
int	buf_valid(buf_t);

/*
 * returns 1 if the buffer was already valid
 * in the cache... i.e. no I/O was performed
 * returns 0 otherwise
 */
int	buf_fromcache(buf_t);

/*
 * returns the UPL associated with buf_t
 */
void *	buf_upl(buf_t);

/*
 * returns the offset into the UPL
 * associated with buf_t which is to be
 * used as the base offset for this I/O
 */
uint32_t buf_uploffset(buf_t);

/*
 * returns read credential associated with buf_t
 * a reference is taken which must be explicilty dropped
 */
ucred_t buf_rcred(buf_t);

/*
 * returns write credential associated with buf_t
 * a reference is taken which must be explicilty dropped
 */
ucred_t buf_wcred(buf_t);

/*
 * returns process handle associated with buf_t
 * i.e identity of task that issued the I/O
 */
proc_t	buf_proc(buf_t);

uint32_t buf_dirtyoff(buf_t);
uint32_t buf_dirtyend(buf_t);
void	buf_setdirtyoff(buf_t, uint32_t);
void	buf_setdirtyend(buf_t, uint32_t);

/*
 * return the errno value associated with buf_t
 */
errno_t	buf_error(buf_t);

/*
 * set errno on buf_t
 */
void	buf_seterror(buf_t, errno_t);

/*
 * set specified flags on buf_t
 * B_LOCKED/B_NOCACHE/B_ASYNC/B_READ/B_WRITE/B_PAGEIO
 */
void	buf_setflags(buf_t, int32_t);

/*
 * clear specified flags on buf_t
 * B_LOCKED/B_NOCACHE/B_ASYNC/B_READ/B_WRITE/B_PAGEIO
 */
void	buf_clearflags(buf_t, int32_t);

/*
 * return external flags associated with buf_t
 * B_CLUSTER/B_PHYS/B_LOCKED/B_DELWRI/B_ASYNC/B_READ/B_WRITE/B_META/B_PAGEIO
 */
int32_t	buf_flags(buf_t);

/*
 * clears I/O related flags (both internal and
 * external) associated with buf_t and allows
 * the following to be set...
 * B_READ/B_WRITE/B_ASYNC/B_NOCACHE
 */
void	buf_reset(buf_t, int32_t);

/*
 * insure that the data storage associated with buf_t
 * is addressable
 */
errno_t	buf_map(buf_t, caddr_t *);

/*
 * release our need to have the storage associated
 * with buf_t in an addressable state
 */
errno_t	buf_unmap(buf_t);

/*
 * set driver specific data for buf_t
 */
void 	buf_setdrvdata(buf_t, void *);

/*
 * retrieve driver specific data associated with buf_t
 */
void *	buf_drvdata(buf_t);

/*
 * set fs specific data for buf_t
 */
void 	buf_setfsprivate(buf_t, void *);

/*
 * retrieve driver specific data associated with buf_t
 */
void *	buf_fsprivate(buf_t);

/*
 * retrieve the phsyical block number associated with buf_t
 */
daddr64_t buf_blkno(buf_t);

/*
 * retrieve the logical block number associated with buf_t
 * i.e. the block number derived from the file offset
 */
daddr64_t buf_lblkno(buf_t);

/*
 * set the phsyical block number associated with buf_t
 */
void	buf_setblkno(buf_t, daddr64_t);

/*
 * set the logical block number associated with buf_t
 * i.e. the block number derived from the file offset
 */
void	buf_setlblkno(buf_t, daddr64_t);

/*
 * retrieve the count of valid bytes associated with buf_t
 */
uint32_t buf_count(buf_t);

/*
 * retrieve the size of the data store assoicated with buf_t
 */
uint32_t buf_size(buf_t);

/*
 * retrieve the residual I/O count assoicated with buf_t
 * i.e. number of bytes that have not yet been completed
 */
uint32_t buf_resid(buf_t);

/*
 * set the count of bytes associated with buf_t
 * typically used to set the size of the I/O to be performed
 */
void	buf_setcount(buf_t, uint32_t);

/*
 * set the size of the buffer store associated with buf_t
 * typically used when providing private storage to buf_t
 */
void	buf_setsize(buf_t, uint32_t);

/*
 * set the size in bytes of the unfinished I/O associated with buf_t
 */
void	buf_setresid(buf_t, uint32_t);

/*
 * associate kernel addressable storage with buf_t
 */
void	buf_setdataptr(buf_t, uintptr_t);

/*
 * retrieve pointer to buffer associated with buf_t
 * if non-null, than guaranteed to be kernel addressable
 * size of buffer can be retrieved via buf_size
 * size of valid data can be retrieved via buf_count
 * if NULL, than use buf_map/buf_unmap to manage access to the underlying storage
 */
uintptr_t buf_dataptr(buf_t);

/*
 * return the vnode_t associated with buf_t
 */
vnode_t	buf_vnode(buf_t);

/*
 * assign vnode_t to buf_t... the 
 * device currently associated with
 * but_t is not changed.
 */
void	buf_setvnode(buf_t, vnode_t);

/*
 * return the dev_t associated with buf_t
 */
dev_t	buf_device(buf_t);

/*
 * assign the dev_t associated with vnode_t
 * to buf_t
 */
errno_t	buf_setdevice(buf_t, vnode_t);

errno_t	buf_strategy(vnode_t, void *);

/*
 * flags for buf_invalblkno
 */
#define	BUF_WAIT	0x01

errno_t	buf_invalblkno(vnode_t, daddr64_t, int);


/*
 * return the callback function pointer
 * if the callback is still valid
 * returns NULL if a buffer that was not
 * allocated via buf_alloc is specified
 * or if a callback has not been set or
 * it has already fired...
 */
void * buf_callback(buf_t);

/*
 * assign a one-shot callback function (driven from biodone)
 * to a buf_t allocated via buf_alloc... a caller specified
 * arg is passed to the callback function
 */
errno_t	buf_setcallback(buf_t, void (*)(buf_t, void *), void *);

/*
 * add a upl_t to a buffer allocated via buf_alloc
 * and set the offset into the upl_t (must be page
 * aligned).
 */
errno_t	buf_setupl(buf_t, upl_t, uint32_t);

/*
 * allocate a buf_t that is a clone of the buf_t
 * passed in, but whose I/O range is a subset...
 * if a callback routine is specified, it will
 * be called from buf_biodone with the bp and
 * arg specified.
 * it must be freed via buf_free
 */
buf_t	buf_clone(buf_t, int, int, void (*)(buf_t, void *), void *);

/*
 * allocate a buf_t associated with vnode_t
 * that has NO storage associated with it
 * but is suitable for use in issuing I/Os
 * after storage has been assigned via buf_setdataptr
 * or buf_addupl
 */
buf_t 	buf_alloc(vnode_t);

/*
 * free a buf_t that was allocated via buf_alloc
 * any private storage associated with buf_t is the
 * responsiblity of the caller to release
 */
void	buf_free(buf_t);

/*
 * flags for buf_invalidateblks
 */
#define	BUF_WRITE_DATA	0x0001		/* write data blocks first */
#define	BUF_SKIP_META	0x0002		/* skip over metadata blocks */

int	buf_invalidateblks(vnode_t, int, int, int);
/*
 * flags for buf_flushdirtyblks and buf_iterate
 */
#define BUF_SKIP_NONLOCKED	0x01
#define BUF_SKIP_LOCKED		0x02

void	buf_flushdirtyblks(vnode_t, int, int, char *);
void	buf_iterate(vnode_t, int (*)(buf_t, void *), int, void *);

#define	BUF_RETURNED		0
#define BUF_RETURNED_DONE	1
#define BUF_CLAIMED		2
#define	BUF_CLAIMED_DONE	3

/*
 * zero the storage associated with buf_t
 */
void	buf_clear(buf_t);

errno_t	buf_bawrite(buf_t);
errno_t	buf_bdwrite(buf_t);
errno_t	buf_bwrite(buf_t);

void	buf_biodone(buf_t);
errno_t	buf_biowait(buf_t);
void	buf_brelse(buf_t);

errno_t	buf_bread(vnode_t, daddr64_t, int, ucred_t, buf_t *);
errno_t	buf_breadn(vnode_t, daddr64_t, int, daddr64_t *, int *, int, ucred_t, buf_t *);
errno_t	buf_meta_bread(vnode_t, daddr64_t, int, ucred_t, buf_t *);
errno_t	buf_meta_breadn(vnode_t, daddr64_t, int, daddr64_t *, int *, int, ucred_t, buf_t *);

u_int	minphys(buf_t bp);
int	physio(void (*)(buf_t), buf_t, dev_t, int ,  u_int (*)(buf_t), struct uio *, int );


/*
 * Flags for operation type in getblk()
 */
#define	BLK_READ	0x01	/* buffer for read */
#define	BLK_WRITE	0x02	/* buffer for write */
#define	BLK_META	0x10	/* buffer for metadata */
/*
 * modifier for above flags...  if set, getblk will only return
 * a bp that is already valid... i.e. found in the cache
 */
#define BLK_ONLYVALID	0x80000000
                                  
/* timeout is in msecs */
buf_t	buf_getblk(vnode_t, daddr64_t, int, int, int, int);
buf_t	buf_geteblk(int);

__END_DECLS


/* Macros to clear/set/test flags. */
#define	SET(t, f)	(t) |= (f)
#define	CLR(t, f)	(t) &= ~(f)
#define	ISSET(t, f)	((t) & (f))


#endif /* !_SYS_BUF_H_ */

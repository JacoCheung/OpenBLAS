/***************************************************************************
Copyright (c) 2019, The OpenBLAS Project
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
3. Neither the name of the OpenBLAS project nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE OPENBLAS PROJECT OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "common.h"

#define NBMAX 2048

static void dgemv_kernel_4x4(BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y) {
  register FLOAT *ap0 = ap[0];
  register FLOAT *ap1 = ap[1];
  register FLOAT *ap2 = ap[2];
  register FLOAT *ap3 = ap[3];

  __asm__("vzero %%v0\n\t"
    "vzero %%v1\n\t"
    "vzero %%v2\n\t"
    "vzero %%v3\n\t"
    "vzero %%v4\n\t"
    "vzero %%v5\n\t"
    "vzero %%v6\n\t"
    "vzero %%v7\n\t"
    "xgr   %%r1,%%r1\n\t"
    "lghi    %%r0,-16\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      1f\n\t"
    "srlg  %%r0,%%r0,4\n\t"
    "0:\n\t"
    "pfd 1,1024(%%r1,%[ap0])\n\t"
    "pfd 1,1024(%%r1,%[ap1])\n\t"
    "pfd 1,1024(%%r1,%[ap2])\n\t"
    "pfd 1,1024(%%r1,%[ap3])\n\t"
    "pfd 1,1024(%%r1,%[x])\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v18,32(%%r1,%[x])\n\t"
    "vl  %%v19,48(%%r1,%[x])\n\t"
    "vl  %%v20,64(%%r1,%[x])\n\t"
    "vl  %%v21,80(%%r1,%[x])\n\t"
    "vl  %%v22,96(%%r1,%[x])\n\t"
    "vl  %%v23,112(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,0(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v16,%%v25,%%v1\n\t"
    "vl  %%v26,0(%%r1,%[ap2])\n\t"
    "vfmadb   %%v2,%%v16,%%v26,%%v2\n\t"
    "vl  %%v27,0(%%r1,%[ap3])\n\t"
    "vfmadb   %%v3,%%v16,%%v27,%%v3\n\t"
    "vl  %%v28,16(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v17,%%v28,%%v4\n\t"
    "vl  %%v29,16(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v17,%%v29,%%v5\n\t"
    "vl  %%v30,16(%%r1,%[ap2])\n\t"
    "vfmadb   %%v6,%%v17,%%v30,%%v6\n\t"
    "vl  %%v31,16(%%r1,%[ap3])\n\t"
    "vfmadb   %%v7,%%v17,%%v31,%%v7\n\t"
    "vl  %%v24,32(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v18,%%v24,%%v0\n\t"
    "vl  %%v25,32(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v18,%%v25,%%v1\n\t"
    "vl  %%v26,32(%%r1,%[ap2])\n\t"
    "vfmadb   %%v2,%%v18,%%v26,%%v2\n\t"
    "vl  %%v27,32(%%r1,%[ap3])\n\t"
    "vfmadb   %%v3,%%v18,%%v27,%%v3\n\t"
    "vl  %%v28,48(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v19,%%v28,%%v4\n\t"
    "vl  %%v29,48(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v19,%%v29,%%v5\n\t"
    "vl  %%v30,48(%%r1,%[ap2])\n\t"
    "vfmadb   %%v6,%%v19,%%v30,%%v6\n\t"
    "vl  %%v31,48(%%r1,%[ap3])\n\t"
    "vfmadb   %%v7,%%v19,%%v31,%%v7\n\t"
    "vl  %%v24,64(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v20,%%v24,%%v0\n\t"
    "vl  %%v25,64(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v20,%%v25,%%v1\n\t"
    "vl  %%v26,64(%%r1,%[ap2])\n\t"
    "vfmadb   %%v2,%%v20,%%v26,%%v2\n\t"
    "vl  %%v27,64(%%r1,%[ap3])\n\t"
    "vfmadb   %%v3,%%v20,%%v27,%%v3\n\t"
    "vl  %%v28,80(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v21,%%v28,%%v4\n\t"
    "vl  %%v29,80(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v21,%%v29,%%v5\n\t"
    "vl  %%v30,80(%%r1,%[ap2])\n\t"
    "vfmadb   %%v6,%%v21,%%v30,%%v6\n\t"
    "vl  %%v31,80(%%r1,%[ap3])\n\t"
    "vfmadb   %%v7,%%v21,%%v31,%%v7\n\t"
    "vl  %%v24,96(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v22,%%v24,%%v0\n\t"
    "vl  %%v25,96(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v22,%%v25,%%v1\n\t"
    "vl  %%v26,96(%%r1,%[ap2])\n\t"
    "vfmadb   %%v2,%%v22,%%v26,%%v2\n\t"
    "vl  %%v27,96(%%r1,%[ap3])\n\t"
    "vfmadb   %%v3,%%v22,%%v27,%%v3\n\t"
    "vl  %%v28,112(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v23,%%v28,%%v4\n\t"
    "vl  %%v29,112(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v23,%%v29,%%v5\n\t"
    "vl  %%v30,112(%%r1,%[ap2])\n\t"
    "vfmadb   %%v6,%%v23,%%v30,%%v6\n\t"
    "vl  %%v31,112(%%r1,%[ap3])\n\t"
    "vfmadb   %%v7,%%v23,%%v31,%%v7\n\t"
    "agfi   %%r1,128\n\t"
    "brctg  %%r0,0b\n\t"
    "1:\n\t"
    "lghi    %%r0,12\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      3f\n\t"
    "srlg  %%r0,%%r0,2\n\t"
    "2:\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,0(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v16,%%v25,%%v1\n\t"
    "vl  %%v26,0(%%r1,%[ap2])\n\t"
    "vfmadb   %%v2,%%v16,%%v26,%%v2\n\t"
    "vl  %%v27,0(%%r1,%[ap3])\n\t"
    "vfmadb   %%v3,%%v16,%%v27,%%v3\n\t"
    "vl  %%v28,16(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v17,%%v28,%%v4\n\t"
    "vl  %%v29,16(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v17,%%v29,%%v5\n\t"
    "vl  %%v30,16(%%r1,%[ap2])\n\t"
    "vfmadb   %%v6,%%v17,%%v30,%%v6\n\t"
    "vl  %%v31,16(%%r1,%[ap3])\n\t"
    "vfmadb   %%v7,%%v17,%%v31,%%v7\n\t"
    "agfi   %%r1,32\n\t"
    "brctg  %%r0,2b\n\t"
    "3:\n\t"
    "vfadb  %%v0,%%v0,%%v4\n\t"
    "vfadb  %%v1,%%v1,%%v5\n\t"
    "vfadb  %%v2,%%v2,%%v6\n\t"
    "vfadb  %%v3,%%v3,%%v7\n\t"
    "vrepg  %%v4,%%v0,1\n\t"
    "adbr   %%f0,%%f4\n\t"
    "std    %%f0,0(%[y])\n\t"
    "vrepg  %%v4,%%v1,1\n\t"
    "adbr   %%f1,%%f4\n\t"
    "std    %%f1,8(%[y])\n\t"
    "vrepg  %%v4,%%v2,1\n\t"
    "adbr   %%f2,%%f4\n\t"
    "std    %%f2,16(%[y])\n\t"
    "vrepg  %%v4,%%v3,1\n\t"
    "adbr   %%f3,%%f4\n\t"
    "std    %%f3,24(%[y])"
    : "=m"(*(struct { FLOAT x[4]; } *) y)
    : [y] "a"(y), "m"(*(const struct { FLOAT x[n]; } *) ap0),[ap0] "a"(ap0),
       "m"(*(const struct { FLOAT x[n]; } *) ap1),[ap1] "a"(ap1),
       "m"(*(const struct { FLOAT x[n]; } *) ap2),[ap2] "a"(ap2),
       "m"(*(const struct { FLOAT x[n]; } *) ap3),[ap3] "a"(ap3),
       "m"(*(const struct { FLOAT x[n]; } *) x),[x] "a"(x),[n] "r"(n)
    : "cc", "r0", "r1", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
       "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25",
       "v26", "v27", "v28", "v29", "v30", "v31");
}

static void dgemv_kernel_4x2(BLASLONG n, FLOAT **ap, FLOAT *x, FLOAT *y) {
  register FLOAT *ap0 = ap[0];
  register FLOAT *ap1 = ap[1];

  __asm__("vzero %%v0\n\t"
    "vzero %%v1\n\t"
    "vzero %%v2\n\t"
    "vzero %%v3\n\t"
    "vzero %%v4\n\t"
    "vzero %%v5\n\t"
    "vzero %%v6\n\t"
    "vzero %%v7\n\t"
    "xgr   %%r1,%%r1\n\t"
    "lghi    %%r0,-16\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      1f\n\t"
    "srlg  %%r0,%%r0,4\n\t"
    "0:\n\t"
    "pfd 1,1024(%%r1,%[ap0])\n\t"
    "pfd 1,1024(%%r1,%[ap1])\n\t"
    "pfd 1,1024(%%r1,%[x])\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v18,32(%%r1,%[x])\n\t"
    "vl  %%v19,48(%%r1,%[x])\n\t"
    "vl  %%v20,64(%%r1,%[x])\n\t"
    "vl  %%v21,80(%%r1,%[x])\n\t"
    "vl  %%v22,96(%%r1,%[x])\n\t"
    "vl  %%v23,112(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,0(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v16,%%v25,%%v1\n\t"
    "vl  %%v26,16(%%r1,%[ap0])\n\t"
    "vfmadb   %%v2,%%v17,%%v26,%%v2\n\t"
    "vl  %%v27,16(%%r1,%[ap1])\n\t"
    "vfmadb   %%v3,%%v17,%%v27,%%v3\n\t"
    "vl  %%v28,32(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v18,%%v28,%%v4\n\t"
    "vl  %%v29,32(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v18,%%v29,%%v5\n\t"
    "vl  %%v30,48(%%r1,%[ap0])\n\t"
    "vfmadb   %%v6,%%v19,%%v30,%%v6\n\t"
    "vl  %%v31,48(%%r1,%[ap1])\n\t"
    "vfmadb   %%v7,%%v19,%%v31,%%v7\n\t"
    "vl  %%v24,64(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v20,%%v24,%%v0\n\t"
    "vl  %%v25,64(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v20,%%v25,%%v1\n\t"
    "vl  %%v26,80(%%r1,%[ap0])\n\t"
    "vfmadb   %%v2,%%v21,%%v26,%%v2\n\t"
    "vl  %%v27,80(%%r1,%[ap1])\n\t"
    "vfmadb   %%v3,%%v21,%%v27,%%v3\n\t"
    "vl  %%v28,96(%%r1,%[ap0])\n\t"
    "vfmadb   %%v4,%%v22,%%v28,%%v4\n\t"
    "vl  %%v29,96(%%r1,%[ap1])\n\t"
    "vfmadb   %%v5,%%v22,%%v29,%%v5\n\t"
    "vl  %%v30,112(%%r1,%[ap0])\n\t"
    "vfmadb   %%v6,%%v23,%%v30,%%v6\n\t"
    "vl  %%v31,112(%%r1,%[ap1])\n\t"
    "vfmadb   %%v7,%%v23,%%v31,%%v7\n\t"
    "agfi   %%r1,128\n\t"
    "brctg  %%r0,0b\n\t"
    "1:\n\t"
    "lghi    %%r0,12\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      3f\n\t"
    "srlg  %%r0,%%r0,2\n\t"
    "2:\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[ap0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,0(%%r1,%[ap1])\n\t"
    "vfmadb   %%v1,%%v16,%%v25,%%v1\n\t"
    "vl  %%v26,16(%%r1,%[ap0])\n\t"
    "vfmadb   %%v2,%%v17,%%v26,%%v2\n\t"
    "vl  %%v27,16(%%r1,%[ap1])\n\t"
    "vfmadb   %%v3,%%v17,%%v27,%%v3\n\t"
    "agfi   %%r1,32\n\t"
    "brctg  %%r0,2b\n\t"
    "3:\n\t"
    "vfadb  %%v0,%%v0,%%v2\n\t"
    "vfadb  %%v0,%%v0,%%v4\n\t"
    "vfadb  %%v0,%%v0,%%v6\n\t"
    "vfadb  %%v1,%%v1,%%v3\n\t"
    "vfadb  %%v1,%%v1,%%v5\n\t"
    "vfadb  %%v1,%%v1,%%v7\n\t"
    "vrepg  %%v2,%%v0,1\n\t"
    "adbr   %%f0,%%f2\n\t"
    "std    %%f0,0(%[y])\n\t"
    "vrepg  %%v2,%%v1,1\n\t"
    "adbr   %%f1,%%f2\n\t"
    "std    %%f1,8(%[y])"
    : "=m"(*(struct { FLOAT x[2]; } *) y)
    : [y] "a"(y), "m"(*(const struct { FLOAT x[n]; } *) ap0),[ap0] "a"(ap0),
       "m"(*(const struct { FLOAT x[n]; } *) ap1),[ap1] "a"(ap1),
       "m"(*(const struct { FLOAT x[n]; } *) x),[x] "a"(x),[n] "r"(n)
    : "cc", "r0", "r1", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
       "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25",
       "v26", "v27", "v28", "v29", "v30", "v31");
}

static void dgemv_kernel_4x1(BLASLONG n, FLOAT *a0, FLOAT *x, FLOAT *y) {
  __asm__("vzero %%v0\n\t"
    "vzero %%v1\n\t"
    "vzero %%v2\n\t"
    "vzero %%v3\n\t"
    "vzero %%v4\n\t"
    "vzero %%v5\n\t"
    "vzero %%v6\n\t"
    "vzero %%v7\n\t"
    "xgr   %%r1,%%r1\n\t"
    "lghi    %%r0,-16\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      1f\n\t"
    "srlg  %%r0,%%r0,4\n\t"
    "0:\n\t"
    "pfd 1,1024(%%r1,%[a0])\n\t"
    "pfd 1,1024(%%r1,%[x])\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v18,32(%%r1,%[x])\n\t"
    "vl  %%v19,48(%%r1,%[x])\n\t"
    "vl  %%v20,64(%%r1,%[x])\n\t"
    "vl  %%v21,80(%%r1,%[x])\n\t"
    "vl  %%v22,96(%%r1,%[x])\n\t"
    "vl  %%v23,112(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[a0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,16(%%r1,%[a0])\n\t"
    "vfmadb   %%v1,%%v17,%%v25,%%v1\n\t"
    "vl  %%v26,32(%%r1,%[a0])\n\t"
    "vfmadb   %%v2,%%v18,%%v26,%%v2\n\t"
    "vl  %%v27,48(%%r1,%[a0])\n\t"
    "vfmadb   %%v3,%%v19,%%v27,%%v3\n\t"
    "vl  %%v28,64(%%r1,%[a0])\n\t"
    "vfmadb   %%v4,%%v20,%%v28,%%v4\n\t"
    "vl  %%v29,80(%%r1,%[a0])\n\t"
    "vfmadb   %%v5,%%v21,%%v29,%%v5\n\t"
    "vl  %%v30,96(%%r1,%[a0])\n\t"
    "vfmadb   %%v6,%%v22,%%v30,%%v6\n\t"
    "vl  %%v31,112(%%r1,%[a0])\n\t"
    "vfmadb   %%v7,%%v23,%%v31,%%v7\n\t"
    "agfi   %%r1,128\n\t"
    "brctg  %%r0,0b\n\t"
    "1:\n\t"
    "lghi    %%r0,12\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      3f\n\t"
    "srlg  %%r0,%%r0,2\n\t"
    "2:\n\t"
    "vl  %%v16,0(%%r1,%[x])\n\t"
    "vl  %%v17,16(%%r1,%[x])\n\t"
    "vl  %%v24,0(%%r1,%[a0])\n\t"
    "vfmadb   %%v0,%%v16,%%v24,%%v0\n\t"
    "vl  %%v25,16(%%r1,%[a0])\n\t"
    "vfmadb   %%v1,%%v17,%%v25,%%v1\n\t"
    "agfi   %%r1,32\n\t"
    "brctg  %%r0,2b\n\t"
    "3:\n\t"
    "vfadb  %%v0,%%v0,%%v1\n\t"
    "vfadb  %%v0,%%v0,%%v2\n\t"
    "vfadb  %%v0,%%v0,%%v3\n\t"
    "vfadb  %%v0,%%v0,%%v4\n\t"
    "vfadb  %%v0,%%v0,%%v5\n\t"
    "vfadb  %%v0,%%v0,%%v6\n\t"
    "vfadb  %%v0,%%v0,%%v7\n\t"
    "vrepg  %%v1,%%v0,1\n\t"
    "adbr   %%f0,%%f1\n\t"
    "std    %%f0,0(%[y])"
    : "=m"(*(FLOAT (*)[1]) y)
    : [y] "a"(y), "m"(*(const struct { FLOAT x[n]; } *) a0),[a0] "a"(a0),
       "m"(*(const struct { FLOAT x[n]; } *) x),[x] "a"(x),[n] "r"(n)
    : "cc", "r0", "r1", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
       "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25",
       "v26", "v27", "v28", "v29", "v30", "v31");
}

static void copy_x(BLASLONG n, FLOAT *src, FLOAT *dest, BLASLONG inc_src) {
  BLASLONG i;
  for (i = 0; i < n; i++) {
    dest[i] = *src;
    src += inc_src;
  }
}

static void add_y_kernel_4(BLASLONG n, FLOAT da, FLOAT *src, FLOAT *dest) {
  __asm__("vlrepg %%v0,%[da]\n\t"
    "xgr   %%r1,%%r1\n\t"
    "lghi    %%r0,-16\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      1f\n\t"
    "srlg  %%r0,%%r0,4\n\t"
    "0:\n\t"
    "pfd 1,1024(%%r1,%[src])\n\t"
    "pfd 2,1024(%%r1,%[dest])\n\t"
    "vl  %%v16,0(%%r1,%[src])\n\t"
    "vl  %%v17,16(%%r1,%[src])\n\t"
    "vl  %%v18,32(%%r1,%[src])\n\t"
    "vl  %%v19,48(%%r1,%[src])\n\t"
    "vl  %%v20,64(%%r1,%[src])\n\t"
    "vl  %%v21,80(%%r1,%[src])\n\t"
    "vl  %%v22,96(%%r1,%[src])\n\t"
    "vl  %%v23,112(%%r1,%[src])\n\t"
    "vl  %%v24, 0(%%r1,%[dest])\n\t"
    "vfmadb   %%v24,%%v16,%%v0,%%v24\n\t"
    "vst  %%v24, 0(%%r1,%[dest])\n\t"
    "vl  %%v25, 16(%%r1,%[dest])\n\t"
    "vfmadb   %%v25,%%v17,%%v0,%%v25\n\t"
    "vst  %%v25, 16(%%r1,%[dest])\n\t"
    "vl  %%v26, 32(%%r1,%[dest])\n\t"
    "vfmadb   %%v26,%%v18,%%v0,%%v26\n\t"
    "vst  %%v26, 32(%%r1,%[dest])\n\t"
    "vl  %%v27, 48(%%r1,%[dest])\n\t"
    "vfmadb   %%v27,%%v19,%%v0,%%v27\n\t"
    "vst  %%v27, 48(%%r1,%[dest])\n\t"
    "vl  %%v28, 64(%%r1,%[dest])\n\t"
    "vfmadb   %%v28,%%v20,%%v0,%%v28\n\t"
    "vst  %%v28, 64(%%r1,%[dest])\n\t"
    "vl  %%v29, 80(%%r1,%[dest])\n\t"
    "vfmadb   %%v29,%%v21,%%v0,%%v29\n\t"
    "vst  %%v29, 80(%%r1,%[dest])\n\t"
    "vl  %%v30, 96(%%r1,%[dest])\n\t"
    "vfmadb   %%v30,%%v22,%%v0,%%v30\n\t"
    "vst  %%v30, 96(%%r1,%[dest])\n\t"
    "vl  %%v31, 112(%%r1,%[dest])\n\t"
    "vfmadb   %%v31,%%v23,%%v0,%%v31\n\t"
    "vst  %%v31, 112(%%r1,%[dest])\n\t"
    "agfi   %%r1,128\n\t"
    "brctg  %%r0,0b\n\t"
    "1:\n\t"
    "lghi    %%r0,12\n\t"
    "ngr     %%r0,%[n]\n\t"
    "ltgr    %%r0,%%r0\n\t"
    "jz      3f\n\t"
    "srlg  %%r0,%%r0,2\n\t"
    "2:\n\t"
    "vl  %%v16,0(%%r1,%[src])\n\t"
    "vl  %%v17,16(%%r1,%[src])\n\t"
    "vl  %%v24, 0(%%r1,%[dest])\n\t"
    "vfmadb   %%v24,%%v16,%%v0,%%v24\n\t"
    "vst  %%v24, 0(%%r1,%[dest])\n\t"
    "vl  %%v25, 16(%%r1,%[dest])\n\t"
    "vfmadb   %%v25,%%v17,%%v0,%%v25\n\t"
    "vst  %%v25, 16(%%r1,%[dest])\n\t"
    "agfi   %%r1,32\n\t"
    "brctg  %%r0,2b\n\t"
    "3:\n\t"
    "nop"
    : "+m"(*(struct { FLOAT x[n]; } *) dest)
    : [dest] "a"(dest),[da] "Q"(da), "m"(*(const struct { FLOAT x[n]; } *) src),
       [src] "a"(src),[n] "r"(n)
    : "cc", "r0", "r1", "v0", "v16", "v17", "v18", "v19", "v20", "v21",
       "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30",
       "v31");
}
static void add_y(BLASLONG n, FLOAT da, FLOAT *src, FLOAT *dest,
                  BLASLONG inc_dest) {
  if (inc_dest == 1)
    add_y_kernel_4(n, da, src, dest);
  else {
    BLASLONG i;
    for (i = 0; i < n; i++) {
      *dest += src[i] * da;
      dest += inc_dest;
    }
  }
}

int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha, FLOAT *a,
          BLASLONG lda, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y,
          FLOAT *buffer) {
  BLASLONG register i;
  BLASLONG register j;
  FLOAT *a_ptr;
  FLOAT *x_ptr;
  FLOAT *y_ptr;
  BLASLONG n0;
  BLASLONG n1;
  BLASLONG m1;
  BLASLONG m2;
  BLASLONG m3;
  BLASLONG n2;
  FLOAT ybuffer[2] __attribute__ ((aligned(16)));
  FLOAT *xbuffer;
  FLOAT *ytemp;

  if (m < 1)
    return (0);
  if (n < 1)
    return (0);

  xbuffer = buffer;
  ytemp = buffer + (m < NBMAX ? m : NBMAX);

  n0 = n / NBMAX;
  n1 = (n % NBMAX) >> 2;
  n2 = n & 3;

  m3 = m & 3;
  m1 = m & -4;
  m2 = (m & (NBMAX - 1)) - m3;

  BLASLONG NB = NBMAX;

  while (NB == NBMAX) {
    m1 -= NB;
    if (m1 < 0) {
      if (m2 == 0)
        break;
      NB = m2;
    }

    y_ptr = y;
    a_ptr = a;
    x_ptr = x;

    if (inc_x == 1)
      xbuffer = x_ptr;
    else
      copy_x(NB, x_ptr, xbuffer, inc_x);

    FLOAT *ap[4];
    FLOAT *yp;
    BLASLONG register lda4 = 4 * lda;
    ap[0] = a_ptr;
    ap[1] = a_ptr + lda;
    ap[2] = ap[1] + lda;
    ap[3] = ap[2] + lda;

    if (n0 > 0) {
      BLASLONG nb1 = NBMAX / 4;
      for (j = 0; j < n0; j++) {

        yp = ytemp;
        for (i = 0; i < nb1; i++) {
          dgemv_kernel_4x4(NB, ap, xbuffer, yp);
          ap[0] += lda4;
          ap[1] += lda4;
          ap[2] += lda4;
          ap[3] += lda4;
          yp += 4;
        }
        add_y(nb1 * 4, alpha, ytemp, y_ptr, inc_y);
        y_ptr += nb1 * inc_y * 4;
        a_ptr += nb1 * lda4;

      }

    }

    yp = ytemp;

    for (i = 0; i < n1; i++) {
      dgemv_kernel_4x4(NB, ap, xbuffer, yp);
      ap[0] += lda4;
      ap[1] += lda4;
      ap[2] += lda4;
      ap[3] += lda4;
      yp += 4;
    }
    if (n1 > 0) {
      add_y(n1 * 4, alpha, ytemp, y_ptr, inc_y);
      y_ptr += n1 * inc_y * 4;
      a_ptr += n1 * lda4;
    }

    if (n2 & 2) {

      dgemv_kernel_4x2(NB, ap, xbuffer, ybuffer);
      a_ptr += lda * 2;
      *y_ptr += ybuffer[0] * alpha;
      y_ptr += inc_y;
      *y_ptr += ybuffer[1] * alpha;
      y_ptr += inc_y;

    }

    if (n2 & 1) {

      dgemv_kernel_4x1(NB, a_ptr, xbuffer, ybuffer);
      // a_ptr  += lda;
      *y_ptr += ybuffer[0] * alpha;
      // y_ptr  += inc_y;

    }
    a += NB;
    x += NB * inc_x;
  }

  if (m3 == 0)
    return (0);

  x_ptr = x;
  a_ptr = a;
  if (m3 == 3) {
    FLOAT xtemp0 = *x_ptr * alpha;
    x_ptr += inc_x;
    FLOAT xtemp1 = *x_ptr * alpha;
    x_ptr += inc_x;
    FLOAT xtemp2 = *x_ptr * alpha;

    FLOAT *aj = a_ptr;
    y_ptr = y;

    if (lda == 3 && inc_y == 1) {

      for (j = 0; j < (n & -4); j += 4) {

        y_ptr[j] += aj[0] * xtemp0 + aj[1] * xtemp1 + aj[2] * xtemp2;
        y_ptr[j + 1] += aj[3] * xtemp0 + aj[4] * xtemp1 + aj[5] * xtemp2;
        y_ptr[j + 2] += aj[6] * xtemp0 + aj[7] * xtemp1 + aj[8] * xtemp2;
        y_ptr[j + 3] += aj[9] * xtemp0 + aj[10] * xtemp1 + aj[11] * xtemp2;
        aj += 12;
      }

      for (; j < n; j++) {
        y_ptr[j] += aj[0] * xtemp0 + aj[1] * xtemp1 + aj[2] * xtemp2;
        aj += 3;
      }

    } else {

      if (inc_y == 1) {

        BLASLONG register lda2 = lda << 1;
        BLASLONG register lda4 = lda << 2;
        BLASLONG register lda3 = lda2 + lda;

        for (j = 0; j < (n & -4); j += 4) {

          y_ptr[j] += *aj * xtemp0 + *(aj + 1) * xtemp1 + *(aj + 2) * xtemp2;
          y_ptr[j + 1] +=
            *(aj + lda) * xtemp0 + *(aj + lda + 1) * xtemp1 + *(aj + lda +
                                                                2) * xtemp2;
          y_ptr[j + 2] +=
            *(aj + lda2) * xtemp0 + *(aj + lda2 + 1) * xtemp1 + *(aj + lda2 +
                                                                  2) * xtemp2;
          y_ptr[j + 3] +=
            *(aj + lda3) * xtemp0 + *(aj + lda3 + 1) * xtemp1 + *(aj + lda3 +
                                                                  2) * xtemp2;
          aj += lda4;
        }

        for (; j < n; j++) {

          y_ptr[j] += *aj * xtemp0 + *(aj + 1) * xtemp1 + *(aj + 2) * xtemp2;
          aj += lda;
        }

      } else {

        for (j = 0; j < n; j++) {
          *y_ptr += *aj * xtemp0 + *(aj + 1) * xtemp1 + *(aj + 2) * xtemp2;
          y_ptr += inc_y;
          aj += lda;
        }

      }

    }
    return (0);
  }

  if (m3 == 2) {
    FLOAT xtemp0 = *x_ptr * alpha;
    x_ptr += inc_x;
    FLOAT xtemp1 = *x_ptr * alpha;

    FLOAT *aj = a_ptr;
    y_ptr = y;

    if (lda == 2 && inc_y == 1) {

      for (j = 0; j < (n & -4); j += 4) {
        y_ptr[j] += aj[0] * xtemp0 + aj[1] * xtemp1;
        y_ptr[j + 1] += aj[2] * xtemp0 + aj[3] * xtemp1;
        y_ptr[j + 2] += aj[4] * xtemp0 + aj[5] * xtemp1;
        y_ptr[j + 3] += aj[6] * xtemp0 + aj[7] * xtemp1;
        aj += 8;

      }

      for (; j < n; j++) {
        y_ptr[j] += aj[0] * xtemp0 + aj[1] * xtemp1;
        aj += 2;
      }

    } else {
      if (inc_y == 1) {

        BLASLONG register lda2 = lda << 1;
        BLASLONG register lda4 = lda << 2;
        BLASLONG register lda3 = lda2 + lda;

        for (j = 0; j < (n & -4); j += 4) {

          y_ptr[j] += *aj * xtemp0 + *(aj + 1) * xtemp1;
          y_ptr[j + 1] += *(aj + lda) * xtemp0 + *(aj + lda + 1) * xtemp1;
          y_ptr[j + 2] += *(aj + lda2) * xtemp0 + *(aj + lda2 + 1) * xtemp1;
          y_ptr[j + 3] += *(aj + lda3) * xtemp0 + *(aj + lda3 + 1) * xtemp1;
          aj += lda4;
        }

        for (; j < n; j++) {

          y_ptr[j] += *aj * xtemp0 + *(aj + 1) * xtemp1;
          aj += lda;
        }

      } else {
        for (j = 0; j < n; j++) {
          *y_ptr += *aj * xtemp0 + *(aj + 1) * xtemp1;
          y_ptr += inc_y;
          aj += lda;
        }
      }

    }
    return (0);

  }

  FLOAT xtemp = *x_ptr * alpha;
  FLOAT *aj = a_ptr;
  y_ptr = y;
  if (lda == 1 && inc_y == 1) {
    for (j = 0; j < (n & -4); j += 4) {
      y_ptr[j] += aj[j] * xtemp;
      y_ptr[j + 1] += aj[j + 1] * xtemp;
      y_ptr[j + 2] += aj[j + 2] * xtemp;
      y_ptr[j + 3] += aj[j + 3] * xtemp;
    }
    for (; j < n; j++) {
      y_ptr[j] += aj[j] * xtemp;
    }

  } else {
    if (inc_y == 1) {

      BLASLONG register lda2 = lda << 1;
      BLASLONG register lda4 = lda << 2;
      BLASLONG register lda3 = lda2 + lda;
      for (j = 0; j < (n & -4); j += 4) {
        y_ptr[j] += *aj * xtemp;
        y_ptr[j + 1] += *(aj + lda) * xtemp;
        y_ptr[j + 2] += *(aj + lda2) * xtemp;
        y_ptr[j + 3] += *(aj + lda3) * xtemp;
        aj += lda4;
      }

      for (; j < n; j++) {
        y_ptr[j] += *aj * xtemp;
        aj += lda;
      }

    } else {
      for (j = 0; j < n; j++) {
        *y_ptr += *aj * xtemp;
        y_ptr += inc_y;
        aj += lda;
      }

    }
  }

  return (0);
}

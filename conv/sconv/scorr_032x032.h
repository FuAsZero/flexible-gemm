#define scorr_32x32(grad,suffix)\
__global__ void dk_scorr_32x32##suffix(\
          char*              d_c   ,\
    const char* __restrict__ d_a   ,\
    const char* __restrict__ d_b   ,\
    const char* __restrict__ d_x   ,\
    float                    alpha ,\
    int                      ldc   ,\
    int                      lda   ,\
    int                      ldb   ,\
    int                      cx    ,\
    int                      cy    ,\
    int                      ax    ,\
    int                      ay    ,\
    int                      fnn   ,\
    int                      cnr   ,\
    int                      pnc   ,\
    int                      qnc ){ \
    __shared__ char smem[4096];  \
    float c[32];                 \
    float4 a[2], b[4];           \
    unsigned int bx=blockIdx.x;  \
    unsigned int by=blockIdx.y;  \
    unsigned int tid=threadIdx.x;\
    unsigned int x=(bx<<5)+tid;  \
    unsigned int su=x  <cnr?x  :(cnr-1);\
    unsigned int sv=tid<pnc?tid:(pnc-1);\
    unsigned int cxy=cx*cy;  \
    unsigned int idx=su%cxy; \
    unsigned int bnr=qnc*fnn;\
    d_a+=by*qnc*lda+((((su/cxy)*ax+(idx/cy))*ay+(idx%cy))<<2);\
    d_b+=by*qnc*ldb+sv*fnn*4;\
    d_c+=by*pnc*ldc+(x<<2);  \
    const char* d_ao=&cmem[0];\
    const char* d_bo=&cmem[bnr*4];\
    uint4 ao0=*((const uint4*)&d_ao[ 0] );\
	uint4 ao1=*((const uint4*)&d_ao[16] );\
    uint4 bo0=*((const uint4*)&d_bo[ 0] );\
	uint4 bo1=*((const uint4*)&d_bo[16] );\
    float p0=*((const float*)&d_a[ao0.x]);\
    float p1=*((const float*)&d_a[ao0.y]);\
    float p2=*((const float*)&d_a[ao0.z]);\
    float p3=*((const float*)&d_a[ao0.w]);\
    float p4=*((const float*)&d_a[ao1.x]);\
    float p5=*((const float*)&d_a[ao1.y]);\
    float p6=*((const float*)&d_a[ao1.z]);\
    float p7=*((const float*)&d_a[ao1.w]);\
    float q0=*((const float*)&d_b[bo0.x]);\
    float q1=*((const float*)&d_b[bo0.y]);\
    float q2=*((const float*)&d_b[bo0.z]);\
    float q3=*((const float*)&d_b[bo0.w]);\
    float q4=*((const float*)&d_b[bo1.x]);\
    float q5=*((const float*)&d_b[bo1.y]);\
    float q6=*((const float*)&d_b[bo1.z]);\
    float q7=*((const float*)&d_b[bo1.w]);\
    char* __restrict__ sst_base =&smem[tid<<2];\
    char* __restrict__ asld_base=&smem[(tid&0xe)<<3];\
    char* __restrict__ bsld_base=&smem[0x400+(((tid&0x10)<<1)|((tid&0x1)<<4))];\
    char* __restrict__ asld=asld_base;\
    char* __restrict__ bsld=bsld_base;\
    *((float*)&sst_base[ 0*128])=p0;\
    *((float*)&sst_base[ 1*128])=p1;\
    *((float*)&sst_base[ 2*128])=p2;\
    *((float*)&sst_base[ 3*128])=p3;\
    *((float*)&sst_base[ 4*128])=p4;\
    *((float*)&sst_base[ 5*128])=p5;\
    *((float*)&sst_base[ 6*128])=p6;\
    *((float*)&sst_base[ 7*128])=p7;\
    *((float*)&sst_base[ 8*128])=q0;\
    *((float*)&sst_base[ 9*128])=q1;\
    *((float*)&sst_base[10*128])=q2;\
    *((float*)&sst_base[11*128])=q3;\
    *((float*)&sst_base[12*128])=q4;\
    *((float*)&sst_base[13*128])=q5;\
    *((float*)&sst_base[14*128])=q6;\
    *((float*)&sst_base[15*128])=q7;\
    __syncthreads();\
    SZERO32(c)\
    b[0]=*((float4*)&bsld[0x00]);\
    a[0]=*((float4*)&asld[0x00]);\
    b[1]=*((float4*)&bsld[0x40]);\
    unsigned int ofs=0x800;      \
    for( int k=bnr-8; k>0; k-=8 )\
    {\
        d_bo+=32; d_ao+=32;\
        bo0=*((const uint4*)&d_bo[ 0] );\
	    bo1=*((const uint4*)&d_bo[16] );\
        q0=*((const float*)&d_b[bo0.x]);\
        q1=*((const float*)&d_b[bo0.y]);\
        q2=*((const float*)&d_b[bo0.z]);\
        q3=*((const float*)&d_b[bo0.w]);\
        q4=*((const float*)&d_b[bo1.x]);\
        q5=*((const float*)&d_b[bo1.y]);\
        q6=*((const float*)&d_b[bo1.z]);\
        q7=*((const float*)&d_b[bo1.w]);\
        ao0=*((const uint4*)&d_ao[ 0] );\
	    ao1=*((const uint4*)&d_ao[16] );\
        p0=*((const float*)&d_a[ao0.x]);\
        p1=*((const float*)&d_a[ao0.y]);\
        p2=*((const float*)&d_a[ao0.z]);\
        p3=*((const float*)&d_a[ao0.w]);\
        p4=*((const float*)&d_a[ao1.x]);\
        p5=*((const float*)&d_a[ao1.y]);\
        p6=*((const float*)&d_a[ao1.z]);\
        p7=*((const float*)&d_a[ao1.w]);\
        b[2]=*((float4*)&bsld[1*0x80+0x00]);\
        a[1]=*((float4*)&asld[1*0x80+0x00]);\
        b[3]=*((float4*)&bsld[1*0x80+0x40]);\
        BOP4x8(c,&a[0],&b[0])               \
        b[0]=*((float4*)&bsld[2*0x80+0x00]);\
        a[0]=*((float4*)&asld[2*0x80+0x00]);\
        b[1]=*((float4*)&bsld[2*0x80+0x40]);\
        BOP4x8(c,&a[1],&b[2])               \
        b[2]=*((float4*)&bsld[3*0x80+0x00]);\
        a[1]=*((float4*)&asld[3*0x80+0x00]);\
        b[3]=*((float4*)&bsld[3*0x80+0x40]);\
        BOP4x8(c,&a[0],&b[0])               \
        b[0]=*((float4*)&bsld[4*0x80+0x00]);\
        a[0]=*((float4*)&asld[4*0x80+0x00]);\
        b[1]=*((float4*)&bsld[4*0x80+0x40]);\
        BOP4x8(c,&a[1],&b[2])               \
        b[2]=*((float4*)&bsld[5*0x80+0x00]);\
        a[1]=*((float4*)&asld[5*0x80+0x00]);\
        b[3]=*((float4*)&bsld[5*0x80+0x40]);\
        char* __restrict__ sst=sst_base+ofs;\
        BOP4x8(c,&a[0],&b[0])               \
        b[0]=*((float4*)&bsld[6*0x80+0x00]);\
        a[0]=*((float4*)&asld[6*0x80+0x00]);\
        b[1]=*((float4*)&bsld[6*0x80+0x40]);\
        *((float*)&sst[0*128])=p0;\
        *((float*)&sst[1*128])=p1;\
        *((float*)&sst[2*128])=p2;\
        *((float*)&sst[3*128])=p3;\
        *((float*)&sst[4*128])=p4;\
        *((float*)&sst[5*128])=p5;\
        *((float*)&sst[6*128])=p6;\
        *((float*)&sst[7*128])=p7;\
        BOP4x8(c,&a[1],&b[2])\
        b[2]=*((float4*)&bsld[7*0x80+0x00]);\
        a[1]=*((float4*)&asld[7*0x80+0x00]);\
        b[3]=*((float4*)&bsld[7*0x80+0x40]);\
        BOP4x8(c,&a[0],&b[0])\
        *((float*)&sst[ 8*128])=q0;\
        *((float*)&sst[ 9*128])=q1;\
        *((float*)&sst[10*128])=q2;\
        *((float*)&sst[11*128])=q3;\
        *((float*)&sst[12*128])=q4;\
        *((float*)&sst[13*128])=q5;\
        *((float*)&sst[14*128])=q6;\
        *((float*)&sst[15*128])=q7;\
        asld=asld_base+ofs;\
        bsld=bsld_base+ofs;\
        __syncthreads();\
        b[0]=*((float4*)&bsld[0x00]);\
        a[0]=*((float4*)&asld[0x00]);\
        b[1]=*((float4*)&bsld[0x40]);\
        BOP4x8(c,&a[1],&b[2])\
        ofs^=0x800;\
    }\
    b[2]=*((float4*)&bsld[1*0x80+0x00]);\
    a[1]=*((float4*)&asld[1*0x80+0x00]);\
    b[3]=*((float4*)&bsld[1*0x80+0x40]);\
    BOP4x8(c,&a[0],&b[0])               \
    b[0]=*((float4*)&bsld[2*0x80+0x00]);\
    a[0]=*((float4*)&asld[2*0x80+0x00]);\
    b[1]=*((float4*)&bsld[2*0x80+0x40]);\
    BOP4x8(c,&a[1],&b[2])               \
    b[2]=*((float4*)&bsld[3*0x80+0x00]);\
    a[1]=*((float4*)&asld[3*0x80+0x00]);\
    b[3]=*((float4*)&bsld[3*0x80+0x40]);\
    BOP4x8(c,&a[0],&b[0])               \
    b[0]=*((float4*)&bsld[4*0x80+0x00]);\
    a[0]=*((float4*)&asld[4*0x80+0x00]);\
    b[1]=*((float4*)&bsld[4*0x80+0x40]);\
    BOP4x8(c,&a[1],&b[2])               \
    b[2]=*((float4*)&bsld[5*0x80+0x00]);\
    a[1]=*((float4*)&asld[5*0x80+0x00]);\
    b[3]=*((float4*)&bsld[5*0x80+0x40]);\
    BOP4x8(c,&a[0],&b[0])               \
    b[0]=*((float4*)&bsld[6*0x80+0x00]);\
    a[0]=*((float4*)&asld[6*0x80+0x00]);\
    b[1]=*((float4*)&bsld[6*0x80+0x40]);\
    BOP4x8(c,&a[1],&b[2])               \
    b[2]=*((float4*)&bsld[7*0x80+0x00]);\
    a[1]=*((float4*)&asld[7*0x80+0x00]);\
    b[3]=*((float4*)&bsld[7*0x80+0x40]);\
    BOP4x8(c,&a[0],&b[0])\
    BOP4x8(c,&a[1],&b[2])\
    sgemm_epilog32x32##suffix( d_c, grad?d_x:0, smem, c, tid, ldc, x, cnr, pnc, alpha );\
}

scorr_32x32(0,)
scorr_32x32(1,_drelu)
scorr_32x32(1,_xdrv)
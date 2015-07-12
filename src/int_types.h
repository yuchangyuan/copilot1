#ifndef _INT_TYPES_H_
#define _INT_TYPES_H_

#include <stdint.h>

typedef uint8_t u8;
typedef int8_t  s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;

typedef int64_t s64;
typedef uint64_t u64;

typedef volatile u8 vu8;
typedef volatile s8 vs8;
typedef volatile u16 vu16;
typedef volatile s16 vs16;
typedef volatile u32 vu32;
typedef volatile s32 vs32;

#ifndef NO_STDBOOL
#include <stdbool.h>
#else
#define bool int
#define false 0
#define true 1
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif

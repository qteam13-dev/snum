/*********************************************************************************************/

#ifndef S_INC_SNUM
#   define S_INC_SNUM 1

#   ifdef __cplusplus
extern "C" {
#   endif

/*********************************************************************************************/

typedef          void  void_t;
typedef          char  char_t;
typedef signed   char  sint1_t;
typedef signed   short sint2_t;
typedef signed   long  sint4_t;
typedef unsigned char  uint1_t;
typedef unsigned short uint2_t;
typedef unsigned long  uint4_t;
typedef          float real4_t;

typedef union conv4_t {
    char_t  ch[4];
    sint1_t s1[4];
    sint2_t s2[2];
    sint4_t s4;
    uint1_t u1[4];
    uint2_t u2[2];
    uint4_t u4;
    real4_t r4;
} conv4_t;

/* ========================================================================================= */

#   define SNUM_NONE 0
#   define SNUM_BIN  1
#   define SNUM_OCT  2
#   define SNUM_HEX  4
#   define SNUM_REAL 8
#   define SNUM_SIGN 16
#   define SNUM_1B   32
#   define SNUM_2B   64
#   define SNUM_4B   128
#   define SNUM_INF  256
#   define SNUM_NAN  512

/* ========================================================================================= */

typedef struct num4_t {
    uint4_t isec;
    uint4_t dsec;
    uint2_t flags;
    uint1_t dpos;
} num4_t;

/* ========================================================================================= */

/*
 * @why:  to convert a string to a number structure
 * @in:
 *        dst: destination structure
 *        src: source string
 * @out:  returns dst if it has been successful,
 *        otherwise NULL
 * @info:
 *        binary   | 0b101[1]
 *        octal    | 052
 *        hexadec. | 0x40[1]
 *        decimal  | 123 or 456.1607
 *        sign(+)  | supported
 *        sign(-)  | supported
 *        size(1)  | supported
 *        size(2)  | supported
 *        size(4)  | supported
 *        size(8)  | unsupported
 *        reals    | supported (base-10 only)
 *        exponent | unsupported
 *        infinity | supported (INF[1])
 *        NaN      | supported (NAN[1])
 *        unsigned | supported (automatic only)
 * [1] case insensitive
 */
num4_t* snum_ston(num4_t* dst, const char_t* src);

/*
 * @why:  to convert a number structure into a value
 * @in:   src: source structure
 * @out:  returns the value converted
 */
conv4_t snum_ntov(num4_t* src);

/*********************************************************************************************/

#   ifdef __cplusplus
}
#   endif

#endif

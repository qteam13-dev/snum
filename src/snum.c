#include "snum.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*********************************************************************************************/

num4_t* snum_ston(num4_t* dst, const char_t* src) {

    /* parse sign */
    switch (*src) {
    case 45: dst->flags |= SNUM_SIGN;
    case 43: ++src;                   break;
    default:                          break;
    }

    /* parse constants */
    if (stricmp(src, "INF") == 0) {
        dst->flags |= SNUM_INF;
        dst->isec = dst->dsec = 0;
        dst->dpos = 0;
        return dst;
    } else if (stricmp(src, "NAN") == 0) {
        dst->flags |= SNUM_NAN;
        dst->isec = dst->dsec = 0;
        dst->dpos = 0;
        return dst;
    }

    /* parse base */
    uint1_t base = 10;
    switch (*src) {
    case 48:
        switch (*(++src)) {
        case 98:  case 66: /* bin */ dst->flags |= SNUM_BIN; ++src; base = 2;  break;
        case 120: case 88: /* hex */ dst->flags |= SNUM_HEX; ++src; base = 16; break;
        default:           /* oct */ dst->flags |= SNUM_OCT; ++src; base = 8;  break;
        }
        break;
    default: break;
    }

    while (*src) {
        /* parse real section */
        if (dst->flags & SNUM_REAL) {
            if (*src >= 48 && *src <= 57) dst->dsec += (*src - 48);
            else { fprintf(stderr, "\nsnum> [error] excepted a value in range [0-9] (got \'%c\')\n", *src); goto error; }
            if (*(src + 1)) dst->dsec *= 10;
            ++dst->dpos;
        } /* parse integer section */ else {
            if (dst->flags & SNUM_BIN) {
                if (*src == 48 || *src == 49) dst->isec += (*src - 48);
                else { fprintf(stderr, "\nsnum> [error] excepted 1 or 0 (got \'%c\')\n", *src); goto error; }
            } else if (dst->flags & SNUM_OCT) {
                if (*src >= 48 && *src <= 55) dst->isec += (*src - 48);
                else { fprintf(stderr, "\nsnum> [error] excepted a value in range [0-7] (got \'%c\')\n", *src); goto error; }
            } else if (dst->flags & SNUM_HEX) {
                if (*src >= 48 && *src <= 57)       dst->isec += (*src - 48);
                else if (*src >= 65 && *src <= 70)  dst->isec += (*src - 55);
                else if (*src >= 97 && *src <= 102) dst->isec += (*src - 87);
                else { fprintf(stderr, "\nsnum> [error] excepted a value in range [0-9], [A-F] or [a-f] (got \'%c\')\n", *src); goto error; }
            } else {
                if (*src >= 48 && *src <= 57) dst->isec += (*src - 48);
                else if (*src == 46) { dst->flags |= SNUM_REAL; ++src; continue; }
                else { fprintf(stderr, "\nsnum> [error] excepted a value in range [0-9] or \'.\' (got \'%c\')\n", *src); goto error; }
            }
            if (*(src + 1) && *(src + 1) != 46) dst->isec *= base;
        }
        ++src;
    }

    /* parse size */
    if (dst->flags & SNUM_REAL) {
        if (!dst->dpos) { fprintf(stderr, "\nsnum> [error] unexcepted '.'\n", *src); goto error; }
        dst->flags |= SNUM_4B;
    }
    else if (dst->flags & SNUM_SIGN) {
        if      (dst->isec <= 0x80)   dst->flags |= SNUM_1B;
        else if (dst->isec <= 0x8000) dst->flags |= SNUM_2B;
        else                          dst->flags |= SNUM_4B;
    } else {
        if      (dst->isec <= 0xFF)   dst->flags |= SNUM_1B;
        else if (dst->isec <= 0xFFFF) dst->flags |= SNUM_2B;
        else                          dst->flags |= SNUM_4B;
    }
    return dst;

error:
    dst->isec  = dst->dsec = 0;
    dst->flags = dst->dpos = 0;
    return NULL;
}

/* ========================================================================================= */

conv4_t snum_ntov(num4_t* src) {
    conv4_t conv;
    conv.u4 = 0;
    if (src->flags & SNUM_INF) {
        if (src->flags & SNUM_SIGN) conv.r4 = -INFINITY;
        else                        conv.r4 = INFINITY;
    } else if (src->flags & SNUM_INF) conv.r4 = NAN;
    else if (src->flags & SNUM_REAL) {
        uint1_t pos = src->dpos;
        uint4_t dec = src->dsec;
        uint4_t k   = 1;
        while (pos--) k *= 10;
        conv.r4 = src->isec;
        conv.r4 += src->dsec / (real4_t)k;
        if (src->flags & SNUM_SIGN) conv.u4 |= 0x80000000;
    } else if (src->flags & SNUM_SIGN) {
        if      (src->flags & SNUM_1B) { conv.s1[0] = src->isec; conv.u1[0] |= 0x80; }
        else if (src->flags & SNUM_2B) { conv.s2[0] = src->isec; conv.u2[0] |= 0x8000; }
        else                           { conv.s4 = src->isec;    conv.u4 |= 0x80000000; }
    } else {
        if      (src->flags & SNUM_1B) conv.s1[0] = src->isec;
        else if (src->flags & SNUM_2B) conv.s2[0] = src->isec;
        else                           conv.s4 = src->isec;
    }
    return conv;
}

/*********************************************************************************************/

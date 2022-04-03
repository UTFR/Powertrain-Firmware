/* This file contains helpful macros and constants for
 * the University of Toronto's Formula SAE Racing Team.
 * 
 * Contributors: Jake Sprenger, 2021
 */

#ifndef LIB_UTIL_H
#define LIB_UTIL_H

#define BIT_X(pos, size)                    ((uint##size##_t)((uint##size##_t)1U << ((uint_least8_t)(pos))))
#define GET_BIT_X(data, pos, size)          ((bool)(((uint##size##_t)(data) & BIT_X((pos), size)) != 0U))
#define SET_BIT_X(data, pos, size)          ((uint##size##_t)(data) | BIT_X((pos), size))

#define GET_BIT_U8(data, pos)               GET_BIT_X((data), (pos), 8)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr)                     ((size_t)(sizeof(arr)/sizeof((arr)[0])))
#endif

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

template <class X, class M, class N, class O, class Q>
X map_Generic(X x, M in_min, N in_max, O out_min, Q out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#endif /* LIB_UTIL_H


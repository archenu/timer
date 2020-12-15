#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__




















typedef unsigned char sl_uint8_t;
typedef unsigned short sl_uint16_t;
typedef unsigned int   sl_uint32_t;
typedef unsigned long  sl_uint64_t;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
typedef signed char  sl_int8_t;
typedef signed short sl_int16_t;
typedef signed int   sl_int32_t;
typedef signed long  sl_int64_t;    


#define  ERR_OK     1
#define  ERR_FULL   -1
#define  ERR_EMPTY  -2
#define  ERR_NOT_EXIST -3

typedef sl_int8_t sl_err_t;


#ifndef  TRUE
#define  TRUE  1
#endif

#ifndef  FALSE
#define  FALSE  0
#endif

#endif
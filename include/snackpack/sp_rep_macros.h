#ifndef _SNACKPACK_SP_REP_MACROS_H_
#define _SNACKPACK_SP_REP_MACROS_H_


/* These are used to statically allocate memory for vectors and matrices via
 * the SP_VECTOR_NEW / SP_MATRIX_NEW macros.
 */
#define SP_REP0(...)
#define SP_REP1(...)        __VA_ARGS__,
#define SP_REP2(...)        SP_REP1(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP3(...)        SP_REP2(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP4(...)        SP_REP3(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP5(...)        SP_REP4(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP6(...)        SP_REP5(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP7(...)        SP_REP6(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP8(...)        SP_REP7(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP9(...)        SP_REP8(__VA_ARGS__)    SP_REP1(__VA_ARGS__)

#define SP_REP10(...)       SP_REP9(__VA_ARGS__)    SP_REP1(__VA_ARGS__)
#define SP_REP11(...)       SP_REP10(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP12(...)       SP_REP11(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP13(...)       SP_REP12(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP14(...)       SP_REP13(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP15(...)       SP_REP14(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP16(...)       SP_REP15(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP17(...)       SP_REP16(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP18(...)       SP_REP17(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP19(...)       SP_REP18(__VA_ARGS__)   SP_REP1(__VA_ARGS__)

#define SP_REP20(...)       SP_REP19(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP21(...)       SP_REP20(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP22(...)       SP_REP21(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP23(...)       SP_REP22(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP24(...)       SP_REP23(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP25(...)       SP_REP24(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP26(...)       SP_REP25(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP27(...)       SP_REP26(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP28(...)       SP_REP27(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP29(...)       SP_REP28(__VA_ARGS__)   SP_REP1(__VA_ARGS__)

#define SP_REP30(...)       SP_REP29(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP31(...)       SP_REP30(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP32(...)       SP_REP31(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP33(...)       SP_REP32(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP34(...)       SP_REP33(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP35(...)       SP_REP34(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP36(...)       SP_REP35(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP37(...)       SP_REP36(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP38(...)       SP_REP37(__VA_ARGS__)   SP_REP1(__VA_ARGS__)
#define SP_REP39(...)       SP_REP38(__VA_ARGS__)   SP_REP1(__VA_ARGS__)

#define SP_REP40(...)       SP_REP39(__VA_ARGS__)   SP_REP1(__VA_ARGS__)


#define SP_REP0X(X, ...)
#define SP_REP1X(X, ...)    SP_REP##X(__VA_ARGS__)
#define SP_REP2X(X, ...)    SP_REP1X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP3X(X, ...)    SP_REP2X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP4X(X, ...)    SP_REP3X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP5X(X, ...)    SP_REP4X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP6X(X, ...)    SP_REP5X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP7X(X, ...)    SP_REP6X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP8X(X, ...)    SP_REP7X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP9X(X, ...)    SP_REP8X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)
#define SP_REP10X(X, ...)   SP_REP9X(X, __VA_ARGS__)    SP_REP##X(__VA_ARGS__)

#define SP_REP11X(X, ...)   SP_REP10X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP12X(X, ...)   SP_REP11X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP13X(X, ...)   SP_REP12X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP14X(X, ...)   SP_REP13X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP15X(X, ...)   SP_REP14X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP16X(X, ...)   SP_REP15X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP17X(X, ...)   SP_REP16X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP18X(X, ...)   SP_REP17X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP19X(X, ...)   SP_REP18X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP20X(X, ...)   SP_REP19X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)

#define SP_REP21X(X, ...)   SP_REP20X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP22X(X, ...)   SP_REP21X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP23X(X, ...)   SP_REP22X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP24X(X, ...)   SP_REP23X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP25X(X, ...)   SP_REP24X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP26X(X, ...)   SP_REP25X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP27X(X, ...)   SP_REP26X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP28X(X, ...)   SP_REP27X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP29X(X, ...)   SP_REP28X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)
#define SP_REP30X(X, ...)   SP_REP29X(X, __VA_ARGS__)   SP_REP##X(__VA_ARGS__)

#endif

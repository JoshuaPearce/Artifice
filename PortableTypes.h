//PortableTypes.h

#ifndef PortableTypes_h
#define PortableTypes_h

#include "endian.hpp"

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef unsigned char uchar;
typedef unsigned int uint;

////////////////////////////////
/* //Use this section if for some reason <stdint.h> isn't available (or use Boost::Integer, of course)
template<int I> struct ListOfIntegerTypes;
template<> struct ListOfIntegerTypes<0> { typedef char i; typedef unsigned char ui; };
template<> struct ListOfIntegerTypes<1> { typedef short i; typedef unsigned short ui; };
template<> struct ListOfIntegerTypes<2> { typedef int i; typedef unsigned int ui; };
template<> struct ListOfIntegerTypes<3> { typedef long i; typedef long ui; };
template<> struct ListOfIntegerTypes<4> { typedef long long i; typedef unsigned long long ui; };

template<int bytes, int counter=0, bool matched= (sizeof(typename ListOfIntegerTypes<counter>::i) == bytes)> struct ChooseInteger {
	typedef typename ChooseInteger<bytes, counter + 1>::i i;
	typedef typename ChooseInteger<bytes, counter + 1>::ui ui;
};

template<int bytes, int counter> struct ChooseInteger<bytes, counter, true> {
	typedef typename ListOfIntegerTypes<counter>::i i;
	typedef typename ListOfIntegerTypes<counter>::ui ui;
};

typedef ChooseInteger<1>::i int8;
typedef ChooseInteger<2>::i int16;
typedef ChooseInteger<4>::i int32;
typedef ChooseInteger<8>::i int64;
typedef ChooseInteger<1>::ui uint8;
typedef ChooseInteger<2>::ui uint16;
typedef ChooseInteger<4>::ui uint32;
typedef ChooseInteger<8>::ui uint64;
*/

////////////////////////////////
#ifndef byte
  typedef uint8 byte;
#endif

template<bool C> class StaticAssert_t { char errorbait[C ? 1 : -1]; };

#define STATIC_ASSERT(CONDITION) namespace { extern const StaticAssert_t<!!(CONDITION)> ___assertion; };
#define STATIC_MEMBERASSERT(CONDITION, NAME) struct NAME { StaticAssert_t<!!(CONDITION)> ___assertion; };

STATIC_ASSERT(sizeof(int8) == 1);
STATIC_ASSERT(sizeof(int16) == 2);
STATIC_ASSERT(sizeof(int32) == 4);
STATIC_ASSERT(sizeof(int64) == 8);
//STATIC_ASSERT(sizeof(float32) == 4);
//STATIC_ASSERT(sizeof(float64) == 8);

////////////////////////////////
struct NullArgument { };
const NullArgument NullArg;
template <typename T> struct NullArgument_t { };

template<typename TYPE> class QualifyType {
  public: typedef typename TYPE plain;
  public: typedef typename const TYPE constant;
  public: typedef typename TYPE unconst;
  public: typedef typename TYPE unref;
  public: typedef typename const TYPE constunref;
  public: typedef typename TYPE& ref;
  public: typedef typename const TYPE& constref;
};

template<typename TYPE> class QualifyType<const TYPE> {
  public: typedef typename TYPE plain;
  public: typedef typename const TYPE constant;
  public: typedef typename TYPE unconst;
  public: typedef typename const TYPE unref;
  public: typedef typename const TYPE constunref;
  public: typedef typename const TYPE& ref;
  public: typedef typename const TYPE& constref;
};

template<typename TYPE> class QualifyType<TYPE&> {
  public: typedef typename TYPE plain;
  public: typedef typename const TYPE& constant;
  public: typedef typename TYPE& unconst;
  public: typedef typename TYPE unref;
  public: typedef typename const TYPE constunref;
  public: typedef typename TYPE& ref;
  public: typedef typename const TYPE& constref;
};

template<typename TYPE> class QualifyType<const TYPE&> {
  public: typedef typename TYPE plain;
  public: typedef typename const TYPE& constant;
  public: typedef typename TYPE& unconst;
  public: typedef typename const TYPE unref;
  public: typedef typename const TYPE constunref;
  public: typedef typename const TYPE& ref;
  public: typedef typename const TYPE& constref;
};

////////////////////////////////
template<typename A, typename B> struct is_same { static const bool value= false; };
template<typename A> struct is_same<A, A> { static const bool value= true; };

////////////////////////////////
template<bool Condition, typename type_if_true, typename type_if_false= NullArgument> 
  struct conditional_type { typedef type_if_false value; };
template<typename type_if_true, typename type_if_false> 
  struct conditional_type<true, type_if_true, type_if_false> { typedef type_if_true value; };

////////////////////////////////
template<bool Condition, typename T>
  struct const_if: conditional_type<Condition, typename QualifyType<T>::constant, typename QualifyType<T>::unconst> { };

////////////////////////////////
template<bool Condition, typename T>
  struct inherit_if: conditional_type<Condition, T, NullArgument_t<T>> { };

////////////////////////////////
template<typename T> struct IsReference { static const bool value= false; };
template<typename T> struct IsReference<T&> { static const bool value= true; };

//IsPointer should be overloaded for smart pointers or similar wrappers
template<typename T> struct IsPointer { static const bool value= false; };
template<typename T> struct IsPointer<T*> { static const bool value= true; };

////////////////////////////////
//These templates exist to get around the trouble which can occur when adding unsigned to template args, or when trying to remove it
template<typename TYPE> struct SignedInteger;
template<typename TYPE> struct UnsignedInteger;

template<> struct SignedInteger<char> { typedef signed char t; };
template<> struct SignedInteger<signed char> { typedef signed char t; };
template<> struct SignedInteger<int16> { typedef int16 t; };
template<> struct SignedInteger<int32> { typedef int32 t; };
template<> struct SignedInteger<int64> { typedef int64 t; };
template<> struct SignedInteger<long> { typedef long t; };

template<> struct SignedInteger<unsigned char> { typedef signed char t; };
template<> struct SignedInteger<uint16> { typedef int16 t; };
template<> struct SignedInteger<uint32> { typedef int32 t; };
template<> struct SignedInteger<uint64> { typedef int64 t; };
template<> struct SignedInteger<unsigned long> { typedef long t; };

template<> struct UnsignedInteger<char> { typedef unsigned char t; };
template<> struct UnsignedInteger<signed char> { typedef unsigned char t; };
template<> struct UnsignedInteger<int16> { typedef uint16 t; };
template<> struct UnsignedInteger<int32> { typedef uint32 t; };
template<> struct UnsignedInteger<int64> { typedef uint64 t; };
template<> struct UnsignedInteger<long> { typedef unsigned long t; };

template<> struct UnsignedInteger<unsigned char> { typedef unsigned char t; };
template<> struct UnsignedInteger<uint16> { typedef uint16 t; };
template<> struct UnsignedInteger<uint32> { typedef uint32 t; };
template<> struct UnsignedInteger<uint64> { typedef uint64 t; };
template<> struct UnsignedInteger<unsigned long> { typedef unsigned long t; };

#ifndef ssize_t
	typedef SignedInteger<size_t>::t ssize_t;
#endif

////////////////////////////////
template<typename TYPE> struct BestFloat { typedef double t; };
template<> struct BestFloat<char> { typedef float t; };
template<> struct BestFloat<signed char> { typedef float t; };
template<> struct BestFloat<unsigned char> { typedef float t; };
template<> struct BestFloat<int16> { typedef float t; };
template<> struct BestFloat<uint16> { typedef float t; };
template<> struct BestFloat<float> { typedef float t; };
template<> struct BestFloat<wchar_t> { typedef float t; };

////////////////////////////////
template<typename TYPE> inline void DefaultInitializer(TYPE&) {};
template<typename TYPE> inline void DefaultInitializer(TYPE*& v) { v= 0; };
inline void DefaultInitializer(bool& v) { v= false; };
inline void DefaultInitializer(char& v) { v= 0; };
inline void DefaultInitializer(signed char& v) { v= 0; };
inline void DefaultInitializer(unsigned char& v) { v= 0; };
inline void DefaultInitializer(int16& v) { v= 0; };
inline void DefaultInitializer(int32& v) { v= 0; };
inline void DefaultInitializer(int64& v) { v= 0; };
inline void DefaultInitializer(uint16& v) { v= 0; };
inline void DefaultInitializer(uint32& v) { v= 0; };
inline void DefaultInitializer(uint64& v) { v= 0; };
inline void DefaultInitializer(wchar_t& v) { v= 0; };
inline void DefaultInitializer(long& v) { v= 0; };
inline void DefaultInitializer(unsigned long& v) { v= 0; };
inline void DefaultInitializer(float& v) { v= 0; };
inline void DefaultInitializer(double& v) { v= 0; };
inline void DefaultInitializer(long double& v) { v= 0; };

//This "class" provides a simple way to implement a move constructor.
//It's in this file because I couldn't think of a better header, and it wasn't worth a new file
//Suggested usage: Foo::Foo(MoveOperator, Foo& source) { ... };
//Calling it: Foo foo_original(...); Foo foo(Move, foo_original);
class MoveOperator { };
const MoveOperator Move;

class NoCopy { private: NoCopy(const NoCopy&); };
class NoAssignment { private: void operator= (const NoAssignment&); };

#define DISABLE_COPY(CLASS) \
  private: CLASS(const CLASS&); \
  private: CLASS& operator=(const CLASS&);

#endif //#ifndef PortableTypes_h


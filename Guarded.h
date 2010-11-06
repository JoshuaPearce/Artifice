//Guarded.h
#ifndef Guarded_h
#define Guarded_h

#include "PortableTypes.h"

//This macro will need to be updated for extra pedantic compilers which don't support undecorated friend declarations
//That may require giving public write access, effectively making Guarded a lot less useful for that compiler

template<typename T> struct IndirectTypedef { typedef T v; };

#if _MSC_VER
	//Technically VS is not C++ standard in this regard, but it's a lot easier because of that
	#define GUARDED_FRIEND(___OWNER) friend ___OWNER;
	#define GUARDED_ACCESSOR protected
#elif __GNUC__
  //GCC is a bit pickier than VS, but still not completely conforming. It requires some misdirection
	#define GUARDED_FRIEND(___OWNER) friend IndirectTypedef<___OWNER>::v;
	#define GUARDED_ACCESSOR protected
#elif __COMO__
  //Comeau is very strict in this regard to the standard, so we have to disable the feature
	#define GUARDED_FRIEND(___OWNER)
	#define GUARDED_ACCESSOR public
#else
	#define GUARDED_FRIEND(___OWNER)
	#define GUARDED_ACCESSOR public
#endif

#define GUARDED_ASSIGNMENT_MEMBERS \
	template<typename THAT> TYPE &operator = (const THAT &that) { return (*(TYPE*)this = that); }; \
	template<typename THAT> TYPE &operator += (const THAT &that) { return (*(TYPE*)this += that); }; \
	template<typename THAT> TYPE &operator -= (const THAT &that) { return (*(TYPE*)this -= that); }; \
	template<typename THAT> TYPE &operator /= (const THAT &that) { return (*(TYPE*)this /= that); }; \
	template<typename THAT> TYPE &operator %= (const THAT &that) { return (*(TYPE*)this %= that); }; \
	template<typename THAT> TYPE &operator *= (const THAT &that) { return (*(TYPE*)this *= that); }; \
	template<typename THAT> TYPE &operator &= (const THAT &that) { return (*(TYPE*)this &= that); }; \
	template<typename THAT> TYPE &operator |= (const THAT &that) { return (*(TYPE*)this |= that); }; \
	template<typename THAT> TYPE &operator ^= (const THAT &that) { return (*(TYPE*)this ^= that); }; \
	template<typename THAT> TYPE &operator <<= (const THAT &that) { return (*(TYPE*)this <<= that); }; \
	TYPE &operator ++() { return ++(*(TYPE*)this); }; \
	TYPE operator ++(int) { return (*(TYPE*)this)++; }; \
	TYPE &operator --() { return --(*(TYPE*)this); }; \
	TYPE operator --(int) { return (*(TYPE*)this)--; };

////////////////////////////////////////////////////////////////
template<typename TYPE, class OWNER, class OWNER2=NullArgument> class Guarded :
//This is the default Guarded specialization, used for classes
//It provides public access to members, for simplicity's sake
	public TYPE
{
public:
	Guarded() {	};
	Guarded(const TYPE &that) : TYPE(that) { };

GUARDED_ACCESSOR:
	TYPE& get() { return value; };
	TYPE& operator= (const TYPE& v) { return *(TYPE*)this= v; };
	using TYPE::operator=;
	GUARDED_ASSIGNMENT_MEMBERS;

	GUARDED_FRIEND(OWNER);
	GUARDED_FRIEND(OWNER2);
};

////////////////////////////////////////////////////////////////
template<class OWNER, class OWNER2> class Guarded<bool, OWNER, OWNER2> {
public:
	Guarded() : value(false) {	};
	Guarded(bool v) : value(v) { };

	operator const bool&() const { return value; };

GUARDED_ACCESSOR:
	bool& get() { return value; };
	bool& operator = (bool v) { return value= v; };
	bool& operator |= (bool v) { return value |= v; };
	bool& operator &= (bool v) { return value &= v; };

private:
	bool value;

	GUARDED_FRIEND(OWNER);
	GUARDED_FRIEND(OWNER2);
};

////////////////////////////////////////////////////////////////

template<typename TYPE, class OWNER, class OWNER2> class GuardedInteger {
public:
	GuardedInteger() : value(0) { };
	GuardedInteger(TYPE v) : value(v) { };

	operator const TYPE&() const { return value; };

GUARDED_ACCESSOR: 
	TYPE& get() { return value; };
	TYPE& operator = (TYPE v) { return value= v; };
	template<typename THAT> TYPE& operator += (const THAT& v) { return value += v; };
	template<typename THAT> TYPE& operator -= (const THAT& v) { return value -= v; };
	template<typename THAT> TYPE& operator *= (const THAT& v) { return value *= v; };
	template<typename THAT> TYPE& operator /= (const THAT& v) { return value /= v; };
	template<typename THAT> TYPE& operator %= (const THAT& v) { return value %= v; };
	template<typename THAT> TYPE& operator |= (const THAT& v) { return value |= v; };
	template<typename THAT> TYPE& operator &= (const THAT& v) { return value &= v; };
	template<typename THAT> TYPE& operator ^= (const THAT& v) { return value ^= v; };
	template<typename THAT> TYPE& operator <<= (const THAT& v) { return value <<= v; };
	template<typename THAT> TYPE& operator >>= (const THAT& v) { return value >>= v; };
	TYPE& operator ++ () { return ++value; };
	TYPE operator ++ (int) { return value++; };
	TYPE& operator -- () { return --value; };
	TYPE operator -- (int) { return value--; };

private:
	TYPE value;

	GUARDED_FRIEND(OWNER);
	GUARDED_FRIEND(OWNER2);
};

#define GUARDED_INTEGER(TYPE) \
template<class OWNER, class OWNER2> class Guarded<TYPE, OWNER, OWNER2> : \
  public GuardedInteger<TYPE, OWNER, OWNER2> \
{ \
public: \
	Guarded() { }; \
	Guarded(TYPE v) : GuardedInteger(v) { }; \
\
GUARDED_ACCESSOR: \
	using GuardedInteger<TYPE, OWNER, OWNER2>::operator=; \
\
	GUARDED_FRIEND(OWNER); \
	GUARDED_FRIEND(OWNER2); \
};

GUARDED_INTEGER(char);
GUARDED_INTEGER(signed char);
GUARDED_INTEGER(unsigned char);
//GUARDED_INTEGER(int8);
//GUARDED_INTEGER(uint8);
GUARDED_INTEGER(int16);
GUARDED_INTEGER(uint16);
GUARDED_INTEGER(int32);
GUARDED_INTEGER(uint32);
GUARDED_INTEGER(int64);
GUARDED_INTEGER(uint64);
GUARDED_INTEGER(long);
GUARDED_INTEGER(unsigned long);

////////////////////////////////////////////////////////////////
#define GUARDED_FLOATINGPOINT(TYPE) \
template<class OWNER, class OWNER2> class Guarded<TYPE, OWNER, OWNER2> { \
public: \
	Guarded() : value(0) { }; \
	Guarded(TYPE v) : value(v) { }; \
	operator const TYPE&() const { return value; }; \
GUARDED_ACCESSOR: \
	TYPE& get() { return value; }; \
	TYPE& operator = (TYPE v) { return value= v; }; \
	template<typename THAT> TYPE& operator += (const THAT& v) { return value += v; }; \
	template<typename THAT> TYPE& operator -= (const THAT& v) { return value -= v; }; \
	template<typename THAT> TYPE& operator *= (const THAT& v) { return value *= v; }; \
	template<typename THAT> TYPE& operator /= (const THAT& v) { return value /= v; }; \
	TYPE& operator ++ () { return ++value; }; \
	TYPE operator ++ (int) { return value++; }; \
	TYPE& operator -- () { return --value; }; \
	TYPE operator -- (int) { return value--; }; \
private: \
	TYPE value; \
	GUARDED_FRIEND(OWNER); \
	GUARDED_FRIEND(OWNER2); \
};

GUARDED_FLOATINGPOINT(float);
GUARDED_FLOATINGPOINT(double);
GUARDED_FLOATINGPOINT(long double);

////////////////////////////////////////////////////////////////
template<typename TYPE, class OWNER, class OWNER2> class Guarded<TYPE*, OWNER, OWNER2> { 
public:
	Guarded() : value(0) { };
	Guarded(TYPE* v) : value(v) { };

	operator TYPE* () const { return value; };
	TYPE* operator -> () const { return value; };
	TYPE& operator [] (int X) const { return value[X]; };
	TYPE& operator *() const { return *value; };

	TYPE* operator + (size_t X) const { return (value + X); };
	TYPE* operator - (size_t X) const { return (value - X); };
	size_t operator + (TYPE* that) const {  return (value + X); }; 
	size_t operator - (TYPE* that) const {  return (value - X); }; 

GUARDED_ACCESSOR:
	TYPE& get() { return value; };
	TYPE*& operator = (TYPE* that) { return (value= that); };
	TYPE*& operator += (size_t X) { return (value += X); };
	TYPE*& operator -= (size_t X) { return (value -= X); };

	TYPE*& operator ++() { return ++value; };
	TYPE* operator ++(int) { return value++; };
	TYPE*& operator --() { return --value; };
	TYPE* operator --(int) { return value--; };

private:
	TYPE* value;

	GUARDED_FRIEND(OWNER);
	GUARDED_FRIEND(OWNER2);
};

#endif //#ifndef Guarded_h
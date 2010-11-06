//Singleton.h (pronounced SingleToN)
//usage: const Singleton<FooClass> Foo; 
//After declaring that, you can use Foo as if it were a valid FooClass* (EG Foo->member(); or FooClass* foo2= Foo; )
//FooClass::FooClass() will be called the first time Foo is used (even in a header, etc, across translation units)
//No definition of Foo is needed other than the declaration.
//The FooClass will be destroyed when all Singleton<FooClass> are out of scope (which means end of program for global scope objects)
//There's no reason not to use multiple FooClass instances, if you want, they won't conflict with the one stored in the Singleton
//FooClass doesn't need any special behaviors, or static members.

/*
	This file was written by Joshua Pearce, http://joshuapearce.com , joshua@bbeg.org
 */

/*LICENSING:
	This library is under the Ms-RL license (see http://www.opensource.org/licenses/ms-rl.html )
	In layman's terms, the license is as follows:
		Nobody owes you (the end user) any responsibility or liability for how well this code works, or for any unintended side effects it has.
		You can use compiled versions of this code (executables or linked libaries of any sort) for any purpose you wish, and so can anyone else.
		You are not required to distribute this code with compiled versions, nor to provide it to anyone.
		If you distribute the code, or altered versions of it, that distributed code must retain this license (and this notice or one like it).
		You may not take credit for writing the original library, and must give credit in any source code distributions to the original authors.

	In even more layman terms (completely unofficial):
		Source code must always remain completely open source
		Compiled versions are completely free (as in speech and beer)
		The code being open source is not allowed to interfere with the unrestricted rights for compiled versions.

	In even less official terms:
		If you distribute the code, it must remain open and under this license. (this directly makes it incompatible with GPL, but useable by/with GPL projects)
		Binaries are unrestricted and are released under whatever terms YOU want (link it statically or dynamically, change it, alter it in any way, whatever)
 */

#include "SmartPointer.h"

#ifndef Singleton_h
#define Singleton_h

#ifdef _DEBUG
void OutputDebugMessage(const char* sz); //This function is not defined by this library, that's up to each application (even if it does nothing)
#endif

namespace Artifice {

////////////////////////////////

#define ARTIFICE_EXTERN_SINGLETON_MUTEX //comment this out if you don't want to define a mutex handler

#ifdef ARTIFICE_EXTERN_SINGLETON_MUTEX
	//These two functions needs to be defined elsewhere with the desired mutex.
	//That mutex should be static and global
	bool SingletonMutex_Lock();
	void SingletonMutex_Release();
#else
	inline bool SingletonMutex_Lock() { return true; };
	inline void SingletonMutex_Release() { };
#endif

const bool InitializingSingletonMutex= SingletonMutex_Lock() && (SingletonMutex_Release(), true); //making sure it's called before main()

template<class TYPE> TYPE* SingletonCreator() { static TYPE* v= new TYPE(); return v; };

template<class TYPE> class StaticSingletonBase {
public:
	//This is provided in case your platform/compiler can optimize static memory better
	//This can replace parameters 2 and 3 of the Singleton template; EG: const Singleton<Foo, StaticSingletonBase<Foo>, StaticSingletonBase<Foo>::f> foo;
	//As a shortcut, you can use StaticSingleton<>, which is declared after Singleton<>, and serves the same purpose (but with different default args)
	StaticSingletonBase() { };
	~StaticSingletonBase() { f()->TYPE::~TYPE(); }
	TYPE& operator*() { return *f(); };
	static TYPE* f() { static TYPE v; return &v; };
	void operator=(TYPE*) { };
};

////////////////////////////////
template<
	class TYPE,
	typename Pointer= SmartPointer<TYPE>, //Replace this if you have a particular pointer container you need to use (one that adds refcounts or etc)
	TYPE*(*AllocatorFunction)()= SingletonCreator<TYPE> //Pass a TYPE* function(void), which allocates, constructs, and returns a pointer to TYPE; This is useful if you need to use a non default constructor, or want mutliple "singletons" of the same class
> 
class Singleton {
public:
	Singleton(bool DelayConstruction=true) { 
		lock();
		count(+1); 
		release();
		if(!DelayConstruction) { Get(); }; 
	};
	~Singleton() { 
		lock();
		if(!count(-1) && initialized()) {
			GetLink()= (TYPE*)0; //safest way to zero the value of pointers, in case the wrapper's destructor doesn't (this is being extra cautious)
			GetLink().Pointer::~Pointer();
		}
		release();
	};

	static TYPE* Get() { return &*GetLink(); };
protected:
	static Pointer& GetLink() { 
		static unsigned char pointer_memory[sizeof(Pointer)];

		if(!initialized()) {
			lock();
			if(!initialized()) {
				Pointer& v= *(new(pointer_memory) Pointer());
				#ifdef _DEBUG
					OutputDebugMessage("Singleton created: " __FUNCTION__ "\n");
				#endif
				initialized()= true;
				v= AllocatorFunction();
			}
			release();
		}
		return *(Pointer*)(void*)pointer_memory;	
	};

	static int count(int change= 0) { static int i= 0; i += change; if(i < 0) i= 0; return i; };
	static bool& initialized() { static bool b= false; return b; };
	static void lock() { SingletonMutex_Lock(); };
	static void release() { SingletonMutex_Release(); };

public:
	TYPE* operator ->() const {	return Get(); };
	operator TYPE*() const { return Get(); };
	TYPE& operator*() const {	return *Get(); };
};

////////////////////////////////
template<class TYPE, TYPE*(*AllocatorFunction)()= StaticSingletonBase<TYPE>::f> class StaticSingleton:
	Singleton<TYPE, StaticSingletonBase<TYPE>, AllocatorFunction>
{
public:
	typedef Singleton<TYPE, StaticSingletonBase<TYPE>, StaticSingletonBase<TYPE>::f> Base;
	StaticSingleton(bool DelayConstruction=true) :
		Base(DelayConstruction)
	{ };

	using Base::operator*;
	using Base::operator->;
	operator TYPE*() const { return *(Base*)this; };
};



////////////////
}; //end namespace
#endif //ifndef Singleton_h
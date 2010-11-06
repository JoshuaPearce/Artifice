//SmartPointer.h

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


#ifndef SmartPointer_h 
#define SmartPointer_h

namespace Artifice { 

//////////////////////////////////
template <typename TYPE> class SmartPointer { 
public:
	SmartPointer() { v= 0; };
	SmartPointer(TYPE *p) { v= p; };
	~SmartPointer() { erase(); };
private:
	SmartPointer(const SmartPointer&); //intentionally undefined

public:
	SmartPointer<TYPE> &operator= (TYPE *p) { 
		if(v != p) {
			erase();
			v= p;
		}
		return *this;
	};
	template<typename THAT> SmartPointer<TYPE> &operator= (const SmartPointer<THAT> &that) { SmartPointers_cant_be_assigned_directly__to_other_SmartPointers FAIL; };
	SmartPointer<TYPE> &operator <<= (SmartPointer<TYPE> &that) { replace(that); };

	TYPE *operator ->() const { return v; };
	TYPE &operator *() const { return *v; };
	operator TYPE*() const { return v; };

	TYPE* forget() { TYPE* temp= v; v= 0; return temp; };
	TYPE* forget(const TYPE* check) { if(v != check) return 0; TYPE* temp= v; v= 0; return temp; };

	void replace(SmartPointer<TYPE> &that) {
		if(!&that || (this == &that)) return;
		erase();
		v= that.v;
		that.v= 0;
	};

	void swapwith(SmartPointer<TYPE> &that) {
		TYPE *temp= v;
		v= that.v;
		that.v= temp;
	};

	void erase() { 
		TYPE *temp= v; //in case the destructor tries to wipe this pointer itself, prevents loops
		v= 0;
		delete temp;
	};

protected:
	TYPE *v;
};

}; //end namespace

#endif //include guard

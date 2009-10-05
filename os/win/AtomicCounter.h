// The Repertoire Project copyright 2006 by John M. Dlugosz : see <http://www.dlugosz.com/Repertoire/>
// File: classics\AtomicCounter.h
// Revision: public build 9, shipped on 18-Oct-2006

#pragma once

#ifndef __OS_ATOMICCOUNTER_H__
#define __OS_ATOMICCOUNTER_H__

#pragma warning (disable: 4146)

#include "core/types.h"

namespace os {

namespace internal {
   // the nf_ functions are the same, but without __fastcall.
   int nf_Xadd (volatile int*,int);  // 4 u8s
   short nf_Xadd (volatile short*,int);  // 2 u8s
   char nf_Xadd (volatile char*,int);  // 1 u8
#ifndef _MANAGED
   int __fastcall Xadd (volatile int*,int);  // 4 u8s
   short __fastcall Xadd (volatile short*,int);  // 2 u8s
   char __fastcall Xadd (volatile char*,int);  // 1 u8
#else
   inline int Xadd (volatile int* p, int delta)
      { return nf_Xadd (p, delta); }
   inline short Xadd (volatile short* p, int delta)
      { return nf_Xadd (p, delta); }
   inline char Xadd (volatile char* p, int delta)
      { return nf_Xadd (p, delta); }
#endif
   inline long Xadd (volatile long* p, int delta)
      { return Xadd ((volatile int*)p, delta); }
   inline unsigned Xadd (volatile unsigned* p, int delta)
      { return Xadd ((volatile int*)p, delta); }
   inline u16 Xadd (volatile u16* p, int delta)
      { return Xadd ((volatile short*)p, delta); }
   inline u32 Xadd (volatile u32* p, int delta)
      { return Xadd ((volatile long*)p, delta); }
#if defined _MSC_VER && _MSC_VER < 1300
// Older versions of Microsoft VC++ doesn't like this.  It thinks char and signed char are
// the same thing, not distinct types for overloading.
   inline signed char Xadd (volatile signed char* p, int delta)
      { return Xadd ((volatile char*)p, delta); }
#endif
   inline u8 Xadd (volatile u8* p, int delta)
      { return Xadd ((volatile char*)p, delta); }

   // the nf_ functions are the same, but without __fastcall.
   void nf_Inc (volatile int*);  // 4 u8s
   void nf_Inc (volatile short*);  // 2 u8s
   void nf_Inc (volatile char*);  // 1 u8
#ifndef _MANAGED
   void __fastcall Inc (volatile int*);  // 4 u8s
   void __fastcall Inc (volatile short*);  // 2 u8s
   void __fastcall Inc (volatile char*);  // 1 u8
#else
   inline void Inc (volatile int* p)
      { return nf_Inc (p); }
   inline void Inc (volatile short* p)
      { return nf_Inc (p); }
   inline void Inc (volatile char* p)
      { return nf_Inc (p); }
#endif
   inline void Inc (volatile long* p)      { Inc ((volatile int*)p); }
   inline void Inc (volatile unsigned* p)      { Inc ((volatile int*)p); }
   inline void Inc (volatile u16* p)      { Inc ((volatile short*)p); }
   inline void Inc (volatile u32* p)      { Inc ((volatile long*)p); }
#if defined _MSC_VER && _MSC_VER < 1300
   // Older versions of Microsoft VC++ doesn't like this.  It thinks char and signed char are
   // the same thing, not distinct types for overloading.
   inline void Inc (volatile signed char* p)      { Inc ((volatile char*)p); }
#endif
   inline void Inc (volatile u8* p)      { Inc ((volatile char*)p); }

   // the nf_ functions are the same, but without __fastcall.
   bool nf_Dec (volatile int*);  // 4 u8s
   bool nf_Dec (volatile short*);  // 2 u8s
   bool nf_Dec (volatile char*);  // 1 u8
#ifndef _MANAGED
   bool __fastcall Dec (volatile int*);  // 4 u8s
   bool __fastcall Dec (volatile short*);  // 2 u8s
   bool __fastcall Dec (volatile char*);  // 1 u8
#else
   inline bool Dec (volatile int* p)
      { return nf_Dec (p); }
   inline bool Dec (volatile short* p)
      { return nf_Dec (p); }
   inline bool Dec (volatile char* p)
      { return nf_Dec (p); }
#endif
   inline bool Dec (volatile long* p)      { return Dec ((volatile int*)p); }
   inline bool Dec (volatile unsigned* p)      { return Dec ((volatile int*)p); }
   inline bool Dec (volatile u16* p)      { return Dec ((volatile short*)p); }
   inline bool Dec (volatile u32* p)      { return Dec ((volatile long*)p); }
#if defined _MSC_VER && _MSC_VER < 1300
      // Older versions of Microsoft VC++ doesn't like this.  It thinks char and signed char are
      // the same thing, not distinct types for overloading.
   inline bool Dec (volatile signed char* p)      { return Dec ((volatile char*)p); }
#endif
   inline bool Dec (volatile u8* p)      { return Dec ((volatile char*)p); }

   // the nf_ functions are the same, but without __fastcall.
   __int64 nf_Xexchange (volatile __int64*,__int64);  // 8 u8s
   int nf_Xexchange (volatile int*,int);  // 4 u8s
   short nf_Xexchange (volatile short*,int);  // 2 u8s
   char nf_Xexchange (volatile char*,int);  // 1 u8
#ifndef _MANAGED
   __int64 __fastcall Xexchange (volatile __int64*,__int64);  // 8 u8s
   int __fastcall Xexchange (volatile int*,int);  // 4 u8s
   short __fastcall Xexchange (volatile short*,int);  // 2 u8s
   char __fastcall Xexchange (volatile char*,int);  // 1 u8
#else
   inline __int64 Xexchange (volatile __int64* p, __int64 newvalue)
      { return nf_Xexchange (p, newvalue); }
   inline int Xexchange (volatile int* p, int newvalue)
      { return nf_Xexchange (p, newvalue); }
   inline short Xexchange (volatile short* p, short newvalue)
      { return nf_Xexchange (p, newvalue); }
   inline char Xexchange (volatile char* p, char newvalue)
      { return nf_Xexchange (p, newvalue); }
#endif
   inline long Xexchange (volatile long* p, int newvalue)
      { return Xexchange ((volatile int*)p, newvalue); }
   inline unsigned Xexchange (volatile unsigned* p, int newvalue)
      { return Xexchange ((volatile int*)p, newvalue); }
   inline u16 Xexchange (volatile u16* p, int newvalue)
      { return Xexchange ((volatile short*)p, newvalue); }
   inline u32 Xexchange (volatile u32* p, int newvalue)
      { return Xexchange ((volatile long*)p, newvalue); }
   inline u8 Xexchange (volatile u8* p, int newvalue)
      { return Xexchange ((volatile char*)p, newvalue); }

   // the nf_ functions are the same, but without __fastcall.
   bool nf_CompareAndSwap (volatile __int64* dest, __int64 source, __int64 comparend);  // 8 u8s
   bool nf_CompareAndSwap (volatile int* dest, int source, int comparend);  // 4 u8s
   bool nf_CompareAndSwap (volatile short* dest, int source, int comparend);  // 2 u8s
   bool nf_CompareAndSwap (volatile char* dest, int source, int comparend);  // 1 u8
#ifndef _MANAGED
   bool __fastcall CompareAndSwap (volatile __int64* dest, __int64 source, __int64 comparend);  // 8 u8s
   bool __fastcall CompareAndSwap (volatile int* dest, int source, int comparend);  // 4 u8s
   bool __fastcall CompareAndSwap (volatile short* dest, int source, int comparend);  // 2 u8s
   bool __fastcall CompareAndSwap (volatile char* dest, int source, int comparend);  // 1 u8
#else
   inline bool CompareAndSwap (volatile __int64* dest, __int64 source, __int64 comparend)
      { return nf_CompareAndSwap (dest, source, comparend); }
   inline bool CompareAndSwap (volatile int* dest, int source, int comparend)
      { return nf_CompareAndSwap (dest, source, comparend); }
   inline bool CompareAndSwap (volatile short* dest, int source, int comparend)
      { return nf_CompareAndSwap (dest, source, comparend); }
   inline bool CompareAndSwap (volatile char* dest, int source, int comparend)
      { return nf_CompareAndSwap (dest, source, comparend); }
#endif
   inline long CompareAndSwap (volatile long* dest, int source, int comparend)
      { return CompareAndSwap ((volatile int*)dest, source, comparend); }
   inline long CompareAndSwap (volatile unsigned* dest, int source, int comparend)
      { return CompareAndSwap ((volatile int*)dest, source, comparend); }
   inline long CompareAndSwap (volatile u16* dest, int source, int comparend)
      { return CompareAndSwap ((volatile short*)dest, source, comparend); }
   inline long CompareAndSwap (volatile u32* dest, int source, int comparend)
      { return CompareAndSwap ((volatile long*)dest, source, comparend); }
   inline long CompareAndSwap (volatile u8* dest, int source, int comparend)
      { return CompareAndSwap ((volatile char*)dest, source, comparend); }
} // end of internal


/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ */

template <typename T>
class AtomicCounter {
protected:
   T value;
   // >> need a critical section, too.
public:
   operator T() const volatile { return value; }
   T operator= (AtomicCounter x) volatile { value = x.value; }
   AtomicCounter (T x) : value(x) {}
   AtomicCounter() {}
   T operator++() volatile;
   T operator--() volatile;
   T operator++ (int) volatile;
   T operator-- (int) volatile;
   T operator+= (T) volatile;
   T operator-= (T) volatile;
   T exchange (T) volatile;
   bool compare_and_swap (T source, T compare) volatile;
   void inc() volatile;
   bool dec() volatile;  // returns true if it became equal to zero
   };

// The implementation of the general case requires a critical
// section.  No implementation is provided at this time.

/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ */

template <typename T>
class AtomicIntegerXX {
protected:
   T value;
public:
   operator T() const volatile { return value; }
   AtomicIntegerXX (T x) : value(x) {}
   AtomicIntegerXX() {}
   T operator++() volatile
      { return 1+ internal::Xadd (&value,1); }
   T operator--() volatile
      { return internal::Xadd (&value,-1) -1; }
   T operator++ (int) volatile
      { return internal::Xadd (&value,1); }
   T operator-- (int) volatile
      { return internal::Xadd (&value,-1); }
   T operator+= (T delta) volatile
      { return delta + internal::Xadd (&value,delta); }
   T operator-= (T delta) volatile
      { return internal::Xadd (&value,-delta) - delta; }
   T exchange (T newval) volatile
      { return internal::Xexchange (&value, newval); }
   bool compare_and_swap (T source, T compare) volatile
      { return internal::CompareAndSwap (&value, source, compare); }
   bool dec() volatile
      { return internal::Dec (&value); }
   void inc() volatile
      { internal::Inc (&value); }
   };

/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ */

template<>
class AtomicCounter<int> : public AtomicIntegerXX<int> {
public:
   AtomicCounter() {}
   AtomicCounter (int x) : AtomicIntegerXX<int>(x) {}
   int operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<unsigned> : public AtomicIntegerXX<unsigned> {
public:
   AtomicCounter() {}
   AtomicCounter (unsigned x) : AtomicIntegerXX<unsigned>(x) {}
   unsigned operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<short> : public AtomicIntegerXX<short> {
public:
   AtomicCounter() {}
   AtomicCounter (short x) : AtomicIntegerXX<short>(x) {}
   short operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<long> : public AtomicIntegerXX<long> {
public:
   AtomicCounter() {}
   AtomicCounter (long x) : AtomicIntegerXX<long>(x) {}
   long operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<u32> : public AtomicIntegerXX<u32> {
public:
   AtomicCounter() {}
   AtomicCounter (u32 x) : AtomicIntegerXX<u32>(x) {}
   u32 operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<u16> : public AtomicIntegerXX<u16> {
public:
   AtomicCounter() {}
   AtomicCounter (u16 x) : AtomicIntegerXX<u16>(x) {}
   u16 operator= (AtomicCounter x) volatile { return value = x.value; }
   };

template<>
class AtomicCounter<char> : public AtomicIntegerXX<char> {
public:
   AtomicCounter() {}
   AtomicCounter (char x) : AtomicIntegerXX<char>(x) {}
   char operator= (AtomicCounter x) volatile { return value = x.value; }
   };

#if defined _MSC_VER && _MSC_VER < 1300
// MSVC++ strikes again
template<>
class AtomicCounter<signed char> : public AtomicIntegerXX<signed char> {
public:
   AtomicCounter() {}
   AtomicCounter (signed char x) : AtomicIntegerXX<signed char>(x) {}
   signed char operator= (AtomicCounter x) volatile { return value = x.value; }
   };
#endif

template<>
class AtomicCounter<u8> : public AtomicIntegerXX<u8> {
public:
   AtomicCounter() {}
   AtomicCounter (u8 x) : AtomicIntegerXX<u8>(x) {}
   u8 operator= (AtomicCounter x) volatile { return value = x.value; }
   };
#pragma warning (default:4146)

}  //end of namespace

#endif //__OS_ATOMICCOUNTER_H__

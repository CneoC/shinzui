#include "Util.h"

#include <stdlib.h>

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) || \
	defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define GET_16_BITS(d) (*((const u16 *) (d)))
#else
#define GET_16_BITS(d) ((((u32)(((const u8 *)(d))[1])) << 8) \
					  +  (u32)(((const u8 *)(d))[0]) )
#endif

u32 Util::hashString(const char * data, u32 len)
{
	u32 hash = len;
	u32 tmp;
	int rem;

	if (len <= 0 || data == NULL) return 0;

	rem = len & 3;
	len >>= 2;

	// Main loop
	for (; len > 0; len--)
	{
		hash   += GET_16_BITS(data);
		tmp    = (GET_16_BITS(data + 2) << 11) ^ hash;
		hash   = (hash << 16) ^ tmp;
		data   += 2 * sizeof(u16);
		hash   += hash >> 11;
	}

	// Handle end cases
	switch (rem)
	{
		case 3:
			hash += GET_16_BITS(data);
			hash ^= hash << 16;
			hash ^= data[sizeof(u16)] << 18;
			hash += hash >> 11;
			break;
		case 2:
			hash += GET_16_BITS(data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
			hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
	}

	// Force "avalanching" of final 127 bits
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

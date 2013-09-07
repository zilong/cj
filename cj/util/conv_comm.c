// Use of this source code is governed by a Apache license
// that can be found in the License file.
//
//convert common function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gbk_to_utf8.h"

/**
 * UTF-16 to UTF-8 encoding conversion function
 *
 * @param u16 	String to be converted
 * @param u8 	Characters output
 * @return u8   The number of bytes u8
 */
int
conv_single_utf16to8(uint16_t u16, unsigned char * u8)
{
	if (u16 <= 0x7f){
		// 1bytes
		u8[0] = (u16 & 0xff);
		return 1;
	}
	else if (u16 <= 0x7ff){
		// 2bytes
		u8[0] = 0xc0 | ((u16 & 0x7c0) >> 6);
		u8[1] = 0x80 | (u16 & 0x3f);
		return 2;
	}

	// 3bytes
	u8[0] = 0xe0 | ((u16 & 0xf000) >> 12);
	u8[1] = 0x80 | ((u16 & 0xfc0) >> 6);
	u8[2] = 0x80 | (u16 & 0x3f);

	return 3;
}

/**
 * UTF-8 to UTF-16 encoding conversion function
 *
 * @param u8 	String to be converted
 * @param u16 	Characters output
 * @return Number of characters converted u16, 1 indicates successful conversion, 0 means no conversion
 */
int
conv_single_utf8to16(const unsigned char * u8, uint16_t * u16)
{
	switch(conv_UTF8_LENGTH(u8[0])){
	case 1:
		(*u16) = u8[0];
		break;
	case 2:
		(*u16) = ((u8[0] & 0x1f) >> 2);
		(*u16) <<= 8;
		(*u16) |= ((u8[0] & 0x3) << 6) | (u8[1] & 0x3f);
		break;
	case 3:
		(*u16) = ((u8[0] & 0xf) << 4) | ((u8[1] & 0x3f) >> 2);
		(*u16) <<= 8;
		(*u16) |= ((u8[1] & 0x3) << 6) | (u8[2] & 0x3f);
		break;
	default:
		// 不处理4bytes的
		(*u16) = 0;
		return 0;
		break;
	}

	return 1;
}

/**
 * UTF-16 to UTF-8 encoding conversion function, the end of the fill 0
 *
 * @param u16 	      String to be converted
 * @param u16len      u16 number of characters (two bytes per character)
 * @param u8 	      Characters output
 * @param u8maxlen    The maximum number of bytes u8
 * @return The number of bytes u8
 */
int
conv_utf16to8(const uint16_t * u16,
			size_t u16len,
			unsigned char * u8,
			size_t u8maxlen)
{
	const uint16_t * u16end = u16 + u16len;
	int u8len = 0;
	if (u8maxlen <= 1){
		*u8 = 0;
		return 0;
	}
	--u8maxlen;

	for (; u16 < u16end; ++u16){
		if ((*u16) <= 0x7f){
			// 1bytes
			if (u8len + 1 > u8maxlen){
				// space not enough
				break;
			}
			u8[0] = (*u16 & 0xff);
			++u8len;
			++u8;
		}
		else if ((*u16) <= 0x7ff){
			// 2bytes
			if (u8len + 2 > u8maxlen){
				// space not enough
				break;
			}
			u8[0] = 0xc0 | ((*u16 & 0x7c0) >> 6);
			u8[1] = 0x80 | (*u16 & 0x3f);
			u8len += 2;
			u8 += 2;
		}
		else{
			// 3bytes
			if (u8len + 3 > u8maxlen){
				// space not enough
				break;
			}
			u8[0] = 0xe0 | ((*u16 & 0xf000) >> 12);
			u8[1] = 0x80 | ((*u16 & 0xfc0) >> 6);
			u8[2] = 0x80 | (*u16 & 0x3f);
			u8len += 3;
			u8 += 3;
		}
	}

	*u8 = 0;
	return u8len;
}

/**
 * UTF-8 to UTF-16 encoding conversion function, the end of the fill 0
 *
 * @param u8 	       String to be converted
 * @param u8len        The number of bytes u8
 * @param u16          Characters output
 * @param u16maxlen    u16 maximum number of characters (two bytes per character)
 * @return The number of bytes u16
 */
int
conv_utf8to16(const unsigned char * u8,
			size_t u8len,
			uint16_t * u16,
			size_t u16maxlen)
{
	const unsigned char * u8end = u8 + u8len;
	int u8bytes = 0;
	int u16len = 0;

	if (u16maxlen <= 1) {
		u16[0] = 0;
		return 0;
	}
	--u16maxlen;

	for (; u8 < u8end && u16len < u16maxlen; u8 += u8bytes)
	{
		u8bytes = conv_UTF8_LENGTH(u8[0]);

		if (u8bytes + u8 > u8end)
			break;

		if (u8bytes == 1){
			(*u16) = u8[0];
		}
		else if (u8bytes == 2){
			(*u16) = ((u8[0] & 0x1f) >> 2);
			(*u16) <<= 8;
			(*u16) |= ((u8[0] & 0x3) << 6) | (u8[1] & 0x3f);
		}
		else if (u8bytes == 3){
			(*u16) = ((u8[0] & 0xf) << 4) | ((u8[1] & 0x3f) >> 2);
			(*u16) <<= 8;
			(*u16) |= ((u8[1] & 0x3) << 6) | (u8[2] & 0x3f);
		}
		else {
			u8bytes = 1;
			continue;
		}
		// Does not deal with the situation 0,4,5,6,7,8 bytes

		++u16, ++u16len;
	}

	*u16 = 0;

	return u16len;
}


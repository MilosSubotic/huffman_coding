/*
 * @license MIT
 * @brief Huffman codec, encoder and decoder.
 */

#ifdef __cplusplus
extern "C" {
#endif
 
//////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////

int huffman_encode(
	const uint8_t* data,
	size_t data_len,
	uint8_t* compressed,
	size_t compressed_max_len
);

int huffman_decode(
	const uint8_t* compressed,
	size_t compressed_len,
	uint8_t* data,
	size_t data_max_len
);

//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

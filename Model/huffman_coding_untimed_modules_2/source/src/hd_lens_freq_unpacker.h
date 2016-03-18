/**
 * @license MIT
 * @brief
 */


#ifndef HD_LENS_FREQ_UNPACKER_H
#define HD_LENS_FREQ_UNPACKER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"
#include "bit_extracter_channel.h"

#include "huffman_coding_interfaces.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class hd_lens_freq_unpacker : sc_module {
	public:
		bit_extracter_client_port<enc_chunk_t, enc_chunk_size_t>
			input_bitstream;

/*
		axis_out<lens_freq_t> lens_freq;
*/
	    SC_CTOR(hd_lens_freq_unpacker) {
	        //SC_THREAD(unpack);
	    }
/*
	private:
	    void unpack();
*/
	};
}

///////////////////////////////////////////////////////////////////////////////

#endif // HD_LENS_FREQ_UNPACKER_H

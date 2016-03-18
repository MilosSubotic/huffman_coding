/**
 * @license MIT
 * @brief
 */


#ifndef HD_LENS_FREQ_UNPACKER_H
#define HD_LENS_FREQ_UNPACKER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "bit_extract_channel.h"

#include "huffman_coding_interfaces.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class hd_lens_freq_unpacker : sc_module {
	public:
		bit_extract_client_port<enc_bit_acc_t, enc_bit_acc_size_t,
				enc_bit_acc_extract_t> bit_stream;


		axis_out<lens_freq_t> out_lens_freq;

		SC_CTOR(hd_lens_freq_unpacker) {
			SC_THREAD(unpack);
	    }

	private:
	    void unpack();

	};
}

///////////////////////////////////////////////////////////////////////////////

#endif // HD_LENS_FREQ_UNPACKER_H

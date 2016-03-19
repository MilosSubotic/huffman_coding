/**
 * @license MIT
 * @brief
 */


#ifndef HD_SYM_UNPACKER_H
#define HD_SYM_UNPACKER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "bit_extract_channel.h"

#include "huffman_coding_interfaces.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	SC_MODULE(hd_sym_unpacker) {
	public:
		bit_extract_client_port<enc_bit_acc_t, enc_bit_acc_size_t,
				enc_bit_acc_extract_t> bit_stream;

		axis_in<lens_freq_t> in_lens_freq;

		axis_out<codes_len_and_code_table_t> out_codes_len_and_code_table;


		SC_CTOR(hd_sym_unpacker) {
			SC_THREAD(unpack);
	    }

	private:
	    void unpack();

	};
}

///////////////////////////////////////////////////////////////////////////////

#endif // HD_SYM_UNPACKER_H

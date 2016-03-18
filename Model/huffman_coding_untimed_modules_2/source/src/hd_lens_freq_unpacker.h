/**
 * @license MIT
 * @brief
 */


#ifndef HD_LENS_FREQ_UNPACKER_H
#define HD_LENS_FREQ_UNPACKER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "huffman_coding_interfaces.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class hd_lens_freq_unpacker : sc_module {
	public:
		sc_in<enc_bit_acc_t> enc_bit_acc;
		sc_in<enc_bit_acc_size_t> acc_size;
		sc_out<enc_bit_acc_shift_t> remove_bits;
		sc_out<bool> need_more_bits;
		sc_out<bool> done;

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

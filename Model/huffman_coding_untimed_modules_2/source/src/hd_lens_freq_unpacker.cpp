/**
 * @license MIT
 * @brief
 */

///////////////////////////////////////////////////////////////////////////////

#include "hd_lens_freq_unpacker.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void hd_lens_freq_unpacker::unpack() {
		enc_bit_acc_t mask = ((enc_bit_acc_t(1) << len_freq_width) - 1);

		for(int block = 0; ; block++){
			bit_stream->connect();

			algo_log << "block: " << block << endl << endl;

			algo_log << "Unpacking bit-lengths count..." << endl;

			lens_freq_t lens_freq;

			for(int len = 1; len < len_freq_num; len++){

				if(bit_stream->get_size() < len_freq_width){
					bit_stream->need_more_bits();
				}
				lens_freq[len] = bit_stream->get_acc() & mask;
				bit_stream->extract_bits(len_freq_width);
			}

			bit_stream->done();

			algo_log << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				algo_log << len << ": "
					<< lens_freq[len] << endl;
			}
			algo_log << endl << endl;

			out_lens_freq->write(lens_freq);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////

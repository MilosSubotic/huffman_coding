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
/*
		for(int block = 0; ; block++){
			algo_log << "block: " << block << endl << endl;

			algo_log << "Unpacking bit-lengths count..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num);

			for(int len = 1; len < len_freq_num; len++){
				len_freq_t len_freq = len_freq_width;

				lens_freq[len] = unpack();
			}

			algo_log << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				algo_log << len << ": "
					<< lens_freq[len] << endl;
			}
			algo_log << endl << endl;
		}
*/
	}

}

///////////////////////////////////////////////////////////////////////////////

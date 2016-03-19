/**
 * @license MIT
 * @brief
 */

///////////////////////////////////////////////////////////////////////////////

#include "hd_sym_unpacker.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void hd_sym_unpacker::unpack() {
		sym_t mask = ((enc_bit_acc_t(1) << sym_width) - 1);

		for(int block = 0; ; block++){

			bit_stream->connect();

			algo_log << "block: " << block << endl << endl;

			algo_log << "Unpacking symbols and creating canonical code table..."
				<< endl;

			codes_len_t codes_len;
			for(auto& code_len : codes_len) {
				code_len = 0;
			}
			code_table_t code_table;
			lens_freq_t lens_freq;
			bool last_lens_freq;
			in_lens_freq->read(lens_freq, last_lens_freq);
			assert(!last_lens_freq);

			code_t code = 0;
			for(len_t len = 1; len < len_freq_num; len++){
				for(int freq = lens_freq[len]; freq > 0; freq--){
					// Extract symbol.
					if(bit_stream->get_size() < sym_width){
						bit_stream->need_more_bits();
					}
					sym_t sym = bit_stream->get_acc() & mask;
					bit_stream->extract_bits(sym_width);

					code_table[sym] = code;
					codes_len[sym] = len;
					// Increment to next code.
					code += 1 << (code_width - len);
				}
			}

			bit_stream->done();

			algo_log << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": "
					<< codes_len[sym] << endl;
			}
			algo_log << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": " << code_table[sym] << endl;
			}
			algo_log << endl << endl;

			codes_len_and_code_table_t clact;
			clact.codes_len = codes_len;
			clact.code_table = code_table;
			out_codes_len_and_code_table->write(clact);

		}
	}

}

///////////////////////////////////////////////////////////////////////////////

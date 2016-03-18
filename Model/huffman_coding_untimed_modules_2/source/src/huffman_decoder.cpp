/**
 * @license MIT
 * @brief
 */

///////////////////////////////////////////////////////////////////////////////

#include "huffman_decoder.h"
#include "huffman_coding_interfaces.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void huffman_decoder::decode() {
		assert(enc_chunk_width == 32);

		algo_log << "Decoding..." << endl << endl;

		bool last = false;

		uint64_t acc = 0;
		unsigned acc_len = 0;
		auto unpack = [&](unsigned len) -> uint32_t {
			assert(len <= 32);
			uint32_t ret;
			if(acc_len < len){
				assert(!last); // Encoded stream isn't crippled.
				enc_chunk_t data;
				in_enc_data->read(data, last);

				acc |= uint64_t(data) << acc_len;
				acc_len += 32;
			}
			ret = acc & ((uint64_t(1) << len) - 1);
			acc >>= len;
			acc_len -= len;
			return ret;
		};


		for(int block = 0; ; block++){
			algo_log << "block: " << block << endl << endl;

			algo_log << "Unpacking bit-lengths count..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num);

			for(int len = 1; len < len_freq_num; len++){
				lens_freq[len] = unpack(len_freq_width);
			}

			algo_log << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				algo_log << len << ": "
					<< lens_freq[len] << endl;
			}
			algo_log << endl << endl;




			algo_log << "Unpacking symbols and creating canonical code table..."
				<< endl;

			vector<len_t> codes_len(sym_num, 0);
			vector<code_t> code_table(sym_num);

			code_t code = 0;
			for(len_t len = 1; len < len_freq_num; len++){
				for(int freq = lens_freq[len]; freq > 0; freq--){
					sym_t sym = unpack(sym_width);
					code_table[sym] = code;
					codes_len[sym] = len;
					// Increment to next code.
					code += 1 << (code_width - len);
				}
			}


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



			algo_log << "Mirroring codes for little endian decoding..." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				code_t src = code_table[sym];
				code_t dst;
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst;
			}

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




			algo_log << "Decoding data..." << endl;

			vector<sym_t> od(block_len);

			for(int d = 0; d < block_len; d++){
				const int null_code_len = (1 << len_width) - 1;

				len_t best_len = null_code_len;
				code_t best_code;
				sym_t best_sym;

				// Try with bits what we have.
				for(int sym = 0; sym < sym_num; sym++){
					len_t code_len = codes_len[sym];
					if(code_len != 0 && code_len < best_len
							&& code_len <= acc_len){
						code_t mask = (1 << code_len) - 1;
						code_t enc_code = acc & mask;
						if(enc_code == code_table[sym]){
							best_len = code_len;
							best_code = code_table[sym];
							best_sym = sym;
						}
					}
				}

				// If didn't found symbol read one more chunk and try again.
				if(best_len == null_code_len){
					assert(!last); // Encoded stream isn't crippled.
					enc_chunk_t data;
					in_enc_data->read(data, last);
					acc |= uint64_t(data) << acc_len;
					acc_len += 32;

					for(int sym = 0; sym < sym_num; sym++){
						len_t code_len = codes_len[sym];
						if(code_len != 0 && code_len < best_len){
							code_t mask = (1 << code_len) - 1;
							code_t enc_code = acc & mask;
							if(enc_code == code_table[sym]){
								best_len = code_len;
								best_code = code_table[sym];
								best_sym = sym;
							}
						}
					}
				}

				assert(0 < best_len && best_len <= max_code_len);

				// Remove decoded symbol.
				acc >>=  best_len;
				acc_len -=  best_len;

				od[d] = best_sym;

				algo_log << "iter " << d << ":" << endl;
				algo_log << "best_len: " << best_len << endl;
				algo_log << "best_code: " << best_code << endl;
				algo_log << "best_sym: " << best_sym << endl;
				algo_log << endl;
			}

			if(!last){
				for(int d = 0; d < block_len; d++){
					out_data->write(od[d], false);
				}
			}else{
				int d;
				for(d = 0; d < block_len-1; d++){
					out_data->write(od[d], false);
				}
				out_data->write(od[d], true);

				acc = 0;
				acc_len = 0;
				last = false;
			}

		}


	}

}

///////////////////////////////////////////////////////////////////////////////

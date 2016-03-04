/**
 * @license MIT
 * @brief Test Huffman coding.
 */

///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

#include "huffman_encoder.h"

using namespace huffman_coding;

///////////////////////////////////////////////////////////////////////////////

#define DATA_LEN block_len*2
#define PROB_LAST_NUM 2

///////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)


///////////////////////////////////////////////////////////////////////////////

static vector<sym_t> in_data(DATA_LEN);
static int in_last;

class generator : sc_module {
public:
	axis_out<sym_t> encoder_in_data;

    SC_CTOR(generator) {
        SC_THREAD(generate);
    }

private:
    void generate() {
    	unsigned seed = time(NULL);
    	cout << "seed: " << seed << endl;
    	srand(seed);

    	for(int i = 0; i < DATA_LEN; i++){
    		sym_t s = rand() % (1 << sym_width);
    		in_data[i] = s;

    		bool last = i == DATA_LEN-1;
#if PROB_LAST_NUM != 0
    		last |= !(rand() % (DATA_LEN/PROB_LAST_NUM));
#endif
    		if(last){
    			in_last++;
    		}
    		encoder_in_data->write(s, last);
    	}
    }
};


static vector<enc_chunk_t> enc_data;

class enc_data_channel : sc_module {
public:
	axis_in<enc_chunk_t> encoder_out_enc_data;

    SC_CTOR(enc_data_channel) {
        SC_THREAD(channel_enc_data);
    }

private:
    void channel_enc_data() {
    	while(true){
    		bool last;
    		enc_chunk_t e;

    		encoder_out_enc_data->read(e, last);

    		enc_data.push_back(e);

    		// TODO Real decoder code.
    	}

    }
};

int sc_main(int, char**) {

	auto g = new generator("g");
	auto g_e = new axis_channel<sym_t>("g_e");
	auto e = new huffman_encoder("e");
	auto e_ec = new axis_channel<enc_chunk_t>("e_ec");
	auto ec = new enc_data_channel("ec");


	g->encoder_in_data(*g_e);
	e->in_data(*g_e);
	e->out_enc_data(*e_ec);
	ec->encoder_out_enc_data(*e_ec);


	sc_start();

	cout << "in_last: " << in_last << endl;

	cout << "in_data size: " << in_data.size() << endl;
	cout << "enc_data size: " << enc_data.size() << endl;
	//cout << "out_data size: " << out_data.size() << endl;

	cout << "Comparing input and output..." << endl;
/*
	if(in_data != out_data){
		cout << "Mismatch!" << endl;
		for(int i = 0; i < in_data.size(); i++){
			if(in_data[i] != out_data[i]){
				cout << "Mismatch at pos " << i << endl;
				break;
			}
		}
	}else{
		cout << "Match!" << endl;
	}
	cout << endl << endl;

*/
	cout << "End." << endl;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

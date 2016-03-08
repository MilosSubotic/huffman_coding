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

#include "huffman_coding.h"

using namespace huffman_coding;

///////////////////////////////////////////////////////////////////////////////

#define DATA_LEN block_len*2
//#define PROB_LAST_NUM 2

//#define DATA_LEN 10
#define PROB_LAST_NUM 0


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
	axis_out<enc_chunk_t> decoder_int_enc_data;

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
    		cout << "e = 0x" << hex << setw(8) << e << dec
    				<< (last ? " last" : "")<< endl;

    		decoder_int_enc_data->write(e, last);
    	}
    }
};

static vector<sym_t> out_data;
static int out_last;

class monitor : sc_module {
public:
	axis_in<sym_t> decoder_out_data;

    SC_CTOR(monitor) {
        SC_THREAD(collect);
    }

private:
    void collect() {
    	while(true){
    		bool last;
    		sym_t s;

    		decoder_out_data->read(s, last);

    		out_data.push_back(s);

    		if(last){
    			out_last++;
    		}
    	}
    }
};

int sc_main(int, char**) {

	// TODO Local vars.
	auto g = new generator("g");
	auto g_e = new axis_channel<sym_t>("g_e");
	auto e = new huffman_encoder("e");
	auto e_ec = new axis_channel<enc_chunk_t>("e_ec");
	auto ec = new enc_data_channel("ec");
	auto ec_d = new axis_channel<enc_chunk_t>("ec_d");
	auto d = new huffman_decoder("d");
	auto d_m = new axis_channel<sym_t>("d_m");
	auto m = new monitor("m");

	g->encoder_in_data(*g_e);
	e->in_data(*g_e);
	e->out_enc_data(*e_ec);
	ec->encoder_out_enc_data(*e_ec);
	ec->decoder_int_enc_data(*ec_d);
	d->in_enc_data(*ec_d);
	d->out_data(*d_m);
	m->decoder_out_data(*d_m);


	sc_start();

	cout << "in_last: " << in_last << endl;
	cout << "out_last: " << out_last << endl;

	cout << "in_data size: " << in_data.size() << endl;
	cout << "enc_data size: " << enc_data.size() << endl;
	cout << "out_data size: " << out_data.size() << endl;

	cout << "Comparing input and output..." << endl;
	if(in_data != out_data){
		cout << endl << "Mismatch!!!" << endl;
		for(int i = 0; i < in_data.size(); i++){
			if(in_data[i] != out_data[i]){
				cout << "Mismatch at pos " << i << endl;
				break;
			}
		}
		return 1;
	}else{
		cout << "Match." << endl;
	}
	cout << endl << endl;

	cout << "Comparing input and output last..." << endl;
	if(in_data != out_data){
		cout << endl << "Mismatch!!!" << endl;
		return 1;
	}else{
		cout << "Match." << endl;
	}

	cout << "End." << endl;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

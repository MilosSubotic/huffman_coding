/**
 * @license MIT
 * @brief SystemC channel for extracting bits from type.
 */

#ifndef AXIS_BIT_EXTRACT_SERVER_H
#define AXIS_BIT_EXTRACT_SERVER_H

///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <sstream>

#include "axis_channel.h"
#include "bit_extract_channel.h"

///////////////////////////////////////////////////////////////////////////////

// C is short for Chunk type.
// BA is short for Bit Accumulator type.
// BAS is short for Bit Accumulator Size type.
// BAE is short for Bit Accumulator Extract type.


template<typename C, typename BA, typename BAS, typename BAE>
SC_MODULE(axis_bit_extract_server) {
public:

	SC_CTOR(axis_bit_extract_server) {
    	chunk_stream = nullptr;
        SC_THREAD(serve);
    }

    void set_axis_chunk_in_if(axis_in<C>& in_chunk_if){
    	chunk_stream = &in_chunk_if;
    }

private:
    using channel_t = bit_extract_channel<BA, BAS, BAE>;
public:

    using client_if_t = bit_extract_client_if<BA, BAS, BAE>;
    client_if_t* get_new_client_if() {
    	// Make name for bit extract channel.
    	int idx = servers_port.size();
    	std::ostringstream oss;
    	oss << basename() << "__bit_extract_channel__"<< idx;

    	auto ch = new channel_t(oss.str().c_str());
    	channels.push_back(ch);

    	auto sp = new server_port_t();
    	sp->operator()(*ch);
    	servers_port.push_back(sp);

    	return ch;
    }

private:
	axis_in<C>* chunk_stream;
	using server_port_t = bit_extract_server_port<BA, BAS, BAE>;
	std::vector<channel_t*> channels;
    std::vector<server_port_t*> servers_port;

    void serve() {
    	while(true){
			for(server_port_t* server : servers_port){
				BA acc;
	    		BAS acc_size;
				(*server)->open_connection(acc, acc_size);

	    		C chunk;
				bool last_chunk = false;
				assert(!last_chunk);
				(*chunk_stream)->read(chunk, last_chunk);
				acc = chunk;
	    		acc_size = chunk.length();
	    		BAE extract;
				while(true){
					switch((*server)->wait_operation()){
					case EXTRACT_BITS:
						extract = (*server)->get_extract();
						assert(extract <= acc_size);
						acc_size -= extract;
						acc >>= extract;
						break;
					case NEED_MORE_BITS:
						assert(!last_chunk);
						(*chunk_stream)->read(chunk, last_chunk);
						acc |= BA(chunk) << acc_size;
						acc_size += chunk.length();
						break;
					case DONE:
						goto client_done;
					}
				}
				client_done: ;
			}
    	}
    }

};


///////////////////////////////////////////////////////////////////////////////

#endif // AXIS_BIT_EXTRACT_SERVER_H


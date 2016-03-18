/**
 * @license MIT
 * @brief SystemC channel for extracting bits from type.
 */

#ifndef BIT_EXTRACTER_CHANNEL_H
#define BIT_EXTRACTER_CHANNEL_H

///////////////////////////////////////////////////////////////////////////////

#include <systemc.h>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////

// CT is short for Chunk Type.
// CST is short for Chunk Size Type.

template<typename CT, typename CST>
class bit_extracter_server_if : virtual public sc_interface {
public:
	virtual void set_chunk_ref(const CT& chunk) = 0;
	virtual void set_chunk_size_ref(const CST& size) const = 0;
	/**
	 * @return If return true need more bits, else client is done.
	 */
	virtual bool does_need_more_bits() = 0;
	virtual void new_chunk_added() = 0;
};

template<typename CT, typename CST>
class bit_extracter_client_if : virtual public sc_interface {
public:
	virtual const CT& get_chunk_ref() const = 0;
	virtual const CST& get_chunk_size_ref() const = 0;
	virtual void remove_bits(CST bits_to_remove) = 0;
	virtual void need_more_bits() = 0;
	virtual void done() = 0;
};


template<typename CT, typename CST>
class bit_extracter_channel :
	public sc_channel,
	public bit_extracter_server_if<CT, CST>,
	public bit_extracter_client_if<CT, CST> {

public:
	bit_extracter_channel(sc_module_name name)
		: sc_channel(name), chunk(0), size {
	}

	virtual bool set_chunk(CT chunk) {
		/*
		if(have_data){
			wait(read_event);
		}

		this->data = data;
		this->last = last;

		have_data = true;
		write_event.notify();
		*/

		wait(client_event);
	}

	virtual void read(CT& data, bool& last) {
		/*
		if(!have_data){
			wait(write_event);
		}

		data = this->data;
		last = this->last;

		have_data = false;
		read_event.notify();
		*/
	}

protected:

	CT chunk;
	CST size;
	bool done;

	sc_event client_event;
};


template<typename CT, typename CST>
class bit_extracter_server_port :
	public sc_port<bit_extracter_server_if<CT, CST>> {

};

template<typename CT, typename CST>
class bit_extracter_client_port :
	public sc_port<bit_extracter_client_if<CT, CST>> {

};

///////////////////////////////////////////////////////////////////////////////

#endif // BIT_EXTRACTER_CHANNEL_H


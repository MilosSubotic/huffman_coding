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
// CST is short for Chunk Shift Type.

template<typename CT, typename CST>
class bit_extracter_server_if : virtual public sc_interface {
public:
	virtual void write(const CT& data, bool last = false) = 0;
};

template<typename CT, typename CST>
class bit_extracter_client_if : virtual public sc_interface {
public:
	virtual void read(CT& data, bool& last) = 0;
};


template<typename CT, typename CST>
class bit_extracter_channel :
	public sc_channel,
	public bit_extracter_server_if<CT, CST>,
	public bit_extracter_client_if<CT, CST> {

public:
	bit_extracter_channel(sc_module_name name)
		: sc_channel(name) {
	}

	virtual void write(const CT& data, bool last = false) {
		/*
		if(have_data){
			wait(read_event);
		}

		this->data = data;
		this->last = last;

		have_data = true;
		write_event.notify();
		*/
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
	CST shift;
	bool done;

	sc_event write_event, read_event;
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


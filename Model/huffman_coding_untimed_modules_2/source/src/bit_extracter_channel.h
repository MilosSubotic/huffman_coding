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

// BAT is short for Bit Accumulator Type.
// BAST is short for Bit Accumulator Size Type.

template<typename BAT, typename BAST>
class bit_extracter_server_if : virtual public sc_interface {
public:
	virtual void open_connection(const BAT& chunk, const BAST& size) = 0;
	enum operation_t {
		REMOVE_BITS,
		NEED_MORE_BITS,
		DONE
	};
	virtual operation_t wait_operation() = 0;
};

template<typename BAT, typename BAST>
class bit_extracter_client_if : virtual public sc_interface {
public:
	virtual void connect() = 0;
	virtual const BAT& get_chunk_ref() const = 0;
	virtual const BAST& get_chunk_size_ref() const = 0;
	virtual void remove_bits(BAST bits_to_remove) = 0;
	virtual void need_more_bits() = 0;
	virtual void done() = 0;
};


template<typename BAT, typename BAST>
class bit_extracter_channel :
	public sc_channel,
	public bit_extracter_server_if<BAT, BAST>,
	public bit_extracter_client_if<BAT, BAST> {

public:
	bit_extracter_channel(sc_module_name name)
		: sc_channel(name), chunk(0) {
	}


protected:

	BAT chunk;
	BAST size;
	bool done;

	sc_event client_event;
};


template<typename BAT, typename BAST>
class bit_extracter_server_port :
	public sc_port<bit_extracter_server_if<BAT, BAST>> {

};

template<typename BAT, typename BAST>
class bit_extracter_client_port :
	public sc_port<bit_extracter_client_if<BAT, BAST>> {

};

///////////////////////////////////////////////////////////////////////////////

#endif // BIT_EXTRACTER_CHANNEL_H


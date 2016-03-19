/**
 * @license MIT
 * @brief SystemC channel for extracting bits from type.
 */

#ifndef BIT_EXTRACT_CHANNEL_H
#define BIT_EXTRACT_CHANNEL_H

///////////////////////////////////////////////////////////////////////////////

#include <systemc.h>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////

// BA is short for Bit Accumulator type.
// BAS is short for Bit Accumulator Size type.
// BAE is short for Bit Accumulator Extract type.

enum bit_extract_operation_t {
	EXTRACT_BITS,
	NEED_MORE_BITS,
	DONE
};

template<typename BA, typename BAS, typename BAE>
class bit_extract_server_if : virtual public sc_interface {
public:
	virtual void open_connection(const BA& acc, const BAS& size) = 0;
	virtual bit_extract_operation_t wait_operation() = 0;
	virtual BAE get_extract() const = 0;
};

template<typename BA, typename BAS, typename BAE>
class bit_extract_client_if : virtual public sc_interface {
public:
	virtual void connect() = 0;
	virtual const BA& get_acc() const = 0;
	virtual const BAS& get_size() const = 0;
	virtual void extract_bits(BAE bits_to_extract) = 0;
	virtual void need_more_bits() = 0;
	virtual void done() = 0;
};


template<typename BA, typename BAS, typename BAE>
class bit_extract_channel :
	public sc_channel,
	public bit_extract_server_if<BA, BAS, BAE>,
	public bit_extract_client_if<BA, BAS, BAE> {

public:
	bit_extract_channel(sc_module_name name)
		: sc_channel(name), connection_opened(false),
		  acc(nullptr), size(nullptr), op(DONE) {
	}

	virtual void open_connection(const BA& acc, const BAS& size) {
		this->acc = &acc;
		this->size = &size;
		connection_opened = true;
		connection_event.notify();
	}
	virtual bit_extract_operation_t wait_operation() {
		server_done_op.notify();
		wait(client_set_op);
		return op;
	}
	virtual BAE get_extract() const {
		return extract;
	}

	virtual void connect() {
		if(!connection_opened){
			wait(connection_event);
		}
	}
	virtual const BA& get_acc() const {
		return *acc;
	}
	virtual const BAS& get_size() const {
		return *size;
	}
	virtual void extract_bits(BAE bits_to_extract) {
		extract = bits_to_extract;
		op = bit_extract_operation_t::EXTRACT_BITS;
		client_set_op.notify();
		wait(server_done_op);
	}
	virtual void need_more_bits() {
		op = bit_extract_operation_t::NEED_MORE_BITS;
		client_set_op.notify();
		wait(server_done_op);
	}
	virtual void done() {
		connection_opened = false;
		op = bit_extract_operation_t::DONE;
		client_set_op.notify();
	}

protected:

	bool connection_opened;
	sc_event connection_event;

	const BA* acc;
	const BAS* size;
	BAE extract;
	bit_extract_operation_t op;

	sc_event client_set_op, server_done_op;
};


template<typename BA, typename BAS, typename BAE>
class bit_extract_server_port :
	public sc_port<bit_extract_server_if<BA, BAS, BAE>> {

};

template<typename BA, typename BAS, typename BAE>
class bit_extract_client_port :
	public sc_port<bit_extract_client_if<BA, BAS, BAE>> {

};

///////////////////////////////////////////////////////////////////////////////

#endif // BIT_EXTRACT_CHANNEL_H


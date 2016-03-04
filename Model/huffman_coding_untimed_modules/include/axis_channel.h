/**
 * @license MIT
 * @brief SystemC channel for AXI4-Stream.
 */

#ifndef AXIS_CHANNEL_H
#define AXIS_CHANNEL_H

///////////////////////////////////////////////////////////////////////////////

#include <systemc.h>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////

// TODO Debug.
#include <iostream>
using namespace std;
#define DD(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)
#define TT(s) \
	do{ \
		cout << s << endl; \
	}while(0)

// DI is short for Data Interface

template<typename DI>
class axis_write_if : virtual public sc_interface {
public:
	virtual void write(const DI& data, bool last = false) = 0;
};

template<typename DI>
class axis_read_if : virtual public sc_interface {
public:
	virtual void read(DI& data, bool& last) = 0;
};


template<typename DI>
class axis_channel :
	public sc_channel,
	public axis_write_if<DI>,
	public axis_read_if<DI> {

public:
	axis_channel(sc_module_name name)
			: sc_channel(name), valid(false), ready(false) {
	}

	virtual void write(const DI& data, bool last = false) {
		this->data = &data;
		this->last = last;
		valid = true;
		valid_event.notify();
		if(!ready){
			wait(ready_event);
		}
		ready = false;
	}

	virtual void read(DI& data, bool& last) {
		ready = true;
		ready_event.notify();
		if(!valid){
			wait(valid_event);
		}
		valid = false;
		data = *this->data;
		last = this->last;
	}

protected:
	const DI* data;
	bool last;
	bool valid, ready;
	sc_event valid_event, ready_event;
};


template<typename DI>
class axis_out : public sc_port<axis_write_if<DI>> {

};

template<typename DI>
class axis_in : public sc_port<axis_read_if<DI>> {

};

///////////////////////////////////////////////////////////////////////////////

#endif // AXIS_CHANNEL_H


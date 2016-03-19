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
			: sc_channel(name), have_data(false), last(false) {
	}

	virtual void write(const DI& data, bool last = false) {
		if(have_data){
			wait(read_event);
		}

		this->data = data;
		this->last = last;

		have_data = true;
		write_event.notify();
	}

	virtual void read(DI& data, bool& last) {
		if(!have_data){
			wait(write_event);
		}

		data = this->data;
		last = this->last;

		have_data = false;
		read_event.notify();
	}

protected:
	bool have_data;
	DI data;
	bool last;
	sc_event write_event, read_event;
};


template<typename DI>
class axis_out : public sc_port<axis_write_if<DI>> {

};

template<typename DI>
class axis_in : public sc_port<axis_read_if<DI>> {

};

///////////////////////////////////////////////////////////////////////////////

#endif // AXIS_CHANNEL_H


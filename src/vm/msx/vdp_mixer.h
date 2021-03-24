#pragma once

#ifndef __VDP_MIXER__
#define __VDP_MIXER__

#include "../vm.h"
#include "../../emu.h"
#include "../device.h"
#include "../v9938.h"

class vdp_mixer_device : public DEVICE {
protected:
	v99x8_device* p_vdp;
	v99x8_device* s_vdp;
	int disp_vdp;
	int port_swap;
	outputs_t outputs_irq;
public:
	vdp_mixer_device(VM_TEMPLATE* parent_vm, EMU* parent_emu, v9958_device* primary, v9958_device* secondary);
	// common functions
	void initialize();
	void reset();
	void write_io8(uint32_t addr, uint32_t data);
	uint32_t read_io8(uint32_t addr);
	bool process_state(FILEIO* state_fio, bool loading);
	void write_signal(int id, uint32_t data, uint32_t mask);
	void set_context_irq(DEVICE* device, int id, uint32_t mask)
	{
		register_output_signal(&outputs_irq, device, id, mask);
	}
	const _TCHAR* get_device_name()
	{
		return _T("VDP MIXER");
	}
	void draw_screen();
};

#endif

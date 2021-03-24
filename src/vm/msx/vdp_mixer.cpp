#include "vdp_mixer.h"

#define	SIG_PRIMARY_VDP_IRQ	0
#define	SIG_SECONDARY_VDP_IRQ	1

vdp_mixer_device::vdp_mixer_device(VM_TEMPLATE* parent_vm, EMU* parent_emu, v9958_device* primary, v9958_device* secondary)
	: DEVICE(parent_vm, parent_emu), p_vdp(primary), s_vdp(secondary), disp_vdp(0), port_swap(0)
{
	initialize_output_signals(&outputs_irq);
	p_vdp->set_context_irq(this, SIG_PRIMARY_VDP_IRQ, 0xffffffff);
	p_vdp->set_context_irq(this, SIG_SECONDARY_VDP_IRQ, 0xffffffff);
}

void vdp_mixer_device::write_io8(uint32_t addr, uint32_t data)
{
	switch (addr & 0xf8) {
	case 0xb8:
		// control register
		port_swap = data & 1;
		disp_vdp = data >> 4;
		break;
	case 0x98:
		if (port_swap == 0) {
			p_vdp->write_io8(addr, data);
			s_vdp->write_io8(addr, data);
		}
		else {
			p_vdp->write_io8(addr, data);
		}
		break;
	case 0x88:
		if (port_swap == 1) {
			s_vdp->write_io8(addr, data);
		}
		break;
	}
}

uint32_t vdp_mixer_device::read_io8(uint32_t addr)
{
	switch (addr & 0xf8) {
	case 0xb8:
		return (disp_vdp << 4) | port_swap;
		break;
	case 0x98:
		return p_vdp->read_io8(addr);
		break;
	case 0x88:
		if (port_swap == 1) {
			return s_vdp->read_io8(addr);
		}
		break;
	}
	return 0xff;
}

void vdp_mixer_device::initialize()
{
	disp_vdp = 0;
	port_swap = 0;
	p_vdp->initialize();
	s_vdp->initialize();
}

void vdp_mixer_device::reset()
{
	disp_vdp = 0;
	port_swap = 0;
	p_vdp->reset();
	s_vdp->reset();
}

void vdp_mixer_device::write_signal(int id, uint32_t data, uint32_t mask)
{
	if (id == SIG_PRIMARY_VDP_IRQ) {
		write_signals(&outputs_irq, data & mask);
	}
	else if (id == SIG_VDP_COMMAND_COMPLETION) {
		p_vdp->write_signal(id, data, mask);
		s_vdp->write_signal(id, data, mask);
	}
}

#define STATE_VERSION	2

bool vdp_mixer_device::process_state(FILEIO* state_fio, bool loading)
{
	if (!state_fio->StateCheckUint32(STATE_VERSION)) {
		return false;
	}
	if (!state_fio->StateCheckInt32(this_device_id)) {
		return false;
	}
	if (!loading) {
		state_fio->Fwrite(&port_swap, sizeof(port_swap), 1);
		state_fio->Fwrite(&disp_vdp, sizeof(disp_vdp), 1);
	}
	else {
		state_fio->Fread(&port_swap, sizeof(port_swap), 1);
		state_fio->Fread(&disp_vdp, sizeof(disp_vdp), 1);
	}
	return true;
}

void vdp_mixer_device::draw_screen()
{
	scrntype_t* dst;
	scrntype_t* buf = new scrntype_t[SCREEN_WIDTH];
	int y;
	p_vdp->prepare_screen();
	s_vdp->prepare_screen();
	for (y = 0; y < SCREEN_HEIGHT; y++) {
		if ((dst = emu->get_screen_buffer(y)) != NULL) {
			memset(dst, 0, SCREEN_WIDTH * sizeof(scrntype_t));
			switch (disp_vdp) {
			case 0:	// Primary VDP only
				p_vdp->draw_line_transparent(dst, y);
				break;
			case 1:	// Secondary VDP only
				s_vdp->draw_line_transparent(dst, y);
				break;
			case 2:	// Primary over Secondary
				s_vdp->draw_line_transparent(dst, y);
				p_vdp->draw_line_transparent(dst, y);
				break;
			case 3:	// Secondary over Primary
				p_vdp->draw_line_transparent(dst, y);
				s_vdp->draw_line_transparent(dst, y);
				break;
			}
		}
	}
	delete[] buf;
}

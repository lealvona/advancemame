/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

#include "jsvgab.h"
#include "log.h"
#include "oslinux.h"
#include "error.h"

#include <vgajoystick.h>

#include <stdio.h>

#define JOYSTICK_MAX 4 /**< Max number of joysticks */

struct joystickb_svgalib_context {
	unsigned counter; /**< Number of joysticks active. */
	char axe_name_buffer[32];
	char button_name_buffer[32];
	char stick_name_buffer[32];
};

static struct joystickb_svgalib_context svgalib_state;

static adv_device DEVICE[] = {
{ "auto", -1, "SVGALIB joystick" },
{ 0, 0, 0 }
};

adv_error joystickb_svgalib_init(int joystickb_id)
{
	unsigned i;

	log_std(("josytickb:svgalib: joystickb_svgalib_init(id:%d)\n", joystickb_id));

	if (!os_internal_svgalib_get()) {
		log_std(("joystickb:svgalib: svgalib not initialized\n"));
		error_nolog_cat("svgalib: Not supported without the svgalib library\n");
		return -1;
	}

	for(i=0;i<4;++i) {
		if (joystick_init(i, 0)<=0) {
			break;
		}
	}

	if (i==0) {
		log_std(("joystickb:svgalib: no joystick found\n"));
		error_nolog_cat("svgalib: No joystick found\n");
		return -1;
	}

	svgalib_state.counter = i;

	return 0;
}

void joystickb_svgalib_done(void)
{
	unsigned i;

	log_std(("josytickb:svgalib: joystickb_svgalib_done()\n"));

	for(i=0;i<svgalib_state.counter;++i)
		joystick_close(i);
}

unsigned joystickb_svgalib_count_get(void)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_count_get()\n"));

	return svgalib_state.counter;
}

unsigned joystickb_svgalib_stick_count_get(unsigned j)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_count_get()\n"));

	assert(j < joystickb_svgalib_count_get());

	return 1;
}

unsigned joystickb_svgalib_stick_axe_count_get(unsigned j, unsigned s)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_axe_count_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(s < joystickb_svgalib_stick_count_get(j) );

	(void)s;

	return joystick_getnumaxes(j);
}

unsigned joystickb_svgalib_button_count_get(unsigned j)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_button_count_get()\n"));

	assert(j < joystickb_svgalib_count_get());

	return joystick_getnumbuttons(j);
}

const char* joystickb_svgalib_stick_name_get(unsigned j, unsigned s)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_name_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(s < joystickb_svgalib_stick_count_get(j) );

	(void)j;

	snprintf(svgalib_state.stick_name_buffer, sizeof(svgalib_state.stick_name_buffer), "S%d", s+1);

	return svgalib_state.stick_name_buffer;
}

const char* joystickb_svgalib_stick_axe_name_get(unsigned j, unsigned s, unsigned a)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_axe_name_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(s < joystickb_svgalib_stick_count_get(j) );
	assert(a < joystickb_svgalib_stick_axe_count_get(j, s) );

	(void)j;
	(void)s;

	snprintf(svgalib_state.axe_name_buffer, sizeof(svgalib_state.axe_name_buffer), "A%d", a+1);

	return svgalib_state.axe_name_buffer;
}

const char* joystickb_svgalib_button_name_get(unsigned j, unsigned b)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_button_name_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(b < joystickb_svgalib_button_count_get(j) );

	(void)j;

	snprintf(svgalib_state.button_name_buffer, sizeof(svgalib_state.button_name_buffer), "B%d", b+1);

	return svgalib_state.button_name_buffer;
}

unsigned joystickb_svgalib_button_get(unsigned j, unsigned b)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_button_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(b < joystickb_svgalib_button_count_get(j) );

	return joystick_getbutton(j, b) != 0;
}

unsigned joystickb_svgalib_stick_axe_digital_get(unsigned j, unsigned s, unsigned a, unsigned d)
{
	int r;
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_axe_digital_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(s < joystickb_svgalib_stick_count_get(j) );
	assert(a < joystickb_svgalib_stick_axe_count_get(j, s) );

	r = joystick_getaxis(j, a);
	if (d)
		return r < -64;
	else
		return r > 64;

	return 0;
}

int joystickb_svgalib_stick_axe_analog_get(unsigned j, unsigned s, unsigned a)
{
	int r;
	log_debug(("joystickb:svgalib: joystickb_svgalib_stick_axe_analog_get()\n"));

	assert(j < joystickb_svgalib_count_get());
	assert(s < joystickb_svgalib_stick_count_get(j) );
	assert(a < joystickb_svgalib_stick_axe_count_get(j, s) );

	r = joystick_getaxis(j, a);
	if (r > 64) /* adjust the upper limit from 127 to 128 */
		++r;
	return r;
}

void joystickb_svgalib_calib_start(void)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_calib_start()\n"));
}

const char* joystickb_svgalib_calib_next(void)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_calib_next()\n"));
	return 0;
}

void joystickb_svgalib_poll(void)
{
	log_debug(("joystickb:svgalib: joystickb_svgalib_poll()\n"));

	joystick_update();
}

unsigned joystickb_svgalib_flags(void)
{
	return 0;
}

adv_error joystickb_svgalib_load(adv_conf* context)
{
	return 0;
}

void joystickb_svgalib_reg(adv_conf* context)
{
}

/***************************************************************************/
/* Driver */

joystickb_driver joystickb_svgalib_driver = {
	"svgalib",
	DEVICE,
	joystickb_svgalib_load,
	joystickb_svgalib_reg,
	joystickb_svgalib_init,
	joystickb_svgalib_done,
	joystickb_svgalib_flags,
	joystickb_svgalib_count_get,
	joystickb_svgalib_stick_count_get,
	joystickb_svgalib_stick_axe_count_get,
	joystickb_svgalib_button_count_get,
	joystickb_svgalib_stick_name_get,
	joystickb_svgalib_stick_axe_name_get,
	joystickb_svgalib_button_name_get,
	joystickb_svgalib_button_get,
	joystickb_svgalib_stick_axe_digital_get,
	joystickb_svgalib_stick_axe_analog_get,
	joystickb_svgalib_calib_start,
	joystickb_svgalib_calib_next,
	joystickb_svgalib_poll
};


/*
 *  Copyright (C) anyka 2012
 *  Wangsheng Gao <gao_wangsheng@anyka.oa>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 */
#ifndef SPI_ANYKA_SLAVE_H_
#define SPI_ANYKA_SLAVE_H_


/*
  * Spi command definitions
  * Support 4294967295 commands
  */

 #define OPEN_WIFI			0x00000001
 #define CLOSE_WIFI			0x00000002

 #define MAX_COMMAND	0xffffffff

#endif /*SPI_ANYKA_SLAVE_H_*/

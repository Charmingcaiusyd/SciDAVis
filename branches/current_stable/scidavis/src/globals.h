/***************************************************************************
    File                 : globals.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Definition of global constants

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

//  Don't forget to change the Doxyfile (and the splash screen) 
//  when changing these!
//! SciDAVis version number
/**
 * 0xMMmmbb means MM.mm.bb with<br>
 * MM = major version
 * mm = minor version
 * bb = bugfix version
 */
int scidavis_version = 0x000104;
//! Extra version information string (like "-alpha", "-beta", "-rc1", etc...)
const char * extra_version = "";
//! Copyright string containing the author names etc.
const char * copyright_string = "Developers (alphabetical order):\nTilman Benkert\nKnut Franke\n\nDocumentation:\nRoger Gadiou\n\nSpecial thanks to (alphabetical order):\nBurkhard Bunk\nQuentin Denis\nGudjon I. Gudjonsson\nAlex Kargovsky\nYu-Hung Lien\nEric Tanguy\nMauricio Troviano\nIon Vasilief\n\nThanks to (no particular order):\nthe developers of Qt, Qwt, QwtPlot3D, GSL, muParser, zlib, Python, PyQt, and liborigin\nall bug reporters, translators and other contributors";

//! Release date as a string
const char * release_date = "2009-02-08";

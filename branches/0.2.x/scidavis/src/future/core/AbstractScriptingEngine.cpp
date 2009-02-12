/***************************************************************************
    File                 : AbstractScriptingEngine.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2009 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Implementations of generic scripting classes
                           
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
#include "AbstractScriptingEngine.h"

AbstractScriptingEngine::AbstractScriptingEngine(const char *lang_name)
{
	setObjectName(lang_name);
	d_initialized=false;
	d_refcount=0;
}

const QString AbstractScriptingEngine::nameAndPatterns() const
{
	QStringList extensions = fileExtensions();
	if (extensions.isEmpty())
		return "";
	else
		return tr("%1 Source (*.%2)").arg(objectName()).arg(extensions.join(" *."));
}

void AbstractScriptingEngine::incref()
{
	d_refcount++;
}

void AbstractScriptingEngine::decref()
{
	d_refcount--;
	if (d_refcount==0)
		delete this;
}

/******************************************************************************\
 *Helper classes for managing instances of AbstractScriptingEngine subclasses.*
\******************************************************************************/

scripted::scripted(AbstractScriptingEngine *engine)
{
	if (engine)
		engine->incref();
	d_scripting_engine = engine;
}

scripted::~scripted()
{
	if (d_scripting_engine)
		d_scripting_engine->decref();
}

void scripted::scriptingChangeEvent(ScriptingChangeEvent *sce)
{
	d_scripting_engine->decref();
	sce->scriptingEngine()->incref();
	d_scripting_engine = sce->scriptingEngine();
}

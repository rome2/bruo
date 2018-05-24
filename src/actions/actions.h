////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    actions.h
///\ingroup bruo
///\brief   Action definition header collector.
///\author  Rolf Meyerhoff (badlantic@gmail.com)
///\version 1.0
/// This file is part of the bruo audio editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\par
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///\par
/// You should have received a copy of the GNU General Public License along with
/// this program; see the file COPYING. If not, see http://www.gnu.org/licenses/
/// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
/// Floor, Boston, MA 02110-1301, USA.
////////////////////////////////////////////////////////////////////////////////
#ifndef __ACTIONS_H_INCLUDED__
#define __ACTIONS_H_INCLUDED__

// Base classes:
#include "selectionaction.h"
#include "activedocumentaction.h"

// Action definitions:
#include "aboutaction.h"
#include "aboutqtaction.h"
#include "clearrecentfilesaction.h"
#include "clearundoaction.h"
#include "closealldocumentsaction.h"
#include "closedocumentaction.h"
#include "copyaction.h"
#include "cutaction.h"
#include "deleteaction.h"
#include "exitaction.h"
#include "extendselectiondoublelengthaction.h"
#include "extendselectiontoallchannelsaction.h"
#include "extendselectiontocursoraction.h"
#include "extendselectiontoendaction.h"
#include "extendselectiontonextmarkeraction.h"
#include "extendselectiontopreviousmarkeraction.h"
#include "extendselectiontostartaction.h"
#include "gohomeaction.h"
#include "newdocumentaction.h"
#include "newfromclipboardaction.h"
#include "norecentfilesaction.h"
#include "opendocumentaction.h"
#include "openrecentfileaction.h"
#include "pasteaction.h"
#include "printpreviewaction.h"
#include "printstatsaction.h"
#include "redoaction.h"
#include "savealldocumentsaction.h"
#include "savedocumentaction.h"
#include "savedocumentasaction.h"
#include "selectallaction.h"
#include "selectnothingaction.h"
#include "settingsaction.h"
#include "showmorerecentfilesaction.h"
#include "showstatsaction.h"
#include "shrinkselectionhalflengthaction.h"
#include "undoaction.h"
#include "zoomselectionaction.h"

#endif // __ACTIONS_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////

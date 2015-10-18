////////////////////////////////////////////////////////////////////////////////
// (c) 2015 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    extendselectiontopreviousmarkeraction.cpp
///\ingroup bruo
///\brief   Extend selection to previous marker action implementation.
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
#include "extendselectiontopreviousmarkeraction.h"
#include "../mainframe.h"

////////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToPreviousMarkerAction::ExtendSelectionToPreviousMarkerAction()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this action.
///\param   [in] parent:  Parent object for this action.
///\remarks Initializes the action states, strings, events and icons.
////////////////////////////////////////////////////////////////////////////////
ExtendSelectionToPreviousMarkerAction::ExtendSelectionToPreviousMarkerAction(MainFrame* parent) :
  QAction(QIcon(":/images/edit-select-to-prev-marker.png"), tr("Extend to &previous marker"), parent),
  m_parent(parent)
{
  setEnabled(false);
  setStatusTip(tr("Extend selection to the previus marker"));
  connect(parent->manager(), SIGNAL(documentCreated(Document*)), this, SLOT(documentCreated(Document*)));
  connect(parent->manager(), SIGNAL(activeDocumentChanged()), this, SLOT(selectionChanged()));
  connect(this, SIGNAL(triggered()), this, SLOT(fired()));
}

////////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToPreviousMarkerAction::documentCreated()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's documentCreated signal.
///\remarks This one is called whenever a new document was created. At this
///         stage the document is still empty but this handler is good place to
///         attach per document signal handlers etc.
////////////////////////////////////////////////////////////////////////////////
void ExtendSelectionToPreviousMarkerAction::documentCreated(Document* doc)
{
  // Connect to the document's selection slot:
  connect(doc, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  connect(doc, SIGNAL(markerChanged()), this, SLOT(selectionChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToPreviousMarkerAction::selectionChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for selection state changes.
///\remarks This one is called whenever the selection was changed.
////////////////////////////////////////////////////////////////////////////////
void ExtendSelectionToPreviousMarkerAction::selectionChanged()
{
  // Get document:
  Document* doc = m_parent->manager()->activeDocument();

  // Update enabled state:
  setEnabled(doc != 0 && doc->selectionLength() > 0);
}

//////////////////////////////////////////////////////////////////////////////
// ExtendSelectionToPreviousMarkerAction::fired()
//////////////////////////////////////////////////////////////////////////////
///\brief The function where the action happens.
//////////////////////////////////////////////////////////////////////////////
void ExtendSelectionToPreviousMarkerAction::fired()
{
}

///////////////////////////////// End of File //////////////////////////////////

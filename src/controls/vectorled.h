////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    vectorled.h
///\ingroup bruo
///\brief   Vector based LED widget definition.
///\author  Rolf Meyerhoff (rm@matrix44.de)
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
#ifndef __VECTORLED_H_INCLUDED__
#define __VECTORLED_H_INCLUDED__

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

////////////////////////////////////////////////////////////////////////////////
///\class VectorLED vectorled.h
///\brief A vectorbased LED control.
/// This widget is just a simple LED with interaction.
////////////////////////////////////////////////////////////////////////////////
class VectorLED :
  public QWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::VectorLED()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  VectorLED(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::~VectorLED()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~VectorLED();

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this LED.
  ///\remarks The value is either true or false.
  //////////////////////////////////////////////////////////////////////////////
  bool value() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this LED.
  ///\remarks The widget will be repainted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const bool newVal);

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::tag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Tag property.
  ///\return  The currently stored user value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  int tag() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::setTag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Tag property.
  ///\param   [in] newTag: The new user defined value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  void setTag(const int newTag);

  const QColor& backColor() const;
  void setBackColor(const QColor& color);

signals:

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::clicked()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Clicked event.
  ///\remarks This event is fired when this widget was clicked.
  //////////////////////////////////////////////////////////////////////////////
  void clicked();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::paintEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the paint signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void paintEvent(QPaintEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorLED::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse released signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  virtual void mouseReleaseEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::changeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for general state change signals.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void changeEvent(QEvent* event);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  bool m_value;       ///\> The current value of this LED.
  int  m_tag;         ///\> User defined value.
  QColor m_backColor; ///\> Background color.
};

#endif // __VECTORLED_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////

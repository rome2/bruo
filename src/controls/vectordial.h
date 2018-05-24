////////////////////////////////////////////////////////////////////////////////
// (c) 2017 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    vectordial.h
///\ingroup bruo
///\brief   Vector based rotary widget definition.
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
#ifndef __VECTORDIAL_H_INCLUDED__
#define __VECTORDIAL_H_INCLUDED__

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

////////////////////////////////////////////////////////////////////////////////
///\class VectorDial vectordial.h
///\brief An vector based dial control.
/// This widget serves basically the same purpose as the QDial control but it is
/// based on custom drawing with some extra functions. Additionally it works in
/// linear mode as well and it supports relative movements. A default value can
/// be set that will be recalled when a user double clicks the widget.
////////////////////////////////////////////////////////////////////////////////
class VectorDial :
  public QWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::VectorDial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  VectorDial(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::~VectorDial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~VectorDial();

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is always in the range [0,1].
  //////////////////////////////////////////////////////////////////////////////
  double value() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this dial.
  ///\remarks The value is clipped to the range [0,1]. If this value is set then
  ///         the widget will be repainted and a valueChanged event will be
  ///         emitted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const double newVal);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::defaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DefaultValue property.
  ///\return  The default value of this dial.
  ///\remarks The value is always in the range [0,1]. To load the default value
  ///\        just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  double defaultValue() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setDefaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the DefaultValue property.
  ///\param   [in] newVal: The new default value of this dial.
  ///\remarks The value is clipped to the range [0,1]. To load the default value
  ///\        just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  void setDefaultValue(const double newVal);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::absoluteMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the AbsoluteMode property.
  ///\return  Whether absolute mode is enabled or not.
  ///\remarks In absolute mode the dial value will instantly change to the
  ///         value indicated by the mouse click position. In relative mode it
  ///         will be changed relative to the click position.
  //////////////////////////////////////////////////////////////////////////////
  bool absoluteMode() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setAbsoluteMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the AbsoluteMode property.
  ///\param   [in] newState: Whether to enable absolute mode or not.
  ///\remarks In absolute mode the dial value will instantly change to the
  ///         value indicated by the mouse click position. In relative mode it
  ///         will be changed relative to the click position.
  //////////////////////////////////////////////////////////////////////////////
  void setAbsoluteMode(const bool newState);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::circularMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the CircularMode property.
  ///\return  Whether circular motion is enabled or linear.
  ///\remarks In circular mode value can be changed by a circular move around
  ///         the center of the widget. In linear mode it is just an up down
  ///         movement. The linear movement range is defined by the LinearSize
  ///         property.
  //////////////////////////////////////////////////////////////////////////////
  bool circularMode() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setCircularMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the CircularMode property.
  ///\param   [in]WnewState: Whether to enable circular motion or linear.
  ///\remarks In circular mode value can be changed by a circular move around
  ///         the center of the widget. In linear mode it is just an up down
  ///         movement. The linear movement range is defined by the LinearSize
  ///         property.
  //////////////////////////////////////////////////////////////////////////////
  void setCircularMode(const bool newState);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::linearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LinearSize property.
  ///\return  The current extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,1] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  int linearSize() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setLinearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the LinearSize property.
  ///\param   [in] newSize: The new extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,1] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  void setLinearSize(const int newSize);

  double arcBase() const;
  void setArcBase(const double base);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::tag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Tag property.
  ///\return  The currently stored user value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  int tag() const;

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::setTag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Tag property.
  ///\param   [in] newTag: The new user defined value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  void setTag(const int newTag);

signals:

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief This signal is emitted when the value of this dial changes.
  ///\param [in] newValue: The new value of this control.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged(double newValue);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::mouseReleased()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief This signal is emitted when the mouse was released.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleased();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::wheelEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse wheel signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::mousePressEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse button pressed signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mousePressEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse button released signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::mouseDoubleClickEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse double click signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseDoubleClickEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::mouseMoveEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse moved signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseMoveEvent(QMouseEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::paintEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the paint signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void paintEvent(QPaintEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::changeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for general state change signals.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void changeEvent(QEvent* event);

private:

  //////////////////////////////////////////////////////////////////////////////
  // VectorDial::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Internal helper to calc a dial value for mouse coordinates.
  ///\param  [in] mx: Input position, X component.
  ///\param  [in] my: Input position, Y component.
  ///\return The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  double valueFromMousePos(const int mx, const int my) const;

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  double m_value;        ///\> The current value of this dial.
  double m_defaultValue; ///\> The default value of this value.
  bool   m_absoluteMode; ///\> Use absolute or relative movement?
  bool   m_circularMode; ///\> Use linear or circular movement?
  int    m_startY;       ///\> Mouse down position for linear movement.
  double m_startVal;     ///\> Value when the movement starts.
  int    m_linearSize;   ///\> Scaling for linear movement.
  int    m_tag;          ///\> User defined value.
  bool   m_drawArc;      ///\> Draw the value arc?
  double m_arcBase;      ///\> Where is the zero position of the arc?
};

#endif // __VECTORDIAL_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
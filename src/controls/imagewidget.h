////////////////////////////////////////////////////////////////////////////////
// (c) 2014 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    imagewidget.h
///\ingroup bruo
///\brief   Image based widget definition.
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
#ifndef __IMAGEWIDGET_H_INCLUDED__
#define __IMAGEWIDGET_H_INCLUDED__

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

////////////////////////////////////////////////////////////////////////////////
///\class ImageWidget imagewidget.h
///\brief Abstract base class for all image based widgets.
////////////////////////////////////////////////////////////////////////////////
class ImageWidget :
  public QWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::ImageWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  ImageWidget(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::~ImageWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~ImageWidget();

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::image()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property.
  ///\return  The current image.
  ///\remarks This image is the source for the widget.
  //////////////////////////////////////////////////////////////////////////////
  QPixmap& image();

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::image()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property, const version.
  ///\return  The current image.
  ///\remarks This image is the source for the widget.
  //////////////////////////////////////////////////////////////////////////////
  const QPixmap& image() const;

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::setImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Image property.
  ///\param   [in] newImage: The new image.
  ///\remarks This image is the source for the widget.
  //////////////////////////////////////////////////////////////////////////////
  void setImage(const QPixmap& newImage);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::disabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DisabledImage property.
  ///\return  The current disabled state image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the knob movie image.
  //////////////////////////////////////////////////////////////////////////////
  const QPixmap& disabledImage() const;

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::setDisabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the DisabledImage property.
  ///\param   [in] newImage: The new image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the knob movie image.
  //////////////////////////////////////////////////////////////////////////////
  void setDisabledImage(const QPixmap& newImage);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::tag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Tag property.
  ///\return  The currently stored user value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  int tag() const;

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::setTag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Tag property.
  ///\param   [in] newTag: The new user defined value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  void setTag(const int newTag);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::paintEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the paint signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void paintEvent(QPaintEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::changeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for general state change signals.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void changeEvent(QEvent* event);

  //////////////////////////////////////////////////////////////////////////////
  // ImageWidget::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  virtual void drawWidget(QPainter& qp) = 0;

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QPixmap m_image;         ///\> The knob movie image strip.
  QPixmap m_disabledImage; ///\> The knob movie image strip.
  int     m_tag;           ///\> User defined value.
};

#endif // __IMAGEWIDGET_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////

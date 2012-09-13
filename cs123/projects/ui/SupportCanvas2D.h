/*!
   @file   SupportCanvas2D.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef SUPPORTCANVAS2D_H
#define SUPPORTCANVAS2D_H

#include <QWidget>
#include "CS123Common.h"

class SupportCanvas2D : public QWidget
{
    Q_OBJECT
public:
    SupportCanvas2D();
    virtual ~SupportCanvas2D();

    /*! Return an array containing the rows of the image one after another, starting from the top.  Each row
     is width() wide and there are height() rows, so this array will have width() * height() elements. */
    BGRA *data() { return (BGRA *)m_image->bits(); }

    //! Resize the canvas (this will resize the window too and set the image to black)
    void resize(int width, int height);

    //! Load or save the canvas given a specific file path.  Return true on success.
    bool loadImage(const QString &file);
    bool saveImage(const QString &file);

    //! Return two corners of the marquee selection (if there is no selection, the corners will be the same)
    QPoint marqueeStart() { return m_marqueeStart; }
    QPoint marqueeStop() { return m_marqueeStop; }


    //! Clears the current selection.
    void clearSelection() { m_marqueeStart = m_marqueeStop; }
    // Developer's note: An empty selection is one where both corners are the same



    //! Load a black 500x500 image
    void newImage();

    //! Show a file dialog and perform the save action if the user picks a file.  Return true on success.
    bool saveImage();

public slots:
    bool revertImage();

protected:
    // overridden from QWidget
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void mouseDown(int x, int y) = 0;
    virtual void mouseDragged(int x, int y) = 0;
    virtual void mouseUp(int x, int y) = 0;

    //! This will be called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h) = 0;

    //! This will be called when the settings have changed
    virtual void settingsChanged() = 0;

    QImage *m_image;

private:
    QPoint m_marqueeStart, m_marqueeStop;
    QString m_lastfile;
    bool m_draggingRight;
    bool m_draggingLeft;
};

#endif // SUPPORTCANVAS2D_H

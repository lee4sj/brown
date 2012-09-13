/*!
   @file   Databinding.h
   @author Evan Wallace (evan.exe@gmail.com)
   @author Ben Herila (ben@herila.net)
   @date   Fall 2010
*/

#ifndef DATABINDING_H
#define DATABINDING_H

#include <QObject>
#include <QVariant>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDockWidget>
#include <QTabWidget>
#include <QDial>

#include <CS123Algebra.h>

/*!

@class DataBinding
@brief Binds a slider and a textbox to an int.

This class binds a slider and a textbox to an int, so when either the slider or the textbox
is changed, the other is also changed and the int is updated with the new value.  This does
not update either the slider or the textbox when the int is set to a new value.

**/
class DataBinding : public QObject
{
    Q_OBJECT

public:
    DataBinding(QObject *parent = 0) : QObject(parent) {}
    virtual ~DataBinding() {}

signals:
    void dataChanged();

};

/*!

@class IntBinding
@brief Binds a slider and a textbox to an int.

This class binds a slider and a textbox to an int, so when either the slider or the textbox
is changed, the other is also changed and the int is updated with the new value.  This does
not update either the slider or the textbox when the int is set to a new value.

**/
class IntBinding : public DataBinding
{
    Q_OBJECT
public:
    virtual ~IntBinding() {}

    static IntBinding* bindSliderAndTextbox(QSlider *slider, QLineEdit *textbox, int &value, int minValue, int maxValue);
    static IntBinding* bindTextbox(QLineEdit *textbox, int &value);

private slots:
    void intChanged(int newValue);
    void stringChanged(QString newValue);

signals:
    void updateInt(int newValue);
    void updateString(QString newValue);

private:
    IntBinding(int &value) : DataBinding(), m_value(value) {}

    int &m_value;
};

/*!

@class FloatBinding
@brief Binds a slider and a textbox to a REAL.

This class binds a slider and a textbox to a REAL, so when either the slider or the textbox
is changed, the other is also changed and the REAL is updated with the new value.  This does
not update either the slider or the textbox when the REAL is set to a new value.

**/
class FloatBinding : public DataBinding
{
    Q_OBJECT
public:
    virtual ~FloatBinding() {}

    static FloatBinding* bindSliderAndTextbox(QSlider *slider, QLineEdit *textbox, REAL &value, REAL minValue, REAL maxValue);
    static FloatBinding* bindDial(QDial *dial, REAL &value, REAL minValue, REAL maxValue, bool wrappingExtendsRange);

private slots:
    void intChanged(int newValue);
    void stringChanged(QString newValue);

signals:
    void updateInt(int newValue);
    void updateString(QString newValue);

private:
    FloatBinding(REAL &value) : DataBinding(), m_value(value), m_maxValue(0), m_minValue(0), m_offset(0), m_wrappingExtendsRange(false) {}

    REAL &m_value, m_maxValue, m_minValue, m_offset;
    bool m_wrappingExtendsRange;
};

/*!

@class BoolBinding
@brief Binds a checkbox to a Boolean value.

This class binds a checkbox to a bool, so when the checkbox is changed the bool is updated
with the new value.  This does not update the checkbox when the bool is set to a new value.

**/
class BoolBinding : public DataBinding
{
    Q_OBJECT
public:
    virtual ~BoolBinding() {}

    static BoolBinding* bindCheckbox(QCheckBox *checkbox, bool &value);
    static BoolBinding* bindDock(QDockWidget *dock, bool &value);

private slots:
    void boolChanged(bool newValue);

private:
    BoolBinding(bool &value) : DataBinding(), m_value(value) {}

    bool &m_value;
};

/*!

@class ChoiceBinding
@brief Binds a group of radio buttons to an int (enum) value.

This class binds a group of radio buttons to an int, so when a radio button is clicked the
int is updated with the index of the currently checked button in the group.  This does not
update the radio buttons when the int is set to a new value.

**/
class ChoiceBinding : public DataBinding
{
    Q_OBJECT
public:
    virtual ~ChoiceBinding() {}

    static ChoiceBinding* bindRadioButtons(int numRadioButtons, int &value, ...);
    static ChoiceBinding* bindTabs(QTabWidget *tabs, int &value);

private slots:
    void intChanged(int newValue);

private:
    ChoiceBinding(int &value) : DataBinding(), m_value(value) {}

    int &m_value;
};

#endif // DATABINDING_H

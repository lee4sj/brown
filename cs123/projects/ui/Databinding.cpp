#include "Databinding.h"
#include <math.h>
#include <QVariant>

////////////////////////////////////////////////////////////////////////////////
// class IntBinding
////////////////////////////////////////////////////////////////////////////////

IntBinding* IntBinding::bindSliderAndTextbox(QSlider *slider, QLineEdit *textbox, int &value, int minValue, int maxValue)
{
    // Bind the slider, the textbox, and the value together
    IntBinding *binding = new IntBinding(value);
    connect(slider, SIGNAL(sliderMoved(int)), binding, SLOT(intChanged(int)));
    connect(textbox, SIGNAL(textChanged(QString)), binding, SLOT(stringChanged(QString)));
    connect(binding, SIGNAL(updateInt(int)), slider, SLOT(setValue(int)));
    connect(binding, SIGNAL(updateString(QString)), textbox, SLOT(setText(QString)));

    // Set the range and initial value
    slider->setMinimum(minValue);
    slider->setMaximum(maxValue);
    slider->setValue(value);
    textbox->setText(QString::number(value));

    return binding;
}

IntBinding* IntBinding::bindTextbox(QLineEdit *textbox, int &value)
{
    // Bind the the textbox and the value together
    IntBinding *binding = new IntBinding(value);
    connect(textbox, SIGNAL(textChanged(QString)), binding, SLOT(stringChanged(QString)));

    // Set the initial value
    textbox->setText(QString::number(value));

    return binding;
}

void IntBinding::intChanged(int newValue)
{
    if (m_value != newValue)
    {
        m_value = newValue;
        emit updateString(QString::number(m_value));
        emit dataChanged();
    }
}

void IntBinding::stringChanged(QString newValue)
{
    int intValue = newValue.toInt();
    if (m_value != intValue)
    {
        m_value = intValue;
        emit updateInt(m_value);
        emit dataChanged();
    }
}

////////////////////////////////////////////////////////////////////////////////
// class FloatBinding
////////////////////////////////////////////////////////////////////////////////

// Sliders can only work in ints, so use 100 slider units for every value unit (two decimals of resolution)
inline REAL mapValue(int i) { return (REAL)i * 0.01f; }
inline int mapValue(REAL f) { return (int)ceilf(f * 100 - 0.5f); }

FloatBinding* FloatBinding::bindSliderAndTextbox(QSlider *slider, QLineEdit *textbox, REAL &value, REAL minValue, REAL maxValue)
{
    // Bind the slider, the textbox, and the value together
    FloatBinding *binding = new FloatBinding(value);
    connect(slider, SIGNAL(sliderMoved(int)), binding, SLOT(intChanged(int)));
    connect(textbox, SIGNAL(textChanged(QString)), binding, SLOT(stringChanged(QString)));
    connect(binding, SIGNAL(updateInt(int)), slider, SLOT(setValue(int)));
    connect(binding, SIGNAL(updateString(QString)), textbox, SLOT(setText(QString)));
    //connect(binding, SIGNAL(updateString(QString)), binding, SLOT(dataChanged()));

    // Set the range and initial value
    slider->setMinimum(mapValue(minValue));
    slider->setMaximum(mapValue(maxValue));
    slider->setValue(mapValue(value));
    textbox->setText(QString::number(value));

    return binding;
}

FloatBinding* FloatBinding::bindDial(QDial *dial, REAL &value, REAL minValue, REAL maxValue, bool wrappingExtendsRange)
{
    // Bind the dial and the value together
    FloatBinding *binding = new FloatBinding(value);
    connect(dial, SIGNAL(valueChanged(int)), binding, SLOT(intChanged(int)));

    // Store state for computing dial wrapping
    binding->m_maxValue = maxValue;
    binding->m_minValue = minValue;
    binding->m_wrappingExtendsRange = wrappingExtendsRange;

    // Set the initial value
    dial->setMinimum(mapValue(minValue));
    dial->setMaximum(mapValue(maxValue));
    dial->setValue(mapValue(value));

    return binding;
}

void FloatBinding::intChanged(int newValue)
{
    REAL REALValue = mapValue(newValue);

    if (m_wrappingExtendsRange)
    {
        REAL lowerBound = m_minValue * 0.75f + m_maxValue * 0.25f;
        REAL upperBound = m_minValue * 0.25f + m_maxValue * 0.75f;

        // This is a hack to get dials to wrap around infinitely
        //
        //        A           B           C           D
        //  |-----------|-----------|-----------|-----------|
        // min     lowerBound              upperBound      max
        //
        // Any jump from A -> D or D -> A is considered a wrap around

        if (m_value - m_offset < lowerBound && REALValue > upperBound)
            m_offset -= m_maxValue - m_minValue;
        else if (m_value - m_offset > upperBound && REALValue < lowerBound)
            m_offset += m_maxValue - m_minValue;
    }

    if (m_value - m_offset != REALValue)
    {
        m_value = REALValue + m_offset;
        emit updateString(QString::number(m_value - m_offset));
        emit dataChanged();
    }
}

void FloatBinding::stringChanged(QString newValue)
{
    REAL REALValue = newValue.toFloat();
    if (m_value - m_offset != REALValue)
    {
        m_value = REALValue + m_offset;
        emit updateInt(mapValue(m_value - m_offset));
        emit dataChanged();
    }
}

////////////////////////////////////////////////////////////////////////////////
// class BoolBinding
////////////////////////////////////////////////////////////////////////////////

BoolBinding* BoolBinding::bindCheckbox(QCheckBox *checkbox, bool &value)
{
    // Bind the checkbox and the value together
    BoolBinding *binding = new BoolBinding(value);
    connect(checkbox, SIGNAL(toggled(bool)), binding, SLOT(boolChanged(bool)));

    // Set the initial value
    checkbox->setChecked(value);

    return binding;
}

BoolBinding* BoolBinding::bindDock(QDockWidget *dock, bool &value)
{
    // Bind the checkbox and the value together
    BoolBinding *binding = new BoolBinding(value);
    connect(dock, SIGNAL(visibilityChanged(bool)), binding, SLOT(boolChanged(bool)));

    // Set the initial value
    dock->setVisible(value);

    return binding;
}

void BoolBinding::boolChanged(bool newValue)
{
    m_value = newValue;
    emit dataChanged();
}

////////////////////////////////////////////////////////////////////////////////
// class ChoiceBinding
////////////////////////////////////////////////////////////////////////////////

ChoiceBinding* ChoiceBinding::bindRadioButtons(int numRadioButtons, int &value, ...)
{
    // Create a button group from the variable argument list following initialValue
    QButtonGroup *buttonGroup = new QButtonGroup;
    va_list args;
    va_start(args, value);
    for (int id = 0; id < numRadioButtons; id++)
        buttonGroup->addButton(va_arg(args, QRadioButton *), id);
    va_end(args);

    // Bind the button group and the value together
    ChoiceBinding *binding = new ChoiceBinding(value);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), binding, SLOT(intChanged(int)));

    // Set the initial value
    value = qMax(0, qMin(numRadioButtons - 1, value));
    buttonGroup->button(value)->click();

    return binding;
}

ChoiceBinding* ChoiceBinding::bindTabs(QTabWidget *tabs, int &value)
{
    // Bind the tabs and the value together
    ChoiceBinding *binding = new ChoiceBinding(value);
    connect(tabs, SIGNAL(currentChanged(int)), binding, SLOT(intChanged(int)));

    // Set the initial value
    value = qMax(0, qMin(tabs->count() - 1, value));
    tabs->setCurrentIndex(value);

    return binding;
}

void ChoiceBinding::intChanged(int newValue)
{
    m_value = newValue;
    emit dataChanged();
}

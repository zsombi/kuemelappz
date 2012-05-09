/**
    Style - non-visual element that defines the basic proeprties of a style element.
    The element serves as type identification helper.

    A style element is identified by a name and the layout type. The element can be
    used in defining component styles aswell as defining local styles (see StyledItem
    for local styles). Component style elements can define component specific proeprties,
    which will be passed to the components upon use.

    Properties:

        proeprty name: string
            Defines the style name.

        property type: StyleType
            Defines the style type. This can be one of the following values:
            Style.Normal - normal layout
            Style.Pressed - pressed layout
            Style.Dimmed - dimmed (disabled) layout
            Style.Highlighted - higlighted layout, e.g. used when mouse hovering over
                                the component
            Style.Expanded - expanded layout, used in expand/collapse type components

  */
#include "style.h"
#include "globaldefs.h"

//#define TRACE_STYLE

Style::Style(QObject *parent) :
    QObject(parent),
    m_name(""),
    m_type(LastStyleType)
{
}

Style::~Style()
{
#ifdef TRACE_STYLE
    qDebug() << "clearing" << m_name << "type" << m_type;
#endif
}

// name property getter/setter
QString Style::name() const
{
    return m_name;
}
void Style::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        // set the type to Normal if it hasn't been set yet
        if (!m_name.isEmpty() && (m_type == LastStyleType))
            m_type = Normal;
        emit nameChanged();
    }
}

// type proeprty getter/setter
Style::StyleType Style::type() const
{
    return m_type;
}
void Style::setType(Style::StyleType set)
{
    if (set != m_type) {
        m_type = set;
        emit typeChanged();
    }
}
QVariantList Style::types() const
{
    return m_types;
}

void Style::setTypes(const QVariantList &t)
{
    m_types = t;
    qDebug() << "Types=" << t;
}

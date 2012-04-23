#include "style.h"
#include "globaldefs.h"

Style::Style(QObject *parent) :
    QObject(parent),
    m_name(""),
    m_set(LastStyleType)
{
}

Style::~Style()
{
    qDebug() << "clearing" << m_name << "type" << m_set;
}

void Style::updateStyle()
{
    //TODO: loop through the current properties and reset values
}

QString Style::name() const
{
    return m_name;
}
void Style::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        if (!m_name.isEmpty() && (m_set == LastStyleType))
            m_set = Normal;
        emit nameChanged();
    }
}

Style::Set Style::setType() const
{
    return m_set;
}

void Style::setSetType(Style::Set set)
{
    if (set != m_set) {
        m_set = set;
        // TODO: load set values and update properties
        //qDebug() << "Style:" << m_name << "type" << m_set;
        emit setChanged();
    }
}

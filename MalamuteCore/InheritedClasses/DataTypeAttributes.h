#ifndef DATATYPEATTRIBUTES_H
#define DATATYPEATTRIBUTES_H

#include <QString>
#include <QStringList>

class DataTypeAttributes
{
public:
    QString name() const;
    QString color() const;
    QString abbreviation() const;
    QString description() const;
    QStringList authors() const;
    QString defaultDisplay() const;
    QString defaultSource() const;

    int id() const;
    void setID(int value);


protected:
    QString m_name = "Unnamed";
    QString m_color = "black";
    QString m_abbreviation = "???";
    QString m_description = "Give the data type a description";
    QStringList m_authors = QStringList();
    QString m_defaultDisplay = "";
    QString m_defaultSource = "";

private:
    int m_id = -1;
};

#endif // DATATYPEATTRIBUTES_H

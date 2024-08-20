#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamWriter>
#include <QString>
#include <QList>

class Stock;

class XMLWriter
{
public:
    XMLWriter(QString fn);
    void write(QList<Stock*> list);

private:
    QXmlStreamWriter writer;
    QString filename;
};

#endif // XMLWRITER_H

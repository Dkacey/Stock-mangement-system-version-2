#include "xmlwriter.h"
#include "confectionery.h"
#include "readingmaterial.h"
#include <QFile>

XMLWriter::XMLWriter(QString fn): filename{fn}
{
}

void XMLWriter::write(QList<Stock *> list)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    writer.setDevice(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("stockList");

    for (int i=0; i<list.size(); i++)
    {
        Stock *item = list.at(i);
        const QMetaObject* meta = item->metaObject();
        QString classname = QString(meta->className());

        //stockItem tag
        writer.writeStartElement("stockItem");
        QXmlStreamAttribute attr("type", classname);
        writer.writeAttribute(attr);

        //add other tags
        writer.writeTextElement("item", item->getItem());
        writer.writeTextElement("quantity", QString::number(item->getQuantity()));
        if (classname == "Confectionery")
        {
            Confectionery* c = qobject_cast<Confectionery*>(item);
            writer.writeTextElement("weight", QString::number(c->getWeight()));
        }
        else if (classname == "ReadingMaterial")
        {
            ReadingMaterial* rm = qobject_cast<ReadingMaterial*>(item);
            writer.writeTextElement("frequency", rm->getFrequency());
        }
        writer.writeEndElement(); // end of stockItem element
    }

    writer.writeEndElement(); // end of stockList element

    writer.writeEndDocument();
    file.close();
}

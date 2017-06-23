#include "rbsdocument.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>
#include "logging.h"

RBSDocument::RBSDocument(ADeviceImplementor *_impl,
                         const QString &_encoding) :
    Device(_impl),
    encoding_(_encoding)
{
}

RBSDocument::~RBSDocument()
{
}

QString RBSDocument::getText()
{
    QString text;
    if (open(ReadOnly))
    {
        QByteArray raw_data = readAll();
        close();
        QTextCodec* tc = QTextCodec::codecForName(qPrintable(encoding_));

        if (tc == NULL)
        {
            // если кодировка была не пустая, то она странная.
            if (!encoding_.isEmpty())
                qWarning()<<tr("У документа %1 была странная кодировка (%2).\n"
                               "Кодировка будет изменена на UTF-8")
                            .arg(getAbsolutePath()).arg(encoding_);
            setEncoding("UTF-8");
            text = getText();
        }
        else
        {

            text = tc->toUnicode(raw_data);
        }
    }

    return text;
}

void RBSDocument::setText(const QString &_text)
{
    QTextCodec* tc = QTextCodec::codecForName(qPrintable(encoding_));
    if (!tc)
    {
        setEncoding("UTF-8");
        setText(_text);
    }
    else
    {

        QByteArray raw_data = tc->fromUnicode(_text);
        if (open(WriteOnly | Truncate))
        {
            write(raw_data);
            close();
        }
    }
}

void RBSDocument::setEncoding(const QString &_encoding)
{
    if (encoding_ != _encoding)
    {
        encoding_ = _encoding;
        emit encodingChanged(encoding_);
    }
}

QString RBSDocument::getEncoding() const
{
    return encoding_;
}

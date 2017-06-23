#include "rbsfile.h"
#include <QDebug>

#define MAX_FILE_SIZE 20971520 //~20 MB
RBSFile::RBSFile(const QString &_abspath) :
    QFile(_abspath)
{
//    qDebug()<<"create RBSFile for"<<_abspath<<size();
}

bool RBSFile::open(QIODevice::OpenMode flags)
{
    if (size() > MAX_FILE_SIZE)
    {
        qWarning()<<"RBS не открывает файлы больше 20 мб";
        return false;
    }
    emit rbsOpenFile(flags);
    return QFile::open(flags);
}

void RBSFile::close()
{
    QFile::close();
    emit rbsCloseFile();
}

qint64 RBSFile::writeData(const char *data, qint64 len)
{
    emit rbsWriteToFile();
    return QFile::writeData(data,len);
}

qint64 RBSFile::readData(char *data, qint64 maxlen)
{
    return QFile::readData(data,maxlen);
}

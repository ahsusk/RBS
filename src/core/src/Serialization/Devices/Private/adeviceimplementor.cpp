#include "adeviceimplementor.h"
#include "rbsfile.h"

ADeviceImplementor::ADeviceImplementor(const QString &_absolute_path) :
    file_(new RBSFile(_absolute_path))
{
    setObjectName(_absolute_path);

    connect(file_,
            SIGNAL(rbsCloseFile()),
            this,
            SIGNAL(rbsCloseFile()),
            Qt::DirectConnection);

    connect(file_,
            &RBSFile::rbsOpenFile,
            [this](QIODevice::OpenMode _flags)
    {
        if (_flags & QIODevice::WriteOnly)
            emit rbsOpenFile();
    });
}

ADeviceImplementor::~ADeviceImplementor()
{
    delete file_;
}

QByteArray ADeviceImplementor::readAll()
{
    QByteArray result;
    if (open(QIODevice::ReadOnly))
    {
        qint64 readed;
        do
        {
            char buff[1024];
            readed = read(buff,1024);
            if (readed > 0)
                result.append(buff,readed);

        }while(readed > 0);
        close();
    }
    return result;
}


bool ADeviceImplementor::needAskForSaving() const
{
    return true;
}

bool ADeviceImplementor::isWriteOnlyDevice() const
{
    return false;
}

QUrl ADeviceImplementor::getUrl() const
{
    return QUrl::fromLocalFile(getAbsolutePath());
}

QString ADeviceImplementor::getFileName() const
{
    return getFileInfo().fileName();
}

QDir ADeviceImplementor::getDirectory() const
{
    return getFileInfo().absoluteDir();
}

QString ADeviceImplementor::getAbsoluteDir() const
{
    return getFileInfo().absolutePath();
}

QFileInfo ADeviceImplementor::getFileInfo() const
{
    return QFileInfo(getAbsolutePath());
}

QString ADeviceImplementor::getAbsolutePath() const
{
    return objectName();
}

bool ADeviceImplementor::checkFile(QIODevice::OpenMode _mode) const
{
    return (file_->openMode() & _mode);
}

QFile& ADeviceImplementor::_get_real_file()
{
    return *file_;
}

bool ADeviceImplementor::_check_or_open(QIODevice::OpenMode _mode)
{
    return checkFile(_mode) || file_->open(_mode);
}


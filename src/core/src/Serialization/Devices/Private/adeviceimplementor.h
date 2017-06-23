#ifndef ADEVICEIMPLEMENTOR_H
#define ADEVICEIMPLEMENTOR_H

#include <QObject>
#include <QDir>
#include <QUrl>

class RBSFile;
class ADeviceImplementor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url
               READ getUrl)

    Q_PROPERTY(QString absolutePath
               READ getAbsolutePath
               USER true)

    Q_PROPERTY(QDir directory
               READ getDirectory)

    Q_PROPERTY(QString absoluteDir
               READ getAbsoluteDir)

    Q_PROPERTY(QFileInfo fileInfo
               READ getFileInfo)
    Q_PROPERTY(QString fileName
               READ getFileName)

public:

    virtual bool open(QIODevice::OpenMode mode) = 0;
    virtual qint64 read(char *_data, qint64 _maxlen) = 0;
    virtual qint64 write(const char *_data, qint64 _len) = 0;
    virtual void close() = 0;

    virtual bool save() = 0;
    virtual bool load() = 0;

    QByteArray readAll();
    bool write(const QByteArray& _data);

    ADeviceImplementor(const QString& _absolute_path);
    ~ADeviceImplementor();

    virtual bool needAskForSaving() const;
    virtual bool isWriteOnlyDevice() const;

public slots:
    // url
    QUrl getUrl() const ;
    // fileName
    QString getFileName() const ;
    // directory
    QDir getDirectory() const ;
    QString getAbsoluteDir() const ;
    // fileInfo
    QFileInfo getFileInfo() const ;
    // absolutePath
    QString getAbsolutePath() const ;

    bool checkFile(QIODevice::OpenMode _mode) const;
protected:
    QFile& _get_real_file() ;
    bool _check_or_open(QIODevice::OpenMode _mode);
signals:
    void rbsOpenFile();
    void rbsWriteToFile();
    void rbsCloseFile();
    void needActualData();
private:
    RBSFile* file_;
};

#endif // ADEVICEIMPLEMENTOR_H

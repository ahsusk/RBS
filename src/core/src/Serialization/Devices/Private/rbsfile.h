#ifndef RBSFILE_H
#define RBSFILE_H

#include <QFile>
class RBSFile final : public QFile
{
    Q_OBJECT
public:
    RBSFile(const QString& _abspath);

    bool open(OpenMode flags);
    void close();

protected:
    qint64 writeData(const char *data, qint64 len) override;
    qint64 readData(char *data, qint64 maxlen) override;
signals:
    void rbsOpenFile(OpenMode _flags);
    void rbsWriteToFile();
    void rbsCloseFile();
};

#endif // RBSFILE_H

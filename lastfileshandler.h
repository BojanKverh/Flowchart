#pragma once

#include <QSettings>
#include <QObject>

/**
 * @brief The LastFilesHandler class This class keeps track about latest used files
 */
class LastFilesHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief LastFilesHandler Constructor
     * @param count Number of files, that will be kept in cache
     */
    LastFilesHandler(int count = 10);
    /**
     * @brief files Returns the last used files list
     * @return last used files list
     */
    QStringList files() const { return m_list; }
    /**
     * @brief recordFile Notifies the system that a new file has been used
     * @param file New file name
     */
    void recordFile(const QString& file);

signals:
    /**
     * @brief filesChanged Emitted when the last files changed
     */
    void filesChanged();

private:
    void read();
    void write();
    void trim();

private:
    int m_count;
    QSettings m_set;
    QStringList m_list;

    const QString m_cqsFiles = "lastFiles";
};

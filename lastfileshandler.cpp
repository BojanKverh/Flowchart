#include "lastfileshandler.h"

LastFilesHandler::LastFilesHandler(int count)
  : QObject()
  , m_count(count)
  , m_set(QSettings::Format::IniFormat, QSettings::Scope::UserScope, "RLNN SKRR", "Flowchart")
{
  read();
}

void LastFilesHandler::recordFile(const QString& file)
{
  auto old = m_list;
  auto i   = m_list.indexOf(file);
  if (i >= 0)
    m_list.remove(i);
  m_list.prepend(file);
  write();

  if (m_list != old)
    emit filesChanged();
}

void LastFilesHandler::read()
{
  m_list = m_set.value(m_cqsFiles).toStringList();
  trim();
}

void LastFilesHandler::write()
{
  trim();
  m_set.setValue(m_cqsFiles, m_list);
}

void LastFilesHandler::trim()
{
  while (m_list.count() > m_count)
    m_list.removeLast();
}

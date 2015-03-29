#include "ini.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMapIterator>

/**
 * @brief INI::INI  Constructor for class INI. Parses provided INI file.
 * @param fileName  Path (absolute or relative) to the INI file.
 */
INI::INI(const QString fileName)
    : m_fileName(fileName)
{
    m_lastErrCode = 0;
    if(fileName.isNull() || fileName.isEmpty()) {
        m_lastErrCode = 3;
        return;
    }

    QFile iniFile;
    iniFile.setFileName(m_fileName);
    if(!(iniFile.open(QIODevice::ReadOnly))) {
        m_lastErrCode = 1;
        return;
    }

    QString sectionName;    // Track the current section.
    QTextStream in(&iniFile);
    while(!(in.atEnd())) {
        QString currentLine = in.readLine().trimmed();  // Don't care about leading or trailing whitespace.

        // Get rid of comments (if any) on the current line.
        QString fixedLine;
        for(int pos = 0; pos < currentLine.length(); ++pos) {
            if(currentLine[pos] == ';' || currentLine[pos] == '#')
                break;

            fixedLine += currentLine[pos];
        }
        QString key = fixedLine.section('=', 0, 0).simplified().replace(" ", "");
        QString value = fixedLine.section('=', 1).trimmed();
        QStringList pair;
        if(!(key.isEmpty() || value.isEmpty())) {
            pair.append(key);
            pair.append(value);
        }

        // Check if we have a new section
        fixedLine = fixedLine.simplified().replace(" ", "");
        if(fixedLine[0] == '[') {
            QString tempSectionName;
            bool sectionBreak = false;
            // Copy the section name between the square brackets.
            for(int pos = 1; pos < fixedLine.length(); ++pos) {
                if(fixedLine[pos] == ']') {
                    sectionBreak = true;
                    break;
                }

                tempSectionName += fixedLine[pos];
            }

            // Check if we've actually copied something.
            if(sectionBreak && !(tempSectionName.isNull() || tempSectionName.isEmpty())) {
                sectionName.clear();
                sectionName = tempSectionName;

                QMap<QString, QString>* tempMap = new QMap<QString, QString>();
                m_iniData.insert(sectionName, tempMap);
            }

            continue;
        }

        if(pair.length() != 2)  // Check if it's a key-value pair. If not, we don't care about it.
            continue;

        if(sectionName.isNull() || sectionName.isEmpty()) {
            m_lastErrCode = 2;
            return;
        }

        // Remove all whitespace from the key.
        pair[0] = pair[0].simplified();
        pair[0].replace(" ", "");

        // Append the key-value pair to the map.
        QMap<QString, QString>* map = m_iniData.value(sectionName);
        map->insert(pair[0], pair[1]);
    }
}

/**
 * @brief INI::~INI Destructor for INI.
 */
INI::~INI()
{
    // Iterate over all values on the map, as these were allocated to the heap earlier.
    QMapIterator<QString, QMap<QString, QString>*> iter(m_iniData);
    while(iter.hasNext()) {
        iter.next();

        QMap<QString, QString>* sectionMap = iter.value();
        delete sectionMap;
    }
}

/**
 * @brief INI::getLastError Returns the last error code.
 * @return Returns 0 if no error has occurred, non-zero otherwise.
 */
int INI::getLastError() const {
    return m_lastErrCode;
}

/**
 * @brief INI::readString   Reads a string from an INI file.
 * @param section   Section name to search under
 * @param key       Key name to look under
 * @param defaultValue If no value was found, or an invalid value was found, then return this instead.
 * @return  The value from the file if successful, defaultValue otherwise.
 */
QString INI::readString(const QString &section, const QString &key, const QString &defaultValue)
{
    QMap<QString, QMap<QString, QString>*>::const_iterator i = m_iniData.find(section);
    if(i == m_iniData.end())
        return defaultValue;

    QMap<QString, QString>::const_iterator j = i.value()->find(key);
    if(j == i.value()->end())
        return defaultValue;

    return j.value();
}

/**
 * @brief INI::readInteger   Reads an integer from an INI file.
 * @param section   Section name to search under
 * @param key       Key name to look under
 * @param defaultValue If no value was found, or an invalid value was found, then return this instead.
 * @return  The value from the file if successful, defaultValue otherwise.
 */
int INI::readInteger(const QString &section, const QString &key, const int &defaultValue)
{
    bool convSucceeded = false;
    int result = this->readString(section, key, QString::number(defaultValue)).toInt(&convSucceeded, 0);

    if(convSucceeded) {
        return result;
    } else {
        return defaultValue;
    }
}

/**
 * @brief INI::readBool   Reads a boolean from an INI file.
 * @param section   Section name to search under
 * @param key       Key name to look under
 * @param defaultValue If no value was found, or an invalid value was found, then return this instead.
 * @return  The value from the file if successful, defaultValue otherwise.
 */
bool INI::readBool(const QString &section, const QString &key, const bool &defaultValue)
{
    bool convSucceeded = false;
    QString result = this->readString(section, key, QString::number(defaultValue));
    if(result.compare("true", Qt::CaseInsensitive) == 0)
        return true;
    if(result.compare("false", Qt::CaseInsensitive) == 0)
        return false;

    int boolInt = result.toInt(&convSucceeded);
    if(convSucceeded) {
        return (bool)boolInt;
    } else {
        return defaultValue;
    }
}

/**
 * @brief INI::readReal   Reads a floating point value from an INI file.
 * @param section   Section name to search under
 * @param key       Key name to look under
 * @param defaultValue If no value was found, or an invalid value was found, then return this instead.
 * @return  The value from the file if successful, defaultValue otherwise.
 */
double INI::readReal(const QString &section, const QString &key, const double &defaultValue)
{
    bool convSucceeded = false;
    double result = this->readString(section, key, QString::number(defaultValue)).toDouble(&convSucceeded);

    if(convSucceeded) {
        return result;
    } else {
        return defaultValue;
    }
}

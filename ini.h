#ifndef INI_H
#define INI_H

#include <QString>
#include <QMap>

class INI
{
public:
    INI(const QString fileName);
    ~INI();

    QString readString(const QString &section, const QString &key, const QString &defaultValue);
    int readInteger(const QString &section, const QString &key, const int &defaultValue);
    bool readBool(const QString &section, const QString &key, const bool &defaultValue);
    double readReal(const QString &section, const QString &key, const double &defaultValue);

    int getLastError() const;
private:
    QString m_fileName; // File path of INI file.
    QMap<QString, QMap<QString, QString>* > m_iniData;  // Stores everything we have loaded from the INI file.
    int m_lastErrCode;
};

#endif // INI_H

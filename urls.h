#ifndef URLS_H
#define URLS_H
#include <QString>

// This file is respnsible for changing the urls whenever needed


class urls
{
public:
    urls();
    static const QString checkemailUrl;
    static const QString getOtpUrl;
    static const QString verifyOTPurl;
    static const QString createUserUrl;
    static const QString addSystemUrl;
    static const QString saveQuery;
    static const QString getPastQuery;
    static const QString deletePastQuery;
    static const QString troubleShoot;
    static const QString askQuestion;
    static const QString adderrorreport;
    static const QString addapplicationinfo;
};

#endif // URLS_H

#include "slexer.h"
#include <QDebug>

SLexer::SLexer(const SLexer& rhs)// : QObject()
{
    operator =(rhs);
}

SLexer& SLexer::operator=( const SLexer& rhs)
{
    if (this == &rhs) return *this;
    user_id = rhs.user_id;
    login = rhs.login;
    lastname  = rhs.lastname;
    name = rhs.name;
    address = rhs.address;
    phone = rhs.phone;
    email = rhs.email;
    type = rhs.type;
//    qDebug()<<login;
    return *this;
}

SLexer::SLexer(long _id)
{
//    QString query = QString("SELECT " \
//                            "id, login, password, lastname, name, address, phone, email, type, reg_date " \
//                            "FROM users WHERE id='%1'").arg(_id);
}

SLexer::SLexer(QString _login)
{
    Q_UNUSED(_login);
    qDebug(":TODO");
}

SLexer::~SLexer() {
    qDebug("SLexer destructor");
}

void SLexer::setUserInfo(QList<QStringList> info)
{
    user_id = info[0].at(0).toLong();
    login = info[0].at(1);
//    password = info[0].at(2);
    lastname  = info[0].at(3);
    name = info[0].at(4);
    address = info[0].at(5);
    phone = info[0].at(6);
    email = info[0].at(7);

    QString str_type = info[0].at(8);
    if (str_type == "CLIENT"  ) type = CLIENT;
    if (str_type == "AUTHOR"  ) type = AUTHOR;
    if (str_type == "SUPPLIER") type = SUPPLIER;
    if (str_type == "OPERATOR") type = OPERATOR;
    if (str_type == "CEO"     ) type = CEO;
    if (str_type == "ADMIN"   ) type = ADMIN;

//    regdate = QDateTime::fromString(info[0].at(9), "yyyy-MM-ddTHH:mm:ss");

    qDebug()<<"\nuser_id : "<<user_id<<"\nlogin : "<<login/*<<"\npassword : "<<password*/<<"\nlastname : "<<lastname<<"\nname : "<<name;
    qDebug()<<"address : "<<address<<"\nphone : "<<phone<<"\nemail : "<<email<<"\ntype : "<<str_type/*<<"\nregdate : "<<regdate*/;
    qDebug("type #%d\n",type);
}

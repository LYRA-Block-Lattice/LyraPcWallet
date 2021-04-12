#ifndef RPCAPI_H
#define RPCAPI_H

#include <QList>
#include "rpc.h"

typedef rpc * connection_t;

class rpcapi
{
public:
    rpcapi(rpc *rpcConn);
    ~rpcapi();
    static connection_t getConnection();
    static QList<QList<QMap<QString, QString>>> history(qint64 timeStart = 0, qint64 timeEnd = std::numeric_limits<qint64>::max(), int count = 100);
    static QMap<QString, double> balance(bool *unreceived);
};

#endif // RPCAPI_H

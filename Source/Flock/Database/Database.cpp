//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"

#include "../Core/Profiler.h"
#include "../Database/Database.h"

namespace FlockSDK
{

Database::Database(Context* context_) :
    Object(context_),
    poolSize_(M_MAX_UNSIGNED)
{
}

DBAPI Database::GetAPI()
{
    return DBAPI_SQLITE;
}

DbConnection* Database::Connect(const String &connectionString)
{
    FLOCKSDK_PROFILE(DatabaseConnect);

    SharedPtr<DbConnection> connection;
    if (IsPooling())
    {
        Vector<SharedPtr<DbConnection>>& connectionsPool = connectionsPool_[connectionString];
        while (!connectionsPool.Empty())
        {
            connection = connectionsPool.Back();
            connectionsPool.Pop();
            if (connection->IsConnected())
                break;
            connection = 0;
        }
    }
    if (!connection)
        connection = new DbConnection(context_, connectionString);
    if (connection->IsConnected())
    {
        connections_.Push(connection);
        return connection;
    }
    else
        return 0;
}

void Database::Disconnect(DbConnection* connection)
{
    if (!connection)
        return;

    FLOCKSDK_PROFILE(DatabaseDisconnect);

    SharedPtr<DbConnection> dbConnection(connection);
    connections_.Remove(dbConnection);

    // Must finalize the connection before closing the connection or returning it to the pool
    connection->Finalize();

    if (IsPooling())
    {
        Vector<SharedPtr<DbConnection>>& connectionsPool = connectionsPool_[connection->GetConnectionString()];
        if (connectionsPool.Size() < poolSize_)
            connectionsPool.Push(dbConnection);
    }
}

}

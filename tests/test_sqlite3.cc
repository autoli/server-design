#include "autoli/db/sqlite3.h"
#include "autoli/log.h"
#include "autoli/util.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void test_batch(autoli::SQLite3::ptr db) {
    auto ts = autoli::GetCurrentMS();
    int n = 1000000;
    autoli::SQLite3Transaction trans(db);
    //db->execute("PRAGMA synchronous = OFF;");
    trans.begin();
    autoli::SQLite3Stmt::ptr stmt = autoli::SQLite3Stmt::Create(db,
                "insert into user(name, age) values(?, ?)");
    for(int i = 0; i < n; ++i) {
        stmt->reset();
        stmt->bind(1, "batch_" + std::to_string(i));
        stmt->bind(2, i);
        stmt->step();
    }
    trans.commit();
    auto ts2 = autoli::GetCurrentMS();

    AUTOLI_LOG_INFO(g_logger) << "used: " << (ts2 - ts) / 1000.0 << "s batch insert n=" << n;
}

int main(int argc, char** argv) {
    const std::string dbname = "test.db";
    auto db = autoli::SQLite3::Create(dbname, autoli::SQLite3::READWRITE);
    if(!db) {
        AUTOLI_LOG_INFO(g_logger) << "dbname=" << dbname << " not exists";
        db = autoli::SQLite3::Create(dbname
                , autoli::SQLite3::READWRITE | autoli::SQLite3::CREATE);
        if(!db) {
            AUTOLI_LOG_INFO(g_logger) << "dbname=" << dbname << " create error";
            return 0;
        }

#define XX(...) #__VA_ARGS__
        int rt = db->execute(
XX(create table user (
        id integer primary key autoincrement,
        name varchar(50) not null default "",
        age int not null default 0,
        create_time datetime
        )));
#undef XX

        if(rt != SQLITE_OK) {
            AUTOLI_LOG_ERROR(g_logger) << "create table error "
                << db->getErrno() << " - " << db->getErrStr();
            return 0;
        }
    }

    for(int i = 0; i < 10; ++i) {
        if(db->execute("insert into user(name, age) values(\"name_%d\",%d)", i, i)
                != SQLITE_OK) {
            AUTOLI_LOG_ERROR(g_logger) << "insert into error " << i << " "
                << db->getErrno() << " - " << db->getErrStr();
        }
    }

    autoli::SQLite3Stmt::ptr stmt = autoli::SQLite3Stmt::Create(db,
                "insert into user(name, age, create_time) values(?, ?, ?)");
    if(!stmt) {
        AUTOLI_LOG_ERROR(g_logger) << "create statement error "
            << db->getErrno() << " - " << db->getErrStr();
        return 0;
    }

    int64_t now = time(0);
    for(int i = 0; i < 10; ++i) {
        stmt->bind(1, "stmt_" + std::to_string(i));
        stmt->bind(2, i);
        stmt->bind(3, now + rand() % 100);
        //stmt->bind(3, autoli::Time2Str(now + rand() % 100));
        //stmt->bind(3, "stmt_" + std::to_string(i + 1));
        //stmt->bind(4, i + 1);

        if(stmt->execute() != SQLITE_OK) {
            AUTOLI_LOG_ERROR(g_logger) << "execute statment error " << i << " "
                << db->getErrno() << " - " << db->getErrStr();
        }
        stmt->reset();
    }

    autoli::SQLite3Stmt::ptr query = autoli::SQLite3Stmt::Create(db,
            "select * from user");
    if(!query) {
        AUTOLI_LOG_ERROR(g_logger) << "create statement error "
            << db->getErrno() << " - " << db->getErrStr();
        return 0;
    }
    auto ds = query->query();
    if(!ds) {
        AUTOLI_LOG_ERROR(g_logger) << "query error "
            << db->getErrno() << " - " << db->getErrStr();
        return 0;
    }

    while(ds->next()) {
        //AUTOLI_LOG_INFO(g_logger) << "query ";
    };

    //const char v[] = "hello ' world";
    const std::string v = "hello ' world";
    db->execStmt("insert into user(name) values (?)", v);

    auto dd = (db->queryStmt("select * from user"));
    while(dd->next()) {
        AUTOLI_LOG_INFO(g_logger) << "ds.data_count=" << dd->getDataCount()
            << " ds.column_count=" << dd->getColumnCount()
            << " 0=" << dd->getInt32(0) << " 1=" << dd->getString(1)
            << " 2=" << dd->getString(2)
            << " 3=" << dd->getString(3);
    }

    test_batch(db);
    return 0;
}

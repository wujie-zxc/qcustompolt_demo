#include "wdatabase.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

//===============inter===================
/**
 * @brief The DB_Inter class 创建数据库db & 执行sql
 */
class DB_Inter
{
    friend class WDataBase;
public:
    DB_Inter(const QString &connect_name, const QString &db_name, const QString &db_path){
        if (QSqlDatabase::contains(connect_name)){
            sql_db_ = QSqlDatabase::database(connect_name);
        }else{
            sql_db_ = QSqlDatabase::addDatabase("QSQLITE", connect_name);
        }
        db_name_ = db_name;
        db_path_ = db_path;
        sql_db_.setDatabaseName(QString("%1%2").arg(db_path_).arg(db_name_));
    }
    ~DB_Inter(){
        closeDb();
    }
    bool openDb(){
        closeDb();
        return sql_db_.open();
    }
    void closeDb(){
        if (sql_db_.isValid() && sql_db_.isOpen()){
            sql_db_.close();
        }
    }
    QStringList tables(){
        return sql_db_.tables();
    }
    bool execSql(const QString &sql) {
        bool result = false;
        if (!sql_db_.isOpen()){
            qWarning() << QString("open % failed: %2").arg(db_name_).arg(sql_db_.lastError().text());
            return result;
        }
        QString msg = QString("[%1]--").arg(db_name_);
        QSqlQuery query(sql_db_);

        sql_db_.transaction();
        QStringList sql_list = sql.split(";", QString::SkipEmptyParts);
        for (const auto &sql : sql_list){
            if (!(result = query.exec(sql))){
                break;
            }
        }
        if (result) {
             msg += QString("sql exec ok: %1").arg(sql);
            sql_db_.commit();
        }else{
             msg += QString("sql exec failed: %1 -- %2").arg(sql).arg(query.lastError().text());
            sql_db_.rollback();
        }
        qInfo() << msg;
        return result;
    }
    bool execSql(const QString &sql, QSqlQuery &query) {
        bool ret = false;
        if (!sql_db_.isOpen()) {
            qWarning() << QString("open %1 failed: %2").arg(db_name_).arg(sql_db_.lastError().text());
            return ret;
        }
        query = sql_db_.exec(sql);
        if (!(ret = (QSqlError::NoError == query.lastError().type()))){
            qWarning() <<QString("[%1 exec with query]---%2---failed---%3").arg(db_name_).arg(sql).arg(query.lastError().text());
        } else {
            qInfo() <<QString("[%1 exec with query]---%2---ok").arg(db_name_).arg(sql);
        }

        return ret;
    }

private:
    QSqlDatabase sql_db_;
    QString db_path_;
    QString db_name_;
};
//=========================================



static const QString st_create_weld_daily_table =
    "DROP TABLE IF EXISTS %1;"
    "CREATE TABLE %1 ( "
    "date TEXT PRIMARY KEY, "
    "welding_time REAL, "
    "teaching_time REAL, "
    "flat_weld_length REAL, "
    "verical_weld_length REAL, "
    "weld_count INTEGER, "
    "daily_movement_duration REAL, "
    "daily_wait_duration REAL, "
    "daily_pause_duration REAL, "
    "daily_idle_duration REAL "
    ")";

static const QString st_create_weld_today_table =
        "DROP TABLE IF EXISTS %1;"
        "CREATE TABLE %1 ( "
        "time TEXT PRIMARY KEY, "
        "state INTEGER"
        ")";


//===============interface===================
WDataBase &WDataBase::instance()
{
    static WDataBase db;
    return db;
}

WDataBase::WDataBase()
{
    //inter_ = std::unique_ptr<DB_Inter>(new DB_Inter("weld_connection", DB_NAME, DB_PATH));
    db_name_ = "weld_history.db";
    db_path_ = "D:/";
    inter_ = std::make_shared<DB_Inter>("weld_connection", db_name_, db_path_);
    inter_->openDb();
    isHistoryTableExist();
    isTodayTableExist();
}

bool WDataBase::isHistoryTableExist()
{
    bool ret = false;
    QString cmd = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1' ").arg(weld_daily_table_);
    QSqlQuery query;
    if (inter_->execSql(cmd, query)){
        query.next();
        if (query.value(0).toInt() == 0) {
            inter_->execSql(st_create_weld_daily_table.arg(weld_daily_table_));
        }
        ret = true;
    }
    return ret;
}

bool WDataBase::isTodayTableExist()
{
    bool ret = true;
    QString today = QDate::currentDate().toString("state_yyyy_MM_dd_table");
    weld_today_table_ = today;
    QString cmd = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1' ").arg(weld_today_table_);
    QSqlQuery query;
    if (inter_->execSql(cmd, query)){
        query.next();
        if (query.value(0).toInt() == 0) {
            inter_->execSql(st_create_weld_today_table.arg(weld_today_table_));
        }
        ret = true;
    }

    return ret;
}

bool WDataBase::addDailyWeldData(const DailyWeldData &data)
{
    QString check_cmd = QString("SELECT COUNT(*) FROM %1 WHERE date='%2'").arg(weld_daily_table_).arg(data.date);
    QSqlQuery query;
    if (!inter_->execSql(check_cmd, query) || !query.next()) {
        qWarning() << "failed to check existing data";
        return false;
    }
    bool exists = query.value(0).toInt() > 0;

    QString cmd;
    if (exists) {
        cmd = QString("UPDATE %1 SET "
                      "welding_time='%2', teaching_time='%3', flat_weld_length='%4', "
                      "verical_weld_length='%5', weld_count='%6', daily_movement_duration='%7', "
                      "daily_wait_duration='%8', daily_pause_duration='%9', daily_idle_duration='%10' "
                      "WHERE date='%11'")
                .arg(weld_daily_table_)
                .arg(data.welding_time)
                .arg(data.teaching_time)
                .arg(data.flat_weld_length)
                .arg(data.verical_weld_length)
                .arg(data.weld_count)
                .arg(data.daily_movement_duration)
                .arg(data.daily_wait_duration)
                .arg(data.daily_pause_duration)
                .arg(data.daily_idle_duration)
                .arg(data.date);
    }else{
        cmd =  QString("INSERT INTO %1 "
                       "(date, welding_time, teaching_time, flat_weld_length, verical_weld_length, "
                       "weld_count, daily_movement_duration, daily_wait_duration, daily_pause_duration, daily_idle_duration) "
                       "VALUES('%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11')")
                .arg(weld_daily_table_)
                .arg(data.date)
                .arg(data.welding_time)
                .arg(data.teaching_time)
                .arg(data.flat_weld_length)
                .arg(data.verical_weld_length)
                .arg(data.weld_count)
                .arg(data.daily_movement_duration)
                .arg(data.daily_wait_duration)
                .arg(data.daily_pause_duration)
                .arg(data.daily_idle_duration);
    }
    if (!inter_->execSql(cmd)) {
        qWarning() << "failed to " << (exists ? "update" : "insert") << " daily weld data";
        return false;
    }

    //最多保存30天数据
    QString count_cmd = QString("SELECT COUNT(*) FROM %1").arg(weld_daily_table_);
    if (!inter_->execSql(count_cmd, query) || !query.next()) {
        qWarning() << "failed to count records";
        return false;
    }
    int count = query.value(0).toInt();
    if (count > 30) {
        QString delete_cmd = QString("DELETE FROM %1 WHERE date = (SELECT date FROM %1 ORDER BY date ASC LIMIT 1)")
                .arg(weld_daily_table_);
        if (!inter_->execSql(delete_cmd)) {
            qWarning() << "failed to delete oldest record";
            return false;
        }
    }

    return true;
}

bool WDataBase::getWeldDataInDateRange(const QDate &start_date, const QDate &end_date, QMap<QDate, DailyWeldData> &date_data_map)
{
    date_data_map.clear();
    if (!start_date.isValid() || !end_date.isValid() || start_date > end_date){
        qWarning() << "Invalid date range";
        return false;
    }
    for (QDate date = start_date; date <= end_date; date = date.addDays(1)) {
        DailyWeldData obj;
        date_data_map[date] =  obj;
        date_data_map[date].date = date.toString("yyyy_MM_dd");
    }
    QString cmd = QString("SELECT * FROM %1 WHERE date BETWEEN '%2' AND '%3' ORDER BY date")
            .arg(weld_daily_table_)
            .arg(start_date.toString("yyyy_MM_dd"))
            .arg(end_date.toString("yyyy_MM_dd"));
    QSqlQuery query;
    if (!inter_->execSql(cmd, query)) {
        qWarning() << "failed to exec query for date range";
        return false;
    }

    while (query.next()) {
        QDate date = QDate::fromString(query.value("date").toString(), "yyyy_MM_dd");
        if (date_data_map.contains(date)) {
            DailyWeldData &data = date_data_map[date];
            data.welding_time = query.value("welding_time").toDouble();
            data.teaching_time = query.value("teaching_time").toDouble();
            data.flat_weld_length = query.value("flat_weld_length").toDouble();
            data.verical_weld_length = query.value("verical_weld_length").toDouble();
            data.weld_count = query.value("weld_count").toInt();
            data.daily_movement_duration = query.value("daily_movement_duration").toDouble();
            data.daily_wait_duration = query.value("daily_wait_duration").toDouble();
            data.daily_pause_duration = query.value("daily_pause_duration").toDouble();
            data.daily_idle_duration = query.value("daily_idle_duration").toDouble();
        }
    }
    return true;
}

bool WDataBase::addTodayProjectState()
{
    QStringList invalid_tables;
    qDebug() << inter_->tables();
    for (const auto &name : inter_->tables()){
        if (name.startsWith("state_") && name != weld_today_table_){
            invalid_tables.append(name);
        }
    }

    if (invalid_tables.count() == 0) {
        return true;
    }
    QString delete_cmds;
    delete_cmds = (QString("DROP TABLE IF EXISTS %1").arg(invalid_tables[0]));
    inter_->execSql( delete_cmds);
}

bool WDataBase::getTodayProjectStates(QMap<QTime, MinuteProjectStatus> &map)
{

}





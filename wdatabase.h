#ifndef WDATABASE_H
#define WDATABASE_H

#include <QDebug>
#include <QString>
#include <QTime>
#include <QDate>
#include <memory>
#include <mutex>

struct DailyWeldData
{
    //日期格式2024_09_23
    QString date;
    //焊接时长
    double welding_time{0};
    //示教时长
    double teaching_time{0};
    //平焊长度
    double flat_weld_length{0};
    //立焊长度
    double verical_weld_length{0};
    //焊缝个数
    int weld_count{0};
    //当天运动时长
    double daily_movement_duration{0};
    //当天等待时长
    double daily_wait_duration{0};
    //当天暂停时长
    double daily_pause_duration{0};
    // 当天闲置时长
    double daily_idle_duration{0};
};

enum ProjectStatus
{
    RUN,
    WAIT,
    PAUSE,
    IDLE
};

struct MinuteProjectStatus
{
    QTime time;
    int state;
};





class DB_Inter;
class WDataBase
{
public:
    static WDataBase& instance();

public:
    //每天历史数据
    bool addDailyWeldData(const DailyWeldData &data);
    bool getWeldDataInDateRange(const QDate &start_date, const QDate &end_date, QMap<QDate, DailyWeldData> &map);
    //当天工程状态
    bool addTodayProjectState();
    bool getTodayProjectStates(QMap<QTime, MinuteProjectStatus> &map);

private:
    WDataBase();
    bool isHistoryTableExist();
    bool isTodayTableExist();

private:
    std::mutex mtx_;
    std::shared_ptr<DB_Inter> inter_{nullptr};

    QString db_path_;
    QString db_name_;
    QString weld_daily_table_{"weld_daily_table"};
    QString weld_today_table_{"state_xx_table"};
};

#endif // WDATABASE_H

#ifndef RCSCLIENT_H
#define RCSCLIENT_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class RcsClient; }
QT_END_NAMESPACE

class RcsClient : public QMainWindow
{
    Q_OBJECT

public:
    RcsClient(QWidget *parent = nullptr);
    ~RcsClient();

private slots:
    void on_btn_upload_clicked();
    void on_btn_refresh_clicked();

    void get_results();
    void post_results(QByteArray results);
    void get_winner(QString ref);

    void on_get_results(QNetworkReply *reply);
    void on_post_results(QNetworkReply *reply);
    void on_get_winner(QNetworkReply *reply);

    void on_lv_results_clicked(const QModelIndex &index);

private:
    Ui::RcsClient *ui;
    QStringListModel *model;
    QNetworkAccessManager
        _get_results,
        _post_results,
        _get_winner;
};
#endif // RCSCLIENT_H

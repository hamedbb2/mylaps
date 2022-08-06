#include "rcs_client.h"
#include "ui_rcs_client.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkReply>
#include <QFileDialog>

RcsClient::RcsClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RcsClient)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    ui->lv_results->setModel(model);
    connect(&_get_results, &QNetworkAccessManager::finished, this, &RcsClient::on_get_results);
    connect(&_post_results, &QNetworkAccessManager::finished, this, &RcsClient::on_post_results);
    connect(&_get_winner, &QNetworkAccessManager::finished, this, &RcsClient::on_get_winner);
}

RcsClient::~RcsClient()
{
    delete ui;
    delete model;
}


void RcsClient::get_results() {
    QNetworkRequest request(QUrl("http://localhost:9080/results"));
    request.setRawHeader("Content-Type", "application/fhir+json");
    _get_results.get(request);
}

void RcsClient::post_results(QByteArray results) {
    QNetworkRequest request(QUrl("http://localhost:9080/results"));
    request.setRawHeader("Content-Type", "application/fhir+json");
    _post_results.post(request, results);
}

void RcsClient::get_winner(QString ref) {
    QNetworkRequest request(QUrl("http://localhost:9080/winner/" + ref));
    request.setRawHeader("Content-Type", "application/fhir+json");
    _get_winner.get(request);
}

void RcsClient::on_btn_upload_clicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this,
             tr("Choose file"), "/home", tr("CSV Files (*.csv)"));
    QFile file(file1Name);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QJsonObject results{};
    file.readLine(); //skip header
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        auto kv = line.split(',');
        auto key = QString(kv.first());
        auto value = QString(kv.back());
        if (results.find(key) == results.end()) {
            results.insert(key, QJsonArray{});
        }
        results[key] = results[key].toArray() << value; //Terrible solution but it's late at night and I'm new with Qt json, sorry.
    }
    QJsonDocument doc{results};
    post_results(doc.toJson());
}

void RcsClient::on_btn_refresh_clicked()
{
    get_results();
}

void RcsClient::on_lv_results_clicked(const QModelIndex &index)
{
    get_winner(model->data(index).toString());
}


void RcsClient::on_get_results(QNetworkReply *reply)
{
    QStringList List;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    for (auto&& v: doc.object().value("available").toArray()) {
        List << v.toString();
    }
    model->setStringList(List);
}

void RcsClient::on_post_results([[maybe_unused]] QNetworkReply *reply)
{
    get_results();
}

void RcsClient::on_get_winner(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    ui->lb_winner->setText(doc.toJson());
}




#include <QCoreApplication>
//#include <iostream>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkAccessManagerWithPatch.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QTextStream>

QTextStream cout(stdout);
QTextStream cin(stdin);

void resultRequest(QNetworkReply *reply) {
    QByteArray data = reply->readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(data, &error);

    if(!document.isEmpty() || (!document.isNull()) || error.error == QJsonParseError::NoError) {
        const QJsonObject obj = document.object();
        QJsonObject::const_iterator it_obj = obj.constBegin();

        for ( ; it_obj != obj.constEnd(); ++it_obj){
            const QJsonValue it_val = it_obj.value();

            if(it_val.type() == QJsonValue::Object) {           // если объект { }
                qDebug() << it_obj.key() << ":";

                const QJsonObject obj_val = it_val.toObject();
                QJsonObject::const_iterator it2 = obj_val.begin();

                for ( ; it2 != obj_val.constEnd(); ++it2){
                    qDebug() << "\t" << it2.key() << ":" << it2.value().toString();
                    //ss << '"' << it2.key() << ": " << it2.value().toString();
                }
            } else if(it_val.type() == QJsonValue::String) {    // если строка String
                qDebug() << it_obj.key() << ":" << it_obj.value().toString();
            } // можно расширить далее для разных типов
              // Array, Bool, Double
        }

    } else {
        qDebug()<< "reply->readAll(), error:  " << reply->errorString();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkAccessManagerWithPatch *managerPatch = new QNetworkAccessManagerWithPatch();

    QString adrUrl = "http://httpbin.org/";
    QString str;

    cout << "\nEnter metod: "; cout.flush();
    cin >> str;
    //cout << str; cout.flush();

    if (str == "get") {
        QUrl url(adrUrl + str);
        QNetworkRequest request(url);
        manager->get(request);
        QObject::connect(manager, &QNetworkAccessManager::finished, resultRequest);
    } else if (str == "headers") {
        QUrl url(adrUrl + str);
        QNetworkRequest request(url);
        manager->get(request);
        QObject::connect(manager, &QNetworkAccessManager::finished, resultRequest);
    } else if (str == "post") {
        QByteArray token;
        token.append("Key");    // ключ
        token.append('=');      // знак обязательный, так как делит ключ и значение
        token.append("Value");  // значение
        QUrl url(adrUrl + str);
        QNetworkRequest request(url);
        manager->post(request, token);
        QObject::connect(manager, &QNetworkAccessManager::finished, resultRequest);
    } else if (str == "put") {
        QUrl url(adrUrl + str);
        QNetworkRequest request(url);
        manager->put(request, "30.07.2022");
        QObject::connect(manager, &QNetworkAccessManager::finished, resultRequest);
    } else if (str == "delete") {
        QUrl url(adrUrl);
        QNetworkRequest request(url);
        manager->deleteResource(request);
        QObject::connect(manager, &QNetworkAccessManager::finished, resultRequest);
    } else if (str == "patch") {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart textPart;

        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
        textPart.setBody("my text");

        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));

        /*QFile *file = new QFile("image.png");
        file->open(QIODevice::ReadOnly);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart);*/

        QByteArray fileFlow;
        QFile file("image.png");

        if(file.open(QIODevice::ReadOnly)){
            fileFlow = file.readAll();
            imagePart.setBody(fileFlow.toBase64());
        }

        multiPart->append(textPart);
        multiPart->append(imagePart);

        QUrl url(adrUrl + str);
        QNetworkRequest request(url);
        managerPatch->patch(request, multiPart);
        QObject::connect(managerPatch, &QNetworkAccessManagerWithPatch::finished, resultRequest);
    }

    return a.exec();
}

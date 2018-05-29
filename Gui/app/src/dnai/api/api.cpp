#include <QJsonArray>
#include <QHttpMultiPart>

#include "dnai/api/api.h"
#include "dnai/app.h"
#include "dnai/http/service.h"

namespace dnai {
const QString api::client_id = "sINQmt18nib3vVlI4B71NKaQjXGWMYxrNJWuuS6e";
const QString api::client_secret = "pMi9ScKMPv3IgHgCQmKHKX7yxJY5KMd2KXfWKRMa2jk1qyiSz7AJqllnvpFIfstnIDkausSlqgoWJabYIkXnPGiXgaKE9ikPeILVvoWlifaFSngX2QIA3sJFWH0EO9oH";
api::User api::user = {};
bool api::refreshing_token = false;
quint64 api::refreshing_delta = 3600;
const QString api::settings_key = "/current/user";
const Config api::http_config = {
    "https://api.preprod.dnai.io/",
      {},
      {
        [](Url *url) {
            auto token = getToken();
            if (!token.isEmpty()) {
                url->addHeader("Authorization", "Bearer " + token);
            }
        },
        [](Url *url) {
            Q_UNUSED(url)
            if (api::refreshing_token == false && QDateTime::currentDateTime().addSecs(api::refreshing_delta) >= api::user.expire_date) {
                api::refresh_token();
            }
        }
    }
  };

    Observable &api::signin(const QString &login, const QString &password)
    {
        return Service::url("signin")
                ->headers(
                    Headers{
                    //    {"Authorization", "Basic " + QString(client_id + ":" + client_secret).toUtf8().toBase64()},
                        {"Content-Type", "application/json"}
                    })
                ->post(QJsonObject{
                          // {"grant_type", "password"},
                           {"login", login},
                           {"password", password}
                       })
                .map([](Response response) -> Response {
            api::setUser({
                response.body["token"].toString(),
                response.body["refreshToken"].toString(),
                response.body["user_id"].toString(),
                QDateTime::currentDateTime().addSecs(6000000)
            });
            return response;
        });
    }

    Observable &api::refresh_token()
    {
        qDebug() << "REFRESHTOKEN: " << api::user.refresh_token;
        api::refreshing_token = true;
        return Service::url("refresh")
                ->headers(
                    Headers{
                       // {"Authorization", "Basic " + QString(client_id + ":" + client_secret).toUtf8().toBase64()},
                        {"Content-Type", "application/json"}
                    })
                ->post(QJsonObject{
                          // {"grant_type", "refresh_token"},
                           {"refreshToken", api::user.refresh_token}
                       })
                .map([](Response response) -> Response {
            api::setUser({
                             response.body["token"].toString(),
                             response.body["refreshToken"].toString(),
                             response.body["user_id"].toString(),
                             QDateTime::currentDateTime().addSecs(6000000)
            });
            api::refreshing_token = false;
            return response;
        });
    }

    Observable &api::get_current_user()
    {
        //TODO when API work `/users/me` and remove John Doe
        return Service::url("users", api::user.id.toLatin1().data())->get().map([](Response response) -> Response {

            return response;
        });
    }

    Observable &api::get_files()
    {
        return Service::url("files")->get();
    }

    Observable &api::get_download_object(QString const &platform, QString const &slug)
    {
        return Service::url("download/softwares", platform.toLatin1().data(), slug.toLatin1().data())->get();
    }

    Observable &api::get_file(QString const &id)
    {
        return Service::url("files", id.toLatin1().data())->get();
    }

    Observable &api::get_raw_file(QString const &id)
    {
        return Service::url("uploaded_files", id.toLatin1().data())->get();
    }

    Observable &api::post_file(QString const &title, QFile *file)
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        multiPart->append(http::Service::makeHttpPart("file_type_id", "1"));
        multiPart->append(http::Service::makeHttpPart("title", title));
        multiPart->append(http::Service::makeHttpPart("file", file));
        file->setParent(multiPart);

        return Service::url("files")->post(multiPart);
    }

    void api::logout()
    {
        qDebug() << "LOGOUT ?";
        api::setUser({});
    }

    QString const &api::getToken()
    {
        return user.token;
    }

    QString const &api::getId() {
        return user.id;
    }

    void api::setUser(const api::User &user)
    {
        api::user = user;
        qDebug() << "user: "<<  api::user.id;
        App::currentInstance()->settings().setAPIValue(api::settings_key, QVariant::fromValue(api::user));
        qDebug() << "SETTINGS user";
    }
}

QDataStream &operator<<(QDataStream &out, const dnai::api::User &v)
{
    out << v.token << v.refresh_token << v.id << v.expire_date;
    return out;
}

QDataStream &operator>>(QDataStream &in, dnai::api::User &v)
{
    in >> v.token;
    in >> v.refresh_token;
    in >> v.id;
    in >> v.expire_date;
    return in;
}
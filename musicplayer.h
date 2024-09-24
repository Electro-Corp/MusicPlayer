#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "qgraphicsscene.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <QFile>
#include <QThread>
#include <QGraphicsVideoItem>
#include <QVideoSink>
#include <QVideoFrame>

#include <filesystem>

using recurDir = std::filesystem::recursive_directory_iterator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicPlayer;
}
QT_END_NAMESPACE

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();


    void loadMusic();

public slots:
    void songSelect(QListWidgetItem* cur, QListWidgetItem* prev);

    void songPosChange(qint64 pos);

    void songVolChange();

    void vidChange(const QVideoFrame &frame);

    void timeBarChanged();

private:
    Ui::MusicPlayer *ui;

    std::string musicLibraryPath;

    QMediaPlayer* player;
    QAudioOutput* audOut;

    QGraphicsScene* scene;
    QGraphicsVideoItem* videoItem;
    QVideoSink* sink;
};
#endif // MUSICPLAYER_H

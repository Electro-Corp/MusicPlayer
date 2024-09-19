#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <iostream>

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);

    // Start finding music files
    #ifdef _WIN32
    musicLibraryPath = "C:\\Users\\segfault\\Music";
    #else
    musicLibraryPath = "/home/segfault/Music";
    #endif
    loadMusic();

    player = new QMediaPlayer;
    audOut = new QAudioOutput;
    player->setAudioOutput(audOut);

    QObject::connect(ui->songList, &QListWidget::currentItemChanged, this, &MusicPlayer::songSelect);

    QObject::connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayer::songPosChange);

    scene = new QGraphicsScene(this);

    videoItem = new QGraphicsVideoItem();

}

void MusicPlayer::loadMusic(){
    //
    printf("Loading music from %s\n", musicLibraryPath.c_str());
    QStringListModel *model = new QStringListModel(this);
    QStringList items;;
    for (const auto& dirEntry : recurDir(musicLibraryPath.c_str())){
        if(dirEntry.is_directory()){
            printf("Hell nawww\n");
        }else{
            std::string fName = (dirEntry.path().filename()).u8string();
            if(dirEntry.path().filename().extension().u8string().find("mp3") != std::string::npos){
                //items.append(tr(fName.c_str()));
                QListWidgetItem *fListIt = new QListWidgetItem(tr((dirEntry.path()).u8string().c_str()), ui->songList);

            }
        }

    }
    model->setStringList(items);


}


void MusicPlayer::songSelect(QListWidgetItem* cur, QListWidgetItem* prev){
    std::cout << cur->text().toStdString() <<"\n";

    player->setSource(QUrl::fromLocalFile(cur->text()));

    QMediaMetaData metaData = player->metaData();

    QVariant songName = metaData.value(QMediaMetaData::Title);
    QVariant artistName = metaData.value(QMediaMetaData::ContributingArtist);
    QVariant albumName = metaData.value(QMediaMetaData::AlbumTitle);
    QVariant coverImageVariant = metaData.value(QMediaMetaData::CoverArtImage);

    QString songStr = songName.isValid() ? songName.toString() : "Unknown";
    QString artistStr = artistName.isValid() ? artistName.toString() : "Unknown";
    QString albumStr = albumName.isValid() ? albumName.toString() : "Unknown";
    QImage coverImage = coverImageVariant.value<QImage>();

    scene->clear();
    if (coverImage.isNull()) {
        qDebug() << "Cover image is null.";

        // Look at the second stream
        if(player->hasVideo()){
            qDebug() << "Vid Stream.";
            player->setVideoOutput(videoItem);
            scene->addItem(videoItem);
            scene->addText("Swag");
        }else{
            qDebug() << "No video stream.";
        }
    }else{

        scene->addPixmap(QPixmap::fromImage(coverImage));
        scene->setSceneRect(QPixmap::fromImage(coverImage).rect());

    }

    ui->cover->setScene(scene);
    ui->cover->update();

    ui->songTitle->setText(songStr);
    ui->artistName->setText(artistStr);
    ui->albumName->setText(albumStr);




    ui->songProg->setMaximum(player->duration());

    audOut->setVolume(50);
    player->play();
}

void MusicPlayer::songPosChange(qint64 pos){
    ui->songProg->setValue(pos);
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

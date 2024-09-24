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
    videoItem = new QGraphicsVideoItem();
    player->setAudioOutput(audOut);
    player->setVideoOutput(videoItem);

    scene = new QGraphicsScene(this);


    sink = new QVideoSink();

    QObject::connect(ui->songList, &QListWidget::currentItemChanged, this, &MusicPlayer::songSelect);

    QObject::connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayer::songPosChange);

    QObject::connect(ui->songProg, &QSlider::sliderReleased, this, &MusicPlayer::timeBarChanged);

    QObject::connect(ui->volumeSlider, &QSlider::sliderMoved, this, &MusicPlayer::songVolChange);

    QObject::connect(sink, &QVideoSink::videoFrameChanged, this, &MusicPlayer::vidChange);



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
            if(dirEntry.path().filename().extension().u8string().find("mp3") != std::string::npos || dirEntry.path().filename().extension().u8string().find("wav") != std::string::npos){
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

    //C:\Users\segfault\Videos
    //player->setSource(QUrl::fromLocalFile(("C:\\Users\\segfault\\Videos\\mlg.mp4")));

    player->play();

    QMediaMetaData metaData = player->metaData();

    QVariant songName = metaData.value(QMediaMetaData::Title);
    QVariant artistName = metaData.value(QMediaMetaData::ContributingArtist);
    QVariant albumName = metaData.value(QMediaMetaData::AlbumTitle);
    QVariant coverImageVariant = metaData.value(QMediaMetaData::CoverArtImage);
    if(player->videoTracks().length() > 0){
        qDebug() << "Chat chat we got tracks! " << player->videoTracks().length();
        for(auto& thing : player->videoTracks()){
            for(auto& keyVal : thing.keys()){
                qDebug() << keyVal;
            }
        }
        coverImageVariant = player->videoTracks().at(0).value(QMediaMetaData::CoverArtImage);
        qDebug() << player->videoTracks().at(0).stringValue(QMediaMetaData::CoverArtImage) << " | " << coverImageVariant.toString();
    }
    QString songStr = songName.isValid() ? songName.toString() : cur->text();
    QString artistStr = artistName.isValid() ? artistName.toString() : "Unknown Artist";
    QString albumStr = albumName.isValid() ? albumName.toString() : "Unknown Album";
    QImage coverImage = coverImageVariant.value<QImage>();


    scene->clear();
    if (coverImage.isNull()) {
        qDebug() << "Cover image is null.";

        // Look at the second stream
        if(player->hasVideo()){
            qDebug() << "Vid Stream.";


            //player->setActiveVideoTrack(1);

            scene->addText("Swag");

            videoItem = new QGraphicsVideoItem;
            player->setVideoOutput(videoItem);
            scene->addItem(videoItem);

            // Grab
            /*sink = new QVideoSink;
            player->setVideoSink(sink);
            coverImage = sink->videoFrame().toImage();

            coverImage.save("swag.png");

            if(coverImage.isNull()){
                qDebug() << "bad news non-null fans";
            }

            scene->addPixmap(QPixmap::fromImage(coverImage));*/


        }else{
            qDebug() << "No video stream.";
        }
    }else{

        scene->addPixmap(QPixmap::fromImage(coverImage));
        scene->setSceneRect(QPixmap::fromImage(coverImage).rect());

    }

    ui->cover->setScene(scene);
    ui->cover->update();
    ui->cover->show();

    ui->songTitle->setText(songStr);
    ui->artistName->setText(artistStr);
    ui->albumName->setText(albumStr);




    ui->songProg->setMaximum(player->duration());

    audOut->setVolume(ui->volumeSlider->sliderPosition());


}

void MusicPlayer::songPosChange(qint64 pos){
    if(!ui->songProg->isSliderDown())
        ui->songProg->setValue(pos);
}

void MusicPlayer::songVolChange(){
    audOut->setVolume(ui->volumeSlider->sliderPosition() / 100.0f);
    qDebug() << ui->volumeSlider->sliderPosition();
}

void MusicPlayer::vidChange(const QVideoFrame &frame){
   /* QImage coverImage;
    qDebug() << "We got the bool!";
    player->setActiveVideoTrack(1);

    videoItem = new QGraphicsVideoItem;
    player->setVideoOutput(videoItem);

    scene->addText("Swag");

    scene->addItem(videoItem);

    scene->addRect(videoItem->boundingRect());

    scene->setSceneRect(videoItem->boundingRect());

    // Grab
    sink = new QVideoSink;
    player->setVideoSink(sink);
    coverImage = sink->videoFrame().toImage();

    coverImage.save("swag.png");

    if(coverImage.isNull()){
        qDebug() << "bad news non-null fans";
    }

    scene->addPixmap(QPixmap::fromImage(coverImage));
    scene->setSceneRect(QPixmap::fromImage(coverImage).rect());
*/
}

void MusicPlayer::timeBarChanged(){
    player->setPosition(ui->songProg->value());
}


MusicPlayer::~MusicPlayer()
{
    delete ui;
}

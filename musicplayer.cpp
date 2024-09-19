#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);

    // Start finding music files
    musicLibraryPath = "/home/segfault/Music";
    loadMusic();

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
            items.append((dirEntry.path().c_str()));
        }

    }
    model->setStringList(items);
    ui->songList->setModel(model);


}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

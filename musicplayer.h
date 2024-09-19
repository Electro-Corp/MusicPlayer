#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QStringListModel>

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

private:
    Ui::MusicPlayer *ui;

    std::string musicLibraryPath;
};
#endif // MUSICPLAYER_H

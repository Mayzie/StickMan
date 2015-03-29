#include <QApplication>
#include <QMessageBox>
#include <QFileInfo>

#include "ini.h"
#include "background.h"
#include "player.h"
#include "stickman.h"

int main(int argc, char *argv[])
{
    QApplication stickman(argc, argv);

    // Parse the INI file and read values we care about.
    INI* iniFile = new INI(".config");
    if(iniFile->getLastError() != 0) {
        QMessageBox::information(0, "Error", "An error has occurred while trying to parse the .config file.\n\nThe program cannot continue.");
        exit(1);
    }
    int stickManWidth = iniFile->readInteger("Stickman", "Width", 1024);
    int stickManHeight = iniFile->readInteger("Stickman", "Height", 768);
    QString imagesDir = iniFile->readString("Stickman", "ImagesDir", ":/images/");
    int stickManFPS = iniFile->readInteger("Stickman", "FPS", 15);

    QString bgImageFile = imagesDir + iniFile->readString("Background", "Image", "background.png");
    if(!QFileInfo::exists(bgImageFile))
        QMessageBox::information(0, "Warning", "Background image file '" + bgImageFile + "' does not exist. Your game will be very bland.");

    int playerStartX = iniFile->readInteger("Player", "X", 5);
    int playerStartY = iniFile->readInteger("Player", "Y", 5);
    int playerVelocity = iniFile->readInteger("Player", "WalkRightVelocity", -15);
    QString playerWalkImages = iniFile->readString("Player", "WalkRight", "walk1.png+walk2.png");
    QString playerDefaultSize = iniFile->readString("Player", "DefaultSize", "Normal");

    // Allow the user to custom
    QString sizesList = iniFile->readString("Sizes", "Sizes", "");
    if(sizesList.isEmpty()) {
        QMessageBox::information(0, "Error", "No player size has been specified.\n\nThe program cannot continue.");
        exit(2);
    }

    // Place the list of sizes and scaling factors into a nice easy to use map.
    QMap<QString, double> sizes;
    {
        QStringList tempList = sizesList.split('+');
        if(tempList.length() == 0) {
            QMessageBox::information(0, "Error", "An error has occurred while parsing player sizes.\n\nThe program cannot continue.");
            exit(3);
        }

        for(int i = 0; i < tempList.length(); ++i) {
            double tempScale = iniFile->readReal("Size" + tempList[i], "Scale", 1.0);
            sizes.insert(tempList[i], tempScale);
        }
    }

    double currentScalingFactor = sizes.find(playerDefaultSize).value();

    // Normalise coordinates (our config fie uses percentages, remember?)
    playerStartX = (playerStartX * stickManWidth) / 100;
    playerStartY = (playerStartY * stickManHeight) / 100;
    playerVelocity = (playerVelocity * stickManWidth) / 100;

    // Setup GUI
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, stickManWidth, stickManHeight);

    // Load two background images, so we can get the effect of a scrolling background without it cutting.
    Background bg1(bgImageFile, playerVelocity);
    Background bg2(bgImageFile, playerVelocity);
    bg2.setX(bg2.boundingRect().width());   // Set the second background to just outside of the viewport.

    // Create the player.
    Player player(stickManFPS);
    {
        QStringList tempList = playerWalkImages.split('+');

        if(tempList.length() == 0) {
            QMessageBox::information(0, "Error", "No player image files specified. You'll have a very boring game.");
        }

        for(int i = 0; i < tempList.length(); ++i) {
            if(QFileInfo::exists(imagesDir + tempList[i]))
                player.addAnimationImage(imagesDir + tempList[i]);
            else
                QMessageBox::information(0, "Warning", "Image file '" + imagesDir + tempList[i] + "' does not exist. Skipping.");
        }
    }
    player.setScale(currentScalingFactor);
    player.setPos(playerStartX, stickManHeight - playerStartY - (player.boundingRect().height() * currentScalingFactor));

    StickMan view(stickManFPS, &player);
    view.setScene(&scene);
    view.resize(stickManWidth, stickManHeight);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add all items we want to be displayed to the current scene.
    scene.addItem(&bg1);
    scene.addItem(&bg2);
    scene.addItem(&player);

    view.show();

    return stickman.exec();
}

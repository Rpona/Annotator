#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QToolBar>
#include <QMenu>
#include <QLabel>

#include <QFileDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void createMenus();
    void createGridGroupBox();
    void label(int num);

    QVBoxLayout *mainLayout;
    QGroupBox *gridGroupBox;

    QToolBar *toolBar;
    QMenu *fileMenu;
    QAction *openAct;
    QAction *correctAct;
    QAction *exitAct;

    QLabel *XRayImg;
    QNetworkAccessManager* m_netwManager;


    QLineEdit *imgInfo;
    QLineEdit *boxInfo;

    QFile *file;
    QFile *outfile;
    QTableView *tableView;
    QStandardItemModel *csvModel;

    QPushButton *newButton;
    QPushButton *cleanButton;
    QPushButton *saveButton;

    QPushButton *lastButton;
    QPushButton *nextButton;
    QPushButton *zeroButton;
    QPushButton *oneButton;
    QPushButton *twoButton;
    QPushButton *threeButton;
    QPushButton *fourButton;
    QPushButton *fiveButton;
    QPushButton *sixButton;
    QPushButton *sevenButton;
    QPushButton *eightButton;
    QPushButton *nineButton;

private slots:
    void openFile();
    void correctPos();
    void openCsv();
    void saveCsv();
    void newColumn();
    void readImg();
    void lastImg();
    void nextImg();
    void cleanBox();
    void loadBox();
    void slot_netwManagerFinished(QNetworkReply *reply);

    void zero();
    void one();
    void two();
    void three();
    void four();
    void five();
    void six();
    void seven();
    void eight();
    void nine();

};

#endif // MainWindow_H

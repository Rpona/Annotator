//UI
#include <QtWidgets>
#include <QApplication>
#include "mainwindow.h"

// STL
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "config.h"

bool positionCheck=false;
bool openCheck=false;
string imgFile = "Q-img-gray.JPG";
QString csvPath;
QString imgDirPath;
int nowID = 1;
int totalColumn = 0;
int totalRaw = 0;
int newNumColumn=0;
bool getImg;

typedef struct
{
    int LT_x[8];
    int LT_y[8];
    int RD_x[8];
    int RD_y[8];
    int num=0;
}Box;

//bounding box
QString boundingBoxString = "";
std::vector<Box> imgBox;

MainWindow::MainWindow()
{
    createMenus();
    createGridGroupBox();

    QWidget *widget = new QWidget;
    mainLayout = new QVBoxLayout;
    mainLayout -> addWidget( gridGroupBox );
    widget -> setLayout( mainLayout );
    widget -> setWindowFlags(Qt::FramelessWindowHint);
    widget -> setStyleSheet("color: #aab7b8; background-color: #17202a; font: 16px;");

    MainWindow::setCentralWidget( widget );
    setWindowTitle( tr( "Quanta Annotation Tool" ) );
}

void MainWindow::createGridGroupBox()
{
    //Declare layout
    gridGroupBox = new QGroupBox();
    QGridLayout *layout = new QGridLayout;

    //XRay
    QPixmap p( QString::fromStdString( imgFile ) );
    XRayImg = new QLabel;
    XRayImg -> setPixmap( p.scaled( 1024, 1024, Qt::KeepAspectRatio ) );
    layout -> addWidget( XRayImg, 0, 0, 8, 4 );
    gridGroupBox -> setLayout( layout );

    //csv
    csvModel = new QStandardItemModel( this );
    tableView = new QTableView;
    tableView -> setAutoScroll(true);
    tableView -> setModel( csvModel );
    tableView -> setGridStyle(Qt::DotLine);
    tableView -> verticalHeader()->setStyleSheet("::section{color: black; Background-color:#aab7b8;}");
    tableView -> horizontalHeader()->setStyleSheet("::section{color: black; Background-color:#aab7b8;}");
    tableView -> setStyleSheet("color: #aab7b8; background-color: #17202a; font: 16px;");
    layout -> addWidget( tableView, 0, 4, 1, 3 );

    //current information
    imgInfo = new QLineEdit();
    layout -> addWidget( imgInfo, 1, 4, 1, 3 );
    boxInfo = new QLineEdit();
    layout -> addWidget( boxInfo, 2, 4, 1, 3 );

    //Buttons
    newButton = new QPushButton(tr( "New column" ), this );
    cleanButton = new QPushButton( tr( "Clean bunding box" ), this );
    saveButton = new QPushButton( tr( "Save CSV file" ), this );
    layout -> addWidget( newButton, 3, 4, 1, 1 );
    layout -> addWidget( cleanButton, 3, 5, 1, 1 );
    layout -> addWidget( saveButton, 3, 6, 1, 1 );

    connect( newButton , SIGNAL( clicked() ), this, SLOT( newColumn() ) );
    connect( cleanButton, SIGNAL(clicked()), this, SLOT( cleanBox() ) );
    connect( saveButton , SIGNAL( clicked() ), this, SLOT( saveCsv() ) );

    lastButton = new QPushButton( tr( "<-" ),this );
    lastButton -> setShortcut( Qt::Key_Left );
    nextButton = new QPushButton( tr( "->" ), this );
    nextButton -> setShortcut( Qt::Key_Right );
    zeroButton = new QPushButton( tr( "0" ), this );
    zeroButton -> setShortcut( Qt::Key_0 );
    oneButton = new QPushButton( tr( "1" ), this );
    oneButton -> setShortcut( Qt::Key_1 );
    twoButton = new QPushButton( tr( "2" ), this );
    twoButton -> setShortcut( Qt::Key_2 );
    threeButton = new QPushButton( tr( "3" ), this );
    threeButton -> setShortcut( Qt::Key_3 );
    fourButton = new QPushButton( tr( "4" ), this );
    fourButton -> setShortcut( Qt::Key_4 );
    fiveButton = new QPushButton( tr( "5" ), this );
    fiveButton -> setShortcut( Qt::Key_5 );
    sixButton = new QPushButton( tr( "6" ), this );
    sixButton -> setShortcut( Qt::Key_6 );
    sevenButton = new QPushButton( tr( "7" ), this );
    sevenButton -> setShortcut( Qt::Key_7 );
    eightButton = new QPushButton( tr( "8" ), this );
    eightButton -> setShortcut( Qt::Key_8 );
    nineButton = new QPushButton( tr( "9") , this );
    nineButton -> setShortcut(Qt::Key_9);

    layout -> addWidget( lastButton, 7, 4, 1, 1 );
    layout -> addWidget( zeroButton, 7, 5, 1, 1 );
    layout -> addWidget( nextButton, 7, 6, 1, 1 );
    layout -> addWidget( oneButton, 6, 4, 1, 1 );
    layout -> addWidget( twoButton, 6, 5, 1, 1 );
    layout -> addWidget( threeButton, 6, 6, 1, 1 );
    layout -> addWidget( fourButton, 5, 4, 1, 1 );
    layout -> addWidget( fiveButton, 5, 5, 1, 1 );
    layout -> addWidget( sixButton, 5, 6, 1, 1 );
    layout -> addWidget( sevenButton, 4, 4, 1, 1 );
    layout -> addWidget( eightButton, 4, 5, 1, 1 );
    layout -> addWidget( nineButton, 4, 6, 1, 1 );

    connect( lastButton, SIGNAL( clicked() ), this, SLOT( lastImg() ));
    connect( nextButton, SIGNAL( clicked() ), this, SLOT( nextImg() ));
    connect( zeroButton, SIGNAL( clicked() ), this, SLOT(zero()));
    connect( oneButton, SIGNAL( clicked() ), this, SLOT(one()));
    connect( twoButton, SIGNAL( clicked() ), this, SLOT(two()));
    connect( threeButton, SIGNAL( clicked() ), this, SLOT(three()));
    connect( fourButton, SIGNAL( clicked() ), this, SLOT(four()));
    connect( fiveButton, SIGNAL( clicked() ), this, SLOT(five()));
    connect( sixButton, SIGNAL( clicked() ), this, SLOT(six()));
    connect( sevenButton, SIGNAL( clicked() ), this, SLOT(seven()));
    connect( eightButton, SIGNAL( clicked() ), this, SLOT(eight()));
    connect( nineButton, SIGNAL( clicked() ), this, SLOT(nine()));

}

void MainWindow::createMenus()
{
    //Menu bar
    fileMenu = menuBar() -> addMenu( tr( "&File" ) );
    openAct = fileMenu -> addAction( tr( "&Open Files" ) );
    correctAct= fileMenu -> addAction(tr("Position correction"));
    exitAct = fileMenu -> addAction( tr( "&Exit" ) );

    //Tool bar
    toolBar = addToolBar( tr( "&File" ) );
    toolBar -> setStyleSheet ("color:  #aab7b8; background-color: #17202a; font: 16px;");
    toolBar -> addAction( openAct );
    toolBar -> addAction( correctAct);
    toolBar -> addAction( exitAct );

    //Action
    connect( openAct, SIGNAL( triggered() ), this, SLOT( openFile() ) );
    connect( correctAct, SIGNAL(triggered()),this,SLOT( correctPos() ) );
    connect( exitAct, SIGNAL( triggered() ), this, SLOT( close() ) );
}

void MainWindow::openFile()
{
    csvPath = QFileDialog::getOpenFileName( this, tr("Open CSV Files"),
                                            "/home", "tr(Files(*.csv)");
    //  imgDirPath = QFileDialog::getExistingDirectory(this, tr("Open Image Directory"),
    //                                                 "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    imgDirPath = "http://dicom.icu:8080/1024";//"https://events17.linuxfoundation.org/sites/events/files";//
    //  imgDirPath = "C:/Users/10508029.QUANTA/Desktop/BU9/Medical/Annotator_windows/XRays";
    if( (csvPath.isEmpty() ) || ( imgDirPath.isEmpty() ) )
    {
        QMessageBox msgBox;
        msgBox.setText( "Failed to open files." );
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            openFile();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
    else
        openCsv();
    readImg();
}

void MainWindow::openCsv()
{
    csvModel -> clear();

    file = new QFile( csvPath );

    if ( !file -> open(QFile::ReadWrite | QFile::Text) )
        qDebug() << "File not exists";
    else
    {
        QTextStream in( file );
        //Reads the data up to the end of file
        totalRaw=0;
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements
            QList < QStandardItem *> standardItemsList;
            // consider that the line separated by semicolons into columns
            for ( QString item : line.split( "," ) ) {
                standardItemsList.append( new QStandardItem( item ) );
            }
            csvModel -> insertRow( csvModel -> rowCount(), standardItemsList );
            totalRaw++;
        }
        file -> close();
    }
    totalColumn = csvModel -> columnCount();
    imgBox.resize(totalRaw);
    imgBox.clear();
}

void MainWindow::newColumn()
{

    bool ok;
    QString text = QInputDialog::getText(this, tr("New column"), tr("Please enter name of the disease?"), QLineEdit::Normal, "",&ok);
    if(!ok || text.isEmpty())
    {
        qDebug() << "Enter erroe";
        return;
    }
    csvModel -> clear();

    newNumColumn += 2;
    totalColumn += 2;

    file = new QFile( csvPath );

    if ( !file -> open ( QFile::ReadWrite | QFile::Text ) )
        qDebug() << "File not exists";
    else
    {
        QTextStream in ( file );
        //Reads the data up to the end of file
        bool firstRaw = true;
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements
            QList<QStandardItem *> standardItemsList;
            // consider that the line separated by semicolons into columns

            bool firstColumn = true;
            for ( QString item : line.split(",") )
            {
                standardItemsList.append( new QStandardItem( item ) );
                if( firstColumn )
                {
                    if( firstRaw )
                    {
                        standardItemsList.append( new QStandardItem( text ) );
                        text.append( " - bounding box" );
                        standardItemsList.append( new QStandardItem( text ) );
                        firstRaw = false;
                    }
                    else
                    {
                        standardItemsList.append( new QStandardItem( " " ) );
                        standardItemsList.append( new QStandardItem( " " ) );
                    }
                    firstColumn = false;
                }
            }
            csvModel -> insertRow( csvModel -> rowCount(), standardItemsList );

        }
        file -> close();
    }

}

void MainWindow::saveCsv()
{
    //Saved as another neme
    csvPath.insert((csvPath.length()-4),QString(QDate::currentDate().toString("yyMMdd")));

    file = new QFile(csvPath);

    if ( !file -> open( QFile::WriteOnly | QFile::Text ) )
        qDebug() << "File not exists";

    else
    {
        QTextStream out(file);
        int row = csvModel -> rowCount();

        for(int i = 0; i < row; i++)
        {
            for(int col = 0; col < (totalColumn-1); col++)
            {
                QString string = csvModel->item( i, col ) -> text();
                out << string << ",";
            }
            out << "\n";
        }
        file -> close();
    }
    openCsv();
}


void MainWindow::readImg()
{
    getImg=true;
    m_netwManager = new QNetworkAccessManager(this);
    connect(m_netwManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_netwManagerFinished(QNetworkReply*)));

    QString image = imgDirPath;
    QString string = csvModel -> item( nowID, 0 ) -> text();
    image.append("/");
    image.append(string);
    image.append(".png");

    QUrl url(image);
    QNetworkRequest request(url);
    m_netwManager->get(request);
    std::cout << image.toUtf8().constData();

    //read infomation
    QString info=QString::number(nowID+1);
    info.append(  " - ");

    int temp=5;
    if( temp > totalColumn-1)
        temp = totalColumn-1;
    for(int col = 0; col < temp; col++)
    {
        info.append(  csvModel->item( nowID, col ) -> text());
        if(col!=totalColumn-2)
            info.append( " ; ") ;
    }

    if(nowID!=0)
        tableView -> selectionModel()->select(tableView->model()->index(nowID-1,0),QItemSelectionModel::Deselect);
    if(nowID!=totalRaw)
        tableView -> selectionModel()->select(tableView->model()->index(nowID+1,0),QItemSelectionModel::Deselect);
    tableView -> selectionModel()->select(tableView->model()->index(nowID,0),QItemSelectionModel::Select);

    imgInfo->setText(info);
    // loadBox();
}

void MainWindow::slot_netwManagerFinished(QNetworkReply *reply)
{


    if(reply->error() != QNetworkReply::NoError)
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();

    QByteArray jpegData = reply->readAll();

    QPixmap pixmap;
    pixmap.loadFromData(jpegData);

    if(pixmap.isNull())
        pixmap.load(QString::fromStdString( imgFile ));

    openCheck = true;
    XRayImg -> setPixmap( pixmap.scaled( 1024,1024,Qt::KeepAspectRatio ) );
}

void MainWindow::lastImg()
{
    if(openCheck)
    {
        if( nowID > 1 )
            nowID--;
        else
            nowID=1;
        readImg();
        loadBox();
    }
}

void MainWindow::nextImg()
{
    if(openCheck)
    {
        if(nowID < ( totalRaw - 1 ))
            nowID++;
        else
            nowID = (totalRaw - 1);
        readImg();
        loadBox();
    }
}

void MainWindow::label(int num)
{
    file = new QFile(csvPath);

    if ( !file -> open(QFile::WriteOnly | QFile::Text) )
        qDebug() << "File not exists";

    else
    {
        QTextStream out(file);
        int row = csvModel -> rowCount();

        QString string;
        for( int i = 0; i < row; i++ )
        {
            for( int col = 0; col < (totalColumn - 1); col++ )
            {
                if( i == nowID && col == (newNumColumn-1) )
                    string = QString::number(num);
                else if(i == nowID && col == (newNumColumn) )
                    string = boundingBoxString;
                else
                    string = csvModel->item(i,col)->text();
                out << string << ",";
            }
            out<<"\n";
        }
        file->close();
    }

    openCsv();

    if(nowID < ( totalRaw - 1 ))
        nowID++;
    else
        nowID = (totalRaw - 1);

    cleanBox();
    readImg();
}

void MainWindow::zero()
{
    if(newNumColumn!=0)
        label(0);
}
void MainWindow::one()
{
    if(newNumColumn!=0)
        label(1);
}
void MainWindow::two()
{
    if(newNumColumn!=0)
        label(2);
}
void MainWindow::three()
{
    if(newNumColumn!=0)
        label(3);
}
void MainWindow::four()
{
    if(newNumColumn!=0)
        label(4);
}
void MainWindow::five()
{
    if(newNumColumn!=0)
        label(5);
}
void MainWindow::six()
{
    if(newNumColumn!=0)
        label(6);
}
void MainWindow::seven()
{
    if(newNumColumn!=0)
        label(7);
}
void MainWindow::eight()
{
    if(newNumColumn!=0)
        label(8);
}
void MainWindow::nine()
{
    if(newNumColumn!=0)
        label(9);
}

void MainWindow::correctPos()
{
    positionCheck=true;
}

QString imgForBox;
double ratioX,ratioY;
double initX=21,initY=91;
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(positionCheck)
    {
        initX = (event->x());
        initY = (event->y());
    }
    else if(openCheck)
    {
        imgForBox = imgDirPath ;
        imgForBox.append("/");
        imgForBox.append(csvModel -> item( nowID, 0 ) -> text());
        imgForBox.append(".png");

        QPixmap p (imgForBox);
        ratioX = (double)p.width()/(double)1024;
        ratioY = (double)p.height()/(double)1024;
        imgBox[nowID].LT_x[imgBox[nowID].num] = ((event->x())-initX) * ratioX;
        imgBox[nowID].LT_y[imgBox[nowID].num] = ((event->y())-initY) * ratioX;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(positionCheck)
    {
        initX = (event->x());
        initY = (event->y());
        positionCheck=false;
    }
    else if(openCheck)
    {
        imgBox[nowID].RD_x[imgBox[nowID].num] = ((event->x())-initX)*ratioX;
        imgBox[nowID].RD_y[imgBox[nowID].num] = ((event->y())-initY)*ratioX;
        if( imgBox[nowID].LT_x[imgBox[nowID].num] > 0 && imgBox[nowID].LT_y[imgBox[nowID].num] > 0 && imgBox[nowID].RD_x[imgBox[nowID].num] > 0 && imgBox[nowID].RD_x[imgBox[nowID].num] > 0)
        {
            if( imgBox[nowID].LT_x[imgBox[nowID].num] <= 1024 && imgBox[nowID].LT_y[imgBox[nowID].num] <= 1024 && imgBox[nowID].RD_x[imgBox[nowID].num] <= 1024 && imgBox[nowID].RD_x[imgBox[nowID].num] <= 1024)
            {
                imgBox[nowID].num++;
                loadBox();
            }
        }
    }
}

void MainWindow::loadBox()
{
    if(imgBox[nowID].num!=0)
    {
        boundingBoxString="";
        for(int i=0; i<imgBox[nowID].num; i++)
        {
            boundingBoxString.append( " (" );
            boundingBoxString.append( QString::number( imgBox[nowID].LT_x[i] ) );
            boundingBoxString.append( ". " );
            boundingBoxString.append( QString::number( imgBox[nowID].LT_y[i] ) );
            boundingBoxString.append( ". " );
            boundingBoxString.append( QString::number( imgBox[nowID].RD_x[i] ) );
            boundingBoxString.append( ". " );
            boundingBoxString.append( QString::number( imgBox[nowID].RD_y[i] ) );
            boundingBoxString.append( ") " );
        }
        boxInfo->setText(boundingBoxString);

        cv::Mat img=cv::imread(imgForBox.toStdString()); //Bounding box drawing
        for(int i=0;i<imgBox[nowID].num;i++)
            cv::rectangle(img,cv::Point( imgBox[nowID].LT_x[i], imgBox[nowID].LT_y[i]),cv::Point( imgBox[nowID].RD_x[i], imgBox[nowID].RD_y[i]),cv::Scalar(0,0,255),2);

        cv::cvtColor(img,img,CV_BGR2RGB); //OpenCV[BGR] -> QT[RGB]
        QPixmap p = QPixmap::fromImage(QImage((unsigned char*) img.data, img.cols, img.rows, QImage::Format_RGB888));
        XRayImg->setPixmap(p.scaled(1024,1024,Qt::KeepAspectRatio));
    }
}

void MainWindow::cleanBox()
{
    boundingBoxString="";
    imgBox[nowID].num=0;
    boxInfo->setText(boundingBoxString);
    readImg();
}

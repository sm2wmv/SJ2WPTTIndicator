#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Software written by Mikael Larsmark, SJ2W and free to use and modify */

#define PIXMAP_BLANK QCoreApplication::applicationDirPath()+"/leds/led_blank_34x34.png"
#define PIXMAP_RED_ON QCoreApplication::applicationDirPath()+"/leds/led_red_on_34x34.png"
#define PIXMAP_GREEN_ON QCoreApplication::applicationDirPath()+"/leds/led_green_on_34x34.png"
#define PIXMAP_RED_OFF QCoreApplication::applicationDirPath()+"/leds/led_red_off_34x34.png"
#define PIXMAP_GREEN_OFF QCoreApplication::applicationDirPath()+"/leds/led_green_off_34x34.png"
#define PIXMAP_YELLOW_ON QCoreApplication::applicationDirPath()+"/leds/led_yellow_on_34x34.png"
#define PIXMAP_YELLOW_OFF QCoreApplication::applicationDirPath()+"/leds/led_yellow_off_34x34.png"

//#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logSoftware = "N1MM"; //Default

    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini",QSettings::IniFormat,0);
        settings.beginGroup("LOG");
            logSoftware = settings.value("LogSoftware").toString();
        settings.endGroup();

        settings.beginGroup("GUI");
            displayName[0] = settings.value("Station1DisplayName").toString();
            displayName[1] = settings.value("Station2DisplayName").toString();
            displayName[2] = settings.value("Station3DisplayName").toString();
            displayName[3] = settings.value("Station4DisplayName").toString();
        settings.endGroup();

        settings.beginGroup("NETWORK");
            networkPort = settings.value("Port").toInt();
            localIP = settings.value("LocalIP").toString();
            networkName[0] = settings.value("Station1NetworkName").toString();
            networkName[1] = settings.value("Station2NetworkName").toString();
            networkName[2] = settings.value("Station3NetworkName").toString();
            networkName[3] = settings.value("Station4NetworkName").toString();
        settings.endGroup();

    ui->labelLEDPTT1->setPixmap(PIXMAP_GREEN_OFF);
    ui->labelLEDPTT2->setPixmap(PIXMAP_GREEN_OFF);
    ui->labelLEDPTT3->setPixmap(PIXMAP_GREEN_OFF);
    ui->labelLEDPTT4->setPixmap(PIXMAP_GREEN_OFF);
    ui->labelPTT1->setText(displayName[0]);
    ui->labelPTT2->setText(displayName[1]);
    ui->labelPTT3->setText(displayName[2]);
    ui->labelPTT4->setText(displayName[3]);

    logServer = new UDPServer();
    logServer->initSocket(localIP,networkPort);
    connect(logServer, SIGNAL(dataAvailable(char * ,qint64 ,QHostAddress *,quint16 *)),SLOT(radioDataAvailable(char *,qint64, QHostAddress *, quint16 *)));

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::radioDataAvailable(char *data, qint64 size, QHostAddress *fromAddr, quint16 *port) {
    QString computerName = "";
    QString xmitStatus = "";
    QString xmitMessage = "";

    /* THIS IS FOR N1MM */
    if (logSoftware == "N1MM") {
        QXmlStreamReader xml(data);

        while(!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                QString name = xml.name().toString();

                if (name == "RadioInfo") {
                    while(!xml.atEnd()) {
                        xml.readNextStartElement();
                        QString subName = xml.name().toString();

                        if (subName == "StationName") {
                            computerName = xml.readElementText();
                        }
                        else if (subName == "IsTransmitting") {
                            xmitStatus = xml.readElementText();
                        }
                        else if (subName == "FunctionKeyCaption") {
                            xmitMessage = xml.readElementText();
                        }
                    }
                    break;
                }
            }
        }
    }
    else if (logSoftware == "DXLog") { /* THIS IS FOR DXLOG */
        //Converting it all to 16 bit
        QByteArray convBuf;
        convBuf.append(data,size);
        const char * dataC=convBuf.constData();
        const qint16 * data16=reinterpret_cast<const qint16 *>(dataC);
        int len=convBuf.size()/(sizeof(qint16));

        //Converting the 16 bit data to QChar in a QByteArray
        QByteArray radioInfo;
        for (int i=0;i<len;i++) {
            radioInfo.append(QChar(data16[i]));
        }

        //Check if this is Station Information
        if (radioInfo.mid(32,3) == "BOK") {
            //Get stationID
            int pos = 0;
            while(radioInfo.at(pos + 16) != 0)
                pos++;

            computerName = radioInfo.mid(16,pos);
            xmitStatus = "True";
        }

        if (radioInfo.mid(32,3) == "BUN") {
            //Get stationID
            int pos = 0;
            while(radioInfo.at(pos + 40) != 0)
                pos++;

            computerName = radioInfo.mid(40,pos);
            xmitStatus = "False";
        }
    }

    if (computerName == networkName[0]) {
        if (xmitStatus == "True")
            ui->labelLEDPTT1->setPixmap(PIXMAP_RED_ON);
        else
            ui->labelLEDPTT1->setPixmap(PIXMAP_GREEN_OFF);
    }
    else if (computerName == networkName[1]) {
        if (xmitStatus == "True")
            ui->labelLEDPTT2->setPixmap(PIXMAP_RED_ON);
        else
            ui->labelLEDPTT2->setPixmap(PIXMAP_GREEN_OFF);
    }
    else if (computerName == networkName[2]) {
        if (xmitStatus == "True")
            ui->labelLEDPTT3->setPixmap(PIXMAP_RED_ON);
        else
            ui->labelLEDPTT3->setPixmap(PIXMAP_GREEN_OFF);
    }
    else if (computerName == networkName[3]) {
        if (xmitStatus == "True")
            ui->labelLEDPTT4->setPixmap(PIXMAP_RED_ON);
        else
            ui->labelLEDPTT4->setPixmap(PIXMAP_GREEN_OFF);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {

}

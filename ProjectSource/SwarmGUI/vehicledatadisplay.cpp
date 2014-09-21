#include "vehicledatadisplay.h"
#include "ui_vehicledatadisplay.h"
#include <stdio.h>
VehicleDataDisplay::VehicleDataDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VehicleDataDisplay)
{
    ui->setupUi(this);
    m_Conversion = new Conversions();
    m_Initialization = new Initialization();
    boolean_Calibration = false;
}

VehicleDataDisplay::~VehicleDataDisplay()
{
    delete ui;
}

void VehicleDataDisplay::addVehicle(const int &VehicleID)
{
    m_currentVehicleID = VehicleID;
}

void VehicleDataDisplay::updateAttitude(const mavlink_common::ATTITUDE &VehicleAttitude)
{
    ui->widget_MainFlightInstrument->setRoll(VehicleAttitude.roll * (180.0/3.14159265));
    ui->widget_MainFlightInstrument->setPitch(VehicleAttitude.pitch * (180.0/3.14159265));
    ui->widget_MainFlightInstrument->update();
}

void VehicleDataDisplay::updatePositioning(const mavlink_common::GLOBAL_POSITION_INT &VehiclePositionGPS)
{
    double GPSdivisor = 10000000.0;
    ui->lineEdit_relativeAlt->setText(QString::number(VehiclePositionGPS.relative_alt / 1000.0));
    ui->lineEdit_Latitude->setText(QString::number(VehiclePositionGPS.lat / GPSdivisor));
    ui->lineEdit_Longitude->setText(QString::number(VehiclePositionGPS.lon / GPSdivisor));
    ui->widget_MainFlightInstrument->setHeading(VehiclePositionGPS.hdg / 100.0);
}

void VehicleDataDisplay::updateFlightMode(const mavlink_common::HEARTBEAT &VehicleHeartbeat)
{
    if(VehicleHeartbeat.custom_mode != m_Conversion->FlightMode_StringtoEnum(ui->lineEdit_FlightMode->text()))
    {
        if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Stabilize)
        {
            ui->lineEdit_FlightMode->setText("Stabilize");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Acro)
        {
            ui->lineEdit_FlightMode->setText("Acro");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.AltHold)
        {
            ui->lineEdit_FlightMode->setText("AltHold");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Auto)
        {
            ui->lineEdit_FlightMode->setText("Auto");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Guided)
        {
            ui->lineEdit_FlightMode->setText("Guided");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Loiter)
        {
            ui->lineEdit_FlightMode->setText("Loiter");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.RTL)
        {
            ui->lineEdit_FlightMode->setText("RTL");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.CircleFM)
        {
            ui->lineEdit_FlightMode->setText("Circle");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Land)
        {
            ui->lineEdit_FlightMode->setText("Land");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Drift)
        {
            ui->lineEdit_FlightMode->setText("Drift");
        }
        else if(VehicleHeartbeat.custom_mode == m_Initialization->m_FlightModeV.Sport)
        {
            ui->lineEdit_FlightMode->setText("Sport");
        }
        ui->comboBox_DesiredFlightMode->setCurrentIndex(ui->comboBox_DesiredFlightMode->findText(ui->lineEdit_FlightMode->text()));
    }
}


void VehicleDataDisplay::on_comboBox_DesiredFlightMode_activated(const QString &arg1)
{
    int DFM = m_Conversion->FlightMode_StringtoEnum(arg1);
    emit(desiredFlightMode(m_currentVehicleID,DFM));
}

void VehicleDataDisplay::updateArmStatus(const bool &Armed)
{
    if(Armed == true)
    {
        ui->pushButton_ARM->setDisabled(true);
        ui->pushButton_DISARM->setDisabled(false);
    }
    else
    {
        ui->pushButton_ARM->setDisabled(false);
        ui->pushButton_DISARM->setDisabled(true);
    }
}


void VehicleDataDisplay::on_pushButton_STREAM_clicked()
{
    int streamTypeInt;
    QString streamString = ui->comboBox_StreamType->currentText();
    int streamRate = ui->spinBox_StreamRate->value();
    streamTypeInt = m_Conversion->StreamMode_StringtoEnum(streamString);
    emit(requestStream(m_currentVehicleID,streamTypeInt,streamRate));
}

void VehicleDataDisplay::on_pushButton_Cal_released()
{
    boolean_Calibration = !boolean_Calibration;
    emit(radioCalibrate(m_currentVehicleID , m_Initialization->m_StreamModeV.RCChannels , boolean_Calibration));
}

void VehicleDataDisplay::updateRCValues(const mavlink_common::RC_CHANNELS_RAW &RCValues)
{
    ui->lineEdit_PitchCurrent->setText(QString::number(RCValues.chan2_raw));
    ui->lineEdit_RollCurrent->setText(QString::number(RCValues.chan1_raw));
    ui->lineEdit_ThrottleCurrent->setText(QString::number(RCValues.chan3_raw));
    ui->lineEdit_YawCurrent->setText(QString::number(RCValues.chan4_raw));

    if(RCValues.chan1_raw > m_RCCalibration.roll_high)
    {
        m_RCCalibration.roll_high = RCValues.chan1_raw;
        ui->lineEdit_RollHigh->setText(QString::number(m_RCCalibration.roll_high));
    }
    if(RCValues.chan1_raw < m_RCCalibration.roll_low)
    {
        m_RCCalibration.roll_low = RCValues.chan1_raw;
        ui->lineEdit_RollLow->setText(QString::number(m_RCCalibration.roll_low));
    }

    if(RCValues.chan2_raw > m_RCCalibration.pitch_high)
    {
        m_RCCalibration.pitch_high = RCValues.chan2_raw;
        ui->lineEdit_PitchHigh->setText(QString::number(m_RCCalibration.pitch_high));
    }
    if(RCValues.chan2_raw < m_RCCalibration.pitch_low)
    {
        m_RCCalibration.pitch_low = RCValues.chan2_raw;
        ui->lineEdit_PitchLow->setText(QString::number(m_RCCalibration.pitch_low));
    }

    if(RCValues.chan3_raw > m_RCCalibration.throttle_high)
    {
        m_RCCalibration.throttle_high = RCValues.chan3_raw;
        ui->lineEdit_ThrottleHigh->setText(QString::number(m_RCCalibration.throttle_high));
    }
    if(RCValues.chan3_raw < m_RCCalibration.throttle_low)
    {
        m_RCCalibration.throttle_low = RCValues.chan3_raw;
        ui->lineEdit_ThrottleLow->setText(QString::number(m_RCCalibration.throttle_low));
    }

    if(RCValues.chan4_raw > m_RCCalibration.yaw_high)
    {
        m_RCCalibration.yaw_high = RCValues.chan4_raw;
        ui->lineEdit_YawHigh->setText(QString::number(m_RCCalibration.yaw_high));
    }
    if(RCValues.chan4_raw < m_RCCalibration.yaw_low)
    {
        m_RCCalibration.yaw_low = RCValues.chan4_raw;
        ui->lineEdit_YawLow->setText(QString::number(m_RCCalibration.yaw_low));
    }

}


void VehicleDataDisplay::on_pushButton_ARM_clicked()
{
    emit(armRequest(m_currentVehicleID,true));
}

void VehicleDataDisplay::on_pushButton_DISARM_clicked()
{
    emit(armRequest(m_currentVehicleID,false));
}

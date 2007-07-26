/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 *   Copyright (C) 2007 by Dominik Riebeling
 *   $Id$
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef INSTALL_H
#define INSTALL_H

#include <QtGui>

#include <QSettings>

#include "ui_installfrm.h"
#include "ui_installprogressfrm.h"
#include "installrb.h"

class Install : public QDialog
{
    Q_OBJECT
    public:
        Install(QWidget *parent = 0);
        void setProxy(QUrl);
        void setReleased(QString);
        void setMountPoint(QString);
        void setUserSettings(QSettings*);
        void setDeviceSettings(QSettings*);
        void setArchivedString(QString);

    public slots:
        void accept(void);
//         void extractBuild(bool);

    private:
        Ui::InstallFrm ui;
        Ui::InstallProgressFrm dp;
        QUrl proxy;
        QString releasever;
        QSettings *devices;
        QSettings *userSettings;
        QDialog *downloadProgress;
        QHttp *download;
        QFile *target;
        QString file;
        QString fileName;
        QString mountPoint;
        QString archived;
        RBInstaller* installer;

    private slots:
        void setCached(bool);
        void browseFolder(void);
        void setDetailsCurrent(bool);
        void setDetailsStable(bool);
        void setDetailsArchived(bool);
        void done(bool);
        
};


#endif

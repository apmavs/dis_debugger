#ifndef PLAYBACKPDUSOURCE_H
#define PLAYBACKPDUSOURCE_H

#include <QThread>
#include <QMutex>
#include <QTime>
#include <iostream>
#include <vector>
#include <stdint.h>

#include "PduSource.h"
#include "PduObserver.h"

// KDIS includes
#include "KDIS/Network/Connection.h"
#include "KDIS/Extras/DIS_Logger_Playback.h"

class PlaybackPduSource : public PduSource
{
private:
    QMutex mutex;
    KDIS::UTILS::DIS_Logger_Playback* playback;
    QTime playback_start_time;
    bool playing;
    bool fast_play;

public:
    PlaybackPduSource();
    virtual ~PlaybackPduSource();

    // fastPlay triggers immediate playback of all data
    bool startPlayback(std::string filePath, bool fastPlay);
    virtual void run();
};

#endif // NETWORKTHREAD_H

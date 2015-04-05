#include <auto_ptr.h>
#include "PlaybackPduSource.h"
#include "KDIS/Extras/PDU_Factory.h"
#include "KDIS/PDU/Header.h"
#include <sstream>

PlaybackPduSource::PlaybackPduSource()
{
    playback  = NULL;
    playing   = false;
    fast_play = false;
    moveToThread(this);
}

PlaybackPduSource::~PlaybackPduSource()
{
    playing = false;
    if(isRunning())
    {
        quit(); // Signal thread to terminate
        wait(); // Wait until thread terminates
        if(playback != NULL)
            delete playback;
    }
}

bool PlaybackPduSource::startPlayback(std::string filePath, bool fastPlay)
{
    bool logHasData = false;

    mutex.lock();
    playback = new KDIS::UTILS::DIS_Logger_Playback(filePath, 0); // 0 to load entire log
    if(!playback->EndOfLogReached())
    {
        logHasData = true;
        playback_start_time.start();
        playing = true;
        fast_play = fastPlay;
        start();
    }
    else
        playing = false;
    mutex.unlock();

    return logHasData;
}

void PlaybackPduSource::run()
{
    if(isRunning())
        setPriority(QThread::HighPriority);
    try
    {
        // Wait until we get a signal to start playing back
        mutex.lock();
        while(isRunning() && !playing)
        {
            mutex.unlock();
            msleep(1000);
            mutex.lock();
        }

        int timePassed;
        KDIS::KOCTET raw_buffer[MAX_PDU_SIZE];
        KDIS::KDataStream dataStream;
        double nextTimestamp;
        bool pduSent;
        while(isRunning() && playing && !playback->EndOfLogReached())
        {
            timePassed = playback_start_time.elapsed() * 0.001;

            // Get next pdu
            playback->GetNext(nextTimestamp, dataStream);
            pduSent = false;

            while(isRunning() && !pduSent)
            {
                if(fast_play || (timePassed >= nextTimestamp))
                {
                    dataStream.CopyIntoBuffer(raw_buffer, MAX_PDU_SIZE);
                    notifyObservers(nextTimestamp, raw_buffer, dataStream.GetBufferSize());
                    dataStream.Clear();
                    pduSent = true;
                }
                else
                {
                    mutex.unlock();
                    msleep(250);
                    mutex.lock();
                    timePassed = playback_start_time.elapsed() * 0.001;
                }
            }
        }
        mutex.unlock();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}






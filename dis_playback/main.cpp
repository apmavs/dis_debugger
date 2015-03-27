#include <QThread>
#include <iostream>
#include <time.h>
#include "KDIS/Extras/DIS_Logger_Playback.h"
#include "KDIS/Network/Connection.h"

using namespace std;
using namespace KDIS;
using namespace DATA_TYPE;
using namespace PDU;
using namespace ENUMS;
using namespace UTILS;
using namespace NETWORK;

int main(int argc, char *argv[])
{
    cout << "Starting program";
    Connection conn("127.0.0.1", 6993);
    KOCTET cBuffer[MAX_PDU_SIZE]; // Somewhere to store the data we receive.

    // Load all data into memory from the log.
    DIS_Logger_Playback Log( "Log2.txt", 0 );

    KUINT32 uiStartTime = time( NULL );
    KUINT32 ui32Timepassed;
    KBOOL bWaiting;

    try
    {
        cout << "Starting playback loop" << endl;
        while( Log.EndOfLogReached() == false )
        {
            ui32Timepassed = time( NULL ) - uiStartTime;

            // Get the next entry in the log
            KUINT32 ui32Stamp;
            KDataStream stream;
            Log.GetNext( ui32Stamp, stream );

            bWaiting = true;

            // Wait untill it is time to send the PDU
            while( bWaiting )
            {
                if( ui32Stamp <= ui32Timepassed )
                {
                    bWaiting = false;
                    stream.CopyIntoBuffer( cBuffer, MAX_PDU_SIZE );

                    static unsigned int packetNum = 0;
                    packetNum++;
                    cout << "Sending packet #" << packetNum << endl;
                    conn.Send( cBuffer, stream.GetBufferSize() );

                    stream.Clear();
                }
                else
                {
                    ui32Timepassed = time( NULL ) - uiStartTime;
                    QThread::msleep(10);
                }
            }
        }
    }
    catch( exception & e )
    {
        cout << e.what() << endl;
    }

    return 0;
}

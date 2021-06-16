#include <qtphonemes.h>

N::SpeechTunes:: SpeechTunes         (void)
               : QList<SpeechTune *> (    )
{
}

N::SpeechTunes::~SpeechTunes(void)
{
}

void N::SpeechTunes::append(QByteArray & tunes)
{
  int ps       = SpeechTune::packetSize() ;
  int total    = tunes.size() / ps        ;
  int residues = tunes.size() % ps        ;
  if ( residues >  0 ) return             ;
  if ( total    <= 0 ) return             ;
  for (int i=0;i<total;i++)               {
    int index = i * ps                    ;
    SpeechTune * tune                     ;
    tune  = new SpeechTune ( )            ;
    tune -> set ( index , tunes )         ;
    push_back   ( tune          )         ;
  }                                       ;
}

int N::SpeechTunes::Packet(QByteArray & data)
{
  for (int i = 0 ; i < count() ; i++ ) {
    ME[i] -> Packet ( data )           ;
  }                                    ;
  return data.size()                   ;
}

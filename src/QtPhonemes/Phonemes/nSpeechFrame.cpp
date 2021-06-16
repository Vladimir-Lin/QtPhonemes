#include <qtphonemes.h>

// flags set for frames within a spectrum sequence
#define FRFLAG_KLATT           0x01   // this frame includes extra data for Klatt synthesizer
#define FRFLAG_VOWEL_CENTRE    0x02   // centre point of vowel
#define FRFLAG_LEN_MOD         0x04   // reduce effect of length adjustment
#define FRFLAG_BREAK_LF        0x08   // but keep f3 upwards
#define FRFLAG_BREAK           0x10   // don't merge with next frame
#define FRFLAG_BREAK_2         0x18   // FRFLAG_BREAK_LF or FRFLAG_BREAK
#define FRFLAG_FORMANT_RATE    0x20   // Flag5 allow increased rate of change of formant freq
#define FRFLAG_MODULATE        0x40   // Flag6 modulate amplitude of some cycles to give trill
#define FRFLAG_DEFER_WAV       0x80   // Flag7 defer mixing WAV until the next frame
#define FRFLAG_LEN_MOD2      0x4000   // reduce effect of length adjustment, used for the start of a vowel
#define FRFLAG_COPIED        0x8000   // This frame has been copied into temporary rw memory

#define KLATT_AV      0
#define KLATT_FNZ     1    // nasal zero freq
#define KLATT_Tilt    2
#define KLATT_Aspr    3
#define KLATT_Skew    4

#define KLATT_Kopen   5
#define KLATT_AVp     6
#define KLATT_Fric    7
#define KLATT_FricBP  8
#define KLATT_Turb    9

static const short sqrt_tab[200] = {
  0, 64, 90,110,128,143,156,169,181,192,202,212,221,230,239,247,
256,263,271,278,286,293,300,306,313,320,326,332,338,344,350,356,
362,367,373,378,384,389,394,399,404,409,414,419,424,429,434,438,
443,448,452,457,461,465,470,474,478,483,487,491,495,499,503,507,
512,515,519,523,527,531,535,539,543,546,550,554,557,561,565,568,
572,576,579,583,586,590,593,596,600,603,607,610,613,617,620,623,
627,630,633,636,640,643,646,649,652,655,658,662,665,668,671,674,
677,680,683,686,689,692,695,698,701,704,706,709,712,715,718,721,
724,726,729,732,735,738,740,743,746,749,751,754,757,759,762,765,
768,770,773,775,778,781,783,786,789,791,794,796,799,801,804,807,
809,812,814,817,819,822,824,827,829,832,834,836,839,841,844,846,
849,851,853,856,858,861,863,865,868,870,872,875,877,879,882,884,
886,889,891,893,896,898,900,902};

N::SpeechFrame:: SpeechFrame (void)
{
}

N::SpeechFrame:: SpeechFrame(const SpeechFrame & frame)
{
  assign ( frame ) ;
}

N::SpeechFrame::~SpeechFrame(void)
{
}

N::SpeechFrame & N::SpeechFrame::operator = (const SpeechFrame & frame)
{
  return assign(frame) ;
}

N::SpeechFrame & N::SpeechFrame::assign(const SpeechFrame & frame)
{
  nMemberCopy ( frame , frflags )                 ;
  nMemberCopy ( frame , length  )                 ;
  nMemberCopy ( frame , rms     )                 ;
  nMemberCopy ( frame , spare   )                 ;
  ::memcpy(ffreq  ,frame.ffreq  ,sizeof(short)*7) ;
  ::memcpy(fheight,frame.fheight,sizeof(char )*8) ;
  ::memcpy(fwidth ,frame.fwidth ,sizeof(char )*6) ;
  ::memcpy(fright ,frame.fright ,sizeof(char )*3) ;
  ::memcpy(bw     ,frame.bw     ,sizeof(char )*4) ;
  ::memcpy(klattp ,frame.klattp ,sizeof(char )*5) ;
  ::memcpy(klattp2,frame.klattp2,sizeof(char )*5) ;
  ::memcpy(klattap,frame.klattap,sizeof(char )*7) ;
  ::memcpy(klattbp,frame.klattbp,sizeof(char )*7) ;
  return ME                                       ;
}

N::SpeechFrame & N::SpeechFrame::copy(const SpeechFrame & frame,bool Copy)
{
  bool dropOut = false                                  ;
  if ( Copy )                                           {
    dropOut = ( ( frame.frflags & FRFLAG_COPIED ) > 0 ) ;
  }                                                     ;
  assign ( frame )                                      ;
  if ( dropOut ) return ME                              ;
  length   = 0                                          ;
  frflags |= FRFLAG_COPIED                              ;
  return ME                                             ;
}

int N::SpeechFrame::VowelCloseness(void)
{ // return a value 0-3 depending on the vowel's f1
  int f1 ;
  if ( ( f1 = ffreq[1] ) < 300 ) return 3 ;
  if (   f1 < 400              ) return 2 ;
  if (   f1 < 500              ) return 1 ;
  return 0                                ;
}

void N::SpeechFrame::setRMS(VoicePacket & voice,int RMS)
{
  // Each frame includes its RMS amplitude value, so to set a new
  // RMS just adjust the formant amplitudes by the appropriate ratio
  int x                           ;
  int h                           ;
  int ix                          ;
  /////////////////////////////////
  if (voice.KlattV[0])            {
    if (RMS==-1)                  {
      klattap[KLATT_AV] = 50      ;
    }                             ;
    return                        ;
  }                               ;
  if (rms==0) return              ;
  /////////////////////////////////
  x = ( RMS * 64 ) / rms          ;
  if ( x >= 200 ) x = 199         ;
  x = sqrt_tab[x]                 ;
  /////////////////////////////////
  for ( ix = 0 ; ix < 8 ; ix++ )  {
    h           = fheight[ix] * x ;
    fheight[ix] = h / 0x200       ;
  }                               ;
}

void N::SpeechFrame::FormantsReduceHF(VoicePacket & voice,int level)
{ //  change height of peaks 2 to 8, percentage
  int ix                              ;
  int x                               ;
  if ( voice.KlattV[0] ) return       ;
  for ( ix = 2 ; ix < 8 ; ix++ )      {
    x           = fheight[ix] * level ;
    fheight[ix] = x           / 100   ;
  }                                   ;
}

void N::SpeechFrame::AdjustFormants (
       VoicePacket & voice          ,
       int           target         ,
       int           min            ,
       int           max            ,
       int           F1             ,
       int           F3             ,
       int           ReduceHF       ,
       int           Flags          )
{
  int x                            ;
  target *= voice.FormantFactor    ;
  target /= 256                    ;
  x       = target - ffreq[2]      ;
  x      /= 2                      ;
  if ( x > max ) x = max           ;
  if ( x < min ) x = min           ;
  ffreq[2] += x                    ;
  ffreq[3] += F3                   ;
  //////////////////////////////////
  if (Flags & 0x20) F3 = -F3       ; //. reverse direction for f4,f5 change
  ffreq[4] += F3                   ;
  ffreq[5] += F3                   ;
  //////////////////////////////////
  if ( F1 == 1 )                   {
    x = ( 235 - ffreq[1] )         ;
    if ( x < -100 ) x = -100       ;
    if ( x > -60  ) x =  -60       ;
    ffreq[1] += x                  ;
  }                                ;
  //////////////////////////////////
  if ( F1 == 2 )                   {
    x = ( 235 - ffreq[1] )         ;
    if ( x < -300 ) x = -300       ;
    if ( x > -150 ) x = -150       ;
    ffreq[1] += x                  ;
    ffreq[0] += x                  ;
  }                                ;
  //////////////////////////////////
  if ( F1 == 3 )                   {
    x = ( 100 - ffreq[1] )         ;
    if(x < -400) x = -400          ;
    if(x > -300) x = -400          ;
    ffreq[1] += x                  ;
    ffreq[0] += x                  ;
  }                                ;
  FormantsReduceHF(voice,ReduceHF) ;
}

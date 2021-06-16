#include <qtphonemes.h>

N::WaveSynthesis:: WaveSynthesis      (void )
                 : EchoLength         (0    )
                 , EchoHead           (0    )
                 , EchoTail           (0    )
                 , EchoAmplitude      (0    )
                 , Amplitude          (60   )
                 , Voicing            (0    )
                 , SampleRate         (22050)
                 , Samples            (0    )
                 , SampleCount        (0    )
                 , SampleIndex        (0    )
                 , WavePhase          (0    )
                 , ConsonantAmplitude (0    )
{
}

N::WaveSynthesis:: WaveSynthesis(const WaveSynthesis & synthesis)
{
  assign ( synthesis ) ;
}

N::WaveSynthesis::~WaveSynthesis(void)
{
}

N::WaveSynthesis & N::WaveSynthesis::assign(const WaveSynthesis & synthesis)
{
  return ME ;
}

N::WaveSynthesis & N::WaveSynthesis::operator = (const WaveSynthesis & synthesis)
{
  return assign ( synthesis ) ;
}

int N::WaveSynthesis::Restraint(int value,int max)
{
  if ( value < 0   ) return 0   ;
  if ( value > max ) return max ;
  return value                  ;
}

void N::WaveSynthesis::EmptyEcho(void)
{
  ::memset ( Echo , 0 , sizeof(short) * 5500 ) ;
  EchoTail = 0                                 ;
}

void N::WaveSynthesis::SetEcho(VoicePacket & voice,int embedH,int amplitude)
{
  int delay = voice.EchoDelay                ;
  int amp   = voice.EchoAmp                  ;
  Voicing   = voice.Voicing                  ;
  ////////////////////////////////////////////
  if ( delay >= 5500 ) delay = 5499          ;
  if ( amp   >  100  ) amp   =  100          ;
  EmptyEcho ( )                              ;
  ////////////////////////////////////////////
  if ( embedH > 0 )                          {
    amp   = embedH                           ;
    delay = 130                              ;
  }                                          ;
  if ( delay == 0 ) amp = 0                  ;
  ////////////////////////////////////////////
  EchoHead   = ( delay * SampleRate ) / 1000 ;
  EchoLength = EchoHead                      ;
  // ensure completion of echo at the end of speech. Use 1 delay period?
  if ( amp ==  0 ) EchoLength = 0            ;
  if ( amp >  20 ) EchoLength = EchoHead * 2 ; // perhaps allow 2 echo periods if the echo is loud.
  // echo_amp units are 1/256ths of the amplitude of the original sound.
  EchoAmplitude = amp                        ;
  // compensate (partially) for increase in amplitude due to echo
  Amplitude = amplitude                      ;
  Amplitude = ((Amplitude * (500-amp))/500)  ;
}

bool N::WaveSynthesis::PlaySilence (
       QByteArray & buffer       ,
       int        & start        ,
       int          length       ,
       int          resume       )
{
  int     value = 0                             ;
  int     bmax  = buffer.size() / sizeof(short) ;
  short * d     = (short *)buffer.data()        ;
  ///////////////////////////////////////////////
  Samples     = 0                               ;
  SampleCount = 0                               ;
  WavePhase   = 0x7fffffff                      ;
  if ( length == 0 ) return false               ;
  if ( resume == 0 ) SampleIndex = length       ;
  ///////////////////////////////////////////////
  while ( SampleIndex-- > 0 )                   {
    value   = Echo[EchoTail]                    ;
    value  *= EchoAmplitude                     ;
    value >>= 8                                 ;
    EchoTail++                                  ;
    if ( EchoTail >= 5500 ) EchoTail = 0        ;
    d    [ start    ] = value                   ;
    start    ++                                 ;
    Echo [ EchoHead ] = value                   ;
    EchoHead ++                                 ;
    if ( EchoHead >= 5500 ) EchoHead = 0        ;
    if ( start >= bmax ) return true            ;
  }                                             ;
  return false                                  ;
}

bool N::WaveSynthesis::PlayWave (
       QByteArray & buffer    ,
       int        & start     ,
       int          length    ,
       int          resume    ,
       QByteArray & data      ,
       int        & index     ,
       int          scale     ,
       int          amp       )
{
  short       * d     = (short       *)buffer . data ( )   ;
  short       * s     = (short       *)data   . data ( )   ;
  signed char * b     = (signed char *)data   . data ( )   ;
  int           bmax  = buffer.size() / sizeof(short)      ;
  int           dmax  = data  .size() / sizeof(short)      ;
  int           value                                      ;
  //////////////////////////////////////////////////////////
  if ( scale  >  0 ) dmax = data.size()                    ;
  if ( resume == 0 )                                       {
    SampleIndex = length                                   ;
    index       = 0                                        ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  Samples     = 0                                          ;
  SampleCount = 0                                          ;
  //////////////////////////////////////////////////////////
  while ( SampleIndex-- > 0)                               {
    if ( index >= dmax ) return true                       ;
    if ( scale == 0 )                                      { // 16 bits data
      value = s[index]                                     ;
    } else                                                 {
      // 8 bit data, shift by the specified scale factor
      value = (signed char)b[index] * scale                ;
    }                                                      ;
    index++                                                ;
    value  *= ConsonantAmplitude                           ;
    value  *= Amplitude                                    ; // reduce strength of consonant
    value   = value >> 10                                  ;
    value  *= amp                                          ;
    value >>= 5                                            ;
    value  += ( ( Echo[EchoTail++] * EchoAmplitude ) >> 8) ;
    ////////////////////////////////////////////////////////
    if ( value    >   32767 ) value    =  32768       ; else
    if ( value    <  -32768 ) value    = -32768            ;
    if ( EchoTail >=   5500 ) EchoTail = 0                 ;
    d [ start ] = (short)value                             ;
    start++                                                ;
    Echo[EchoHead++] = ( value * 3 ) / 4                   ;
    if ( EchoHead >= 5500 ) EchoHead = 0                   ;
    if ( start >= bmax ) return true                       ;
  }                                                        ;
  return false                                             ;
}

/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_PHONEMES_H
#define QT_PHONEMES_H

#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <Essentials>
#include <Mathematics>
#include <QtFFmpeg>
#include <QtAudio>
#include <AudioIO>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTPHONEMES_LIB)
#      define Q_PHONEMES_EXPORT Q_DECL_EXPORT
#    else
#      define Q_PHONEMES_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_PHONEMES_EXPORT
#endif

namespace N {

class Q_PHONEMES_EXPORT VoicePacket         ;
class Q_PHONEMES_EXPORT Resonator           ;
class Q_PHONEMES_EXPORT WavePeaks           ;
class Q_PHONEMES_EXPORT KlattPeak           ;
class Q_PHONEMES_EXPORT KlattFrame          ;
class Q_PHONEMES_EXPORT SonicPitch          ;
class Q_PHONEMES_EXPORT WaveGenerator       ;
class Q_PHONEMES_EXPORT WaveSynthesis       ;
class Q_PHONEMES_EXPORT Klatt               ;
class Q_PHONEMES_EXPORT Syllable            ;
class Q_PHONEMES_EXPORT Syllables           ;
class Q_PHONEMES_EXPORT Phoneme             ;
class Q_PHONEMES_EXPORT Phonemes            ;
class Q_PHONEMES_EXPORT Synthesizer         ;
class Q_PHONEMES_EXPORT PhonemeReplacement  ;
class Q_PHONEMES_EXPORT PhonemeReplacements ;
class Q_PHONEMES_EXPORT SpeechFrame         ;
class Q_PHONEMES_EXPORT SpeechFrames        ;
class Q_PHONEMES_EXPORT SpeechReference     ;
class Q_PHONEMES_EXPORT SpectrumSequence    ;
class Q_PHONEMES_EXPORT PhonemeList         ;
class Q_PHONEMES_EXPORT PhonemeLists        ;
class Q_PHONEMES_EXPORT PhonemeData         ;
class Q_PHONEMES_EXPORT FormantParament     ;
class Q_PHONEMES_EXPORT FormantParaments    ;
class Q_PHONEMES_EXPORT MbrolaTable         ;
class Q_PHONEMES_EXPORT SpeechFactors       ;
class Q_PHONEMES_EXPORT SpeechTune          ;
class Q_PHONEMES_EXPORT SpeechTunes         ;
class Q_PHONEMES_EXPORT SpeechWords         ;
class Q_PHONEMES_EXPORT SpeechMatch         ;
class Q_PHONEMES_EXPORT VoiceFeature        ;
class Q_PHONEMES_EXPORT ParamentStack       ;
class Q_PHONEMES_EXPORT ParamentStacks      ;
class Q_PHONEMES_EXPORT LanguageOptions     ;
class Q_PHONEMES_EXPORT SpeechTranslator    ;

namespace Acoustics
{

Q_PHONEMES_EXPORT extern short NaturalSamples [ 100 ] ;
Q_PHONEMES_EXPORT extern short FormantHz      [  10 ] ;
Q_PHONEMES_EXPORT extern short Bandwidth      [  10 ] ;
Q_PHONEMES_EXPORT extern short ParallelAmp    [  10 ] ;
Q_PHONEMES_EXPORT extern short ParallelBW     [  10 ] ;

Q_PHONEMES_EXPORT bool    DefaultIntonations (QByteArray & Intonation) ;
Q_PHONEMES_EXPORT bool    DefaultPhonemes    (QByteArray & phoneme) ;
Q_PHONEMES_EXPORT QString Name               (Speak::SynthesisTables table,int     value) ;
Q_PHONEMES_EXPORT int     Lookup             (Speak::SynthesisTables table,QString name ) ;
Q_PHONEMES_EXPORT int     BuildPhonemes      (QList<Phonemes> & phonemes,QByteArray & data) ;
}

class Q_PHONEMES_EXPORT VoicePacket
{
  public:

    QString Name              ;

    int     PhonemeTabIX      ; // phoneme table number
    int     PitchBase         ; // Hz<<12
    int     PitchRange        ; // standard = 0x1000

    int     SpeedF1           ;
    int     SpeedF2           ;
    int     SpeedF3           ;

    int     SpeedPercent      ; // adjust the WPM speed by this percentage
    int     Flutter           ;
    int     Roughness         ;
    int     EchoDelay         ;
    int     EchoAmp           ;
    int     nHarmonicPeaks    ; // highest formant which is formed from adding harmonics
    int     PeakShape         ; // alternative shape for formant peaks (0=standard 1=squarer)
    int     Voicing           ; // 100% = 64, level of formant-synthesized sound
    int     FormantFactor     ; // adjust nominal formant frequencies by this  because of the voice's pitch (256ths)
    int     ConsonantAmp      ; // amplitude of unvoiced consonants
    int     ConsonantAmpv     ; // amplitude of the noise component of voiced consonants
    int     SampleRate        ;
    int     KlattV      [ 8 ] ;
    int     FormantRate [ 9 ] ;

    // parameters used by Wavegen
    short   Freq    [Speak::Peaks] ; // 100% = 256
    short   Height  [Speak::Peaks] ; // 100% = 256
    short   Width   [Speak::Peaks] ; // 100% = 256
    short   FreqAdd [Speak::Peaks] ; // Hz

    // copies without temporary adjustments from embedded commands
    short   Freq2   [Speak::Peaks] ; // 100% = 256
    short   Height2 [Speak::Peaks] ; // 100% = 256
    short   Width2  [Speak::Peaks] ; // 100% = 256

    int     Breath  [Speak::Peaks] ; // amount of breath for each formant. breath[0] indicates whether any are set.
    int     Breathw [Speak::Peaks] ; // width of each breath formant

    // This table provides the opportunity for tone control.
    // Adjustment of harmonic amplitudes, steps of 8Hz
    // value of 128 means no change
    unsigned char ToneAdjust [Speak::ToneAdjust] ; //  8Hz steps * 1000 = 8kHz

    explicit VoicePacket      (void) ;
    virtual ~VoicePacket      (void) ;

    void     Reset            (SpeechFactors & factor,int sampleRate) ;
    void     setFormant       (QString formant) ;
    void     setPitch         (QString pitch) ;
    void     setToneAdjust    (int * tonePts) ;
    void     setPitchFormants (int Values[16]) ;
    int      ReadTonePoints   (char * string,int * tonePts) ;

  protected:

  private:

};

/*****************************************************************************\
 *                                                                           *
 * Klatt Synthesizer : Resonator                                             *
 *                                                                           *
 * Some of function namings in this class make completely no sense to anyone *
 * and these functions originally have no name nor any kind of expressions.  *
 * Therefore, please read the explains aside the function before you use it. *
 *                                                                           *
\*****************************************************************************/

class Q_PHONEMES_EXPORT Resonator
{
  public:

    double p1 ; // in wave generator p1 is replaced by x1 , we will use p1 in the end
    double p2 ; // in wave generator p2 is replaced by x2 , we will use p2 in the end
    double A  ;
    double B  ;
    double C  ;
    double dA ;
    double dB ;
    double dC ;

    explicit    Resonator  (void) ;
    virtual    ~Resonator  (void) ;

    Resonator & operator = (Resonator & resonator) ;

    // N += dN , N = A,B,C
    void        dN         (void) ;
     // Mix(input) = ( A * input ) + ( B * p1 ) + ( C * p2 )
    double      Mix        (double input) ;
    double      F1         (double input) ; // call Mix with p1 = result
    double      F2         (double input) ; // call F1 with dN
    double      R1         (double input) ; // call Mix with p1 = input
    double      R2         (double input) ; // call R1 with dN

    // A = 1.0 - B - C
    void        E1         (void) ;
    // convert to antiresonator coefficients (a'=1/a, b'=-b/a, c'=-c/a)
    void        E2         (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT WavePeaks
{
  public:

    int    Freq      ; // Hz     << 16
    int    H         ; // height << 15
    int    L         ; // Hz     << 16
    int    R         ; // Hz     << 16
    double Frequency ; // floating point versions of the above
    double Height    ;
    double Left      ;
    double Right     ;
    double dF        ;    // increment by this every 64 samples
    double dH        ;
    double dL        ;
    double dR        ;

    explicit WavePeaks(void) ;
    virtual ~WavePeaks(void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT KlattPeak
{
  public:

    int    Freq      ; // Hz : (int)Frequency
    int    BW        ; // Klatt bandwidth : (int)Bandwidth
    int    Amp       ; // Parallel amplitude : (int)Amplitude
    int    BP        ; // Parallel bandwidth : (int)Parallel
    double Frequency ; // Hz
    double Bandwidth ; // Klatt bandwidth
    double Amplitude ; // Parallel amplitude
    double Parallel  ; // Parallel bandwidth
    double dF        ; // increment by this every 64 samples
    double dB        ; // incremental for Klatt bandwidth
    double dA        ; // incremental for Amplitude
    double dP        ; // incremental for Parallel bandwidth

    explicit KlattPeak      (void) ;
    virtual ~KlattPeak      (void) ;

    KlattPeak & operator ++ (void) ;
    KlattPeak & operator  = (KlattPeak & peak) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT KlattFrame
{
  public:

    enum Nasal   {
      F_NZ = 0   , // Nasal zero formant
      F1   = 1   ,
      F2   = 2   ,
      F3   = 3   ,
      F4   = 4   ,
      F5   = 5   ,
      F6   = 6   ,
      F_NP = 9     // Nasal pole formant
    } ;

    int F0hz10       ; // Voicing fund freq in Hz
    int AVdb         ; // Amp of voicing in dB           :  0 to   70
    int tmpAVdb      ; // copy of AVdb

    int ASP          ; // Amp of aspiration in dB        :  0 to   70
    int Kopen        ; // # of samples in open period    : 10 to   65
    int Aturb        ; // Breathiness in voicing         :  0 to   80
    int TLTdb        ; // Voicing spectral tilt in dB    :  0 to   24
    int AF           ; // Amp of frication in dB         :  0 to   80
    int Kskew        ; // Skewness of alternate periods  :  0 to   40 in sample#/2

    int AB           ; // Amp of bypass fric. in dB      :  0 to   80
    int AVpdb        ; // Amp of voicing,  par in dB,    :  0 to   70
    int Gain0        ; // Overall gain, 60 dB is unity,  :  0 to   60

    int Fhz     [10] ; // formant Hz, F_NZ to F6 to F_NP
    int Bhz     [10] ;

    int nextFhz [10] ; // Fhz for the next chunk
    int nextBhz [10] ;

    int Ap      [10] ; // Amp of parallel formants in dB :  0 to   80
    int Bphz    [10] ; // Parallel formants bw in Hz     : 40 to 1000

    explicit KlattFrame     (void) ;
    virtual ~KlattFrame     (void) ;

    KlattFrame & operator = (KlattFrame & frame) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SonicPitch
{
  public:

    short * inputBuffer          ;
    short * outputBuffer         ;
    short * pitchBuffer          ;
    short * downSampleBuffer     ;
    float   speed                ;
    float   volume               ;
    float   pitch                ;
    int     numChannels          ;
    int     inputBufferSize      ;
    int     pitchBufferSize      ;
    int     outputBufferSize     ;
    int     numInputSamples      ;
    int     numOutputSamples     ;
    int     numPitchSamples      ;
    int     minPeriod            ;
    int     maxPeriod            ;
    int     maxRequired          ;
    int     remainingInputToCopy ;
    int     sampleRate           ;
    int     prevPeriod           ;
    int     prevMaxDiff          ;
    int     prevMinDiff          ;

    explicit SonicPitch (int SampleRate,int NumChannels) ;
    explicit SonicPitch (void) ;
    virtual ~SonicPitch (void) ;

    bool     Set        (int     SampleRate,int NumChannels) ;
    void     Scale      (short * samples   ,int numSamples,float volume) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT WaveGenerator
{
  public:

    unsigned char * PitchEnv          ;

    int             Pitch             ; // pitch Hz*256
    int             PitchIndex        ; // index into pitch envelope (*256)
    int             dPitch            ; // increment to pitch_ix
    int             PitchBase         ; // Hz*256 low, before modified by envelope
    int             PitchRange        ; // Hz*256 range of envelope

    unsigned char * MixWaveFile       ; // wave file to be added to synthesis
    int             nMixWaveFile      ; // length in bytes
    int             MixWaveScale      ; // 0=2 byte samples
    int             MixWaveAmp        ;
    int             MixWaveFileIndex  ;
    int             MixWaveFileMax    ; // length of available WAV data (in bytes)
    int             MixWaveFileOffset ;

    int             Amplitude         ;
    int             AmplitudeV        ;
    int             AmplitudeFormant  ; // percentage amplitude adjustment for formant synthesis

    explicit WaveGenerator     (void) ;
             WaveGenerator     (const WaveGenerator & generator) ;
    virtual ~WaveGenerator     (void) ;

    WaveGenerator & assign     (const WaveGenerator & generator) ;
    WaveGenerator & operator = (const WaveGenerator & generator) ;

    void setPitch              (KlattFrame & frame) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT WaveSynthesis
{
  public:

    short Echo [ 5500 ]      ;
    int   EchoLength         ;
    int   EchoHead           ;
    int   EchoTail           ;
    int   EchoAmplitude      ;
    int   Amplitude          ;
    int   Voicing            ;
    int   SampleRate         ;
    int   Samples            ;
    int   SampleCount        ;
    int   SampleIndex        ;
    int   WavePhase          ;
    int   ConsonantAmplitude ;

    explicit WaveSynthesis      (void) ;
             WaveSynthesis      (const WaveSynthesis & synthesis) ;
    virtual ~WaveSynthesis      (void) ;

    WaveSynthesis & assign      (const WaveSynthesis & synthesis) ;
    WaveSynthesis & operator =  (const WaveSynthesis & synthesis) ;

    int             Restraint   (int value, int max) ;
    void            EmptyEcho   (void) ;
    void            SetEcho     (VoicePacket & voice       ,
                                 int           embedH      ,
                                 int           amplitude ) ;
    bool            PlaySilence (QByteArray  & buffer   ,
                                 int         & start    ,
                                 int           length   ,
                                 int           resume ) ;
    bool            PlayWave    (QByteArray  & buffer   ,
                                 int         & start    ,
                                 int           length   ,
                                 int           resume   ,
                                 QByteArray  & data     ,
                                 int         & index    ,
                                 int           scale    ,
                                 int           amp    ) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Klatt
{
  public: // Klatt synthesizer

    enum PeakIndex     {
      Rnz       =  0   , // Nasal zero, anti-resonator
      R1c       =  1   ,
      R2c       =  2   ,
      R3c       =  3   ,
      R4c       =  4   ,
      R5c       =  5   ,
      R6c       =  6   ,
      R7c       =  7   ,
      R8c       =  8   ,
      Rnpc      =  9   , // Nasal pole
      Rparallel = 10   ,
      Rnpp      = 10   ,
      R1p       = 11   ,
      R2p       = 12   ,
      R3p       = 13   ,
      R4p       = 14   ,
      R5p       = 15   ,
      R6p       = 16   ,
      RGL       = 17   ,
      RLP       = 18   ,
      Rout      = 19   ,
      N_RSN     = 20 } ;

    Speak::SynthesisModel Model                  ; // cascade-parallel or all-parallel
    Speak::Glottals       Glottis                ; // Type of glottal source
    int                   SampleRate             ; // Number of output samples per second
    int                   FlutterF0              ; // Percentage of f0 flutter 0-100
    int                   OriginalF0             ; // original value of f0 not modified by flutter
    int                   FlutterCount           ;
    int                   T0                     ; // Fundamental period in output samples times 4
    int                   nPer                   ; // Counter for number of samples in a pitch period
    int                   nOpen                  ; // Number of samples in open phase of period
    int                   nModul                 ; // Position in period to begin noise amp. modul
    int                   SampleCount            ;
    int                   OutSL                  ; // Output waveform selector
    int                   FLPhz                  ; // Frequeny of glottal downsample low-pass filter
    int                   BLPhz                  ; // Bandwidth of glottal downsample low-pass filter
    int                   nSPFr                  ; // number of samples per frame
    int                   ns                     ;
    int                   nRand                  ; // Varible used by random number generator
    int                   Skewness               ;
    double                nLast                  ;
    double                NoiseFactor            ;
    double                NaturalFactor          ; // 0.028
    double                vWave                  ;
    double                KlattNoise             ;
    double                KlattVoice             ;
    double                KlattSource            ;
    double                vLast                  ;
    double                GlotLast               ;
    double                PulseShapeA            ; // Makes waveshape of glottal pulse when open
    double                PulseShapeB            ; // Makes waveshape of glottal pulse when open
    double                MinusPiT               ;
    double                TwoPiT                 ;
    double                OneMd                  ;
    double                Decay                  ;
    double                AmpBypas               ; // AB converted to linear gain
    double                AmpVoice               ; // AVdb converted to linear gain
    double                ParAmpVoice            ; // AVpdb converted to linear gain
    double                AmpAspir               ; // AP converted to linear gain
    double                AmpFrica               ; // AF converted to linear gain
    double                AmpBreth               ; // ATURB converted to linear gain
    double                AmpGain0               ; // G0 converted to linear gain

    int                   NumSamples             ; // number of glottal samples
    double                SampleFactor           ; // multiplication factor for glottal samples
    short *               NaturalSamples         ; // pointer to an array of glottal samples
    int                   fadeout                ; // set to 64 to cause fadeout over 64 samples
    int                   ScaleWave              ; // depends on the voicing source

    Resonator             Rsn     [N_RSN       ] ; // internal storage for resonators
    Resonator             NextRsn [N_RSN       ] ;
    Resonator             Breath  [Speak::Peaks] ;

    KlattPeak             Peaks   [Speak::Peaks] ;
    KlattFrame            Frame                  ;

    explicit Klatt           (void) ;
    virtual ~Klatt           (void) ;

    double   Noise           (void) ;

    double   Impulsive       (void) ;
    double   Natural         (void) ;
    double   Sampled         (void) ;

    void     Set             (int frequency,int bandwidth,Resonator & resonator,bool initialize = false) ;
    void     Zero            (int frequency,int bandwidth,Resonator & resonator,bool initialize = false) ;
    void     SetBreath       (int frequency,int bandwidth,bool initialize = true) ;

    void     dRsn            (int index) ;
    double   ChainResonation (double input) ;

    void     Flutter         (KlattFrame & frame) ;
    void     ResetPitch      (KlattFrame & frame) ;
    int      Waveform        (KlattFrame & frame,WaveGenerator & generator) ;
    void     Initialize      (KlattFrame & frame) ;

    void     Reset           (int control) ;
    void     Initialize      (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Syllable
{
  public:

    char          Stress      ;
    char          Envelope    ;
    char          Flags       ;
    char          NextPhoneme ; // next phoneme type
    unsigned char Pitch1      ;
    unsigned char Pitch2      ;

    explicit Syllable     (void) ;
             Syllable     (const Syllable & syllable) ;
    virtual ~Syllable     (void) ;

    Syllable & assign     (const Syllable & syllable) ;
    Syllable & operator = (const Syllable & syllable) ;

    void setPitch         (int base,int drop) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Syllables : public QList<Syllable *>
{
  public:

    int  PitchEnvelope ;
    int  Previous      ;
    int  Body          ;
    int  Tail          ;
    int  LastPrimary   ;
    int  TonePosition  ;
    int  TonePosition2 ;
    bool NoTonic       ;

    explicit Syllables    (void) ;
    virtual ~Syllables    (void) ;

    void PitchVowels      (int Start,int End,int ClauseEnd) ;
    void PitchVowels      (RangeInt range   ,int ClauseEnd) ;
    int  Increments       (int Start,int End,int MinStress) ;
    int  Increments       (RangeInt range   ,int MinStress) ;
    int  Unstressed       (int Start,int End,int Limit    ) ;
    int  Unstressed       (RangeInt range   ,int Limit    ) ;
    void setPitchGradient (int Start,int End,int pitchStart,int pitchEnd) ;
    void setPitchGradient (RangeInt rRange   ,RangeInt pitch) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Phoneme
{
  public:

    unsigned int   Mnemonic  ; // Up to 4 characters.  The first char is in the l.s.byte
    unsigned int   Flags     ; // bits 16-19 place of articulation
    unsigned short Program   ; // index into phondata file
    unsigned char  Code      ; // the phoneme number
    unsigned char  Type      ; // Speak::PhonemeTypes
    unsigned char  StartType ; // Speak::PhonemeTypes
    unsigned char  EndType   ; // Speak::PhonemeTypes
    unsigned char  Length    ; // for vowels, in mS/2;  for phSTRESS phonemes, this is the stress/tone type
    unsigned char  LengthMod ; // a length_mod group number, used to access length_mod_tab

    explicit Phoneme            (void) ;
             Phoneme            (const Phoneme & phoneme) ;
    virtual ~Phoneme            (void) ;

    Phoneme &    assign         (const Phoneme & phoneme) ;
    Phoneme &    operator =     (const Phoneme & phoneme) ;

    static int   packetSize     (void) ;

    QString      MnemonicString (void) ;

    bool         is             (unsigned int mnemonic) ;
    bool         isPause        (void) ;
    bool         isUnstressed   (void) ;

    int          set            (int index,QByteArray & data) ;
    int          Packet         (QByteArray & data) ;

    virtual void report         (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Phonemes : public QMap<int,Phoneme *>
{
  public:

    QString Name        ;
    int     Includes    ;
    int     Equivalence ;

    explicit Phonemes      (void) ;
    virtual ~Phonemes      (void) ;

    Phonemes & operator += (const Phonemes & phoneme) ;

    bool is                (QString name) ;
    int  Code              (unsigned int Mnemonic) ;
    int  Code              (char a,char b) ;
    int  Lookup            (QString code) ;
    int  set               (int index,QByteArray & data) ;
    int  Packet            (QByteArray & data) ;
    int  Syllables         (unsigned char * phonemes) ; // count syllables
    int  toMnemonics       (const char * phonemes ,char * mnemonics) ;
    int  toPhonemes        (const char * mnemonics,char * phonemes,unsigned char * bad) ;
    void append            (SpeechTranslator & translator ,
                            const char       * phoneme    ,
                            char             * string     ,
                            int                size     ) ;
    QString      toString  (unsigned int mnemonic) ;
    unsigned int toWord    (const char * string) ;

    virtual void report    (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT PhonemeList
{
  public:

    unsigned short synthflags  ;
    unsigned char  phcode      ;
    unsigned char  stresslevel ;
    unsigned short sourceix    ; // ix into the original source text string, only set at the start of a word
    unsigned char  wordstress  ; // the highest level stress in this word
    unsigned char  tone_ph     ; // tone phoneme to use with this vowel
    ////////////////////////////
    Phoneme      * ph          ;
    short          length      ; // length_mod
    unsigned char  envelope    ; // pitch envelope number
    unsigned char  type        ;
    unsigned char  prepause    ;
    unsigned char  postpause   ;
    unsigned char  amp         ;
    unsigned char  newword     ; // bit 0=start of word, bit 1=end of clause, bit 2=start of sentence
    unsigned char  pitch1      ;
    unsigned char  pitch2      ;

    explicit PhonemeList     (void) ;
             PhonemeList     (const PhonemeList & list) ;
    virtual ~PhonemeList     (void) ;

    PhonemeList & operator = (const PhonemeList & list) ;
    PhonemeList & assign     (const PhonemeList & list) ;

    void setPhonemeCode      (unsigned char code) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT PhonemeLists : public QList<PhonemeList *>
{
  public:

    explicit PhonemeLists (void) ;
             PhonemeLists (int total) ;
    virtual ~PhonemeLists (void) ;

    int  Substitute       (PhonemeLists        & list             ,
                           Phonemes            & phonemes         ,
                           PhonemeReplacements & replacements   ) ;
    void Make             (SpeechTranslator    & translator       ,
                           Phonemes            & phonemes         ,
                           int                   post_pause       ,
                           int                   start_sentence ) ;
    int  VowelPosition    (int index) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT PhonemeData
{
  public:

    int  pd_control            ;
    int  pd_param         [16] ; // set from group 0 instructions
    int  sound_addr       [ 5] ;
    int  sound_param      [ 5] ;
    int  vowel_transition [ 4] ;
    int  pitch_env             ;
    int  amp_env               ;
    char ipa_string       [18] ;

    explicit PhonemeData     (void) ;
             PhonemeData     (const PhonemeData & data) ;
    virtual ~PhonemeData     (void) ;

    PhonemeData & operator = (const PhonemeData & data) ;
    PhonemeData & assign     (const PhonemeData & data) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechWords
{
  public:

    unsigned int   flags     ;
    unsigned short start     ;
    unsigned char  pre_pause ;
    unsigned char  wmark     ;
    unsigned short sourceix  ;
    unsigned char  length    ;

    explicit SpeechWords     (void) ;
             SpeechWords     (const SpeechWords & words) ;
    virtual ~SpeechWords     (void) ;

    SpeechWords & operator = (const SpeechWords & words) ;
    SpeechWords & assign     (const SpeechWords & words) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechMatch
{
  public:

    int          points   ;
    const char * phonemes ;
    int          end_type ;
    char       * del_fwd  ;

    explicit SpeechMatch     (void) ;
             SpeechMatch     (const SpeechMatch & match) ;
    virtual ~SpeechMatch     (void) ;

    SpeechMatch & operator = (const SpeechMatch & match) ;
    SpeechMatch & assign     (const SpeechMatch & match) ;

  protected:

  private:

};

// MaxLanguageOptions originally was N_LOPTS
#define MaxLanguageOptions 21

class Q_PHONEMES_EXPORT LanguageOptions
{
  public:

    int                  Id                            ;
    int                  wordGap                       ;
    int                  vowelPause                    ;
    int                  stressRule                    ; // 1=first syllable, 2=penultimate,  3=last
    int                  stressFlags                   ;
    int                  unstressedWD1                 ; // stress for $u word of 1 syllable
    int                  unstressedWD2                 ; // stress for $u word of >1 syllable
    int                  numbers                       ;
    int                  numbers2                      ;
    int                  breakNumbers                  ; // which digits to break the number into thousands, millions, etc (Hindi has 100,000 not 1,000,000)
    int                  maxRoman                      ;
    int                  minRoman                      ;
    int                  thousandsSep                  ;
    int                  decimalSep                    ;
    int                  maxDigits                     ; // max number of digits which can be spoken as an integer number (rather than individual digits)
    int                  accents                       ;
    int                  toneLanguage                  ; // 1=tone language
    int                  intonationGroup               ;
    int                  longStop                      ; // extra mS pause for a lengthened stop
    int                  phonemeChange                 ; // TEST, change phonemes, after translation
    int                  testing                       ; // testing options: bit 1= specify stressed syllable in the form:  "outdoor/2"
    int                  listx                         ; // compile *_listx after *list
    int                  ourAlphabet                   ; // offset for main alphabet (if not set in letter_bits_offset)
    int                  altAlphabet                   ; // offset for another language to recognize
    int                  altAlphabetLanguage           ; // language for the alt_alphabet
    int                  maxLengthMod                  ;
    int                  lengthenTonic                 ; // lengthen the tonic syllable
    int                  suffixAddE                    ; // replace a suffix (which has the SUFX_E flag) with this character
    char                 maxInitialConsonants          ;
    char                 spellingStress                ; // 0=default, 1=stress first letter
    char                 toneNumbers                   ;
    char                 ideographs                    ; // treat as separate words
    char                 textmode                      ; // the meaning of FLAG_TEXTMODE is reversed (to save data when *_list file is compiled)
    char                 dotlessI                      ; // uses letter U+0131
    int                  param  [ MaxLanguageOptions ] ;
    int                  param2 [ MaxLanguageOptions ] ;
    unsigned char        tunes  [ 6                  ] ;
    char                 asciiLanguage [ 8 ] ; // switch to this language for Latin characters
    unsigned char      * lengthMods          ;
    unsigned char      * lengthMods0         ;
    const char         * ordinalIndicator    ; // UTF-8 string
    const unsigned int * replaceChars        ; // characters to be substitutes

    explicit LanguageOptions     (void) ;
             LanguageOptions     (const LanguageOptions & options) ;
    virtual ~LanguageOptions     (void) ;

    LanguageOptions & assign     (const LanguageOptions & options) ;
    LanguageOptions & operator = (const LanguageOptions & options) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechTranslator
{
  public:

    LanguageOptions        options               ;
    Phonemes               phonemes              ;
    int                    translatorName        ;
    int                    transposeMax          ;
    int                    transposeMin          ;
    int                    phonemesRepeatCount   ;
    int                    phonemeTabIndex       ;
    int                    dictCondition         ; // conditional apply some pronunciation rules and dict.lookups
    int                    dictMinSize           ;
    int                    dictSkipWords         ;
    int                    nGroups2              ; // number of groups2[] entries used
    int                    expectVerb            ;
    int                    expectPast            ; // expect past tense
    int                    expectVerbs           ;
    int                    expectNoun            ;
    int                    prevLastStress        ;
    int                    wordVowelCount        ; // number of vowels so far
    int                    wordStressedCount     ; // number of vowels so far which could be stressed
    int                    clauseUpperCount      ; // number of upper case letters in the clause
    int                    clauseLowerCount      ; // number of lower case letters in the clause
    int                    prepauseTimeout       ;
    int                    endStressedVowel      ; // word ends with stressed vowel
    int                    clauseTerminator      ;
    int                    letterBitsOffset      ;
    int                    OptionSayAs           ;
    int                    OptionEmphasis        ;
    int                    PrePause              ;
    int                    prevDictFlags  [   2] ; // dictionary flags from previous word
    char                   dictionaryName [  40] ;
    char                   phonemeOut     [ 500] ;
    char                   phonemesRepeat [  20] ;
    char                   wordReplacement[ 160] ;
    char                   wordPhonemes   [ 200] ;
    short                  stressLengths  [   8] ;
    unsigned char          stressAmps     [   8] ;
    unsigned char          stressAmpsR    [   8] ;
    unsigned char          letterBits     [ 256] ;
    unsigned char          groups2Count   [ 256] ; // number of 2 letter groups for this initial letter
    unsigned char          groups2Start   [ 256] ; // index into groups2
    unsigned char          punctToTone    [8][6] ;
    unsigned int           groups2Name    [ 120] ; // the two letter pairs for groups2[]
    char                 * dataDictRules         ; // language_1   translation rules file
    QByteArray             dataDictList          ; // language_2   dictionary lookup file
    char                 * clauseEnd             ;
    const char           * transposeMap          ;
    const short          * frequentPairs         ; // list of frequent pairs of letters, for use in compressed *_list
    const unsigned short * charsetA0             ; // unicodes for characters 0xa0 to oxff
    const unsigned short * charsIgnore           ;
    const wchar_t        * charPlusApostrophe    ; // single chars + apostrophe treated as words
    const wchar_t        * punctWithinWord       ; // allow these punctuation characters within words
    char                 * dictHashTab    [1024] ; // hash table to index dictionary lookup file
    char                 * letterGroups   [  95] ;
    char                 * groups1        [ 256] ; // translation rule lists, index by single letter
    char                 * groups2        [ 120] ; // translation rule lists, indexed by two-letter pairs
    char                 * groups3        [ 128] ; // index by offset letter
    const wchar_t        * wLetterGroups  [   8] ;

    explicit SpeechTranslator      (void) ;
             SpeechTranslator      (const SpeechTranslator & translator) ;
    virtual ~SpeechTranslator      (void) ;

    SpeechTranslator & assign      (const SpeechTranslator & translator) ;
    SpeechTranslator & operator =  (const SpeechTranslator & translator) ;

    int  iswalpha2                 (int c) ;
    int  iswlower2                 (int c) ;
    int  iswupper2                 (int c) ;
    int  IsBracket                 (int c) ;
    int  isHexDigit                (int c) ;
    int  NonAsciiNumber            (int c) ;
    int  towlower2                 (unsigned int c) ;
    int  towupper2                 (unsigned int c) ;
    int  IsRomanU                  (unsigned int c) ;
    int  isspace2                  (unsigned int c) ;
    int  IsAlpha                   (unsigned int c) ;
    int  IsNumber                  (unsigned int c) ;
    int  IsDigit                   (unsigned int c) ;
    int  IsSpace                   (unsigned int c) ;
    int  lookupwchar               (const unsigned short * list,int c) ;
    int  lookupwchar2              (const unsigned short * list,int c) ;
    int  IsAllUpper                (const char * word) ;
    bool thousandsGroup            (const char * word,int groupLen) ;
    bool IsNumberHuE               (const char * word,int thousandplex,int value) ;
    bool irishUpperCase            (char * word,int c) ;

    char * wstrchr                 (const char * s,int c) ;
    int    Substitute              (unsigned int  c           ,
                                    unsigned int  next_in     ,
                                    int         * insert      ,
                                    int         * wordflags ) ;
    wchar_t * SsmlAttribute        (wchar_t    * attr   ,
                                    wchar_t    * pw     ,
                                    const char * name ) ;
    void SpecialAttribute          (char * phonemes,int dict_flags) ;

    bool is                        (QString Name) ;
    void Install                   (QString Name) ;
    bool Load                      (void) ;
    bool Select                    (const char * name) ;
    void Reset                     (void) ;
    void Setup                     (const short         * lengths ,
                                    const unsigned char * amps  ) ;
    void SetupRules                (void) ;

    int  isLetterGroup             (char * word,int group,int pre) ;
    bool isLetter                  (int letter,int group) ;
    bool isVowel                   (int letter) ;
    int  toUtf8                    (unsigned int c,char * buf) ;
    int  toGrapheme                (int & grapheme,const char * buf,bool backward = false) ;
    int  nBytes                    (unsigned char c) ;
    void setSpellingStress         (char * phonemes,int control,int chars) ;
    bool StressCondition           (PhonemeLists & list,int condition,int control,bool translator = true) ;

    int  Hash                      (const char * string) ;

    void PitchesTone               (PhonemeLists  & list) ;
    void Pitches                   (PhonemeLists  & list        ,
                                    SpeechTune    & tune        ,
                                    Syllables     & syllables   ,
                                    int             clauseType  ,
                                    int             toneflags ) ;
    int  VowelStress               (unsigned char * phonemes           ,
                                    signed   char * vowel_stress       ,
                                    int           & vowel_count        ,
                                    int           & stressed_syllable  ,
                                    int             control          ) ;
    void ChangeStress              (char * word,int newStress) ;
    void SetStress                 (char         * output           ,
                                    unsigned int * dictionary_flags ,
                                    int            tonic            ,
                                    int            control        ) ;
    int  Transpose                 (char * text) ;
    void Match                     (SpeechMatch  & result        ,
                                    char         * word[]        ,
                                    char         * word_start    ,
                                    int            group_length  ,
                                    char         * rule          ,
                                    int            word_flags    ,
                                    int            dict_flags  ) ;
    int  Rules                     (char         * p_start       ,
                                    char         * phonemes      ,
                                    int            ph_size       ,
                                    char         * end_phonemes  ,
                                    int            word_flags    ,
                                    unsigned int * dict_flags  ) ;
    const char * Entry             (const char          * word       ,
                                    const char          * word2      ,
                                    char                * phonetic   ,
                                    unsigned int        * flags      ,
                                    int                   end_flags  ,
                                    QList<SpeechWords>  & wtab     ) ;
    bool Lookup                    (char               ** wordptr    ,
                                    char                * ph_out     ,
                                    unsigned int        * flags      ,
                                    int                   end_flags  ,
                                    QList<SpeechWords>  & wtab     ) ;
    bool Lookup                    (const char * word,char * ph_out) ;
    int  LookupFlags               (const char * word) ;
    int  RemoveEnding              (char * word,int endType,char * wordCopy) ;
    void WordEmbeddedCmd           (UUIDs Cmds) ;

    bool GetWord                   (QString & word,char * packet) ;
    bool GetPhonemes               (char * packet,char * phonemes) ;
    int  GetFlags                  (char * packet,char * flags) ;

    void SelectAF                  (void) ;
    void SelectAM                  (void) ;
    void SelectAR                  (void) ;
    void SelectBG                  (void) ;
    void SelectBN                  (void) ;
    void SelectBO                  (void) ;
    void SelectCY                  (void) ;
    void SelectDA                  (void) ;
    void SelectDE                  (void) ;
    void SelectDV                  (void) ;
    void SelectEN                  (void) ;
    void SelectEL                  (void) ;
    void SelectGRC                 (void) ;
    void SelectEO                  (void) ;
    void SelectES                  (void) ;
    void SelectAN                  (void) ;
    void SelectCA                  (void) ;
    void SelectPAP                 (void) ;
    void SelectEU                  (void) ;
    void SelectFA                  (void) ;
    void SelectET                  (void) ;
    void SelectFI                  (void) ;
    void SelectFR                  (void) ;
    void SelectGA                  (void) ;
    void SelectHI                  (void) ;
    void SelectNE                  (void) ;
    void SelectPA                  (void) ;
    void SelectGU                  (void) ;
    void SelectHR                  (void) ;
    void SelectBS                  (void) ;
    void SelectSR                  (void) ;
    void SelectHT                  (void) ;
    void SelectHU                  (void) ;
    void SelectHY                  (void) ;
    void SelectID                  (void) ;
    void SelectMS                  (void) ;
    void SelectIS                  (void) ;
    void SelectIT                  (void) ;
    void SelectJBO                 (void) ;
    void SelectJP                  (void) ;
    void SelectKA                  (void) ;
    void SelectKK                  (void) ;
    void SelectKL                  (void) ;
    void SelectKO                  (void) ;
    void SelectKU                  (void) ;
    void SelectLA                  (void) ;
    void SelectLT                  (void) ;
    void SelectLV                  (void) ;
    void SelectMK                  (void) ;
    void SelectMT                  (void) ;
    void SelectNL                  (void) ;
    void SelectNO                  (void) ;
    void SelectOM                  (void) ;
    void SelectPL                  (void) ;
    void SelectPT                  (void) ;
    void SelectRO                  (void) ;
    void SelectRU                  (void) ;
    void SelectRW                  (void) ;
    void SelectSK                  (void) ;
    void SelectCS                  (void) ;
    void SelectSI                  (void) ;
    void SelectSL                  (void) ;
    void SelectSQ                  (void) ;
    void SelectSV                  (void) ;
    void SelectSW                  (void) ;
    void SelectTN                  (void) ;
    void SelectTA                  (void) ;
    void SelectML                  (void) ;
    void SelectKN                  (void) ;
    void SelectMR                  (void) ;
    void SelectTE                  (void) ;
    void SelectTH                  (void) ;
    void SelectTR                  (void) ;
    void SelectAZ                  (void) ;
    void SelectTT                  (void) ;
    void SelectUK                  (void) ;
    void SelectUR                  (void) ;
    void SelectVI                  (void) ;
    void SelectWO                  (void) ;
    void SelectZH                  (void) ;
    void SelectZHY                 (void) ;

    void setCyrillicLetters        (void) ;
    void setIndicLetters           (void) ;
    void setLanguage               (int name) ;
    void setLetterVowel            (int c) ;
    void setLetterBits             (int group,const char * string) ;
    void setLetterBitsRange        (int group,int first,int last) ;
    void setLengthMods             (int value) ;
    void resetLetterBits           (int groups) ;

    void listings                  (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT FormantParament
{
  public:

    int fmt_control ;
    int use_vowelin ;
    int fmt_addr    ;
    int fmt_length  ;
    int fmt_amp     ;
    int fmt2_addr   ;
    int fmt2_lenadj ;
    int wav_addr    ;
    int wav_amp     ;
    int transition0 ;
    int transition1 ;
    int std_length  ;

    explicit FormantParament     (void) ;
             FormantParament     (const FormantParament & formant) ;
    virtual ~FormantParament     (void) ;

    FormantParament & operator = (const FormantParament & formant) ;
    FormantParament & assign     (const FormantParament & formant) ;

    void Reset                   (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT FormantParaments : public QMap<int,FormantParament *>
{
  public:

    explicit FormantParaments (void) ;
    virtual ~FormantParaments (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT ParamentStack
{
  public:

    int type          ;
    int parameter[16] ;

    explicit ParamentStack     (void) ;
             ParamentStack     (const ParamentStack & stack) ;
    virtual ~ParamentStack     (void) ;

    ParamentStack & operator = (const ParamentStack & stack) ;
    ParamentStack & assign     (const ParamentStack & stack) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT ParamentStacks : public QList<ParamentStack *>
{
  public:

    explicit ParamentStacks (void) ;
    virtual ~ParamentStacks (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechFactors
{
  public:

    int pause_factor        ;
    int clause_pause_factor ;
    int min_pause           ;
    int wav_factor          ;
    int lenmod_factor       ;
    int lenmod2_factor      ;
    int min_sample_len      ;
    int loud_consonants     ;
    int fast_settings [ 8 ] ;

    explicit SpeechFactors     (void) ;
             SpeechFactors     (const SpeechFactors & factors) ;
    virtual ~SpeechFactors     (void) ;

    SpeechFactors & operator = (const SpeechFactors & factors) ;
    SpeechFactors & assign     (const SpeechFactors & factors) ;

    void setSpeed              (VoicePacket & voice         ,
                                int           control       ,
                                Synthesizer & synthesizer ) ;
    int  Length                (int pause,int control) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT PhonemeReplacement
{
  public:

    unsigned char Old  ;
    unsigned char New  ;
    char          Type ; // 0=always replace , 1=only at end of word

    explicit PhonemeReplacement     (void) ;
             PhonemeReplacement     (const PhonemeReplacement & replace) ;
    virtual ~PhonemeReplacement     (void) ;

    PhonemeReplacement & operator = (const PhonemeReplacement & replace) ;
    PhonemeReplacement & assign     (const PhonemeReplacement & replace) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT PhonemeReplacements : public QList<PhonemeReplacement *>
{
  public:

    explicit PhonemeReplacements (void) ;
    virtual ~PhonemeReplacements (void) ;

    void append                  (Phonemes & phonemes,char * string) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechFrame
{
  public:

    short         frflags     ;
    short         ffreq   [7] ;
    unsigned char length      ;
    unsigned char rms         ;
    unsigned char fheight [8] ;
    unsigned char fwidth  [6] ; // width/4  f0-5
    unsigned char fright  [3] ; // width/4  f0-2
    unsigned char bw      [4] ; // Klatt bandwidth BNZ /2, f1,f2,f3
    unsigned char klattp  [5] ; // AV, FNZ, Tilt, Aspr, Skew
    ///////////////////////////////////////////////////////////////////////
    unsigned char klattp2 [5] ; // continuation of klattp[],  Avp, Fric, FricBP, Turb
    unsigned char klattap [7] ; // Klatt parallel amplitude
    unsigned char klattbp [7] ; // Klatt parallel bandwidth  /2
    unsigned char spare       ; // pad to multiple of 4 bytes

    explicit SpeechFrame      (void) ;
             SpeechFrame      (const SpeechFrame & frame) ;
    virtual ~SpeechFrame      (void) ;

    SpeechFrame & operator =  (const SpeechFrame & frame) ;
    SpeechFrame & assign      (const SpeechFrame & frame) ;
    SpeechFrame & copy        (const SpeechFrame & frame,bool Copy) ;

    int  VowelCloseness       (void) ;
    void setRMS               (VoicePacket & voice,int RMS) ;
    void FormantsReduceHF     (VoicePacket & voice,int level) ;
    void AdjustFormants       (VoicePacket & voice     ,
                               int           target    ,
                               int           min       ,
                               int           max       ,
                               int           F1        ,
                               int           F3        ,
                               int           ReduceHF  ,
                               int           Flags   ) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechFrames : public QList<SpeechFrame *>
{
  public:

    explicit SpeechFrames (void) ;
    virtual ~SpeechFrames (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechReference
{
  public:

    short         length  ;
    short         frflags ;
    SpeechFrame * frame   ;

    explicit SpeechReference     (void) ;
             SpeechReference     (const SpeechReference & reference) ;
    virtual ~SpeechReference     (void) ;

    SpeechReference & operator = (const SpeechReference & reference) ;
    SpeechReference & assign     (const SpeechReference & reference) ;

    SpeechFrame * duplicate      (SpeechReference & source,int length) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpectrumSequence
{
  public:

    unsigned char Flags  ;
    SpeechFrames  Frames ;

    explicit SpectrumSequence     (void) ;
             SpectrumSequence     (const SpectrumSequence & spectrum) ;
    virtual ~SpectrumSequence     (void) ;

    SpectrumSequence & operator = (const SpectrumSequence & spectrum) ;
    SpectrumSequence & assign     (const SpectrumSequence & spectrum) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT MbrolaTable
{
  public:

    int           name         ;
    unsigned int  next_phoneme ;
    int           mbr_name     ;
    int           mbr_name2    ;
    int           percent      ; // percentage length of first component
    int           control      ;

    explicit MbrolaTable     (void) ;
             MbrolaTable     (const MbrolaTable & table) ;
    virtual ~MbrolaTable     (void) ;

    MbrolaTable & operator = (const MbrolaTable & table) ;
    MbrolaTable & assign     (const MbrolaTable & table) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechTune
{
  public:

    char          name         [12] ;
    unsigned char flags        [ 4] ;
    signed   char head_extend  [ 8] ;
    unsigned char prehead_start     ;
    unsigned char prehead_end       ;
    unsigned char stressed_env      ;
    unsigned char stressed_drop     ;
    unsigned char secondary_drop    ;
    unsigned char unstressed_shape  ;
    unsigned char onset             ;
    unsigned char head_start        ;
    unsigned char head_end          ;
    unsigned char head_last         ;
    unsigned char head_max_steps    ;
    unsigned char n_head_extend     ;
    signed   char unstr_start  [ 3] ;    // for: onset, head, last
    signed   char unstr_end    [ 3] ;
    unsigned char nucleus0_env      ; // pitch envelope, tonic syllable is at end, no tail
    unsigned char nucleus0_max      ;
    unsigned char nucleus0_min      ;
    unsigned char nucleus1_env      ; // when followed by a tail
    unsigned char nucleus1_max      ;
    unsigned char nucleus1_min      ;
    unsigned char tail_start        ;
    unsigned char tail_end          ;
    unsigned char split_nucleus_env ;
    unsigned char split_nucleus_max ;
    unsigned char split_nucleus_min ;
    unsigned char split_tail_start  ;
    unsigned char split_tail_end    ;
    unsigned char split_tune        ;
    unsigned char spare        [ 8] ;
    int           spare2            ; // the struct length should be a multiple of 4 bytes

    explicit SpeechTune     (void) ;
             SpeechTune     (const SpeechTune & tune) ;
    virtual ~SpeechTune     (void) ;

    SpeechTune & operator = (const SpeechTune & tune) ;
    SpeechTune & assign     (const SpeechTune & tune) ;

    static int   packetSize (void) ;
    void         set        (int index,QByteArray & data) ;
    int          Packet     (QByteArray & data) ;

    int setHeadIntonation   (Syllables & syllables   ,
                             int         start       ,
                             int         end       ) ;
    int Pitches             (Syllables & syllables   ,
                             int         start       ,
                             int         end         ,
                             int         toneflags ) ;
    int Pitches             (Syllables & syllables   ,
                             int         start       ,
                             int         end         ,
                             int         control     ,
                             int         index       ,
                             int         toneflags ) ;

    virtual void report     (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT SpeechTunes : public QList<SpeechTune *>
{
  public:

    explicit SpeechTunes (void) ;
    virtual ~SpeechTunes (void) ;

    void append          (QByteArray & tunes) ;
    int  Packet          (QByteArray & data ) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT VoiceFeature
{
  public:

    QString     Name       ;
    QString     Appear     ;
    QString     Language   ;
    QStringList Languages  ;
    QString     Phoneme    ;
    QString     Translator ;
    QString     Dictionary ;
    QByteArray  Data       ;
    int         Priority   ;
    int         Gender     ; // 0=none 1=male, 2=female,
    int         Age        ; // 0=not specified, or age in years
    int         Variant    ; // only used when passed as a parameter to espeak_SetVoiceByProperties
    int         XX1        ; // for internal use
    int         Score      ; // for internal use
    void      * Spare      ; // for internal use

    explicit    VoiceFeature       (void) ;
    virtual    ~VoiceFeature       (void) ;

    bool        Parse              (int control) ;
    QString     Strip              (QString Line) ;
    bool        Assign             (QString Line,bool toneOnly = false) ;

    void        setName            (QString Line,bool toneOnly = false) ;
    void        setLanguage        (QString Line) ;
    void        setTranslator      (QString Line) ;
    void        setPhoneme         (QString Line) ;
    void        setDictionary      (QString Line) ;
    void        setGender          (QString Line) ;

    bool        is                 (QString Name) ;

    QStringList PossiblePhonemes   (void) ;
    QStringList PossibleDictionary (void) ;

  protected:

  private:

};

class Q_PHONEMES_EXPORT Synthesizer
{
  public:

    enum                {
      Speech        = 0 ,
      Dictionary    = 1 ,
      Voices        = 2 ,
      Mbrola        = 3 ,
      MbrolaPhoneme = 4 ,
      Sources       = 5
    }                   ;

    // embedded command numbers
    enum            {
      EMBED_P  =  1 , // pitch
      EMBED_S  =  2 , // speed (used in setlengths)
      EMBED_A  =  3 , // amplitude/volume
      EMBED_R  =  4 , // pitch range/expression
      EMBED_H  =  5 , // echo/reverberation
      EMBED_T  =  6 , // different tone for announcing punctuation (not used)
      EMBED_I  =  7 , // sound icon
      EMBED_S2 =  8 , // speed (used in synthesize)
      EMBED_Y  =  9 , // say-as commands
      EMBED_M  = 10 , // mark name
      EMBED_U  = 11 , // audio uri
      EMBED_B  = 12 , // break
      EMBED_F  = 13 , // emphasis
      EMBED_C  = 14   // capital letter indication
    }               ;

    enum WCMDL           {
      KLATT         =  1 ,
      KLATT2        =  2 ,
      SPECT         =  3 ,
      SPECT2        =  4 ,
      PAUSE         =  5 ,
      WAVE          =  6 ,
      WAVE2         =  7 ,
      AMPLITUDE     =  8 ,
      PITCH         =  9 ,
      MARKER        = 10 ,
      VOICE         = 11 ,
      EMBEDDED      = 12 ,
      MBROLA_DATA   = 13 ,
      FMT_AMPLITUDE = 14 ,
      SONIC_SPEED   = 15
    }                    ;

    int                      Values [ 16        ] ;
    int                      Max    [ 16        ] ;
    QList<VarArgs>           WCMDQ                ;
    int                      Speed1               ;
    int                      Speed2               ;
    int                      Speed3               ;
    int                      Version              ;
    int                      SampleRate           ;
    int                      samplerate_native    ;
    int                      samplerate_base      ;
    int                      Amplitude            ;
    QByteArray               Data                 ; // Phoneme Data
    QByteArray               Index                ; // Phoneme Index
    EMAPs                    Supports             ;
    IMAPs                    FeatureMap           ;
    IMAPs                    TranslatorMap        ;
    ParamentStacks           Paraments            ;
    FormantParaments         Formants             ;
    SpeechTunes              Tunes                ;
    QList<Phonemes         > phonemes             ;
    QList<VoiceFeature     > Features             ;
    QList<SpeechTranslator > Translators          ;
    QMap <int,QDir         > Directories          ;
    QMap <int,VoicePacket  > VoiceMap             ;
    QMap <int,WaveSynthesis> SynthMap             ;
    QMap <int,WaveGenerator> WaveMap              ;
    QMap <int,SonicPitch   > SonicMap             ;
    QMap <int,PhonemeData  > DataMap              ;
    QMap <int,SpeechFactors> Factors              ;

    explicit Synthesizer         (void) ;
             Synthesizer         (const Synthesizer & synthesizer) ;
    virtual ~Synthesizer         (void) ;

    Synthesizer & operator =     (const Synthesizer & synthesizer) ;
    Synthesizer & assign         (const Synthesizer & synthesizer) ;

    void         setHome         (QString speechHome) ;
    QString      Path            (int catalog,QString filename) ;

    void         SonicSpeed      (int value) ;
    void         ParseData       (void) ;
    int          GetAmplitude    (void) ;

    virtual bool Initialize      (void) ;

    QStringList  SupportedVoices (void) ;
    int          Pick            (int Priority,QString Voice) ;
    bool         Parse           (int Priority,int control) ;
    bool         Assign          (int Priority,QString Line,int control) ;

    int          InstallWords    (unsigned short * program) ;
    bool         containsPhoneme (QString phoneme) ;
    int          PickPhoneme     (QStringList phonemes) ;
    int          PickDictionary  (QStringList Dictionaries) ;
    bool         InstallPhoneme  (int translator,int phoneme,bool recursing) ;

    void         SetEmbedded     (int Priority,int control,int value) ;

  protected:

  private:

};

}

QT_END_NAMESPACE

#endif

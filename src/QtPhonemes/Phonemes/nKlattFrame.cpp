#include <qtphonemes.h>

N::KlattFrame:: KlattFrame ( void )
              : F0hz10     ( 1000 )
              , AVdb       (   59 )
              , ASP        (    0 )
              , Kopen      (   40 )
              , Aturb      (    0 )
              , TLTdb      (    0 )
              , AF         (    0 )
              , Kskew      (    0 )
              , AB         (    0 )
              , AVpdb      (    0 )
              , Gain0      (    0 )
{
}

N::KlattFrame::~KlattFrame(void)
{
}

N::KlattFrame & N::KlattFrame::operator = (KlattFrame & frame)
{
  nMemberCopy ( frame , F0hz10           ) ;
  nMemberCopy ( frame , AVdb             ) ;
  nMemberCopy ( frame , ASP              ) ;
  nMemberCopy ( frame , Kopen            ) ;
  nMemberCopy ( frame , Aturb            ) ;
  nMemberCopy ( frame , TLTdb            ) ;
  nMemberCopy ( frame , AF               ) ;
  nMemberCopy ( frame , Kskew            ) ;
  nMemberCopy ( frame , AB               ) ;
  nMemberCopy ( frame , AVpdb            ) ;
  nMemberCopy ( frame , Gain0            ) ;
  nMemberCopy ( frame , tmpAVdb          ) ;
  //////////////////////////////////////////
  nArrayCopy  ( frame , Fhz     , i , 10 ) ;
  nArrayCopy  ( frame , Bhz     , i , 10 ) ;
  nArrayCopy  ( frame , Ap      , i , 10 ) ;
  nArrayCopy  ( frame , Bphz    , i , 10 ) ;
  nArrayCopy  ( frame , nextFhz , i , 10 ) ;
  nArrayCopy  ( frame , nextBhz , i , 10 ) ;
  return ME                                ;
}

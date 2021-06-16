#include <qtphonemes.h>

N::MbrolaTable:: MbrolaTable  (void)
               : name         (0   )
               , next_phoneme (0   )
               , mbr_name     (0   )
               , mbr_name2    (0   )
               , percent      (0   )
               , control      (0   )
{
}

N::MbrolaTable:: MbrolaTable(const MbrolaTable & table)
{
  assign ( table ) ;
}

N::MbrolaTable::~MbrolaTable(void)
{
}

N::MbrolaTable & N::MbrolaTable::operator = (const MbrolaTable & table)
{
  return assign(table) ;
}

N::MbrolaTable & N::MbrolaTable::assign(const MbrolaTable & table)
{
  nMemberCopy ( table , name         ) ;
  nMemberCopy ( table , next_phoneme ) ;
  nMemberCopy ( table , mbr_name     ) ;
  nMemberCopy ( table , mbr_name2    ) ;
  nMemberCopy ( table , percent      ) ;
  nMemberCopy ( table , control      ) ;
  return ME                            ;
}

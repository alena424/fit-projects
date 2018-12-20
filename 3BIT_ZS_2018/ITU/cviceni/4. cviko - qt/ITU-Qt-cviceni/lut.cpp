#include "lut.h"
#include "lut_table.h"

LUT::LUT( QObject * parent ) :
    QObject( parent )
{
}

double LUT::getValue(int index) {
    int index2 = index;
    if (index < 0) {
        index2 = 0;
    }
    if (index > 200) {
        index2 = 200;
    }
    return LUT_table[index2];
}

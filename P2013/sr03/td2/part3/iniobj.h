#include <stdio.h>

#include "defobj.h"
#define TABLEN 5

obj tobj[TABLEN];
void iniobj(){
    int i;
    for(i=0; i<TABLEN; i++){
        sprintf(tobj[i].ident, "ident_o%d", i+1);
        sprintf(tobj[i].desc, "description_o%d", i+1);
        tobj[i].ii = (i+1)*10+1;
        tobj[i].jj = tobj[i].ii + 1;
        tobj[i].dd = (i+1)*10 + 0.2345;
        tobj[i].fndesc = 0;
    }
    tobj[TABLEN-1].fndesc = -1;

}
void printobj(obj o){
    printf("%s %s %d %d %lf\n", o.ident, o.desc, o.ii, o.jj, o.dd);
}
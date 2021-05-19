#ifndef VALIDACION_H_INCLUDED
#define VALIDACION_H_INCLUDED

#include <iostream>
#include <ctype.h>
#include <regex>
#include <string>

using namespace std;

bool validarCorreo(const string &correo)
{
    bool correo_correcto;
    regex correoValido("([a-zA-Z][\\w\\d]+(([-_\\.][\\w\\d]+)+)?@[a-zA-Z][a-zA-Z_]+.(([a-zA-Z]{3}(.[a-zA-Z]{2})?)|[a-zA-Z]{2}))");
    if(regex_match(correo,correoValido))
    {
        correo_correcto=true;
        for(int i=0;i<correo.length();i++)
        {
            if((((correo)[i]=='.')||((correo)[i]=='-')||((correo)[i]=='_'))&&(((correo)[i+1]=='.')||((correo)[i+1]=='-')||((correo)[i+1]=='_')))
            {
                correo_correcto=false;
            }
        }
    }
    else
    {
        correo_correcto=false;
    }
    return correo_correcto;
}
#endif // VALIDACION_H_INCLUDED

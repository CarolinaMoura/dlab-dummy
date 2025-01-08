
#include "image.h"

Image::Image( const char* filename , const int image_id_num ) : 
    row_index { 0 } ,
    image_id { image_id_num }

{
    image_file = filename ;  
    image_text = "" ; // until we decide what to do 
}

void Image::display( ) 
{
    return ;
}


void Image::render_text() 
{
    return ;
}

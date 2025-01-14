#include "image.h"
#include <Arduino.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

Image::Image( const char* filename , const char* image_file , const int image_id_num ) : 
    row_index { 0 } ,
    image_id { image_id_num }

{
    image_file = filename ;  
    File file = SD.open(image_file , FILE_READ);

    if (! file) {
        Serial.println("Failed to open text file.");
        return;
    }

    image_text = "" ;
    size_t text_size = file.size() ; 

    if ( text_size > 0 ) {
        image_text = ( char* ) malloc( text_size + 1 ) ;

        if ( !image_text ) {
            Serial.println("Memory allocation failed.") ;
            file.close() ;
            return ;
        }

        file.readBytes(image_text, text_size) ;
        image_text[ text_size ] = '\0' ;
    }

    file.close() ;
}

void Image::display() 
{   
    File image_file = SD.open(image_file) ;

    if ( !image_file ) {
        Serial.println("Error opening image file!");
        return;
    }

    uint8_t tmp_buffer [ IMG_DIMENSIONS * 2 ] ;
    for ( int y = 0; y < IMG_DIMENSIONS ; ++y ) {
      image_file.read( tmp_buffer , IMG_DIMENSIONS * 2 ) ;
      for ( int i = 0; i < IMG_DIMENSIONS; i++) {
        row_buffer[i] = ((tmp_buffer[2*i] << 8) | tmp_buffer[2*i + 1]);
      }
      
      tft.setAddrWindow(0, y, imageWidth, y);

      tft.pushColors(row_buffer, imageWidth, true);
    }
    return ;
}


void Image::render_text() 
{
    if (! image_text ) {
        Serial.println("No text available.") ;
        return ;
    }

    tft.fillRect(0, TFT_HEIGHT , TFT_WIDTH , TFT_WIDTH - TFT_HEIGHT , TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(TEXT_SIZE); 
    tft.setCursor(0, TFT_HEIGHT);
    
    return ;
}

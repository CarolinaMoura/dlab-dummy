#ifndef IMAGE_H__
#define IMAGE_H__

#define TFT_WIDTH   480
#define TFT_HEIGHT  320
#define IMG_DIMENSIONS 320 
#define TEXT_SIZE 2

#include <stdint.h> 

class Image {
    public: 
        Image() = delete ; 
        Image( const char* filename , const char* image_file , const int image_id_num ) ;

        void display();
        void render_text(); 
    
    private: 
        const char* image_file; 
        char* image_text; 
        const int image_id;
        uint16_t row_buffer [ IMG_DIMENSIONS ] ;
        int row_index ; 

} ;

#endif


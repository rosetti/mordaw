/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_65180341_INCLUDED
#define BINARYDATA_H_65180341_INCLUDED

namespace TransportImages
{
    extern const char*   play_png;
    const int            play_pngSize = 4024;
    
    extern const char*   pause_png;
    const int            pause_pngSize = 3754;
    
    extern const char*   forward_png;
    const int            forward_pngSize = 4196;
    
    extern const char*   rewind_png;
    const int            rewind_pngSize = 4258;
    
    extern const char*   record_png;
    const int            record_pngSize = 3744;
    
    extern const char*   record_enabled_png;
    const int            record_enabled_pngSize = 3516;
    
    extern const char*   stop_png;
    const int            stop_pngSize = 3276;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 7;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

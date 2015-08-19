/*
  ==============================================================================

    SplashImage.h
    Created: 18 Aug 2015 11:49:59pm
    Author:  Matt

  ==============================================================================
*/

#ifndef SPLASHIMAGE_H_INCLUDED
#define SPLASHIMAGE_H_INCLUDED

namespace SplashImage
{    
    extern const char*   mordaw_jpg;
    const int            mordaw_jpgSize = 64860;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 1;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif  // SPLASHIMAGE_H_INCLUDED

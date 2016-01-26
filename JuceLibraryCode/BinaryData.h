/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_79463784_INCLUDED
#define BINARYDATA_H_79463784_INCLUDED

namespace BinaryData
{
    extern const char*   AddClass128_png;
    const int            AddClass128_pngSize = 3731;

    extern const char*   ButtonTemplate_psd;
    const int            ButtonTemplate_psdSize = 73302;

    extern const char*   DeleteClass128_png;
    const int            DeleteClass128_pngSize = 3508;

    extern const char*   Icon128_png;
    const int            Icon128_pngSize = 17647;

    extern const char*   Icon256_png;
    const int            Icon256_pngSize = 42583;

    extern const char*   Icon512_icns;
    const int            Icon512_icnsSize = 216004;

    extern const char*   Icon512_png;
    const int            Icon512_pngSize = 116845;

    extern const char*   Icon1024_png;
    const int            Icon1024_pngSize = 355229;

    extern const char*   Load128_png;
    const int            Load128_pngSize = 4979;

    extern const char*   Logo_icns;
    const int            Logo_icnsSize = 50125;

    extern const char*   Logo_jpg;
    const int            Logo_jpgSize = 81687;

    extern const char*   Logo_png;
    const int            Logo_pngSize = 97078;

    extern const char*   Logo_psd;
    const int            Logo_psdSize = 417416;

    extern const char*   Play128_png;
    const int            Play128_pngSize = 4312;

    extern const char*   RecordOff128_png;
    const int            RecordOff128_pngSize = 5527;

    extern const char*   RecordOn128_png;
    const int            RecordOn128_pngSize = 5419;

    extern const char*   Save128_png;
    const int            Save128_pngSize = 4902;

    extern const char*   Stop128_png;
    const int            Stop128_pngSize = 4472;

    extern const char*   Tick_wav;
    const int            Tick_wavSize = 28018;

    extern const char*   Tock_wav;
    const int            Tock_wavSize = 26924;

    extern const char*   BodyHits4_txt;
    const int            BodyHits4_txtSize = 9651;

    extern const char*   ENST_txt;
    const int            ENST_txtSize = 7420;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 22;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

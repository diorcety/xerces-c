/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache\@apache.org.
 * 
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/BitOps.hpp>
#include <util/XMLWin1252Transcoder.hpp>
#include <util/TranscodingException.hpp>
#include <memory.h>



// ---------------------------------------------------------------------------
//  Local const data
//
//  g1252XlatTable
//      This is the translation table for 1252 to Unicode. This table
//      contains 256 entries. The entry for x is the Unicode symbol for
//      1252 code x.
// ---------------------------------------------------------------------------
static const XMLCh g1252XlatTable[256] =
{
       0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007
    ,  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F
    ,  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017
    ,  0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F
    ,  0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027
    ,  0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F
    ,  0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037
    ,  0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F
    ,  0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047
    ,  0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F
    ,  0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057
    ,  0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F
    ,  0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067
    ,  0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F
    ,  0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077
    ,  0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F
    ,  0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021
    ,  0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F
    ,  0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014
    ,  0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178
    ,  0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7
    ,  0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF
    ,  0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7
    ,  0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF
    ,  0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7
    ,  0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF
    ,  0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7
    ,  0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF
    ,  0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7
    ,  0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF
    ,  0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7
    ,  0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
};



// ---------------------------------------------------------------------------
//  XMLWin1252Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLWin1252Transcoder::XMLWin1252Transcoder( const   XMLCh* const encodingName
                                            , const unsigned int blockSize) :
    XMLTranscoder(encodingName, blockSize)
{
}


XMLWin1252Transcoder::~XMLWin1252Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLWin1252Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XMLWin1252Transcoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XMLWin1252Transcoder::transcodeOne( const   XMLByte* const  srcData
                                    , const unsigned int    srcBytes
                                    ,       unsigned int&   bytesEaten)
{
    // If no source, give up
    if (!srcBytes)
        return 0;

    // We will eat one byte
    bytesEaten = 1;

    // And use the 0th byte as a look in our table
    return g1252XlatTable[*srcData];
}


unsigned int
XMLWin1252Transcoder::transcodeXML( const   XMLByte* const          srcData
                                    , const unsigned int            srcCount
                                    ,       XMLCh* const            toFill
                                    , const unsigned int            maxChars
                                    ,       unsigned int&           bytesEaten
                                    ,       unsigned char* const    charSizes)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of chars in the source.
    //
    const unsigned int countToDo = srcCount < maxChars ? srcCount : maxChars;

    //
    //  Loop through the count we have to do and map each char via the
    //  lookup table.
    //
    const XMLByte*  srcPtr = srcData;
    const XMLByte*  endPtr = (srcPtr + countToDo);
    XMLCh*          outPtr = toFill;
    while (srcPtr < endPtr)
        *outPtr++ = g1252XlatTable[*srcPtr++];

    // Set the bytes eaten
    bytesEaten = countToDo;

    // Set the character sizes to the fixed size
    memset(charSizes, 1, countToDo);

    // Return the chars we transcoded
    return countToDo;
}



#if 0
struct XlatRecord
{
    XMLCh   xmlCh;
    XMLByte ebcdicCh;
};

static XlatRecord gXlatTable[256] =
{
  { 0x0000, 0x00 }
  , { 0x0001, 0x01 }
  , { 0x0002, 0x02 }
  , { 0x0003, 0x03 }
  , { 0x0004, 0x04 }
  , { 0x0005, 0x05 }
  , { 0x0006, 0x06 }
  , { 0x0007, 0x07 }
  , { 0x0008, 0x08 }
  , { 0x0009, 0x09 }
  , { 0x000A, 0x0A }
  , { 0x000B, 0x0B }
  , { 0x000C, 0x0C }
  , { 0x000D, 0x0D }
  , { 0x000E, 0x0E }
  , { 0x000F, 0x0F }
  , { 0x0010, 0x10 }
  , { 0x0011, 0x11 }
  , { 0x0012, 0x12 }
  , { 0x0013, 0x13 }
  , { 0x0014, 0x14 }
  , { 0x0015, 0x15 }
  , { 0x0016, 0x16 }
  , { 0x0017, 0x17 }
  , { 0x0018, 0x18 }
  , { 0x0019, 0x19 }
  , { 0x001A, 0x1A }
  , { 0x001B, 0x1B }
  , { 0x001C, 0x1C }
  , { 0x001D, 0x1D }
  , { 0x001E, 0x1E }
  , { 0x001F, 0x1F }
  , { 0x0020, 0x20 }
  , { 0x0021, 0x21 }
  , { 0x0022, 0x22 }
  , { 0x0023, 0x23 }
  , { 0x0024, 0x24 }
  , { 0x0025, 0x25 }
  , { 0x0026, 0x26 }
  , { 0x0027, 0x27 }
  , { 0x0028, 0x28 }
  , { 0x0029, 0x29 }
  , { 0x002A, 0x2A }
  , { 0x002B, 0x2B }
  , { 0x002C, 0x2C }
  , { 0x002D, 0x2D }
  , { 0x002E, 0x2E }
  , { 0x002F, 0x2F }
  , { 0x0030, 0x30 }
  , { 0x0031, 0x31 }
  , { 0x0032, 0x32 }
  , { 0x0033, 0x33 }
  , { 0x0034, 0x34 }
  , { 0x0035, 0x35 }
  , { 0x0036, 0x36 }
  , { 0x0037, 0x37 }
  , { 0x0038, 0x38 }
  , { 0x0039, 0x39 }
  , { 0x003A, 0x3A }
  , { 0x003B, 0x3B }
  , { 0x003C, 0x3C }
  , { 0x003D, 0x3D }
  , { 0x003E, 0x3E }
  , { 0x003F, 0x3F }
  , { 0x0040, 0x40 }
  , { 0x0041, 0x41 }
  , { 0x0042, 0x42 }
  , { 0x0043, 0x43 }
  , { 0x0044, 0x44 }
  , { 0x0045, 0x45 }
  , { 0x0046, 0x46 }
  , { 0x0047, 0x47 }
  , { 0x0048, 0x48 }
  , { 0x0049, 0x49 }
  , { 0x004A, 0x4A }
  , { 0x004B, 0x4B }
  , { 0x004C, 0x4C }
  , { 0x004D, 0x4D }
  , { 0x004E, 0x4E }
  , { 0x004F, 0x4F }
  , { 0x0050, 0x50 }
  , { 0x0051, 0x51 }
  , { 0x0052, 0x52 }
  , { 0x0053, 0x53 }
  , { 0x0054, 0x54 }
  , { 0x0055, 0x55 }
  , { 0x0056, 0x56 }
  , { 0x0057, 0x57 }
  , { 0x0058, 0x58 }
  , { 0x0059, 0x59 }
  , { 0x005A, 0x5A }
  , { 0x005B, 0x5B }
  , { 0x005C, 0x5C }
  , { 0x005D, 0x5D }
  , { 0x005E, 0x5E }
  , { 0x005F, 0x5F }
  , { 0x0060, 0x60 }
  , { 0x0061, 0x61 }
  , { 0x0062, 0x62 }
  , { 0x0063, 0x63 }
  , { 0x0064, 0x64 }
  , { 0x0065, 0x65 }
  , { 0x0066, 0x66 }
  , { 0x0067, 0x67 }
  , { 0x0068, 0x68 }
  , { 0x0069, 0x69 }
  , { 0x006A, 0x6A }
  , { 0x006B, 0x6B }
  , { 0x006C, 0x6C }
  , { 0x006D, 0x6D }
  , { 0x006E, 0x6E }
  , { 0x006F, 0x6F }
  , { 0x0070, 0x70 }
  , { 0x0071, 0x71 }
  , { 0x0072, 0x72 }
  , { 0x0073, 0x73 }
  , { 0x0074, 0x74 }
  , { 0x0075, 0x75 }
  , { 0x0076, 0x76 }
  , { 0x0077, 0x77 }
  , { 0x0078, 0x78 }
  , { 0x0079, 0x79 }
  , { 0x007A, 0x7A }
  , { 0x007B, 0x7B }
  , { 0x007C, 0x7C }
  , { 0x007D, 0x7D }
  , { 0x007E, 0x7E }
  , { 0x007F, 0x7F }
  , { 0x0081, 0x81 }
  , { 0x008D, 0x8D }
  , { 0x008F, 0x8F }
  , { 0x0090, 0x90 }
  , { 0x009D, 0x9D }
  , { 0x00A0, 0xA0 }
  , { 0x00A1, 0xA1 }
  , { 0x00A2, 0xA2 }
  , { 0x00A3, 0xA3 }
  , { 0x00A4, 0xA4 }
  , { 0x00A5, 0xA5 }
  , { 0x00A6, 0xA6 }
  , { 0x00A7, 0xA7 }
  , { 0x00A8, 0xA8 }
  , { 0x00A9, 0xA9 }
  , { 0x00AA, 0xAA }
  , { 0x00AB, 0xAB }
  , { 0x00AC, 0xAC }
  , { 0x00AD, 0xAD }
  , { 0x00AE, 0xAE }
  , { 0x00AF, 0xAF }
  , { 0x00B0, 0xB0 }
  , { 0x00B1, 0xB1 }
  , { 0x00B2, 0xB2 }
  , { 0x00B3, 0xB3 }
  , { 0x00B4, 0xB4 }
  , { 0x00B5, 0xB5 }
  , { 0x00B6, 0xB6 }
  , { 0x00B7, 0xB7 }
  , { 0x00B8, 0xB8 }
  , { 0x00B9, 0xB9 }
  , { 0x00BA, 0xBA }
  , { 0x00BB, 0xBB }
  , { 0x00BC, 0xBC }
  , { 0x00BD, 0xBD }
  , { 0x00BE, 0xBE }
  , { 0x00BF, 0xBF }
  , { 0x00C0, 0xC0 }
  , { 0x00C1, 0xC1 }
  , { 0x00C2, 0xC2 }
  , { 0x00C3, 0xC3 }
  , { 0x00C4, 0xC4 }
  , { 0x00C5, 0xC5 }
  , { 0x00C6, 0xC6 }
  , { 0x00C7, 0xC7 }
  , { 0x00C8, 0xC8 }
  , { 0x00C9, 0xC9 }
  , { 0x00CA, 0xCA }
  , { 0x00CB, 0xCB }
  , { 0x00CC, 0xCC }
  , { 0x00CD, 0xCD }
  , { 0x00CE, 0xCE }
  , { 0x00CF, 0xCF }
  , { 0x00D0, 0xD0 }
  , { 0x00D1, 0xD1 }
  , { 0x00D2, 0xD2 }
  , { 0x00D3, 0xD3 }
  , { 0x00D4, 0xD4 }
  , { 0x00D5, 0xD5 }
  , { 0x00D6, 0xD6 }
  , { 0x00D7, 0xD7 }
  , { 0x00D8, 0xD8 }
  , { 0x00D9, 0xD9 }
  , { 0x00DA, 0xDA }
  , { 0x00DB, 0xDB }
  , { 0x00DC, 0xDC }
  , { 0x00DD, 0xDD }
  , { 0x00DE, 0xDE }
  , { 0x00DF, 0xDF }
  , { 0x00E0, 0xE0 }
  , { 0x00E1, 0xE1 }
  , { 0x00E2, 0xE2 }
  , { 0x00E3, 0xE3 }
  , { 0x00E4, 0xE4 }
  , { 0x00E5, 0xE5 }
  , { 0x00E6, 0xE6 }
  , { 0x00E7, 0xE7 }
  , { 0x00E8, 0xE8 }
  , { 0x00E9, 0xE9 }
  , { 0x00EA, 0xEA }
  , { 0x00EB, 0xEB }
  , { 0x00EC, 0xEC }
  , { 0x00ED, 0xED }
  , { 0x00EE, 0xEE }
  , { 0x00EF, 0xEF }
  , { 0x00F0, 0xF0 }
  , { 0x00F1, 0xF1 }
  , { 0x00F2, 0xF2 }
  , { 0x00F3, 0xF3 }
  , { 0x00F4, 0xF4 }
  , { 0x00F5, 0xF5 }
  , { 0x00F6, 0xF6 }
  , { 0x00F7, 0xF7 }
  , { 0x00F8, 0xF8 }
  , { 0x00F9, 0xF9 }
  , { 0x00FA, 0xFA }
  , { 0x00FB, 0xFB }
  , { 0x00FC, 0xFC }
  , { 0x00FD, 0xFD }
  , { 0x00FE, 0xFE }
  , { 0x00FF, 0xFF }
  , { 0x0152, 0x8C }
  , { 0x0153, 0x9C }
  , { 0x0160, 0x8A }
  , { 0x0161, 0x9A }
  , { 0x0178, 0x9F }
  , { 0x017D, 0x8E }
  , { 0x017E, 0x9E }
  , { 0x0192, 0x83 }
  , { 0x02C6, 0x88 }
  , { 0x02DC, 0x98 }
  , { 0x2013, 0x96 }
  , { 0x2014, 0x97 }
  , { 0x2018, 0x91 }
  , { 0x2019, 0x92 }
  , { 0x201A, 0x82 }
  , { 0x201C, 0x93 }
  , { 0x201D, 0x94 }
  , { 0x201E, 0x84 }
  , { 0x2020, 0x86 }
  , { 0x2021, 0x87 }
  , { 0x2022, 0x95 }
  , { 0x2026, 0x85 }
  , { 0x2030, 0x89 }
  , { 0x2039, 0x8B }
  , { 0x203A, 0x9B }
  , { 0x20AC, 0x80 }
  , { 0x2122, 0x99 }
};

#include <search.h>
#include <stdio.h>
int compFunc(const void* p1, const void* p2)
{
    const XlatRecord* rec1 = (const XlatRecord*)p1;
    const XlatRecord* rec2 = (const XlatRecord*)p2;

    return (int)rec1->ebcdicCh - (int)rec2->ebcdicCh;
}

class SpitOutTable
{
public :
    SpitOutTable()
    {
        spitOutTable();
    }

    void spitOutTable()
    {
        qsort
        (
            gXlatTable
            , 256
            , sizeof(XlatRecord)
            , compFunc
        );

        unsigned int curValue = 0;
        for (unsigned int index = 0; index < 256; index++)
        {
            if (gXlatTable[index].ebcdicCh == curValue)
            {
                printf("0x%04X\n", (unsigned int)gXlatTable[index].xmlCh);
                curValue++;
            }
             else
            {
                // We've hit a gap, so catch up
                while (curValue < gXlatTable[index].ebcdicCh)
                {
                    printf("0xFFFF\n");
                    curValue++;
                }
            }
        }
    }
};

SpitOutTable kicker;

#endif

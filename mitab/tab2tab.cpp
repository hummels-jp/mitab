/**********************************************************************
 * $Id: tab2tab.cpp,v 1.1 1999-09-26 14:59:38 daniel Exp $
 *
 * Name:     tab2tab.cpp
 * Project:  MapInfo TAB format Read/Write library
 * Language: C++
 * Purpose:  Copy features from a .TAB dataset to a new one.
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1999, Daniel Morissette
 *
 * All rights reserved.  This software may be copied or reproduced, in
 * all or in part, without the prior written consent of its author,
 * Daniel Morissette (danmo@videotron.ca).  However, any material copied
 * or reproduced must bear the original copyright notice (above), this 
 * original paragraph, and the original disclaimer (below).
 * 
 * The entire risk as to the results and performance of the software,
 * supporting text and other information contained in this file
 * (collectively called the "Software") is with the user.  Although 
 * considerable efforts have been used in preparing the Software, the 
 * author does not warrant the accuracy or completeness of the Software.
 * In no event will the author be liable for damages, including loss of
 * profits or consequential damages, arising out of the use of the 
 * Software.
 * 
 **********************************************************************
 *
 * $Log: tab2tab.cpp,v $
 * Revision 1.1  1999-09-26 14:59:38  daniel
 * Implemented write support
 *
 **********************************************************************/


#include "mitab.h"
#include <ctype.h>

static int Tab2Tab(const char *pszSrcFname, const char *pszDstFname);


#define TAB2TAB_USAGE "Usage: tab2tab <src_filename> <dst_filename>\n"

/**********************************************************************
 *                          main()
 *
 **********************************************************************/
int main(int argc, char *argv[])
{
    const char  *pszSrcFname, *pszDstFname;

/*---------------------------------------------------------------------
 *      Read program arguments.
 *--------------------------------------------------------------------*/
    if (argc<3)
    {
        printf("%s", TAB2TAB_USAGE);
        return 1;
    }
    else
    {
        pszSrcFname = argv[1];
        pszDstFname = argv[2];
    }
    

    return Tab2Tab(pszSrcFname, pszDstFname);
}


/**********************************************************************
 *                          Tab2Tab()
 *
 * Copy features from source dataset to a new dataset
 **********************************************************************/
static int Tab2Tab(const char *pszSrcFname, const char *pszDstFname)
{
    TABFile  oSrcFile, oDstFile;
    int      nFeatureId;
    TABFeature *poFeature;

    /*---------------------------------------------------------------------
     * Try to open source file
     *--------------------------------------------------------------------*/
    if (oSrcFile.Open(pszSrcFname, "rb") != 0)
    {
        printf("Failed to open %s\n", pszSrcFname);
        return -1;
    }

    /*---------------------------------------------------------------------
     * Try to open destination file
     *--------------------------------------------------------------------*/
    if (oDstFile.Open(pszDstFname, "wb") != 0)
    {
        printf("Failed to open %s\n", pszDstFname);
        return -1;
    }

    // __TODO__ Set real bounds
    oDstFile.SetBounds(-1000, -1000, 1000, 1000);

    /*---------------------------------------------------------------------
     * Copy objects until EOF is reached
     *--------------------------------------------------------------------*/
    nFeatureId = -1;
    while ( (nFeatureId = oSrcFile.GetNextFeatureId(nFeatureId)) != -1 )
    {
        poFeature = oSrcFile.GetFeatureRef(nFeatureId);
        if (poFeature)
        {
//            poFeature->DumpReadable(stdout);
//            poFeature->DumpMIF();
            oDstFile.SetFeature(poFeature);
        }
        else
            break;      // GetFeatureRef() failed: Abort the loop
    }

    /*---------------------------------------------------------------------
     * Cleanup and exit.
     *--------------------------------------------------------------------*/
    oDstFile.Close();

    oSrcFile.Close();

    return 0;
}


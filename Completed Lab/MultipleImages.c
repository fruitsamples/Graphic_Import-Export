// Graphics Importer and Exporter Samples// This example demonstrates how to display multiple images// Originally written by Sam Bushell for QuickTime "Live" '99// WWDC 2000 Introduction to QuickTime#include "MacShell.h"void MultipleImage( void ){	OSErr err = noErr;	Handle hOpenTypeList = NewHandle(0);	long  numTypes = 0;	FSSpec	theFSSpec;	GraphicsImportComponent importer = 0;	Rect naturalBounds, windowBounds;	WindowPtr window = NULL;	unsigned long imageCount, imageIndex;	ImageDescriptionHandle desc = NULL;	MatrixRecord matrix;	BuildGraphicsImporterValidFileTypes( hOpenTypeList, &numTypes );	HLock( hOpenTypeList );		err = GetOneFileWithPreview(numTypes, (OSTypePtr)*hOpenTypeList, &theFSSpec, NULL);	DisposeHandle( hOpenTypeList );	if ( err ) return;		// locate and open a graphics importer component	err = GetGraphicsImporterForFile( &theFSSpec, &importer );		// get the native size of the image associated with the importer	err = GraphicsImportGetNaturalBounds( importer, &naturalBounds );	windowBounds = naturalBounds;	OffsetRect( &naturalBounds, 10, 45 );	window = NewCWindow( NULL, &naturalBounds, "\pMultiple Images", true, documentProc, (WindowPtr)-1, true, 0);		// set the graphics port for drawing	err = GraphicsImportSetGWorld( importer, GetWindowPort(window), NULL );		// ask the graphics importer how many images there are in this file	err = GraphicsImportGetImageCount( importer, &imageCount );		for( imageIndex = 1; imageIndex <= imageCount; imageIndex++ ) {			// set the index value for the image we want to draw		err = GraphicsImportSetImageIndex( importer, imageIndex );				// each image in the file can have different dimensions, depth, etc.		// if the image has an alpha, use StraightAlpha graphics mode to draw		err = GraphicsImportGetImageDescription( importer, &desc );		if( (*desc)->depth == 32 )			err = GraphicsImportSetGraphicsMode( importer, graphicsModeStraightAlpha, NULL );		else			err = GraphicsImportSetGraphicsMode( importer, ditherCopy, NULL );		DisposeHandle( (Handle)desc );				// set up the matrix		SetIdentityMatrix( &matrix );		GraphicsImportGetDefaultMatrix( importer, &matrix );		err = GraphicsImportSetMatrix( importer, &matrix );		SetPortWindowPort( window );		EraseRect( &windowBounds );				// draw the image		err = GraphicsImportDraw( importer );		pause();	}		// draw the images again but this type don't erase	for( imageIndex = 2; imageIndex <= imageCount; imageIndex++ ) {		err = GraphicsImportSetImageIndex( importer, imageIndex );				err = GraphicsImportGetImageDescription( importer, &desc );		if( (*desc)->depth == 32 )			err = GraphicsImportSetGraphicsMode( importer, graphicsModeStraightAlpha, NULL );		else			err = GraphicsImportSetGraphicsMode( importer, ditherCopy, NULL );		DisposeHandle( (Handle)desc );				SetIdentityMatrix( &matrix );		GraphicsImportGetDefaultMatrix( importer, &matrix );		err = GraphicsImportSetMatrix( importer, &matrix );		err = GraphicsImportDraw( importer );		pause();	}	CloseComponent( importer );}
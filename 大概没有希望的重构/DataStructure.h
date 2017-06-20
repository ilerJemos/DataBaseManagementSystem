#include<minwinbase.h>

#pragma  once

//////////////////////////////////////////////////////////////////////////
// The definition of basic data types
typedef int INTEGER;			// Integer
typedef bool BOOLE;				// Boolean type
typedef double DOUBLE;			// Floating-point number
typedef char VARCHAR[256];		// String type
typedef SYSTEMTIME DATETIME;	// Date time type

//////////////////////////////////////////////////////////////////////////
// The definition of block structure

// Database block structure
struct DatabaseBlock{
	BOOLE type;			// Database type£ºtrue is system database£¬false is custom database.
	VARCHAR name;		// Database name
	VARCHAR filepath;	// Database file path
	DATETIME crtime;	// Creation time
};
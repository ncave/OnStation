// This file has some of the user defined types and constraints.
//It is included in stdafx.h, so changes here will effect the entire
//project.

//Common classes we reference in many places
class CC3dView;
class CC3dDoc;
class CMainFrame;


enum VIEWTYPE 		{VT_FREEFORM,VT_MIXED,VT_TOP,VT_PROFILE};
enum STATIONSHOW {SHOW_NONE,SHOW_INTERSECT,SHOW_ALL};
enum PASSAGE_TYPE {PT_LINE_PLOT,PT_HEIGHTS,PT_WIDTHS,PT_CROSS_SECTIONS,PT_FULL_PASSAGES_SKINNY,PT_FULL_PASSAGES};
enum LIGHTINGMODIFIER {TOP_LIGHTING,SIDE_LIGHTING,BOTTOM_LIGHTING,NATURAL_LIGHTING};

#define COLOR_INACTIVE  (COLORREF)-2		//don't use for calculations
#define COLOR_INVISIBLE (COLORREF)-1		//don't show.  Still use for calculations

#define CLOSURE_RGB		(COLORREF)-3

//number of pixels in the Surface/Wall/etc.. columns in
//the shot edit grid control
#define CHECK_COL_WIDTH	10

//width of the column that holds comment information
#define COMMENTS_COL_WIDTH	350
#define REGULAR_COL_WIDTH	30

//Width of the column which contains the survey from and to names
#define NAME_COLUMN_WIDTH	40
#define NUMBER_COLUMN_WIDTH	30


//Number of levels of undo while editing the grid control
#define GRID_UNDO_DEPTH	64

enum PREFEREDUNITS {METERS,FEET,METERSANDKM,FEETANDMILES};

//multiply the fixed point scale by this.  The result
//will be in cm, so 1.0 = 1cm fixed points etc..
#define FIXED_POINT_RADIUS		0.5f	

//Defines the maximum and minimum Meters/cm allowed
#define MIN_ZOOM			1.0f		//1 m/cm
#define MAX_ZOOM			15000.0f	//15000 m/cm

//These two numbers are used in junctions to determine how close the incoming
//line comes to the center.  The large the value of CENTER_WEIGHT the small the
//"rooms" around a junction become.
#define CENTER_WEIGHT		0.75f
#define OUTSIDE_WEIGHT		0.25f	//should be 1.0f-CENTER_WEIGHT

#define MAX_DEPTH_COLORS		15
#define MAX_AGE_COLORS			15
#define DEPTH_ROUNDING			10		//number of meters to round to
#define MAX_STATION_NAME_LENGTH		16
#define MAX_FOLDER_NAME_LENGTH	64

#define MAX_REGULAR_COLOR_SCHEMES    8
//Reserved color scheme indexes for these possibilities
#define COLORSCHEME_DEPTH (MAX_REGULAR_COLOR_SCHEMES)
#define COLORSCHEME_AGE	(COLORSCHEME_DEPTH+1)
#define COLORSCHEME_BLUNDER (COLORSCHEME_DEPTH+2)
#define TOTAL_COLOR_SCHEMES	COLORSCHEME_BLUNDER+1

//Maximum entries per survey (This is all that the DATA PUMP can deal with)
#define MAX_GRID_ENTRIES	512

#define THICK_PRINTER_LINES		4

//Maximum levels deep we can go while calculating the GWORLD.
//Nodetree.cpp uses this.
#define MAX_GWORLD_QDEPTH		8000

#define ROTATION_DAMPER			0.3f			//each pixel moved rotates us this many degrees.


//Width in pixels of the pane that gives status information for the current position
#define STATUS_POS_PANE_WIDTH	60
#define STATUS_SIZE_PANE_WIDTH	70
#define STATUS_SPIN_PANE_WIDTH	60
#define STATUS_TILT_PANE_WIDTH 60

//1.0 was original version
//1.1 - Added survey field order preservation
//2.0 - 1.1 was renamed to 2.0, file number did not changed
//2.1 - changed comment handling and tie in architecture
#define CURRENT_VERSION			2.1f

//Width of the ruler in pixels
#define RULER_WIDTH			25
#define VIEWTAB_HEIGHT		15

//Defines for various ornaments on the printed page
//
#define SIDE_RULER_SMALLTICK	25
#define SIDE_RULER_MEDIUMTICK	50
#define SIDE_RULER_LARGESTTICK	75
#define SIDE_RULER_BORDER		125

#define COMPASS_WIDTH			300
#define COMPASS_RULER_SPACING	100
#define COMPASS_YSTART			95
#define SCALEBAR_YSTART			225
#define SCALEBARTITLE_YSTART	25
#define SCALEBARLRTEXT_YSTART	125
#define BOTTOM_WIDGETS_BORDER	COMPASS_WIDTH+COMPASS_YSTART


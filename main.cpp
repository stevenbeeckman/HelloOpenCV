/*
* Copyright (c) 2011-2012 Research In Motion Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <bps/bps.h>
#include <bps/event.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <fcntl.h>
#include <screen/screen.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

static bool shutdown;

#define w 400

using namespace cv;

/// Function headers
void MyEllipse( Mat img, double angle );
void MyFilledCircle( Mat img, Point center );
void MyPolygon( Mat img );
void MyLine( Mat img, Point start, Point end );

static void
handle_screen_event(bps_event_t *event)
{
    int screen_val;

    screen_event_t screen_event = screen_event_get_event(event);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

    switch (screen_val) {
    case SCREEN_EVENT_MTOUCH_TOUCH:
        fprintf(stderr,"Touch event");
        break;
    case SCREEN_EVENT_MTOUCH_MOVE:
        fprintf(stderr,"Move event");
        break;
    case SCREEN_EVENT_MTOUCH_RELEASE:
        fprintf(stderr,"Release event");
        break;
    default:
        break;
    }
    fprintf(stderr,"\n");
}

static void
handle_navigator_event(bps_event_t *event) {
    switch (bps_event_get_code(event)) {
    case NAVIGATOR_SWIPE_DOWN:
        fprintf(stderr,"Swipe down event");
        break;
    case NAVIGATOR_EXIT:
        fprintf(stderr,"Exit event");
        shutdown = true;
        break;
    default:
        break;
    }
    fprintf(stderr,"\n");
}

static void
handle_event()
{
    int domain;

    bps_event_t *event = NULL;
    if (BPS_SUCCESS != bps_get_event(&event, -1)) {
        fprintf(stderr, "bps_get_event() failed\n");
        return;
    }
    if (event) {
        domain = bps_event_get_domain(event);
        if (domain == navigator_get_domain()) {
            handle_navigator_event(event);
        } else if (domain == screen_get_domain()) {
            handle_screen_event(event);
        }
    }
}

/// Function Declaration

/**
 * @function MyEllipse
 * @brief Draw a fixed-size ellipse with different angles
 */
void MyEllipse( Mat img, double angle )
{
  int thickness = 2;
  int lineType = 8;

  ellipse( img,
	   Point( w/2.0, w/2.0 ),
	   Size( w/4.0, w/16.0 ),
	   angle,
	   0,
	   360,
	   Scalar( 255, 0, 0 ),
	   thickness,
	   lineType );
}

/**
 * @function MyFilledCircle
 * @brief Draw a fixed-size filled circle
 */
void MyFilledCircle( Mat img, Point center )
{
  int thickness = -1;
  int lineType = 8;

  circle( img,
	  center,
	  w/32.0,
	  Scalar( 0, 0, 255 ),
	  thickness,
	  lineType );
}

/**
 * @function MyPolygon
 * @function Draw a simple concave polygon (rook)
 */
void MyPolygon( Mat img )
{
  int lineType = 8;

  /** Create some points */
  Point rook_points[1][20];
  rook_points[0][0] = Point( w/4.0, 7*w/8.0 );
  rook_points[0][1] = Point( 3*w/4.0, 7*w/8.0 );
  rook_points[0][2] = Point( 3*w/4.0, 13*w/16.0 );
  rook_points[0][3] = Point( 11*w/16.0, 13*w/16.0 );
  rook_points[0][4] = Point( 19*w/32.0, 3*w/8.0 );
  rook_points[0][5] = Point( 3*w/4.0, 3*w/8.0 );
  rook_points[0][6] = Point( 3*w/4.0, w/8.0 );
  rook_points[0][7] = Point( 26*w/40.0, w/8.0 );
  rook_points[0][8] = Point( 26*w/40.0, w/4.0 );
  rook_points[0][9] = Point( 22*w/40.0, w/4.0 );
  rook_points[0][10] = Point( 22*w/40.0, w/8.0 );
  rook_points[0][11] = Point( 18*w/40.0, w/8.0 );
  rook_points[0][12] = Point( 18*w/40.0, w/4.0 );
  rook_points[0][13] = Point( 14*w/40.0, w/4.0 );
  rook_points[0][14] = Point( 14*w/40.0, w/8.0 );
  rook_points[0][15] = Point( w/4.0, w/8.0 );
  rook_points[0][16] = Point( w/4.0, 3*w/8.0 );
  rook_points[0][17] = Point( 13*w/32.0, 3*w/8.0 );
  rook_points[0][18] = Point( 5*w/16.0, 13*w/16.0 );
  rook_points[0][19] = Point( w/4.0, 13*w/16.0) ;

  const Point* ppt[1] = { rook_points[0] };
  int npt[] = { 20 };

  fillPoly( img,
	    ppt,
	    npt,
            1,
	    Scalar( 255, 255, 255 ),
	    lineType );
}

/**
 * @function MyLine
 * @brief Draw a simple line
 */
void MyLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = 8;
  line( img,
	start,
	end,
	Scalar( 0, 0, 0 ),
	thickness,
	lineType );
}


int
main(int argc, char **argv)
{
    const int usage = SCREEN_USAGE_NATIVE;

    screen_context_t screen_ctx;
    screen_window_t screen_win;
    screen_buffer_t screen_buf = NULL;
    int rect[4] = { 0, 0, 0, 0 };

    /* Setup the window */
    screen_create_context(&screen_ctx, 0);
    screen_create_window(&screen_win, screen_ctx);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
    screen_create_window_buffers(screen_win, 1);

    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
    screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);

    /* Fill the screen buffer with blue */
    int attribs[] = { SCREEN_BLIT_COLOR, 0xff0000ff, SCREEN_BLIT_END };
    screen_fill(screen_ctx, screen_buf, attribs);
    screen_post_window(screen_win, screen_buf, 1, rect, 0);

    /* Signal bps library that navigator and screen events will be requested */
    bps_initialize();
    screen_request_events(screen_ctx);
    navigator_request_events(0);

    while (!shutdown) {
        /* Handle user input */
        handle_event();
        /// Windows names
         char atom_window[] = "Drawing 1: Atom";
         char rook_window[] = "Drawing 2: Rook";

         /// Create black empty images
         Mat atom_image = Mat::zeros( w, w, CV_8UC3 );
         Mat rook_image = Mat::zeros( w, w, CV_8UC3 );

         /// 1. Draw a simple atom:
         /// -----------------------

         /// 1.a. Creating ellipses
         MyEllipse( atom_image, 90 );
         MyEllipse( atom_image, 0 );
         MyEllipse( atom_image, 45 );
         MyEllipse( atom_image, -45 );

         /// 1.b. Creating circles
         MyFilledCircle( atom_image, Point( w/2.0, w/2.0) );

         /// 2. Draw a rook
         /// ------------------

         /// 2.a. Create a convex polygon
         MyPolygon( rook_image );

         /// 2.b. Creating rectangles
         rectangle( rook_image,
       	     Point( 0, 7*w/8.0 ),
       	     Point( w, w),
       	     Scalar( 0, 255, 255 ),
       	     -1,
       	     8 );

         /// 2.c. Create a few lines
         MyLine( rook_image, Point( 0, 15*w/16 ), Point( w, 15*w/16 ) );
         MyLine( rook_image, Point( w/4, 7*w/8 ), Point( w/4, w ) );
         MyLine( rook_image, Point( w/2, 7*w/8 ), Point( w/2, w ) );
         MyLine( rook_image, Point( 3*w/4, 7*w/8 ), Point( 3*w/4, w ) );
    }

    /* Clean up */
    screen_stop_events(screen_ctx);
    bps_shutdown();
    screen_destroy_window(screen_win);
    screen_destroy_context(screen_ctx);
    return 0;
}


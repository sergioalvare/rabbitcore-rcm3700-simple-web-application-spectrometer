/**********************************************************
   browseled.c
   Z-World, 2003

	This program is used with RCM3700 series controllers
	with prototyping boards.

	Description
	===========
	This program demonstrates a basic controller running a
	WEB page. Two "device LED's" are created with two
	buttons to toggle	them.  Users can browse to the device
	and change the	status of the lights. The LED's on the
	prototyping	board will match the ones on the web page.

   This program is adapted from \Samples\TCPIP\ssi.c.

	Instructions
	============
	1. Make changes below in the configuration section to
	   match your application.
	2. Compile and run this program.
   3. With your WEB browser access the WEB page running on
   	the controller.
   4. View LEDS on Web page and the prototyping board to
   	see that	they match-up when changing them via the
   	WEB page	control button.

**********************************************************/
#class auto
#define TCPCONFIG 1

#define _PRIMARY_STATIC_IP "192.168.0.3"
#define _PRIMARY_NETMASK "255.255.255.0"
#define MY_NAMESERVER "192.168.0.1"
#define MY_GATEWAY "192.168.0.1"


/***********************************
 * Configuration Section           *
 * ---------------------           *
 * All fields in this section must *
 * be altered to match your local  *
 * network settings.               *
 ***********************************/
/*
 * Pick the predefined TCP/IP configuration for this sample.  See
 * LIB\TCPIP\TCP_CONFIG.LIB for instructions on how to set the
 * configuration.
 */

/*
 * TCP/IP modification - reduce TCP socket buffer
 * size, to allow more connections. This can be increased,
 * with increased performance, if the number of sockets
 * are reduced.  Note that this buffer size is split in
 * two for TCP sockets--1024 bytes for send and 1024 bytes
 * for receive.
 */
//#define TCP_BUF_SIZE 2048

/*
 * Web server configuration
 */

/*
 * Define the number of HTTP servers and socket buffers.
 * With tcp_reserveport(), fewer HTTP servers are needed.
 */
//#define HTTP_MAXSERVERS 2
//#define MAX_TCP_SOCKET_BUFFERS 2

/*
 * Our web server as seen from the clients.
 * This should be the address that the clients (netscape/IE)
 * use to access your server. Usually, this is your IP address.
 * If you are behind a firewall, though, it might be a port on
 * the proxy, that will be forwarded to the Rabbit board. The
 * commented out line is an example of such a situation.
 */
//#define REDIRECTHOST		_PRIMARY_STATIC_IP
// #define REDIRECTHOST	"my.host.com:8080"

/********************************
 * End of configuration section *
 ********************************/

/*
 *  REDIRECTTO is used by each ledxtoggle cgi's to tell the
 *  browser which page to hit next.  The default REDIRECTTO
 *  assumes that you are serving a page that does not have
 *  any address translation applied to it.
 *
 */

#define USE_RABBITWEB 1
//#define USE_ZHTML 1

#memmap xmem
#use "dcrtcp.lib"
#use http.lib

#use "dcrtcp.lib"
#use "http.lib"

/*
 *  Notice that we have ximported in the source code for
 *  this program.  This allows us to <!--#include file="ssi.c"-->
 *  in the pages/showsrc.shtml.
 *
 */

#ximport "pagina.zhtml" 	pagina
#ximport "java.js"    	java


/*
 *  In this case the .html is not the first type in the
 *  type table.  This causes the default (no extension)
 *  to assume the shtml_handler.
 *
 */

/* the default for / must be first */
SSPEC_MIMETABLE_START


   	SSPEC_MIME_FUNC(".zhtml", "text/html", zhtml_handler),
	SSPEC_MIME(".js", "text/javascript")


SSPEC_MIMETABLE_END






SSPEC_RESOURCETABLE_START
	SSPEC_RESOURCE_XMEMFILE("/", pagina),
    	SSPEC_RESOURCE_XMEMFILE("/pagina.zhtml", pagina),
//	SSPEC_RESOURCE_XMEMFILE("/index.html", pagina),

	SSPEC_RESOURCE_XMEMFILE("/java.js", java),

SSPEC_RESOURCETABLE_END

              #define tamanio 32
struct espectro_struct {
	// Basic data collection rate is one sample every 15 sec, however this
	// data is decimated (1:48) for the 12 hour history.  This data is
	// read-only to the form.
	int puntosEspectro[tamanio];

	// Following vars are read/write and are controllable via a web form.
	int cota_inferior;				// Lower Y bound for graphical display
	int cota_superior;				// Upper Y bound for graphical display
	///////int interval;			// INTERVAL_20min, INTERVAL_12h
	///////int displ_units;		// UNITS_C, UNITS_F : display units
								//  (this struct data always in deg F)
	///////int curr_setpoint;	// Current setpoint
	///////int enabled;			// Whether thermostat enabled
};

// This is web registered for display
struct espectro_struct espectroWeb;

#web espectroWeb groups=all(ro)



main()
{

	//brdInit();				//initialize board for this demo



   sock_init();
   http_init();
   tcp_reserveport(80);

   while (1)
   {

      http_handler();

	}
}

//#nodebug
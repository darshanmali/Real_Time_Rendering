#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>


using namespace std;

bool bFullscreen =false;
Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;
int giWindowWidth=800;
int giWindowHeight=600;

int main(void)
{
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void uninitialize();
	
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;

//New
	static XFontStruct* pxfontstruct = NULL;	
	static GC gc;
	XGCValues xgcvalue;
	XColor GreanColor;

	char str[40] = "Hello XWindow : Darshan Mali";
	int StringLength;
	int stringWidth;
	int FontHeight;

	//Graphics Context(Software HDC) 
	


	CreateWindow();

	XEvent event;
	KeySym keysym;
	
	while(1)
	{
		XNextEvent(gpDisplay,&event);
		switch(event.type)
		{
			case MapNotify:
			pxfontstruct = XLoadQueryFont(gpDisplay,"FIXED");

			break;
			case KeyPress:
			keysym = XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);

			switch(keysym)
			{
				case XK_Escape:
				uninitialize();
				exit(0);
				break;
				case XK_F:
				case XK_f:
					if(bFullscreen == false)
					{
						ToggleFullscreen();
						bFullscreen = true;
					}
					else
					{
						ToggleFullscreen();
						bFullscreen = false;
					}
				break;
				default:
					break;
			}
			break;
			case ButtonPress:
				switch(event.xbutton.button)
				{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					default:
						break;
				}
				break;
			case MotionNotify:
				break;
			case ConfigureNotify:
				winWidth = event.xconfigure.width;
				winHeight = event.xconfigure.height;
				break;
			case Expose:
			gc = XCreateGC(gpDisplay,gWindow,0,&xgcvalue);

			XSetFont(gpDisplay,gc,pxfontstruct->fid);

			XAllocNamedColor(gpDisplay,gColormap,"green",&GreanColor,&GreanColor);

			XSetForeground(gpDisplay,gc,GreanColor.pixel);
			
			StringLength = strlen(str);

			stringWidth = XTextWidth(pxfontstruct,str,StringLength);

			FontHeight = pxfontstruct->ascent + pxfontstruct->descent;
			
			XDrawString(gpDisplay,gWindow,gc,((winWidth / 2)-(stringWidth / 2)),((winHeight / 2)-(FontHeight / 2)),str,StringLength);


				break;
			case DestroyNotify:
				break;
			case 33:

				XUnloadFont(gpDisplay,pxfontstruct->fid);
				XFreeGC(gpDisplay,gc);
				uninitialize();
				exit(0);
			default:
				break;
		}
	}
	XUnloadFont(gpDisplay,pxfontstruct->fid);
	XFreeGC(gpDisplay,gc);

	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	void uninitialize(void);

	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int defaultDepth;
	int stylemask;

	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL)
	{
		printf("ERROR : Unable to Open X Display.\n Exitting Now...\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);
	
	defaultDepth = DefaultDepth(gpDisplay,defaultScreen);

	gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));
	if(gpXVisualInfo == NULL)
	{
		printf("ERROR : Unable To Allocate Memory for Visual Info .\n Exitting Now..\n");
		uninitialize();
		exit(1);
	}
	
	XMatchVisualInfo(gpDisplay,defaultScreen,defaultDepth,TrueColor,gpXVisualInfo);
	if(gpXVisualInfo == NULL)
	{
		printf("ERROR : Unable TO Get Visual. \n Exitting now...\n");
		uninitialize();
		exit(1);
	}
	
	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
					RootWindow(gpDisplay, gpXVisualInfo->screen),
					gpXVisualInfo->visual,
					AllocNone);
	gColormap = winAttribs.colormap;

	winAttribs.background_pixel = BlackPixel(gpDisplay,defaultScreen);

	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	
	stylemask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

	gWindow = XCreateWindow(gpDisplay,
			RootWindow(gpDisplay,gpXVisualInfo->screen),
			0,
			0,
			giWindowWidth,
			giWindowHeight,
			0,
			gpXVisualInfo->depth,
			InputOutput,
			gpXVisualInfo->visual,
			stylemask,
			&winAttribs);
	if(!gWindow)
	{
		printf("ERROR : Failed To Create Main window.\n Exitting Now..\n");
		uninitialize();
		exit(1);
	}
	
	XStoreName ( gpDisplay, gWindow,"Darshan's : First XWindows");
		
	Atom windowManagerDelete = XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
	
	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	Atom wm_state;
	Atom fullscreen;
	XEvent xev = {0};

	
	wm_state = XInternAtom(gpDisplay,"_NET_WM_STATE",false);
	memset(&xev, 0, sizeof(xev));

	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bFullscreen ? 0:1;
	
	fullscreen = XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",false);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay,
			RootWindow(gpDisplay,gpXVisualInfo->screen),
			false,
			StructureNotifyMask,
			&xev);
}

void uninitialize(void)
{

	if(gWindow)
	{
		XDestroyWindow(gpDisplay,gWindow);
	}
	if(gColormap)
	{
		XFreeColormap(gpDisplay,gColormap);
	}
	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}
	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}


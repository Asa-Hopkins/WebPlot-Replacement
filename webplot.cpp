#include "CImg.h"
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <time.h>
#include <fstream>
#define abs(x) (x>0 ? x : -(x))
#define sgn(x) (x>0) - (x<0)
using namespace cimg_library;

#ifndef cimg_imagepath
#define cimg_imagepath ""
#endif

bool fileExists(const std::string& filename)
{
  struct stat buf;
  return stat(filename.c_str(), &buf)!=-1;
}

int main(int argc, char **argv) {
  const int r = 25; // half width of box
  const int scale = 8; //size of zoom view
  CImg<unsigned char> image(argv[1]),graph(argv[1]), zoom(2*r,2*r,1,3), crop(2&r,2*r,1,3);
  const unsigned char grey[] = { 50, 50, 50 };
  const unsigned char white[] = { 255, 255, 255 };
  int oldmouse[2];
  int click[2];
  int end[2];
  int i = 0;
  double edge[2];
  double axes[2] = {-1,-1};
  double limits[2] = {atof(argv[2]), atof(argv[3])};
  int line[3] = {0,0,0}; // line colour
  int colours[2];
  CImgDisplay disp0(image,"Graph"), disp1(scale*2*r,scale*2*r,"Zoomed window");
  bool update = true;
  bool lock = false;
  std::cout.precision(10);
  while (!disp0.is_closed() && !disp1.is_closed()){
    if (disp0.button()){
      lock = !lock;
      update = true;
      while (disp0.button()){usleep(10000);}
    }

    if ((oldmouse[0]!=disp0.mouse_x() or oldmouse[1]!=disp0.mouse_y()) and !lock){
      oldmouse[0]=disp0.mouse_x(); oldmouse[1]=disp0.mouse_y();
      update = true;
    }
    
    if (disp1.button()){
      click[0] = disp1.mouse_x()/scale; click[1] = disp1.mouse_y()/scale;
      crop = graph;
      crop.crop(oldmouse[0]-r,oldmouse[1]-r,oldmouse[0]+r,oldmouse[1]+r);
      while (disp1.button()){
	zoom = crop;
	end[0] = disp1.mouse_x()/scale; end[1]=disp1.mouse_y()/scale;
        zoom.draw_line(click[0],click[1],end[0],end[1],grey);
        zoom.display(disp1);
        usleep(10000);
      }
    colours[0] = crop(click[0],click[1],0); colours[1] = crop(end[0],end[1],0);
    int step = sgn(click[1]-end[1]);
    if (step==0){break;}
    i=0;
    while (crop(click[0],click[1]+i,0)==colours[0]){i-=step;}
    edge[0] = click[1]+i+oldmouse[1]-r-step*(crop(click[0],click[1]+i,0)-line[0])/double(colours[0]-line[0]);
    i=0;
    while (crop(click[0],end[1]+i,0)==colours[0]){i+=step;}
    edge[1] = end[1]+i+oldmouse[1]-r+1+step*(crop(click[0],end[1]+i,0)-line[0])/double(colours[0]-line[0]);
    if (axes[0]==-1){axes[0] = (edge[0]+edge[1])/2;
      std::cout << "edges are " << edge[0] << " " << edge[1] << "\n";}
    else if (axes[1]==-1){axes[1] = (edge[0]+edge[1])/2;
      std::cout << "edges are " << edge[0] << " " << edge[1] << "\n";}
    else{
      std::cout << "edge position is " << (edge[0]+edge[1])/2 << " so value is " << limits[0] + (limits[1] - limits[0])*abs((edge[0]+edge[1])/2-axes[0])/abs(axes[0]-axes[1]) << "\n";
      }
    }

    if (update){
      image.draw_image(0,0,0,0,graph);
      image.draw_line(disp0.mouse_x()-r,disp0.mouse_y()+r,disp0.mouse_x()+r,disp0.mouse_y()+r,grey);
      image.draw_line(disp0.mouse_x()-r,disp0.mouse_y()-r,disp0.mouse_x()+r,disp0.mouse_y()-r,grey);
      image.draw_line(disp0.mouse_x()-r,disp0.mouse_y()-r,disp0.mouse_x()-r,disp0.mouse_y()+r,grey);
      image.draw_line(disp0.mouse_x()+r,disp0.mouse_y()-r,disp0.mouse_x()+r,disp0.mouse_y()+r,grey);
      zoom = graph.get_crop(disp0.mouse_x()-r,disp0.mouse_y()-r,disp0.mouse_x()+r,disp0.mouse_y()+r);
      image.display(disp0);
      zoom.display(disp1);
      update = false;
    }

    usleep(10000);
  }
    return 0;
}


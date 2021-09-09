#ifndef CV_H
#define CV_H

class CV {

public:

  // CV - Hoops
  int askHoops() {return 0;}
  int askHoopX(int n) {return 0;} // X position on camera
  int askHoopY(int n) {return 0;} // Y position on camera
  int askHoopH(int n) {return 0;} // Vertical Radius
  int askHoopW(int n) {return 0;} // Horizontal Radius

  // CV - QR
  int askQRs() {return 0;}
  int askQRX(int n) {return 0;}
  int askQRY(int n) {return 0;}
  int askQRsize(int n) {return 0;}

};

#endif /* end of include guard: CV_H */

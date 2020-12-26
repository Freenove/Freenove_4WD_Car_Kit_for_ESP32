import controlP5.*; //<>//
ControlP5 cp5;
int nx =16;
int ny = 8;
int []cn=new int[20];

int []ax=new int[2600*2];
int []ay=new int[2600*2];

Matrix matrix1;
Matrix matrix2;
Matrix matrix3;
Button B1, B2, B3;
int mn;
int timect=0;
int te=20;
int tn=0;
boolean state=false;
int barn;
int matrixplay=0;
int speed;
int moven=0;

void setup() {
  size(500, 460);
  mn=nx*ny;
  cp5 = new ControlP5(this);
  cp5.addButtonBar("bar")
    .setPosition(50, 40)
    .setSize(400, 20)
    .addItems(split("a b c d e f g h i j k l m n o p q r s t", " "))
    ;
  matrix1=cp5.addMatrix("myMatrix1")
    .setPosition(50, 100)
    .setSize(15*nx, 15*ny)
    .setGrid(nx, ny)
    .setGap(1, 1)
    .setInterval(0)
    .setMode(ControlP5.MULTIPLES)
    .setColorBackground(color(120))
    .setBackground(color(40))
    ;
  matrix1.stop();

  matrix2=cp5.addMatrix("myMatrix2")
    .setPosition(50, 150+15*ny)
    .setSize(15*nx, 15*ny)
    .setGrid(nx, ny)
    .setGap(1, 1)
    .setInterval(0)
    .setMode(ControlP5.MULTIPLES)
    .setColorBackground(color(120))
    .setBackground(color(40))
    ;
  matrix2.stop();

  //cp5.addColorWheel("colorplate", 550, 50, 200 ).setRGB(color(128, 0, 255))
  //   .setColorBackground(color(40));

  cp5.addSlider("SPEED")
    .setPosition(350, 350)
    .setRange(0, 60)
    .setSize(100, 20)
    .setValue(20)
    ;
  cp5.getController("SPEED").getCaptionLabel().align(ControlP5.CENTER, ControlP5.BOTTOM_OUTSIDE).setPaddingX(0);

  // use setMode to change the cell-activation which by 
  // default is ControlP5.SINGLE_ROW, 1 active cell per row, 
  // but can be changed to ControlP5.SINGLE_COLUMN or ControlP5.MULTIPLES

  int bt=25;
  int bpx=350;
  B1=cp5.addButton("PREVIOUS")
    .setValue(0)
    .setPosition(350, 90)
    .setSize(100, bt)
    ;
  cp5.addButton("CLEAR")
    .setValue(0)
    .setPosition(350, 130)
    .setSize(100, bt)
    ;
  B2=cp5.addButton("ON_ALL")
    .setValue(100)
    .setPosition(bpx, 170)
    .setSize(100, bt)
    ;
  B3=cp5.addButton("CLEAR_ALL")
    .setPosition(bpx, 210)
    .setSize(100, bt)
    .setValue(0)
    ;
  cp5.addButton("BlinkShow")
    .setPosition(bpx, 280)
    .setSize(100, bt)
    .setValue(0)
    ;
  cp5.addButton("MoveShow")
    .setPosition(bpx, 310)
    .setSize(100, bt)
    .setValue(0)
    ;
  cp5.addButton("GETCODE")
    .setPosition(bpx, 400)
    .setSize(100, bt)
    .setValue(0)
    ;
  noStroke();
  smooth();
}

void draw() {
  background(30);
  getmatrixdata();
  if (matrixplay==1) {
    showmatrixdata1();
  }
  if (matrixplay==2) {
    showmatrixdata2();
  }
  if (timect>=20*te) {
    timect=0;
  }
  timect++;
}

void showmatrixdata1() {
  te=speed;
  if (timect==(tn+1)*te) {
    matrix2.clear();
    for (int i=mn*tn; i<cn[tn]+mn*tn; i++) {
      matrix2.set(ax[i], ay[i], true);
    }
    tn++;
    if (cn[tn]==0||tn>19) {
      timect=0;
      tn=0;
    }
  }
}

void showmatrixdata2() {
  if (timect==speed) {
    matrix2.clear();

    for (int i=mn*tn; i<cn[tn]+mn*tn; i++) {
      if (ax[i]-moven>=0) {
        matrix2.set(ax[i]-moven, ay[i], true);
      }
    }
    for (int i=mn*(tn+1); i<cn[tn+1]+mn*(tn+1); i++) {
      if ((ax[i]+nx-moven>=0)&&(ax[i]+nx-moven<nx)) {
        matrix2.set(ax[i]+nx-moven, ay[i], true);
      }
    }

    moven++;
    if (moven==nx) {
      moven=0;
      tn++;
    }
    if (cn[tn]==0||tn>19) {
      tn=0;
    }
    timect=0;
  }
}

void getmatrixdata() {
  cn[barn]=0;
  for (int x=0; x<nx; x++) {
    for (int y=0; y<ny; y++) {
      state= matrix1.get(x, y);
      if ( state==true) {
        ax[cn[barn]+barn*mn]=x;
        ay[cn[barn]+barn*mn]=y;
        cn[barn]++;
      }
    }
  }
}

void bar(int n) {
  barn=n;
  matrix1.clear();
  for (int i=mn*barn; i<cn[n]+mn*barn; i++) {
    matrix1.set(ax[i], ay[i], true);
  }
}

void PREVIOUS() {
  matrix1.clear();
  if (barn>0) {
    for (int i=mn*(barn-1); i<cn[barn-1]+mn*(barn-1); i++) {
      matrix1.set(ax[i], ay[i], true);
    }
  }
}
void ON_ALL() {
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
      matrix1.set(i, j, true);
    }
  }
}

void BlinkShow() {
  timect=0;
  if (matrixplay==1) {
    matrixplay=0;
  } else {
    matrixplay=1;
  }
}
void MoveShow() {
  timect=0;
  if (matrixplay==2) {
    matrixplay=0;
  } else {
    matrixplay=2;
  }
}

void GETCODE() {
  println("------------------------------get HEX------------------------------------------------------------------");
  int cnn=0;
  for (int i=0; i<20; i++) {
    if (cn[i]==0) {
      break;
    }
    cnn++;
  }
  String hex;
  int bn=0;
  for (int k=0; k<cnn; k++ ) {
    for (int j=0; j<ny; j++) {
      for (int i= mn*k; i<cn[k]+mn*k; i++) {
        if (ay[i]==j ) {
          bn=bn+(int)pow(2, (7-ax[i]));      
        }
      }
      hex=Integer.toHexString((int)bn).toUpperCase();
      if (bn<10) {
        print("0x0"+hex+",");
      } else {      
        print("0x"+hex+",");
      }
      bn=0;
    }

  }
      println("---------x");
  for (int k=0; k<cnn; k++ ) {
    for (int j=0; j<ny; j++) {
      for (int i= mn*k; i<cn[k]+mn*k; i++) {
        if (ay[i]==j &&ax[i]>7) {
          bn=bn+(int)pow(2, (15-ax[i]));      
        }
      }
      hex=Integer.toHexString((int)bn).toUpperCase();
      if (bn<10) {
        print("0x0"+hex+",");
      } else {      
        print("0x"+hex+",");
      }
      bn=0;
    }
   
  } 
   println("---------y");
}


void CLEAR() {
  matrix1.clear();
}

void CLEAR_ALL() {
  matrix1.clear();
  for (int i=0; i<20; i++) {
    cn[i]=0;
  }
}
void SPEED(int n) {
  speed=n;
  timect=0;
}

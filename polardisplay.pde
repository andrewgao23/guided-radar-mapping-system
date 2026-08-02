import processing.serial.*;

Serial myPort;

int   maxDistanceCM    = 300;
int   dangerThreshold  = 10;
float angleResolution  = 0.5;

float   currentAngle    = 0;
int     currentDistance = 0;
boolean objectDetected  = false;

int NUM_SLOTS = round(180 / angleResolution) + 1;
float[]   mapDistance = new float[NUM_SLOTS];
boolean[] mapHasData  = new boolean[NUM_SLOTS];

PFont radarFont;

void setup() {
  size(800, 800);
  radarFont = createFont("Courier New", 16);
  textFont(radarFont);

  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');

  strokeCap(SQUARE);
}

void draw() {
  background(0);
  translate(width / 2, height / 2);

  drawGrid();
  drawMap();
  drawLiveSweepMarker();
  drawReadout();
}

void serialEvent(Serial p) {
  String line = p.readStringUntil('\n');
  if (line == null) return;
  line = trim(line);
  if (line.length() == 0) return;

  parseSerialLine(line);
}

void parseSerialLine(String line) {
  String[] parts = split(line, ',');
  if (parts.length < 2) return;

  float angle    = float(parts[0]);
  float distance = float(parts[1]);

  if (Float.isNaN(angle) || Float.isNaN(distance)) return;
  if (angle < 0 || angle > 180) return;

  currentAngle    = angle;
  currentDistance = (int) max(distance, 0);
  objectDetected  = currentDistance > 0;

  int idx = constrain(round(angle / angleResolution), 0, NUM_SLOTS - 1);
  mapDistance[idx] = currentDistance;
  mapHasData[idx]  = true;
}

void drawGrid() {
  stroke(0, 80, 0);
  noFill();

  int rings = 4;
  for (int i = 1; i <= rings; i++) {
    float r = map(i, 0, rings, 0, width / 2f - 20);
    ellipse(0, 0, r * 2, r * 2);
  }

  for (int a = 0; a <= 180; a += 30) {
    float rad = radians(a);
    float x = cos(rad) * (width / 2f - 20);
    float y = -sin(rad) * (width / 2f - 20);
    line(0, 0, x, y);
  }

  line(-(width / 2f - 20), 0, width / 2f - 20, 0);
}

void drawMap() {
  float maxR = width / 2f - 20;
  strokeWeight(2);

  for (int idx = 0; idx < NUM_SLOTS; idx++) {
    if (!mapHasData[idx]) continue;

    float angle = idx * angleResolution;
    float dist  = mapDistance[idx];
    float rad   = radians(angle);

    float dirX = cos(rad);
    float dirY = -sin(rad);

    if (dist <= 0) {

      stroke(0, 255, 0);
      line(0, 0, dirX * maxR, dirY * maxR);
    } else {

      float rObject = constrain(map(dist, 0, maxDistanceCM, 0, maxR), 0, maxR);

      stroke(0, 255, 0);
      line(0, 0, dirX * rObject, dirY * rObject);

      stroke(255, 0, 0);
      line(dirX * rObject, dirY * rObject, dirX * maxR, dirY * maxR);
    }
  }
}

void drawLiveSweepMarker() {
  float rad = radians(currentAngle);
  float maxR = width / 2f - 20;

  strokeWeight(3);
  stroke(255, 255, 255, 180);
  line(0, 0, cos(rad) * maxR, -sin(rad) * maxR);
}

void drawReadout() {
  resetMatrix();
  fill(0, 255, 0);
  textAlign(LEFT, TOP);
  text("Angle: "    + nf(currentAngle, 0, 1) + " deg", 20, 20);
  text("Distance: " + (objectDetected ? currentDistance + " cm" : "no echo"), 20, 45);

  if (objectDetected && currentDistance <= dangerThreshold) {
    fill(255, 0, 0);
    text("object close", 20, 70);
  } else if (objectDetected) {
    fill(255, 0, 0);
    text("object detected", 20, 70);
  } else {
    fill(0, 255, 0);
    text("scanning...", 20, 70);
  }
}

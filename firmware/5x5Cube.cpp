#include <math.h>
#include "5x5Cube.h"

/** Construct a new cube.
    @param s Size of one side of the cube in number of LEDs.
    @param mb Maximum brightness value. Used to prevent the LEDs from drawing too much current (which causes the colors to distort).

    @return A new Cube object.
*/
Cube::Cube(unsigned int s, unsigned int mb) : \
    maxBrightness(mb),
    strip(Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE)),
    size(s)
{ }

/** Construct a new cube with default settings.
    @param s Size of one side of the cube in number of LEDs.
    @param mb Maximum brightness value. Used to prevent the LEDs from drawing too much current (which causes the colors to distort).

    @return A new Cube object.
*/
Cube::Cube() : \
    maxBrightness(50),
    strip(Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE)),
    size(5)
{ }

/** Initialization of cube resources and environment. */
void Cube::begin(void)
{
    this->strip.begin();
    this->strip.setBrightness(maxBrightness);
    this->center = point(this->size / 2, this->size / 2, this->size / 2);
}

/*
// Overloaded != operator.
bool operator!= (const Color& a, const Color& b)
{
    if (a.red != b.red) return true;
    if (a.green != b.green) return true;
    if (a.blue != b.blue) return true;
    return false;
}

// Overloaded == operator.
bool operator== (const Color& a, const Color& b)
{
    if (a.red != b.red) return false;
    if (a.green != b.green) return false;
    if (a.blue != b.blue) return false;
    return true;
}
*/

/** Set a voxel at a position to a color.

    @param x, y, z Coordinate of the LED to set.
    @param col Color to set the LED to.
*/
void Cube::setVoxel(int x, int y, int z, color* col)
{
    int index = ((z * (size * size)) + ((x * size) + y));
    this->strip.setPixelColor(index, this->strip.color(col.green, col.red, col.blue));
}

/** Set a voxel at a position to a color.

    @param p Coordinate of the LED to set.
    @param col Color to set the LED to.
*/
void Cube::setVoxel(point p, color* col)
{
    this->setVoxel(p.x, p.y, p.z, col);
}

// Set a pixel at an arbitrary position in the chain.
void Cube::setPixel(uint16_t p, color* col)
{
    this->strip.setPixelColor(p, this->strip.color(col.red, col.green, col.blue));
}

/** Get the color of a voxel at a position.

    @param x, y, z Coordinate of the LED to get the color from.
*/
Color Cube::getVoxel(int x, int y, int z)
{
    int index = (z * this->size * this->size) + (x * this->size) + y;
    uint32_t col = this->strip.getPixelColor(index);
    color pixelColor = color((col >> 16) & 0xff, (col >> 8) & 0xff, col & 0xff);
    return pixelColor;
}
    
/** Get the color of a voxel at a position.

    @param p Coordinate of the LED to get the color from.
*/
Color Cube::getVoxel(point p)
{
    return this->getVoxel(p.x, p.y, p.z);
}

/** Draw a line in 3D space.
    Uses the 3D form of Bresenham's algorithm.

    @param x1, y1, z1 Coordinate of start of line.
    @param x2, y2, z2 Coordinate of end of line.
    @param col Color of the line.
*/
void Cube::line(int x1, int y1, int z1, int x2, int y2, int z2, color* col)
{
    point currentPoint = point(x1, y1, z1);

    int dx = x2 - x1;
    int dy = y2 - y1;
    int dz = z2 - z1;
    int x_inc = (dx < 0) ? -1 : 1;
    int l = abs(dx);
    int y_inc = (dy < 0) ? -1 : 1;
    int m = abs(dy);
    int z_inc = (dz < 0) ? -1 : 1;
    int n = abs(dz);
    int dx2 = l << 1;
    int dy2 = m << 1;
    int dz2 = n << 1;

    if ((l >= m) && (l >= n))
    {
        int err_1 = dy2 - l;
        int err_2 = dz2 - l;

        for (int i = 0; i < l; i++)
        {
            this->setVoxel(currentPoint, col);

            if (err_1 > 0)
            {
                currentPoint.y += y_inc;
                err_1 -= dx2;
            }

            if (err_2 > 0)
            {
                currentPoint.z += z_inc;
                err_2 -= dx2;
            }

            err_1 += dy2;
            err_2 += dz2;
            currentPoint.x += x_inc;
        }
    }
    else if ((m >= l) && (m >= n))
    {
        int err_1 = dx2 - m;
        int err_2 = dz2 - m;

        for (int i = 0; i < m; i++)
        {
            this->setVoxel(currentPoint, col);

            if (err_1 > 0)
            {
                currentPoint.x += x_inc;
                err_1 -= dy2;
            }

            if (err_2 > 0)
            {
                currentPoint.z += z_inc;
                err_2 -= dy2;
            }

            err_1 += dx2;
            err_2 += dz2;
            currentPoint.y += y_inc;
        }
    }
    else
    {
        int err_1 = dy2 - n;
        int err_2 = dx2 - n;

        for (int i = 0; i < n; i++)
        {
            this->setVoxel(currentPoint, col);

            if (err_1 > 0)
            {
                currentPoint.y += y_inc;
                err_1 -= dz2;
            }

            if (err_2 > 0)
            {
                currentPoint.x += x_inc;
                err_2 -= dz2;
            }

            err_1 += dy2;
            err_2 += dx2;
            currentPoint.z += z_inc;
        }
    }

    this->setVoxel(currentPoint, col);
}

/** Draw a line in 3D space.
    Uses the 3D form of Bresenham's algorithm.

    @param p1 Coordinate of start of line.
    @param p2 Coordinate of end of line.
    @param col Color of the line.
*/
void Cube::line(point p1, point p2, color* col)
{
    this->line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, col);
}

/** Draw a filled sphere.

    @param x, y, z Position of the center of the sphere.
    @param r Radius of the sphere.
    @param col Color of the sphere.
*/
void Cube::sphere(int x, int y, int z, int r, color* col)
{
    for (int dx = -r; dx <= r; dx++)
    {
        for (int dy = -r; dy <= r; dy++)
        {
            for (int dz = -r; dz <= r; dz++)
            {
                if (sqrt(dx * dx + dy * dy + dz * dz) <= r)
                {
                    this->setVoxel(x + dx, y + dy, z + dz, col);
                }
            }
        }
    }
}

/** Draw a filled sphere.

    @param p Position of the center of the sphere.
    @param r Radius of the sphere.
    @param col Color of the sphere.
*/
void Cube::sphere(point p, int r, color* col)
{
    this->sphere(p.x, p.y, p.z, r, col);
}

/** Draw a shell (empty sphere).

    @param x Position of the center of the shell.
    @param y Position of the center of the shell.
    @param z Position of the center of the shell.
    @param r Radius of the shell.
    @param col Color of the shell.
*/
void Cube::shell(float x, float y, float z, float r, color* col)
{
    float thickness = 0.1;
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            for (int k = 0; k < this->size; k++)
                if (abs(sqrt((i - x, 2) + pow(j - y, 2) + pow(k - z, 2)) - r) < thickness)
                    this->setVoxel(i, j, k, col);
}

/** Draw a shell (empty sphere).

    @param x Position of the center of the shell.
    @param y Position of the center of the shell.
    @param z Position of the center of the shell.
    @param r Radius of the shell.
    @param thickness Thickness of the shell.
    @param col Color of the shell.
*/
void Cube::shell(float x, float y, float z, float r, float thickness, color* col)
{
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            for (int k = 0; k < this->size; k++)
                if (abs(sqrt(pow(i - x, 2) + pow(j - y, 2) + pow(k - z, 2)) - r) < thickness)
                    this->setVoxel(i, j, k, col);
}

/** Draw a shell (empty sphere).

    @param p Position of the center of the shell.
    @param r Radius of the shell.
    @param col Color of the shell.
*/
void Cube::shell(point p, float r, color* col)
{
    this->shell(p.x, p.y, p.z, r, col);
}

/** Draw a shell (empty sphere).

    @param p Position of the center of the shell.
    @param r Radius of the shell.
    @param thickness Thickness of the shell
    @param col Color of the shell.
*/
void Cube::shell(point p, float r, float thickness, color* col)
{
    this->shell(p.x, p.y, p.z, r, thickness, col);
}

/** Draw an empty circle in the XZ plane.
    Uses the midpoint circle algorithm.

    @param p Position of the center of the circle.
    @param r Radius of the circle.
    @param col Color of the circle.
*/
void Cube::emptyFlatCircle(int x, int y, int z, int r, color* col)
{
    int dx = r;
    int dy = 0;
    int radiusError = 1 - dx;

    while (dx >= dy)
    {
        this->setVoxel(x + dx,  y, z + dy, col);
        this->setVoxel(x + dy,  y, z + dx, col);
        this->setVoxel(x + -dx, y, z + dy, col);
        this->setVoxel(x + -dy, y, z + dx, col);
        this->setVoxel(x + -dx, y, z + -dy, col);
        this->setVoxel(x + -dy, y, z + -dx, col);
        this->setVoxel(x + dx,  y, z + -dy, col);
        this->setVoxel(x + dy,  y, z + -dx, col);

        dy++;

        if (radiusError < 0)
        {
            radiusError += 2 * dy + 1;
        }
        else
        {
            dx--;
            radiusError += 2 * (dy - dx + 1);
        }
    }
}

/** Set the entire cube to one color.

    @param col The color to set all LEDs in the cube to.
*/
void Cube::background(color* col)
{
    for (int x = 0; x < this->size; x++)
        for (int y = 0; y < this->size; y++)
            for (int z = 0; z < this->size; z++)
                this->setVoxel(x, y, z, col);
}

/** Map a value into a color.
    The set of colors fades from blue to green to red and back again.

    @param val Value to map into a color.
    @param minVal Minimum value that val will take.
    @param maxVal Maximum value that val will take.

    @return Color from value.
*/
color colorMap(float val, float min, float max)
{
    float range = 1024;
    val = range * (val - min) / (max - min);
    color colors[6];
    colors[0].red = 0;
    colors[0].green = 0;
    colors[0].blue = this->maxBrightness;

    colors[1].red = 0;
    colors[1].green = this->maxBrightness;
    colors[1].blue = this->maxBrightness;

    colors[2].red = 0;
    colors[2].green = this->maxBrightness;
    colors[2].blue = 0;

    colors[3].red = this->maxBrightness;
    colors[3].green = this->maxBrightness;
    colors[3].blue = 0;

    colors[4].red = this->maxBrightness;
    colors[4].green = 0;
    colors[4].blue = 0;

    colors[5].red = this->maxBrightness;
    colors[5].green = 0;
    colors[5].blue =this->maxBrightness;

    if (val <= range / 6)
        return (this->lerpColor(&colors[0], &colors[1], val, 0, range / 6));
    else if (val <= 2 * range / 6)
        return (this->lerpColor(&colors[1], &colors[2], val, range / 6, 2 * range / 6));
    else if (val <= 3 * range / 6)
        return (this->lerpColor(&colors[2], &colors[3], val, 2 * range / 6, 3 * range / 6));
    else if (val <= 4 * range / 6)
        return (this->lerpColor(&colors[3], &colors[4], val, 3 * range / 6, 4 * range / 6));
    else if (val <= 5 * range / 6)
        return (this->lerpColor(&colors[4], &colors[5], val, 4 * range / 6, 5 * range / 6));
    else
        return (this->lerpColor(&colors[5], &colors[0], val, 5 * range / 6, range));
}
/*
Color Cube::colorMap(float val, float minVal, float maxVal)
{
    const float range = 1024;
    val = range * (val - minVal) / (maxVal - minVal);

    Color colors[6];

    colors[0].red = 0;
    colors[0].green = 0;
    colors[0].blue = this->maxBrightness;

    colors[1].red = 0;
    colors[1].green = this->maxBrightness;
    colors[1].blue = this->maxBrightness;

    colors[2].red = 0;
    colors[2].green = this->maxBrightness;
    colors[2].blue = 0;

    colors[3].red = this->maxBrightness;
    colors[3].green = this->maxBrightness;
    colors[3].blue = 0;

    colors[4].red = this->maxBrightness;
    colors[4].green = 0;
    colors[4].blue = 0;

    colors[5].red = this->maxBrightness;
    colors[5].green = 0;
    colors[5].blue = this->maxBrightness;

    if (val <= range / 6)
        return this->lerpColor(colors[0], colors[1], val, 0, range / 6);
    else if (val <= 2 * range / 6)
        return (this->lerpColor(colors[1], colors[2], val, range / 6, 2 * range / 6));
    else if (val <= 3 * range / 6)
        return (this->lerpColor(colors[2], colors[3], val, 2 * range / 6, 3 * range / 6));
    else if (val <= 4 * range / 6)
        return (this->lerpColor(colors[3], colors[4], val, 3 * range / 6, 4 * range / 6));
    else if (val <= 5 * range / 6)
        return (this->lerpColor(colors[4], colors[5], val, 4 * range / 6, 5 * range / 6));
    else
        return (this->lerpColor(colors[5], colors[0], val, 5 * range / 6, range));
}
*/
/** Linear interpolation between colors.

    @param a, b The colors to interpolate between.
    @param val Position on the line between color a and color b.
    When equal to min the output is color a, and when equal to max the output is color b.
    @param minVal Minimum value that val will take.
    @param maxVal Maximum value that val will take.

    @return Color between colors a and b.
*/
color lerpColor(color* a, color* b, int val, int min, int max)
{
    color lerped;
    lerped.red = a->red + (b->red-a->red) * (val-min) / (max-min);
    lerped.green = a->green + (b->green-a->green) * (val-min) / (max-min);
    lerped.blue = a->blue + (b->blue-a->blue) * (val-min) / (max-min);
    return lerped;
}
/*
Color Cube::lerpColor(Color a, Color b, int val, int minVal, int maxVal)
{
    int red = a.red + (b.red - a.red) * (val - minVal) / (maxVal - minVal);
    int green = a.green + (b.green - a.green) * (val - minVal) / (maxVal - minVal);
    int blue = a.blue + (b.blue - a.blue) * (val - minVal) / (maxVal - minVal);

    return color(red, green, blue);
}
*/

// Make changes to the cube visible.
//  Causes pixel data to be written to the LED strips.
void Cube::show()
{
    this->strip.show();
}
